<div align="center">
    <h1>Generator</h1>
</div>

[![version](https://img.shields.io/badge/version-v0.6.0_beta-blue)](https://github.com/ChuTian-SCPC/problem_tool)
[![license](https://img.shields.io/badge/license-MIT-green)](https://github.com/ChuTian-SCPC/problem_tool/blob/main/LICENSE)

## Generator是什么

Generator是基于testlib.h库封装的一个出题辅助工具库。

希望它能用来帮助Acmer更快的出题和对拍。

[testlib.h](https://github.com/MikeMirzayanov/testlib)是Codeforces出题辅助工具。

Generator可以方便的生成测试数据。

Generator可以方便的在本地进行对拍，内置testlib中常见的checker。

## 如何使用Generator

需要支持C++11。

将`testlib.h`，`generator.h`，文件夹`checker`放入同一个目录下。放在**标准库位置**或**工作目录**均可。

详细的使用文档见：[使用文档](./guide/home.md)

## 开源协议

MIT协议

## 功能实现

- [x] 基础数据生成函数

- [x] 多组输入输出数据生成

- [x] 对拍

- [ ] 树和图生成函数

  - [x] 无权树和图的生成

  - [ ] 带权树和图的生成

