#include <bits/stdc++.h>
#include <map>
using namespace std;

typedef struct data {
    int val[100];
}data;

data Array[100000];

int range = 10;
long long int time_st = -1, time_en = -1;

long long int time_past = -1;

int main() {
    freopen("data_kaist.txt","w",stdout);
    memset(Array, -1, sizeof(Array));

    /*ifstream file0 ("data0.csv");
    string value0;
    while(file0.good()) {
        getline ( file0, value0, '\n' );

		int i=0;
		for(int j=0; j<3; j++) {
			while(value0[i]!=',') i++;
			i++;
		}
        long long int time_cur = 0;
         for(; value0[i]!='\0'; i++) {
            time_cur *= 10;
            time_cur += value0[i]-'0';
        }
        time_cur /= 1000;

        if(time_st == -1) time_st = time_cur;
        else time_st = min(time_st, time_cur);
        if(time_en == -1) time_en = time_cur;
        else time_en = max(time_en, time_cur);
    }
    printf("%lld %lld\n", time_st, time_en);
    printf("%lld\n", (time_en-time_st)/range);*/

    time_st = 1527778801;
    time_en = 1527865200;

    ifstream file ( "data.csv" ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
    string value;
    map <string,int> mp;
    int ls=1; //센서번호
    while ( file.good() )
    {
         getline ( file, value, '\n' ); // read a string until next comma: http://www.cplusplus.com/reference/string/getline/
        // cout << string( value, 0, value.length() ); // display value removing the first and the last character from it
		int i;
        //printf("%s\n",value);
         //if(value[0] != '1') continue;
		for(i=0; value[i]!=','; i++);
		i++;
		string name="";
		int k=0;
		for(;value[i]!=',';i++){
			name+=value[i];
			//printf("%c",name[k]);
		}
		i++;

		//cout << name << "\n";

		if(mp[name]==0) mp[name]=ls++;


		int sensor_value=0;
		if(value[i]>='0'&&value[i]<='9'){
			int sosu=0;//flag역할
			for(;value[i]!=',';i++){
				if(value[i]=='.')sosu=1;
				if(sosu==0){
					sensor_value=sensor_value*10+value[i]-'0';
				}
			}
		}
		else{
			if(value[i]=='T'&&value[i+1]=='R'&&value[i+2]=='U'&&value[i+3]=='E'){
				sensor_value=1;
				i+=4;
			}
			else if(value[i]=='F'&&value[i+1]=='A'&&value[i+2]=='L'&&value[i+3]=='S'&&value[i+4]=='E'){
				sensor_value=0;
				i+=5;
			}
			else{
                for(;value[i]!=',';i++);
			}
		}
		i++;

		long long int time_cur = 0;
         for(; value[i]!='\0'; i++) {
            time_cur *= 10;
            time_cur += value[i]-'0';
        }
        time_cur /= 1000;

        int sensor_num = mp[name];

        /*for(; value[index]==','; index++)
            sensor_num++;*/

        int TimeIndex = (time_cur-time_st)/range;
        //printf("%d: ", TimeIndex);

        Array[TimeIndex].val[sensor_num] = sensor_value;

        //cout << string( value, 0, value.length() );
        //printf(": %lld, %d, %d\n", time_cur, sensor_num, sensor_value);
    }
    for(int i=0; i <= (time_en-time_st)/range; i++) {
        printf("%lld ", time_st + i*range);
        for(int j=1; j <= ls; j++)
            printf("%d ", Array[i].val[j]);
        printf("\n");
    }
    printf("-1");
}

