#include"../../generator.h"
#include<bits/stdc++.h>
using namespace std;
using namespace generator::all;

int main()
{
	init_gen();
	compare(
		1,                
		4,               // test for case 1 ~ case 4
		1000,            // time limit 1000ms
		lcmp,            // use checker lcmp.exe
		"./ac.exe", 
		"./wa.exe",
		"./tle.exe",
		"./tle_ac.exe",
		"./tle_wa.exe"); 
	return 0;
}
