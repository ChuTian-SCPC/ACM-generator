### 点

**注意**：类的含义是点权。

由`NodeWeight<T>`进行存储，分成无权与有权两种类型。

权值的类型为`<T>`，无权不用显式地指定为`<void>`。

在无权、带边权的树图中为无权边，在带点权、带点权和边权的树图中为带权边。

可以通过函数获取点的权值和修改，`const`的目的在于重载输出的时候可能需要用到：

```cpp
T &w()                         //获取权值w
T cw() const
void set_w(T w)                //设置边权w
```

点权都可以通过重载的`std::ostream& <<`输出，也可以通过`println`输出。区别在于`std::ostream& <<`不会换行，而`println`会换行，即等价于`std::cout<<node<<std::endl;`。

可以通过`void set_output_default()`设置默认的输出格式。

默认格式为`w`。

也可以通过`void set_output(std::function<void(std::ostream&, const Edge<T>&))`自行定义输出方式。

**注意**：点权的输出方式每一条都是独立的！对每个点权的设置仅对它自己生效。

[相关示例](../../../examples/print_node.cpp)

[返回上级](./summary.md)

[返回目录](../../home.md)