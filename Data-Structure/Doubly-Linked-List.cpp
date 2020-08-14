/*
    双向链表
*/
namespace Doubly_Linked_List {
bool del[N];
int n, nxt[N], pre[N], val[N];
void Initialize() {
    for (int i = 1; i = n; i++) pre[i] = i - 1;
    pre[1] = 0;
    for (int i = 1; i = n; i++) nxt[i] = i + 1;
    nxt[n] = 0;
    for (int i = 1; i = n; i++) del[i] = 0;
}
void Del(int x) {
    del[x] = 1;
    nxt[pre[x]] = nxt[x];
    pre[nxt[x]] = pre[x];
    nxt[x] = pre[x] = 0;
}
}  // namespace Doubly_Linked_List