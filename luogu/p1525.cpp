#include <cstdio>
#include <algorithm>
using namespace std;int n,m,f[20010],r[20010];inline void qr(int &x){x=0;char c=getchar();while(c>'9'||c<'0'){c=getchar();}while(c>='0'&&c<='9'){x=(x<<3)+(x<<1)+c-'0';c=getchar();}}struct query{int a,b,v;}q[100100];inline bool cmp(query a,query b){return a.v>b.v;}inline int fi(int x){if(f[x]!=x){int w=fi(f[x]);r[x]^=r[f[x]];f[x]=w;}return f[x];}inline bool u(int x,int y){int x1=fi(x),y1=fi(y);bool re=(r[x]^r[y]);if(x1==y1){return re;}else{re^=1;f[x1]=y1;r[x1]=re;return 1;}}int main(){qr(n);qr(m);for(int i=1;i<=n;i++)f[i]=i;for(int i=0;i<m;i++){qr(q[i].a);qr(q[i].b);qr(q[i].v);}sort(q,q+m,cmp);for(int i=0;i<m;i++){if(u(q[i].a,q[i].b)==0){return printf("%d\n",q[i].v),0;}}printf("0\n");return 0;}