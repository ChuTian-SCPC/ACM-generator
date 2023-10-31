<div align="center">
    <h1>Generator</h1>
</div>

[![version](https://img.shields.io/badge/version-v0.3.0-blue)](https://github.com/ChuTian-SCPC/problem_tool)
[![license](https://img.shields.io/badge/license-MIT-green)](https://github.com/ChuTian-SCPC/problem_tool/blob/main/LICENSE)

[中文文档](https://github.com/ChuTian-SCPC/problem_tool/wiki/CN_Home)

[English Wiki](https://github.com/ChuTian-SCPC/problem_tool/wiki/EN_Home)

## Generator是什么

Generator是基于testlib.h库封装的一个出题辅助工具库。（当然它现在功能已经不局限于生成数据了）

用来帮助Acmer更快的出题和对拍。

[testlib.h](https://github.com/MikeMirzayanov/testlib)是Codeforces出题辅助工具。

Generator可以方便的生成测试数据。

Generator可以方便的在本地进行对拍，内置testlib中常见的checker。

## 如何使用Generator

除对拍之外的功能需要支持C++11。对拍需要本地能够支持命令`python`(Windows)。

将`testlib.h`，`generator.h`，文件夹`checker`放入同一个目录下。放在**标准库位置**或**工作目录**均可。

## 开源协议

MIT协议

## 功能实现

**由于所有的测试均在Windows上面完成，所以在Linux上可能会存在较多的问题。**

- [x] 基础数据生成函数

- [x] 多组输入输出数据生成

- [ ] 对拍

  - [x] 支持Windows

  - [ ] 支持Linux

- [ ] 树和图生成函数


