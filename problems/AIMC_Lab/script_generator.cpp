#include <bits/stdc++.h>
using namespace std;
int main() {
    mt19937 g(114514);
    if(!freopen("script.txt","w",stdout)) return 1;
    auto emit=[&](int mode) {
        cout<<"generator "<<mode<<' '<<(g()%(INT_MAX-1u)+1u)<<" > $\n";
    };
    for(int mode=1;mode<=12;++mode) emit(mode);
    for(int i=0;i<6;++i) emit(13);
    for(int mode=14;mode<=18;++mode) for(int i=0;i<10;++i) emit(mode);
    for(int mode=19;mode<=21;++mode) for(int i=0;i<4;++i) emit(mode);
    for(int mode=22;mode<=25;++mode) for(int i=0;i<5;++i) emit(mode);
}
