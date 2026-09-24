#include <bits/stdc++.h>
using namespace std;
using I = __int128_t;
int main(int argc, char** argv) {
    ifstream in(argv[1]);
    vector<long long> need(5001);
    for (int n=1;n<=5000;++n) in>>need[n];
    const I S=10000000000000LL;
    const I low=(S-10000)*(S-10000), high=(S+10000)*(S+10000);
    int checked=0;
    long long minSlack=LLONG_MAX, lastPairs=0;
    for (int kind=0;kind<2;++kind) {
        for (int w=1;w<=71;++w) {
            int first=max(2,(w-1)*(w-1)+1), last=min(5000,w*w);
            first=max(first,kind?100:2);
            last=min(last,kind?5000:99);
            if(first>last)continue;
            vector<pair<long long,long long>> p;
            long long count=0;
            for(int i=0;i<last;++i) {
                long long r=i/w,c=i%w;
                long long x=kind?c*2000000000000LL:c*10000000000000LL+r*5000000000000LL;
                long long y=kind?r*2000000000000LL:r*8660254037844LL;
                assert(abs(x)<=1000000000000000000LL && abs(y)<=1000000000000000000LL);
                for(auto [u,v]:p) {
                    I dx=(I)x-u,dy=(I)y-v,d=dx*dx+dy*dy;
                    assert(d>=1000000000000000000LL);
                    count+=low<=d && d<=high;
                }
                p.emplace_back(x,y);
                int n=i+1;
                if(n>=first) {
                    if(count<need[n]) {cerr<<"FAIL n="<<n<<"\n";return 1;}
                    ++checked;
                    minSlack=min(minSlack,count-need[n]);
                    if(n==5000)lastPairs=count;
                }
            }
        }
    }
    assert(checked==4999);
    cout<<"Exact all-pairs audit: "<<checked<<"/4999 sizes passed; n=5000 pairs="<<lastPairs
        <<", required="<<need[5000]<<", minimum count slack="<<minSlack<<"\n";
}
