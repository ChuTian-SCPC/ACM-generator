#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;
// these namspace for some(mostly?) compiler is unnecessary
namespace generator {
	namespace rand_graph {
		namespace basic {
			template<typename T1, typename T2>
			ostream& operator<<(ostream& os, const pair<T1, T2>& p) {
			    os << p.first << " " << p.second;
			    return os;
			}
		}
	}
}
int main()
{
    init_gen();
	node_weight::NodeWeight<double> n1(3.14);
	cout<<"weighted node 1 weight: ";
	n1.println();
	both_weight::NodeWeight<pair<int,char>> n2(make_pair(9, 'a'));
	cout<<"weighted node 2 weight: ";
	cout<<n2<<endl;
    return 0;
}
/*
output:
weighted node 1 weight: 3.14
weighted node 2 weight: 9 a
*/
