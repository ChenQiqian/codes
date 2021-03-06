#include <cstdio>
#include <algorithm>
using namespace std;

struct wupin{
    int id,h;
}w[100100];

struct splay_t{
    struct node_t{
        int val,size;
        bool rev;
        node_t *p,*son[2],**root,**null;
        inline bool get_p(){return p->son[1] == this;}
        inline void init(node_t **root,node_t **null){this->null = null,this->root = root;}
        inline int lsize(){return son[0]->size;} inline int rsize(){return son[1]->size;}
        inline void update(){size = lsize()+rsize()+1;}
        inline void link(node_t *dst,int tmp){
            dst->son[tmp] = this;
            this->p = dst;
        }
        inline void push_down(){
            if(this == *null) return;
            if(rev){
                swap(son[0],son[1]);
                son[0]->rev^=1,son[1]->rev^=1;
                rev = 0;
            }
        }
        inline void rotate(){
            p->push_down(),push_down();
            node_t *rp = p;bool re = get_p();
            link(rp->p,rp->get_p());
            son[1-re]->link(rp,re);
            rp->link(this,1-re);
            rp->update(),update();
            if(p == *null) *root = this;
        }
        inline node_t* splay(node_t *tar = NULL){
            if(tar == NULL) tar = *null;
            p->push_down(),push_down();
            while(p!=tar){
                if(p->p == tar)
                    rotate();
                else if(p->get_p() == get_p())
                    p->rotate(),rotate();
                else
                    rotate(),rotate();
            }
            return this;
        }
        inline node_t *upper(){
            splay();
            node_t *r = son[1];
            while(true){
                r->push_down();
                if(r->son[0] == *null) break;
                r = r->son[0];
            }
            return r;
        }
    };
    int cnt;
    node_t pool[110000],*to[110000],*null,*root;
    splay_t(){
        cnt = 0;
        null = newnode();
        null->size = 0;
    }
    node_t *newnode(int val = 0){
        node_t *r = &pool[cnt++];
        r->init(&root,&null);
        r->val = val,r->size = 1,r->rev = 0;
        r->p = r->son[0] = r->son[1] = null;
        return r;
    }
    node_t* build(wupin *a,int l,int r){
        if(l>r) return null;
        int mid = (l+r)>>1;
        node_t *ret = newnode(a[mid].h);
        to[a[mid].h] = ret;
        build(a,l,mid-1)->link(ret,0);
        build(a,mid+1,r)->link(ret,1);
        ret->update();
        return ret;
    }
    void print(node_t *r = NULL,int depth = 0){
        if(r == NULL) r = root;
        if(r == null) return;
        else{
            print(r->son[0],depth+1);
            for(int i = 0;i<depth;i++) putchar(' ');
            printf("v:%d,size:%d,rev?:%d,son:%d %d,depth:%d\n",r->val,r->size,r->rev,r->son[0]!=null,r->son[1]!=null,depth);
            print(r->son[1],depth+1);
        }
    }
};

splay_t s;

int n;

bool cmp1(wupin a,wupin b){
    return a.id<b.id;
}

bool cmp2(wupin a,wupin b){
    if(a.h!=b.h) return a.h<b.h;
    else         return a.id<b.id;
}


void find(){
    for(int i = 1;i<=n;i++){
        //找到应操作节点的后继
        splay_t::node_t *rbound = s.to[i]->upper();
        //把前面归位好的最后一个旋到根节点
        s.to[i-1]->splay();
        //把后继旋转到根节点的右儿子
        rbound->splay(s.to[i-1]);
        //打印位置
        printf("%d ",i + rbound->lsize()-1);
        //添加lazy标记
        rbound->son[0]->rev^=1;
    }
}

int main(){
    scanf("%d",&n);
    for(int i = 1;i<=n;i++){
        scanf("%d",&w[i].h);
        w[i].id = i;
    }
    //预处理
    sort(w+1,w+n+1,cmp2);
    for(int i = 1;i<=n;i++)
        w[i].h = i;
    sort(w+1,w+n+1,cmp1);
    w[0].h = 0;
    w[n+1].h = n+1;
    //正式操作
    s.root = s.build(w,0,n+1);
    find();
    return 0;
}
