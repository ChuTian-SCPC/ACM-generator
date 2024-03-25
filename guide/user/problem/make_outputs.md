## 生成标准输出

使用参考示例题目[A+B problem](../../../examples/problemsA+B/generator.cpp)。

生成的数据会在工作目录下新建一个`testcases`文件夹放入。

标准输出文件以`.out`结尾。

可以通过以下宏批量生成标准输出文件。

1. `make_outputs(__start, __end, __Func)`

   生成编号从`__start`到`__end`的标准输出文件，以`__Func`函数生成。

   

2. `fill_outputs(__num,__Func)`

   与第一个函数不同的方式在于，该函数是将所有`.in`标准输入文件都生成对应的`.out`标准输出文件。

如果想调用已经写好的标答的可执行文件，可以使用以下函数：

1. `void make_outputs_exe(int start, int end, T path)`
2. `void fill_outputs_exe(T path)`



函数调用生成输出文件的标答可执行文件的路径为`path`, `path`的类型可以是`const char*`,`std::string`或者`generator::io::Path`类。

`generator::io::Path`类的介绍可以查看：[路径(Path)](./path.md)。



[返回目录](../../home.md)