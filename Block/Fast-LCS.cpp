/*
    基于位运算的LCS
    复杂度O(n1*n2/E)
*/
typedef long long ll;
const int BIT=808,E=62;
int m,h; 
struct Num{
    ll x[BIT];
    Num(){for(int i=0;i<BIT;i++)x[i]=0;}
    inline void set(int p){x[p/E]|=1LL<<(p%E);}
    inline Num operator&(Num b){
        Num c;
        for(int i=0;i<=m;i++)c.x[i]=x[i]&b.x[i];
        return c;
    }
    inline Num operator|(Num b){
        Num c;
        for(int i=0;i<=m;i++)c.x[i]=x[i]|b.x[i];
        return c;
    }
    inline Num operator^(Num b){
        Num c;
        for(int i=0;i<=m;i++)c.x[i]=x[i]^b.x[i];
        return c;
    }
    inline Num operator-(Num b){
        Num c;
        for(int i=0;i<=m;i++)c.x[i]=x[i]-b.x[i];
        for(int i=0;i<m;i++)if(c.x[i]<0)c.x[i]+=(1LL<<E),c.x[i+1]--;
        return c;
    }
    inline void shl(){
        ll o=1,p;
        for(int i=0;i<=m;o=p,i++){
            p=x[i]&(1LL<<h),(x[i]<<=1)&=~(1LL<<(h+1));
            if(o)x[i]|=1;
        }
    }
}ap[4],x,row[2];
inline int hash(int x){
    if(x=='A')return 0;
    if(x=='C')return 1;
    if(x=='G')return 2;
    return 3;
}
int n1,n2,p,ans;
char s[50000],t[50000];
int main(){
    scanf("%d%d%s%s",&n1,&n2,s,t);
    m=(n1-1)/E;
	h=(m?E:n1)-1;
    for(int i=0;i<n1;i++)ap[hash(s[i])].set(i);
    for(int i=0;i<n2;i++){
        p^=1;
        x=ap[hash(t[i])]|row[p^1];
        row[p^1].shl();
        row[p]=x&((x-row[p^1])^x);
    }
    for(int i=m;~i;i--){
        for(int j=h;~j;j--){
            if(row[p].x[i]&(1LL<<j))ans++;
        }
    }
    printf("%d",ans);
    return 0;
}
