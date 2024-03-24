## Path

`Path`类是封装了一下路径的类。

构造函数有:

```cpp
Path();
Path(std::string s);
Path(const char *s);
```

可以通过以下函数获得路径：

```cpp
std::string path();
const char* cname();
```

可以通过以下函数修改路径：

```cpp
void change(std::string s);
void change(const char *s);
void change(Path other_path);
```

另外两个比较有用的函数是：

1. `void full()`

   使路径成为绝对路径。

2. `Path join(const T &*arg*, const Args &... *args*)`

   返回一个路径，路径为当前路径和给出的参数拼接而成。

   **注意**：原本的路径并不会改变。

   ```cpp
   Path a("./folder1");
   Path b = a.join("folder2","file1.txt");
   Path c = a.join("file2.txt");	
   
   // 执行之后
   // a 的路径依然是 ./folder1
   // b 的路径是 ./folder1/folder2/file1.txt
   // c 的路径是 ./folder1/file2.txt
   ```



[返回目录](../../home.md)
