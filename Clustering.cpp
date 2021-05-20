#include <bits/stdc++.h>

using namespace std;

typedef struct data {
	float value[70]; //데이터값
	long long int timestamp; //시간
}data;

typedef struct dataStack {
	float CN;
	float radius;
	data CM;
	vector<data> element;
}dataStack;

typedef struct Cluster {
	vector<dataStack> element_stack;
}Cluster;

int Sensor_Num = 58, Data_Num = 0;
float RemovingNoise = 0.925;
float MovingCM = 0.5;
vector<dataStack> DataStream;
vector<Cluster> ClusterSet;

void input();
void output();
void addDataStream(dataStack); //DataStream에 새로운 Data 추가
float dist(data, data);
void Clustering(dataStack);
void Steal(dataStack, dataStack);
void MakeSmall(dataStack, float);
float distMoving(dataStack, dataStack);

int main() {
	input();
	output();
}

void input() {
	FILE *file;
	
	file = fopen("data_cut.txt", "r");
	
	int cnt = 0;
		
	while(true) {
		bool End = false;
		dataStack New;
		while(!End) {
			data tmp;
			fscanf(file, "%lld", &tmp.timestamp);
			if(tmp.timestamp == -2) break;
			else if(tmp.timestamp == -3) End = true;
			
			if(End) break;
			
			for(int i=1; i<=Sensor_Num; i++)
				fscanf(file, "%f", &tmp.value[i]);
			New.element.push_back(tmp);
			
			
		}
		if(End) break;
		cnt++;
		printf("%d dataStack Reading Complete\n", cnt);
		addDataStream(New);
	}
	fclose(file);
	printf("%d\n", ClusterSet.size());
}

void output() {
	FILE *file_in, *file_out;
	file_in = fopen("data_cut.txt", "r");
	file_out = fopen("data_output.txt", "w");
	
	int Cluster_past = -1;
	bool End = false;
	while(!End) {
		data tmp;
		fscanf(file_in, "%lld", &tmp.timestamp);
		
		int Cluster_cur = -1;
		
		if(tmp.timestamp == -2) continue;
		else if(tmp.timestamp == -3) End = true;
		
		if(End) break;
			
		for(int i=1; i<=Sensor_Num; i++)
			fscanf(file_in, "%f", &tmp.value[i]);
			
		for(int i=0; i<DataStream.size(); i++) {
			//printf("%f %f\n", dist(tmp, DataStream[i].CM), DataStream[i].radius);
			if(dist(tmp, DataStream[i].CM) <= DataStream[i].radius) {
				Cluster_cur = DataStream[i].CN;
				break;
			}
		}
		
		printf("%d\n", Cluster_cur);
		if(Cluster_cur != Cluster_past) {
			fprintf(file_out, "%lld\n", tmp.timestamp);
			printf("%lld\n", tmp.timestamp);
		}
		Cluster_past = Cluster_cur;
	}
	
	fclose(file_in);
	fclose(file_out);
}

void addDataStream(dataStack New) {
	// Find the CM	
	for(int i=1; i<=Sensor_Num; i++) {
		New.CM.value[i] = 0;
		for(int j=0; j<New.element.size(); j++) {
			New.CM.value[i] += New.element[j].value[i];
		}
		New.CM.value[i] /= New.element.size();
	}
	//printf("Find the CM\n");
	
	// Find the radius
	for(int i=0; i<New.element.size(); i++) {
		New.radius = max(New.radius, dist(New.CM, New.element[i]));
	}
	New.radius *= RemovingNoise;
	//printf("Find the Radius\n");
	
	// Remove the noise
	vector<data>::iterator itr = New.element.begin();
	while(itr != New.element.end()) {
		if(dist(New.CM, *itr) >= New.radius) 
			itr = New.element.erase(itr);
		else itr++;
	}
	//printf("Remove the Noise\n");
	
	// Clustering
	Clustering(New);
}

float dist(data a, data b) {
	float re = 0;
	for(int i=1; i<=Sensor_Num; i++) {
		re += pow(a.value[i]-b.value[i], 2);
	}
	re = sqrt(re);
	return re;
}

void Clustering(dataStack New) {
	int ClusterNum = -1;
	for(int i=0; i<ClusterSet.size(); i++) {
		for(int j=0; j<ClusterSet[i].element_stack.size(); j++) {
			dataStack Old = ClusterSet[i].element_stack[j];
			//printf("%f %f\n", Old.radius , dist(Old.CM, New.CM));
			if(Old.radius > dist(Old.CM, New.CM)) {
				ClusterNum = i;
			}
		}
	}
	//printf("%d\n\n", ClusterNum);
	//printf("Find the proper Cluster\n");
	
	for(int i=0; i<ClusterSet.size(); i++) {
		if(ClusterNum == i) continue;
		
		for(int j=0; j<ClusterSet[i].element_stack.size(); j++) {
			dataStack Old = ClusterSet[i].element_stack[j];
			if(Old.radius > dist(Old.CM, New.CM) - New.radius) {
				float d = distMoving(Old, New);
				if(d > MovingCM*Old.radius) {
					printf("ok");
					Steal(Old, New);
				}
				else {
					float NewR = dist(Old.CM, New.CM) - Old.radius;
					float OldR = dist(Old.CM, New.CM) - New.radius;
					MakeSmall(Old, OldR);
					MakeSmall(New, NewR);
				}
			}
		}
	}
	//printf("Adjust the Range");
	
	if(ClusterNum == -1) {
		New.CN = ClusterSet.size();
		DataStream.push_back(New);
		Cluster NewC;
		NewC.element_stack.push_back(New);
		ClusterSet.push_back(NewC);
	}
	else {
		New.CN = ClusterNum;
		DataStream.push_back(New);
		ClusterSet[ClusterNum].element_stack.push_back(New);
	}
	//printf("Clustering End\n");
}

float distMoving(dataStack Origin, dataStack New) {
	data NewCM;
	for(int i=1; i<=Sensor_Num; i++) {
		int data_exist = 0;
		for(int j=0; j<Origin.element.size(); j++) {
			if(dist(Origin.element[j], New.CM) <= New.radius) continue;
			NewCM.value[i] += Origin.element[j].value[i];
			data_exist++;
		}
		if(data_exist == 0) NewCM.value[i] /= data_exist;
		else NewCM.value[i] = Origin.CM.value[i];
	}
	
	return dist(NewCM, Origin.CM);
}

void Steal(dataStack from, dataStack to) {
	vector<data>::iterator itr = from.element.begin();
	while(itr != from.element.end()) {
		if(dist(to.CM, *itr) < to.radius) {
			data New;
			New.timestamp = itr->timestamp;
			for(int i=1; i<=Sensor_Num; i++)
				New.value[i] = itr->value[i];
			to.element.push_back(New);
			itr = from.element.erase(itr);
		}
		else itr++;
	}
}

void MakeSmall(dataStack DS, float R) {
	vector<data>::iterator itr = DS.element.begin();
	while(itr != DS.element.end()) {
		if(dist(DS.CM, *itr) >= R) 
			itr = DS.element.erase(itr);
		else itr++;
	}
}
