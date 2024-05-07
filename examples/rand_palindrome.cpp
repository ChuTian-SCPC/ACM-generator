#include<bits/stdc++.h>
#include"../generator.h"
using namespace std;
using namespace generator::all;

int main()
{
	init_gen();
	for (int i = 1; i <= 10; i++)
	{
		string s = rand_palindrome(10, i);
		cout<<s<<endl;
	}
	return 0;
}
/*
output:
wydamtcugd
rwpyyvinaw
vgnmnbttzt
zsfeefftfn
zxccumucvq
mkpxxpkdhn
cfaudaduau
pmgjjgmpkb
jedkjkdejt
xniqhhqinx
*/