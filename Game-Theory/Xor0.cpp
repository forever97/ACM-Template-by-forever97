/*
    求构造k个石子堆，每堆石子不超过n个，且先手必败的方案数(xor值为0)
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const LL mod=998244353;
LL pw[1010];
LL qpow(LL a,int b){
	LL t=1;
	while(b){
		if(b&1)t=t*a%mod;
		a=a*a%mod;
		b=b>>1;
	}
	return t;
}
int b,c,n,k;
LL inv(LL a,LL m){return(a==1?1:inv(m%a,m)*(m-m/a)%m);}
LL Cal(int n){
	if(n==0)return 1;
	b=floor(log(n)/log(2));
	c=n+1-pw[b];
	if(k&1)return (qpow(pw[b]+c,k)+qpow(pw[b]-c,k))%mod*inv(qpow(2,b+1),mod)%mod;
	else return
	((qpow(pw[b]+c,k)+qpow(pw[b]-c,k)-qpow(c,k)*2%mod+mod)%mod*inv(qpow(2,b+1),mod)%mod+Cal(c-1))%mod;
}
int main(){
	pw[0]=1; 
	for(int i=1;i<=1000;i++)pw[i]=2LL*pw[i-1]%mod;
	scanf("%d%d",&k,&n);
	printf("%lld\n",Cal(n));
	return 0;
}

