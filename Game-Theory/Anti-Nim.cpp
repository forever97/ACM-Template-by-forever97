/*
    Anti-Nim博弈
        n堆石子, 双方轮流从任意一堆石子中取出至少一个, 不能取的一方获胜
    先手必胜：
        1.所有堆的石子数都为1且游戏的SG值为0；
        2.有些堆的石子数大于1且游戏的SG值不为0。
*/
while(T--){
    scanf("%d",&n);
    for(s=tmp=0;n--;)scanf("%d",&x),s^=x,tmp|=(x>1);
    puts((s>0)^tmp?"Win":"Lose");
}