#include"../../generator.h"
#include<bits/stdc++.h>
using namespace std;
using namespace generator::all;
void gen_1() {
	cout<<rand_int(1, 100)<<" "<<rand_int(1, 100)<<endl;
}
void gen_2(int a,int b) {
	cout<<a<<" "<<b<<endl;
}
void gen_3() {
	int limit = opt<int>("opt", 1);
	cout<<rand_int(1, limit)<<" "<<limit<<endl;
}
void std_code() {
	int a,b;
	cin>>a>>b;
	cout<<a+b<<endl;
}
int main()
{
	init_gen();
	make_inputs(1,2,gen_1());          // make 1.in 2.in
	make_inputs(4,4,gen_2(3, 4));      // make 4.in
	fill_inputs(1,gen_3(), "-opt=10"); // make 3.in
	fill_outputs(std_code());          // make 1.out 2.out 3.out 4.out
	//make_outputs(1,4,std_code());    // the same as fill_outputs(std_code());
	return 0;
}
