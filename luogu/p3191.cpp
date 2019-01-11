// luogu-judger-enable-o2
#include <bits/stdc++.h>
#include <unistd.h>
#define fuck printf("fuck\n")
using namespace std;

const int inf = 1e8;

const int MAXN = 40,MAXM = 4000000;

namespace MaxFlow{
  int S,T;
  struct Edge{
    int to,nex;
    int cap,flow;
  }edge[MAXM];int ecnt = 2;
  int fir[MAXN*MAXN],cur[MAXN*MAXN];
  void addedge(int a,int b,int c){
    if(a == -1 || b == -1) return;
    // printf("%d %d %d\n",a,b,c);
    edge[ecnt] = (Edge){b,fir[a],c,0};
    fir[a] = ecnt++;
    edge[ecnt] = (Edge){a,fir[b],0,0};
    fir[b] = ecnt++;
  }
  void clear(){
    memset(fir,0,sizeof(fir));
    ecnt = 2;
  }
  int dis[MAXN*MAXN];
  queue<int> q;
  bool bfs(){
    while(!q.empty()) q.pop();
    memset(dis,0,sizeof(dis));
    memcpy(cur,fir,sizeof(fir));
    dis[S] = 1;q.push(S);
    while(!q.empty()){
      int nown = q.front();q.pop();
      for(int nowe = fir[nown];nowe;nowe = edge[nowe].nex){
        int v = edge[nowe].to;
        if(dis[v] == 0 && edge[nowe].cap > edge[nowe].flow){
          dis[v] = dis[nown] + 1;
          q.push(v);
          if(v == T) return 1;
        }
      }
    }
    return 0;
  }
  int dfs(int nown,int limit = 0x3f3f3f3f){
    if(nown == T || limit == 0){
      return limit;
    }
    for(int &nowe = cur[nown];nowe;nowe = edge[nowe].nex){
      int v = edge[nowe].to;
      if(dis[v] == dis[nown] + 1 && edge[nowe].cap > edge[nowe].flow){
        int f = dfs(v,min(limit,edge[nowe].cap - edge[nowe].flow));
        if(f > 0){
          edge[nowe].flow += f;
          edge[nowe^1].flow -= f;
          return f;
        }
      }
    }
    return 0;
  }
  int dinic(){
    int ans = 0,f;
    // printf("dinic begin\n");
    while(bfs()){
      while((f = dfs(S))>0)
        ans += f;
    }
    // printf("dinic:%d\n",ans);
    return ans;
  }
}
int n,m,CNT;

char s[MAXN][MAXN];

void init(){
  scanf("%d %d",&n,&m);
  for(int i = 0;i<n;i++){
    scanf("%s",s[i]);
    for(int j = 0;j<m;j++){
      if(s[i][j] == '.') CNT++;
    }
  }
}

int _hash(pair<int,int> x){return x.first * m + x.second;}
int _hash(int x,int y){return x * m + y;}
pair<int,int> rhash(int w){return make_pair(w/m,w%m);}

queue<int> q;
int dis[MAXN*MAXN];
int mov[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
vector<pair<int,int> > e[MAXN*MAXN]; // e[hash]-> tot 

bool cmp(pair<int,int> a,pair<int,int> b){
  return a.second < b.second;
}

pair<int,int> move(pair<int,int> now,int dir){
  return make_pair(now.first+mov[dir][0],now.second+mov[dir][1]);
}

void bfs0(int x,int y,int lim){
  while(!q.empty()) q.pop();
  q.push(_hash(x,y));
  memset(dis,0x3f,sizeof(dis));
  dis[_hash(x,y)] = 0;
  while(!q.empty()){
    pair<int,int> nown = rhash(q.front());q.pop();
    if(s[nown.first][nown.second] == 'D'){
      e[_hash(nown)].push_back(make_pair(_hash(x,y),dis[_hash(nown)]));
      // printf("add:%d %d\n",_hash(x,y),dis[_hash(nown)]);
    }
    for(int i = 0;i<4;i++){
      pair<int,int> nex = move(nown,i);
      if(nex.first < 0 || nex.first >= n || nex.second < 0 || nex.second >= m) continue;
      if(s[nex.first][nex.second] == 'X') continue;

      if(dis[_hash(nex)] > 1e9){
        dis[_hash(nex)] = dis[_hash(nown)] + 1;
        q.push(_hash(nex));
      }
    }
  }
}

bool check(int t){
  MaxFlow::clear();
  for(int i = 0;i<n;i++)
    for(int j = 0;j<m;j++)
      e[_hash(i,j)].clear();
  int tcnt = n*m;
  MaxFlow::S = tcnt++,MaxFlow::T = tcnt++;
  for(int i = 0;i<n;i++)
    for(int j = 0;j<m;j++)
      if(s[i][j] == '.') bfs0(i,j,t);
  //build graph
  // fuck;
  for(int i = 0;i<n;i++){
    for(int j = 0;j<m;j++){
      if(s[i][j] == '.'){
        MaxFlow::addedge(MaxFlow::S,_hash(i,j),1);
      }
      if(s[i][j] == 'D'){
        sort(e[_hash(i,j)].begin(),e[_hash(i,j)].end(),cmp);
        int last = -1,tmp = 0;
        for(int x = 1;x<=t;x++){
          int now = tcnt++;
          while(tmp < int(e[_hash(i,j)].size()) && e[_hash(i,j)][tmp].second <= x){
            int from = e[_hash(i,j)][tmp].first;
            // printf("%d\n",from);
            MaxFlow::addedge(from,now,1);
            tmp++;
          }
          MaxFlow::addedge(last,now,inf);
          MaxFlow::addedge(now,MaxFlow::T,1);
          last = now;
        }
      }
    }
  }
  return MaxFlow::dinic() >= CNT;
}

void solve(){
  int L = 1,R = 1001;
  while(L!=R){
    int mid = (L+R-1)/2;
    // printf("L:%d R:%d mid:%d\n",L,R,mid);
    
    if(check(mid))
      R = mid;
    else
      L = mid+1;
    // usleep(10000);
  } 
  if(L >= 1000)
    printf("impossible");
  else
    printf("%d\n",L);
}

int main(){
  init();
  solve();
  return 0;
}