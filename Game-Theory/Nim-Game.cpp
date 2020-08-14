/*
    Nim游戏:
        n堆石子，双方轮流从任意一堆石子中取出至少一个，不能取的人输.
    策略：
        对于一堆x个石子的情况，sg(x)=x.
        所以所有石子个数的异或和为0是必败态，否则为必胜态.
*/
/*
    Example：
        每个人每次可以从一堆石子中取出若干个石子，
        每次取石子的个数有限制，谁不能取石子时就会输掉游戏。 
        小H先进行操作，他想问你他是否有必胜策略，
        如果有，第一步如何取石
*/
const int N=1005;
int SG[N],b[N],hash[N],a[N],sum,tmp,n,m;  
void FSG(int s){  
    SG[0]=0;  
    for(int i=1;i<=s;i++){  
        for(int j=1;b[j]<=i&&j<=m;j++)hash[SG[i-b[j]]]=i;  
        for(int j=0;j<=s;j++)if(hash[j]!=i){SG[i]=j;break;}  
    }  
}  
int main(){  
    // n个数表示每堆石子的个数
    scanf("%d",&n); for(int i=1;i<=n;i++)scanf("%d",&a[i]);  
    // m个数表示每次可以取的石子个数，输入保证这m个数按照递增顺序排列
    scanf("%d",&m); for(int i=1;i<=m;i++)scanf("%d",&b[i]);  
    FSG(N); for(int i=1;i<=n;i++)sum^=SG[a[i]];  
    if(!sum){printf("NO");return 0;}  
    for(int i=1;i<=n;i++){  
        tmp=sum^SG[a[i]];  
        for(int j=1;b[j]<=a[i]&&j<=m;j++)  
        if(!(tmp^SG[a[i]-b[j]])){  
            printf("YES\n%d %d",i,b[j]);  
            return 0;  
        }  
    }  
}  
/*
    Example: 
        取石子游戏新手必胜策略数
*/
void solve(){
    while(~scanf("%d",&n),n){
        int sg=0,ans=0;
        for(int i=0;i<n;i++)scanf("%d",&a[i]),sg^=a[i];
        for(int i=0;i<n;i++)if((sg^a[i])<a[i])ans++;
        printf("%d\n",ans);
    }
}