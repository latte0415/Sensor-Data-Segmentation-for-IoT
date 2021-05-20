#include <bits/stdc++.h>
using namespace std;

typedef struct data{
    long long time;
    float value[70];
}data;
float Max[70], Min[70];
data dataSet[105000];
float mn[95005],mx[95005];
int cnt=0;
float sl[95005][70];
int past[70];

int main()
{
    freopen("data_kaist.txt","r",stdin);
    freopen("data_cut.txt","w",stdout);
    int K;

    scanf("%lld",&dataSet[0].time);
    for(int i=0;i<58;i++){
        scanf("%f",&dataSet[0].value[i]);
    }

    for(int i=0; i<58; i++) {
        Max[i] = mn[i] = mx[i] = -1;
        Min[i] = 100000;
    }

    for(K=1;;K++){
        scanf("%lld",&dataSet[K].time);
        //printf("%lld\n", dataSet[K].time);

        if(dataSet[K].time==-1)break;
        for(int i=0;i<58;i++){
            scanf("%f",&dataSet[K].value[i]);

            if(dataSet[K].value[i] == -1) {
                sl[K][i] = -1;
                continue;
            } else {
                Max[i] = max(Max[i], dataSet[K].value[i]);
                Min[i] = min(Min[i], dataSet[K].value[i]);
            }

            if(dataSet[past[i]].value[i] == -1) {
                sl[K][i] = -1;
                past[i] = K;
                continue;
            }
            sl[K][i]=(float)abs(dataSet[K].value[i]-dataSet[past[i]].value[i])/(K-past[i]);
            past[i] = K;

            if(mn[i] == -1) mn[i] = sl[K][i];
            else mn[i]=min(mn[i],sl[K][i]);
            if(mx[i] == -1) mx[i] = sl[K][i];
            else mx[i]=max(mx[i],sl[K][i]);

        }
    }

    memset(past, 0, sizeof(past));

    double gijoon[70];
    int per=1;//자르는 기준값 설정 위한 정도... min:max를 per:1로 내분
    for(int i=0;i<58;i++) {
        if(mn[i] == -1) gijoon[i] = -1;
        else gijoon[i]=(double)(per*mx[i]+mn[i])/(per+1);
        //printf("%f %f %f\n", mx[i], mn[i], gijoon[i]);
    }

    printf("%lld ",dataSet[0].time);
    for(int j=0;j<58;j++){
        if(dataSet[0].value[j] == -1) printf("-100 ");
        else if(Max[j] != Min[j]) printf("%f ",(dataSet[0].value[j]-Min[j])*100/(Max[j]-Min[j]));
        else printf("100 ");
    }
    printf("\n");

    for(int S=1;S<K;S++){
        int ch=0;
        for(int i=0;i<58;i++){
            if(gijoon[i] != -1 && sl[S][i] > gijoon[i]){
                ch=-1;
                break;
            }
        }
        if(ch==-1){printf("-2\n");cnt++;}
        printf("%lld ",dataSet[S].time);
        for(int j=0;j<58;j++){
            if(dataSet[S].value[j] == -1) printf("-100 ");
            else if(Max[j] != Min[j]) printf("%f ",(dataSet[S].value[j]-Min[j])*100/(Max[j]-Min[j]));
            else printf("100 ");
        }
        printf("\n");
    }
    printf("-3\n");
    printf("%d", cnt);

    //printf("%d",cnt);
    return 0;
}

