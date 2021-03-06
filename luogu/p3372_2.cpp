#include <cstdio>
#include <cstdlib>
#define ll long long
#define lson (nown<<1)
#define rson (nown<<1|1)
using namespace std;

ll n,m;

ll addn[500000],chengn[500000],sumn[500000];

inline ll qr(){
    ll f = 1,s = 0;char ch = getchar();
    while(ch<='0'||ch>'9'){
        if(ch == '-')
            f = -1;
        ch = getchar();
    }
    while(ch>='0'&&ch<='9'){
        s = (s<<3)+(s<<1)+ch-48;
        ch = getchar();
    }
    return f*s;
}

void push_down(int nown,int l,int r){
    if(chengn[nown]!=1||addn[nown] !=0){
        int mid = (l+r)>>1;        
        chengn[lson] *= chengn[nown];chengn[rson] *= chengn[nown];
        addn[lson] *= chengn[nown];addn[rson] *= chengn[nown];
        sumn[lson] *= chengn[nown];sumn[rson] *= chengn[nown];
        
        addn[lson] += addn[nown];  addn[rson] += addn[nown];
        sumn[lson] += addn[nown]*(mid-l+1);
        sumn[rson] += addn[nown]*(r-mid);
        
        /*chengn[lson]%=p;chengn[rson]%=p;
        addn[lson]%=p;addn[rson]%=p;
        sumn[lson]%=p;sumn[rson]%=p;
        */
        addn[nown] = 0;
        chengn[nown] = 1;
    }
}

void push_up(int nown){
    sumn[nown] = sumn[lson] + sumn[rson];
    //sumn[nown] %= p;
}

void build(int nown,int l,int r){
    addn[nown] = 0;
    chengn[nown] = 1; 
    if(l == r){
        sumn[nown] = qr();
        //sumn[nown] %= p;
    }
    else{
        int mid = (l+r)>>1;
        build(lson,l,mid);
        build(rson,mid+1,r);
        push_up(nown);
    }
}

ll query(int nown,int l,int r,int ql,int qr){
    if(ql<=l&&r<=qr)
        return sumn[nown];//%p;
    else{
        int mid = (l+r)>>1;
        push_down(nown,l,r);
        ll res = 0;
        if(ql<=mid)
            res+=query(lson,l,mid,ql,qr);
        //res %= p;
        if(mid+1<=qr)
            res+=query(rson,mid+1,r,ql,qr);
        //res %= p;
        return res;
    }
}

void cheng(ll val,int nown,int l,int r,int ql,int qr){
    if(ql<=l&&r<=qr){
        chengn[nown]*=val;
        addn[nown] *= val;
        sumn[nown] *= val;
        //sumn[nown]%=p;
    }
    else{
        int mid = (l+r)>>1; 
        push_down(nown,l,r);
        if(ql<=mid)
            cheng(val,lson,l,mid,ql,qr);
        if(mid+1<=qr)
            cheng(val,rson,mid+1,r,ql,qr);
        push_up(nown);
    }
}

void add(ll val,int nown,int l,int r,int ql,int qr){
    if(ql<=l&&r<=qr){
        addn[nown]+=val;
        sumn[nown]+=(val*(r-l+1));
        //sumn[nown]%=p;
    }
    else{
        int mid = (l+r)>>1; 
        push_down(nown,l,r);
        if(ql<=mid)
            add(val,lson,l,mid,ql,qr);
        if(mid+1<=qr)
            add(val,rson,mid+1,r,ql,qr);
        push_up(nown);
    }
}


int main(){
    scanf("%lld %lld",&n,&m);
    build(1,1,n);
    for(int i = 0;i<m;i++){
        int f,a,b,c;
        scanf("%d",&f);
        switch(f){
            case 1:
                scanf("%d %d %d",&a,&b,&c);
                add(c,1,1,n,a,b);
                break;
            case 3:
                scanf("%d %d %d",&a,&b,&c);
                add(c,1,1,n,a,b);
                break;
            case 2:
                scanf("%d %d",&a,&b);
                printf("%lld\n",query(1,1,n,a,b));
                break;
        }
    }
    //system("pause");
    return 0;
}