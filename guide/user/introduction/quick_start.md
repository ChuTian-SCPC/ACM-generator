## 快速入门

1、在使用前请确保：

   C++编译器支持C++11及以上版本。

2、引入头文件：

将`testlib.h`,`generator.h`和文件夹`checker`放入同一个目录下。

放在标准库位置或工作目录均可。

更新时候替换相关文件即可以更新到最新版。

在代码中使用命令空间：`using namespace generator::all`;

命令空间`generator::all`中包括以下命令空间，如需单独使用可以自行选择：

```cpp
using namespace generator::rand;
using namespace generator::io;
using namespace generator::graph;
using namespace generator::geometry;
```

3、简单了解和掌握如何用testlib.h出题。

4、简单例子：[A+B问题](../../../examples/problemsA+B/)

制作输入输出数据：

```cpp
#include"generator.h"
#include<bits/stdc++.h>
using namespace std;
using namespace generator::all;
void gen_1() {
	cout<<rand_int(1, 100)<<" "<<rand_int(1, 100)<<endl;
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
	make_inputs(1,2,gen_1);            // make 1.in 2.in
	int a=3,b=4;
	make_inputs(4,4,[a, b](){cout<<a<<' '<<b<<endl;}); // make 4.in
	fill_inputs(1,gen_3, "-opt=10"); // make 3.in
	fill_outputs(std_code);          // make 1.out 2.out 3.out 4.out
	//make_outputs(1,4,std_code);     // the same as fill_outputs(std_code());
	return 0;
}
```

对拍：

```cpp
#include"generator.h"
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

```


[返回目录](../../home.md)
