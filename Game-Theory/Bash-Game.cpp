/*
    巴什博弈：
        有一堆石子，每人最多一次只能取m个石子, 谁没有石子可以取谁输.
    策略：
        SG(x)=x%(m+1)
    Example：
        对于巴什博奕给定石子数，请给出每次取的上限值，使后手必胜
*/
int k,i,ans;
using namespace std;
int main(){
    while(~scanf("%d",&k)){
        if(k%3==0)puts("2");
        else if(k%4==0)puts("3");
        else{
            while(k%2==0)k/=2; ans=k-1;
            for(i=5;i<=int(sqrt((double)k))+1;i++)if(k%i==0){ans=i-1;break;}
            printf("%d\n",ans);
        }
    }return 0;
}