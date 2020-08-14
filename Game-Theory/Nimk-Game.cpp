/*
    Nimk游戏:
        n堆石子，每人一次可以从最多m堆石子中取出每堆至多k个，不能取的人输.
    策略：
        在二进制下各堆石子的sg函数均为(m+1)的倍数的话则为必败态, 否则为必胜态.
*/
using namespace std;
const int N=100010; 
int n,m,k,a[N],sg[N];
bool SG(){
    memset(sg,0,sizeof(sg));
    for(int i=0;i<n;i++)for(int j=0,g=a[i]%(k+1);sg[j]+=g&1,g;j++,g>>=1);
    for(int i=0;i<30;i++)if(sg[i]%(m+1))return 1;
    return 0;
}
int main(){
    while(~scanf("%d%d%d",&n,&m,&k)){
        for(int i=0;i<n;i++)scanf("%d",&a[i]);
        puts(SG()?"Alice":"Bob");
    }return 0;
}
