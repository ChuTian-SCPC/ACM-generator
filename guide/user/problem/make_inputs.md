## 生成标准输入

使用参考示例题目[A+B problem](../../../examples/problemsA+B/generator.cpp)。

生成的数据会在工作目录下新建一个`testcases`文件夹放入。

标准输入文件以`.in`结尾。

在`testlib.h`中，需要通过代码`registerGen(argc, argv, 1)`注册数据生成器。

在`generator.h`中，这一点被简化了。

1. `init_gen()`

    如果不需要传入参数，可以直接使用这个函数更简便的注册数据生成器。

2. `init_gen(int argc,char* argv[])`

   如果你想要和`testlib.h`一样，或者存在传入参数的需求，可以调用这个函数。

如果又想像`testlib.h`一样解析命令行参数，又希望能够在一个`cpp`中实现编写`generator`并批量生成，可以使用以下的宏：

1. `make_inputs(__start, __end, __Func, ...)`

   生成编号从`__start`到`__end`的标准输入文件，以`__Func`函数生成。

   后面的可选参数为传入命令行参数，支持类似`printf`中的格式控制字符串。

   

2. `fill_inputs(__num,__Func,...)`

   与第一个函数不同的方式在于，该函数是从 $1$ 开始，寻找编号最小的还没生成的 `__num`个编号生成。

如果想调用其他已经写好的数据生成器的可执行文件，可以使用以下函数：

1. `void make_inputs_exe(int start,int end,const char* format = "",...)`
2. `void fill_inputs_exe(int sum,const char* format = "",...)`



函数调用生成输入文件的数据生成器可执行文件的路径为：

工作目录下的`data.exe`(Windows)

工作目录下的`data`(Linux)

这个文件路径也可以修改。

代码中控制数据生成器文件路径的变量是`data_path`。

可以通过它们的类函数`change_path(std::string path)`或者`change_path(const char* path)`修改。

类似：`data_path.change_path("./test.exe")`。

[返回目录](../../home.md)