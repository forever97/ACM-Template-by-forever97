/*
    对称性博弈
    Example：
        两个人玩游戏. 每次任选一堆，首先拿掉至少一个石头,
        然后移动任意个石子到任意堆中. 谁不能移动了，谁就输了
    Solution：
        首先如果对于奇数堆，那么先手必胜，因为可以构建必败态
        对于偶数的情况，如果是石子堆两两相同的对称局面，则为必败态，反之必胜
*/
string s[100010];
char tmp[10010];
int n;
int main(){
    scanf("%d",&n);
    if(n&1){puts("first player");return 0;}
    for(int i=1;i<=n;i++){scanf("%s",tmp);s[i]=tmp;}
    sort(s+1,s+n+1);
    for(int i=1;i<=n;i+=2)if(s[i]!=s[i+1]){puts("first player");return 0;}
    puts("second player");
    return 0;
}