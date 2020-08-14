/*
    Multi-Nim游戏
    多个游戏的sg值为sg值的异或值
    Example：
        在一个1*n的方格纸上下棋，谁先连三子谁就赢了，问必胜的是谁。
    Solution:
        我们发现对于一个n规模的游戏。在i位置下棋就能将游戏拆分为i-3和n-i-2两个游戏
        对于可拆分的游戏，其sg函数为拆分后游戏sg值的异或和，
        因此我们用记忆化搜索来记录sg值。
*/
int n,sg[2010];
int SG(int n){
    if(n<0)return 0;
    if(sg[n]>=0)return sg[n];
    bool tmp[2010]={0};
    for(int i=1;i<=n;i++){
        int t=SG(i-3)^SG(n-i-2);
        tmp[t]=1;
    }for(int i=0;;i++){
        if(!tmp[i])return sg[n]=i;
    }
}
int main(){
    memset(sg,-1,sizeof(sg)); 
    while(~scanf("%d",&n)){
        if(SG(n))puts("1");
        else puts("2");
    }return 0;
}