## 对拍

使用参考示例题目[A+B problem](../../../examples/problemsA+B/compare.cpp)

测评的结果会在工作目录下新建一个`cmp`文件放入。

测评的样例是从工作目录下的`testcases`文件夹获取的。关于如何生成测试样例，请参考[生成标准输入](./make_inputs.md)和[生成标准输出](./make_outputs.md)。

每个对拍程序会根据程序名生成相应的文件夹存放程序输出`.ans`文件。

在`testcases`文件夹下也会生成对应程序名的`log`文件。

真实运行时间是时间限制的 $2$ 倍。在时间限制的 $1.5$ 倍内会被checker检查是否正确。

测评结果为：

AC 答案正确

WA 答案错误

TLE 超时 $1.5$ 倍以上

TLE(AC) 超时 $1.5$ 倍之内，并且答案正确

TLE(WA) 超时 $1.5$ 倍之内，并且答案错误

ERROR 程序运行错误

**注意**：出现`ERROR`可能要多试几次，并报告给开发者。

默认的checker是枚举类`Checker`，采用了`testlib`中一些常用的checker。

```cpp
enum Checker {
    lcmp,  // lcmp.exe
    yesno, // yesno.exe
    rcmp4, // rcmp4.exe
    rcmp6, // rcmp6.exe
    rcmp9, // rcmp9.exe
    wcmp   // wcmp.exe
}
```

常用的checker可执行文件放在`checker`文件夹的中对应系统(windows/linux)的文件夹目录下。

对拍的函数有四种：

```cpp
void compare(int start,int end,int time_limit,Checker checker,Args ... args);
void compare(int start,int end,int time_limit,std::string checker,Args ... args);
void compare(int start,int end,int time_limit,const char* checker,Args ... args);
void compare(int start,int end,int time_limit,Path checker,Args ... args);
```

函数唯一的区别在于使用的判题器一个是默认的checker；其余是自定义checker的路径，自定义的路径可以是`const char*`,`std::string`或者`generator::io::Path`类。

`generator::io::Path`类的介绍可以查看：[路径(Path)](./path.md)。

其余参数的含义是：对 $start$ 和 $end$ 的样例测试，时间限制为 $time\textunderscore limit(ms)$ 。对拍的程序路径为 $args$，可以对**多个**程序同时对拍。



[返回目录](../../home.md)
