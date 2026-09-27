#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

struct FlowStats { int phases=0; long long scans=0,value=0; };

// Instrumented full-BFS/current-arc Dinic, used only for generator selection.
FlowStats measureFlow(int n,int m,int h,const vector<array<long long,8>> &a) {
    struct Edge { int to,rev; long long cap; };
    int k=n*m*h,s=7*k,t=s+1;
    vector<vector<Edge>> g(t+1);
    auto add=[&](int u,int v,long long c) {
        g[u].push_back({v,(int)g[v].size(),c});
        g[v].push_back({u,(int)g[u].size()-1,0});
    };
    long long inf=1;
    for(auto &cell:a) for(auto v:cell) inf+=v;
    for(int x=0;x<n;++x) for(int y=0;y<m;++y) for(int z=0;z<h;++z) {
        int u=(x*m+y)*h+z,p=(x+y+z)&1;
        add(s,u,a[u][p]); add(u,t,a[u][p^1]);
        for(int d=1;d<=3;++d) {
            int v=k+6*u+2*(d-1),w=v+1;
            add(s,v,a[u][2*d+p]); add(w,t,a[u][2*d+(p^1)]);
            add(v,u,inf); add(u,w,inf);
            for(int dx=-1;dx<=1;++dx) for(int dy=-1;dy<=1;++dy) for(int dz=-1;dz<=1;++dz) {
                int xx=x+dx,yy=y+dy,zz=z+dz;
                if(dx*dx+dy*dy+dz*dz!=d || xx<0 || xx>=n || yy<0 || yy>=m || zz<0 || zz>=h) continue;
                int q=(xx*m+yy)*h+zz;
                add(v,q,inf); add(q,w,inf);
            }
        }
    }
    FlowStats result;
    vector<int> level(t+1),cur(t+1);
    auto dfs=[&](auto &&self,int u,long long f)->long long {
        if(u==t) return f;
        for(int &i=cur[u];i<(int)g[u].size();++i) {
            ++result.scans;
            Edge &e=g[u][i];
            if(e.cap && level[e.to]==level[u]+1) {
                long long pushed=self(self,e.to,min(f,e.cap));
                if(pushed) { e.cap-=pushed; g[e.to][e.rev].cap+=pushed; return pushed; }
            }
        }
        return 0;
    };
    while(true) {
        fill(level.begin(),level.end(),-1);
        queue<int> q; q.push(s); level[s]=0;
        while(!q.empty()) {
            int u=q.front(); q.pop();
            for(auto &e:g[u]) {
                ++result.scans;
                if(e.cap && level[e.to]<0) level[e.to]=level[u]+1,q.push(e.to);
            }
        }
        if(level[t]<0) break;
        ++result.phases;
        fill(cur.begin(),cur.end(),0);
        while(long long pushed=dfs(dfs,s,inf)) result.value+=pushed;
    }
    return result;
}

// generator <mode:1..25> <seed>; one invocation emits ONE test (no T).
struct Case {
    int n, m, h;
    vector<array<long long, 8>> a;
    Case(int n_, int m_, int h_) : n(n_), m(m_), h(h_), a(n*m*h) {}
};
Case randomShape(int low, int high) {
    vector<array<int, 3>> shapes;
    for (int n=1;n<=high;++n) for(int m=1;n*m<=high;++m)
        for(int h=1;n*m*h<=high;++h) if(n*m*h>=low) shapes.push_back({n,m,h});
    auto s=shapes[rnd.next((int)shapes.size())];
    return Case(s[0],s[1],s[2]);
}
// b is the state AFTER the checkerboard change of variables.
void setReward(Case &t,int u,int d,int b,long long value) {
    int x=u/(t.m*t.h),y=u/t.h%t.m,z=u%t.h;
    t.a[u][2*d+(b^((x+y+z)&1))]=value;
}
void randomValues(Case &t,int style) {
    for(auto &cell:t.a) for(auto &v:cell) {
        if(style==0) v=rnd.next(0,1000000000);
        if(style==1) v=rnd.next(0,3);
        if(style==2) v=rnd.next(0,4)?0:rnd.next(1,1000000000);
        if(style==3) v=1000000000-rnd.next(0,100);
        if(style==4) v=rnd.next(0,1)?rnd.next(0,10):rnd.next(999999990,1000000000);
    }
}
FlowStats score(const Case &t) { return measureFlow(t.n,t.m,t.h,t.a); }
void stress(Case &t,int kind) {
    // Tight cluster rewards and conflicting unary preferences cause residual
    // rerouting. Select candidates by deterministic operation counts, not time.
    for(int u=0;u<(int)t.a.size();++u) {
        int x=u/(t.m*t.h),y=u/t.h%t.m,z=u%t.h;
        int axis=t.n>1?x:t.m>1?y:z;
        for(int d=0;d<4;++d) for(int b=0;b<2;++b)
            setReward(t,u,d,b,d==0?(b==axis%2?50+axis:0):rnd.next(0,30));
    }
    FlowStats initial=score(t),best=initial;
    Case bestCase=t;
    auto key=[&](const FlowStats &f) {
        return kind==0?make_pair((long long)f.phases,f.scans):make_pair(f.scans,(long long)f.phases);
    };
    for(int step=0;step<900;++step) {
        Case candidate=bestCase;
        if(step%150==149) randomValues(candidate,1);
        else {
            int changes=step%5==0?rnd.next(2,12):1;
            for(int j=0;j<changes;++j) {
                int u=rnd.next((int)t.a.size()),k=rnd.next(8),op=rnd.next(4);
                auto &v=candidate.a[u][k];
                if(op==0) v=0;
                if(op==1) v=rnd.next(0,200);
                if(op==2) v=max(0LL,min(1000LL,v+rnd.next(-20,20)));
                if(op==3) swap(v,candidate.a[u][k^1]);
            }
        }
        FlowStats s=score(candidate);
        if(key(s)>=key(best)) best=s,bestCase=std::move(candidate);
    }
    t=std::move(bestCase);
    cerr<<"stress initial_phases="<<initial.phases<<" initial_scans="<<initial.scans
        <<" final_phases="<<best.phases<<" final_scans="<<best.scans<<'\n';
}
int main(int argc,char **argv) {
    registerGen(argc,argv,1);
    ensuref(argc==3,"Usage: generator mode seed");
    int mode=opt<int>(1);
    ensuref(1<=mode && mode<=25,"mode must be in [1,25]");
    Case t(1,1,1);
    if(mode==1) t.a[0]={3,5,100,200,1000,2000,50,60};
    else if(mode==2) {
        t=Case(2,2,2);
        for(auto &v:t.a) v={1,1,0,0,10,0,0,0};
    } else if(mode<=12) {
        const int shapes[10][3]={{1,1,1},{1,1,1},{100,1,1},{1,100,1},{1,1,100},
                               {10,10,1},{10,1,10},{1,10,10},{4,5,5},{5,5,4}};
        auto s=shapes[mode-3]; t=Case(s[0],s[1],s[2]);
        if(mode==4 || mode==6 || mode==8 || mode==10 || mode==12)
            for(auto &v:t.a) v.fill(1000000000);
        else if(mode==7) {
            for(int u=0;u<100;++u) for(int d=0;d<4;++d) setReward(t,u,d,0,1000000000);
        } else if(mode==9) {
            for(int u=0;u<100;++u) t.a[u][4+(u&1)]=1000000000;
        } else if(mode==11) {
            t.a.front()[6]=1000000000; t.a.back()[7]=1000000000;
        }
    } else if(mode==13) { t=randomShape(1,12); randomValues(t,rnd.next(0,4)); }
    else if(mode<=18) { t=randomShape(90,100); randomValues(t,mode-14); }
    else if(mode<=21) {
        t=randomShape(96,100);
        for(int u=0;u<(int)t.a.size();++u) {
            int x=u/(t.m*t.h),y=u/t.h%t.m,z=u%t.h;
            for(int d=0;d<4;++d) for(int b=0;b<2;++b) {
                long long v=0;
                if(mode==19) v=d==0?(b==((x+y+z)&1)?1000000000:0):rnd.next(0,1000000000);
                if(mode==20) v=d==((x+2*y+3*z)%3+1)?rnd.next(0,1000000000):0;
                if(mode==21) v=b==0?rnd.next(1,1000000000):0;
                setReward(t,u,d,b,v);
            }
        }
    } else {
        if(mode==22) {
            int axis=rnd.next(3); t=Case(axis==0?100:1,axis==1?100:1,axis==2?100:1);
        }
        if(mode==23) t=Case(2,5,10);
        if(mode==24) t=Case(1,10,10);
        if(mode==25) t=Case(4,5,5);
        stress(t,mode==22 || mode==24?0:1);
    }
    ensuref(t.n>=1 && t.m>=1 && t.h>=1 && t.n*t.m*t.h<=100,"invalid shape");
    for(auto &cell:t.a) for(auto v:cell) ensuref(0<=v && v<=1000000000,"invalid energy");
    cout<<t.n<<' '<<t.m<<' '<<t.h<<'\n';
    for(int k=0;k<8;++k) for(int x=0;x<t.n;++x) {
        if(k || x) cout<<'\n';
        for(int y=0;y<t.m;++y) {
            for(int z=0;z<t.h;++z) cout<<(z?" ":"")<<t.a[(x*t.m+y)*t.h+z][k];
            cout<<'\n';
        }
    }
}
