/*
    树上删边游戏
        给出一个有 n 个结点的树，有一个点作为树的根节点, 
        双方轮流从树中删去一条边边，之后不与根节点相连的部分将被移走, 无法操作者输.
    策略：
        叶子结点的SG值为0, 其他结点SG值为其每个儿子结点SG值加1后的异或和
*/
int SG(int x){
    int sg=0;
    for(int i=0;i<v[x].size();i++)sg^=SG(v[x][i]);
	val[x]+=sg;
    return val[x];
}
int solve(){
    val[0]=0;
    for(int i=1;i<=n;i++)val[i]=1;
    puts(SG(0)?"Alice":"Bob");
}