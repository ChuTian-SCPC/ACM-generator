<div align="center">
    <h1>Generator</h1>
</div>

[![version](https://img.shields.io/badge/version-v0.9.0_beta-blue)](https://github.com/ChuTian-SCPC/problem_tool)
[![license](https://img.shields.io/badge/license-MIT-green)](https://github.com/ChuTian-SCPC/problem_tool/blob/main/LICENSE)

你是否希望在使用[Testlib](https://github.com/MikeMirzayanov/testlib)出题时：

- 对于generator不需要写批处理脚本。

- 测试数据强度，以及针对错解随机hack数据。

- 能够方便地生成树，包括具有特殊性质的树（链、菊花图、指定高度的树、限制最大度数的树、限制最大儿子数的树、菊花带链）。

- 能够方便地生成图，包括具有特殊性质的图（二分图、有向无环图、环图、轮图、网格图、基环树、仙人掌、森林）。

- 能够方便地生成凸包、简单多边形。

Generator是基于testlib.h库封装的一个出题辅助工具库。

它实现了以上的功能，并且提供了较高的扩展性。

能够简单方便地生成数据：

```cpp
#include "generator.h"
using namespace std;
using namespace generator::all;

void gen_code() {
  int n = rand_int("[1, 1e6]");
  unweight::Tree tree(n); 
  tree.gen(); // 生成一棵n个节点的树，树没有点权和边权，节点边号为1~n
  cout << tree << endl;
}

int main() {
  fill_inputs(5, gen_code);
  return 0;
}
```

方便地进行对拍：

```cpp
#include "generator.h"
using namespace std;
using namespace generator::all;

void gen_code() {
  cout << rand_int(1, 100) << " " << rand_int(1, 100) << endl;
}

void std_code() {
  int a, b;
  cin >> a >> b;
  cout << a + b << endl;
}

int main() {
  // 用gen_code生成测试样例，std_code生成正确答案，使用lcmp作为checker。
  // 对于wa_code时间限制是1000ms，在结果不正确时，将该样例添加到测试样例。
  hack(gen_code, std_code, "wa_code.exe", lcmp, 1000);
  return 0;
}
```


## 如何使用

需要支持C++11。

将`testlib.h`，`generator.h`，文件夹`checker`放入同一个目录下。放在**标准库位置**或**工作目录**均可。

详细的使用文档见：[使用文档](https://chutian-scpc.github.io/generator-docs)。

## 开源协议

代码遵循MIT协议，欢迎使用本项目！如果您觉得这个项目对您有帮助，请给我们一个⭐️Star支持，也可以向其他出题人推荐！

## 提问&贡献

如果您在使用中遇到bug，发现问题以及有需要加入的功能需求，可以提出[Issue](https://github.com/ChuTian-SCPC/ACM-generator/issues/new)或者[Discussion](https://github.com/ChuTian-SCPC/ACM-generator/discussions/new/choose)。

您也可以对仓库fork后修正其中的问题或实现需求，提交[PR](https://github.com/ChuTian-SCPC/ACM-generator/pulls)。


