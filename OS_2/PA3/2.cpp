#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
#define print(x) cout<<x<<'\n'
#define printe(x) cout<<x<<endl
#define prints(x) cout<<x<<" "
#define repi(i,a,b) for (int i=a;i<b;i++)
#define repl(i,a,b) for (ll i=a;i<b;i++)
#define maxVec(v) *max_element(v.begin(),v.end())
#define minVec(v) *min_element(v.begin(),v.end())
#define sortVec(v) sort(v.begin(),v.end())
#define bpcnt(a) __builtin_popcount(a)
#define bpcntll(a) __builtin_popcountll(a)
inline ll lsb(ll n) {return n&-n;}
inline ll msb(ll n) {return (1 << (31 - __builtin_clz(n)));}

string currentTime(tm *t)
{
    string str(asctime(t));
    str.erase(str.end() - 1);
    return str;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    srand(time(0));
    default_random_engine random_number_generator;

    exponential_distribution<double> delay_1(1 / 0.4);
    for (int i = 0; i < 4; i++) {
    double t1 = delay_1(random_number_generator);
    cout<<t1<<endl;
    }
    return 0;
}