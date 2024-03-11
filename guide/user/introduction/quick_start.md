## 快速入门

1、在使用前请确保：

  -  C++编译器支持C++11及以上版本。

  -  支持命令行指令`python`。在Windows中可以通过修改环境变量实现。

2、引入头文件：

将`testlib.h`,`generator.h`和文件夹`checker`放入同一个目录下。

放在标准库位置或工作目录均可。

更新时候替换相关文件即可以更新到最新版。

在代码中使用命令空间：`using namespace generator::all`;

命令空间`generator::all`中包括以下命令空间，如需单独使用可以自行选择：

```cpp
using namespace generator::msg;
using namespace generator::rand;
using namespace generator::io;
using namespace generator::graph;
```

3、简单了解和掌握如何用testlib.h出题。

[返回目录](../../home.md)