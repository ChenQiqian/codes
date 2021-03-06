#include <cstdio>
#include <queue>
#include <cstring>
#include <algorithm>
#define int long long
using namespace std;

const int MAX = 1e10;

const int MAXN = 1000,MAXM = 200000;

struct Edge{
    int from,to;
    int flow,cap;
    int cost;
    int nex;
}edge[MAXM];

int n,m,s,t,ecnt = 2;
int fir[MAXN],pree[MAXN];
int dis[MAXN],instack[MAXN];
queue<int> q;

void addedge(int a,int b,int c,int d){
    //printf("%lld %lld %lld %lld\n",a,b,c,d);
    edge[ecnt].from = a,edge[ecnt].to = b;
    edge[ecnt].cap = c,edge[ecnt].flow = 0;
    edge[ecnt].cost = d,edge[ecnt].nex = fir[a];
    fir[a] = ecnt++;
    edge[ecnt].from = b,edge[ecnt].to = a;
    edge[ecnt].cap = 0,edge[ecnt].flow = 0;
    edge[ecnt].cost = -d,edge[ecnt].nex = fir[b];
    fir[b] = ecnt++;
}

bool spfa(){
    while(!q.empty()) q.pop();
    memset(dis,0x3f,sizeof(dis));
    //memset(instack,0,sizeof(instack));
    dis[s] = 0;q.push(s);
    while(!q.empty()){
        int nown = q.front();q.pop();
        instack[nown] = 0;
        for(int nowe = fir[nown];nowe;nowe = edge[nowe].nex){
            Edge e = edge[nowe];
            if(dis[e.to] > dis[nown] + e.cost && e.cap > e.flow){
                dis[e.to] = dis[nown] + e.cost;
                pree[e.to] = nowe;
                if(instack[e.to] == 0){
                    instack[e.to] = 1;
                    q.push(e.to);
                }
            }
        } 
    }
    return dis[t] < 0x3f3f3f3f3f3f3f3f;
}

void argument(int &sumf,int &sumc){
    int nown = t,delta = MAX,nowe;
    while(nown!=s){
        nowe = pree[nown];
        delta = min(delta,edge[nowe].cap - edge[nowe].flow);
        nown = edge[nowe].from;
    }
    nown = t;
    while(nown!=s){
        nowe = pree[nown];
        edge[nowe].flow+=delta,edge[nowe^1].flow-=delta;
        nown = edge[nowe].from;
    }
    sumf+=delta,sumc+=delta*dis[t];
}

void min_cost_flow(){
    int c = 0,f = 0;
    while(spfa())
        argument(f,c);
    printf("%lld\n",c);
}

void init(){
    scanf("%lld %lld",&m,&n);
    s = m+n+1,t = m+n+2;
    int tmp = 0;
    for(int i = 1;i<=n;i++){
        scanf("%lld",&tmp);
        addedge(m+i,t,tmp,0);
    }
    for(int i = 1;i<=m;i++){
        for(int j = 1;j<=n;j++){
            scanf("%lld",&tmp);
            if(tmp)
                addedge(i,m+j,MAX,0);
        }
    }
    int b[10];
    for(int i = 1;i<=m;i++){
        scanf("%lld",&tmp);
        for(int j = 1;j<=tmp;j++)
            scanf("%lld",&b[j]);
        b[0] = 0;b[tmp+1] = MAX;

        for(int j = 1;j<=tmp+1;j++){
            int w;scanf("%lld",&w);
            addedge(s,i,b[j]-b[j-1],w);
        } 
    }
}

main(){
    init();
    min_cost_flow();
    return 0;
}