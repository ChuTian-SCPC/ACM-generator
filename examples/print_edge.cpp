#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;
void e3_output_format(ostream&os, const edge_weight::Edge<double>& edge)
{
	os << edge.cu() << " --> " << edge.cv() << " : "<<edge.cw();
}
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
    unweight::Edge e1(1, 2);
    cout<<"unweighted edge 1: ";
    cout<<e1<<endl;
	node_weight::Edge e2(3, 4);
	e2.set_output_default(true);
	cout<<"unweighted edge 2: ";
	e2.println();
	edge_weight::Edge<double> e3(5, 6, 3.14);
	e3.set_output(e3_output_format);
	cout<<"weighted edge 3: ";
	e3.println();
	both_weight::Edge<pair<int,char>> e4(7, 8, make_pair(9, 'a'));
	cout<<"weighted edge 4: ";
	cout<<e4<<endl;
    return 0;
}
/*
output:
unweighted edge 1: 1 2
unweighted edge 2: 4 3
weighted edge 3: 5 --> 6 : 3.14
weighted edge 4: 7 8 9 a
*/
