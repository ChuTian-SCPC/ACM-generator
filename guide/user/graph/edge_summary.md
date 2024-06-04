## 边

边由`class Edge<T>`进行存储，分成无权与有权两种类型。

边权的类型为`<T>`，无权边不用显式地指定边权为`<void>`。

在无权、带点权的树图中为无权边，在带边权、带点权和边权的树图中为带权边。

可以通过函数获取边的结点以及修改，`const`的目的在于重载输出的时候可能需要用到：

```cpp
int u() const                      //获取第一个结点u
int v() const                      //获取第二个结点v
T w() const                        //有权边获取权值w

int& u_ref() 
int& v_ref() 
T& w_ref() 

std::tuple<int, int> edge() const   //无权边获取由tuple包裹的u,v
std::tuple<int, int ,T> edge() const //有权边获取由tuple包裹的u,v,w

void set_u(int u)              //设置第一个结点u
void set_v(int v)              //设置第二个结点v
void set_w(T w)                //设置边权w
```

边都可以通过重载的`std::ostream& <<`输出，也可以通过`println`输出。区别在于`std::ostream& <<`不会换行，而`println`会换行，即等价于`std::cout<<edge<<std::endl;`。

可以通过`void set_output_default(bool swap_node = false)`设置默认的输出格式。

在`swap_node = false`的时候，无权边输出`u v`，有权边输出`u v w`。这也是默认的输出方式。

在`swap_node = true`的时候，无权边输出`v u`， 有权边输出`v u w`。

也可以通过`void set_output(std::function<void(std::ostream&, const Edge<T>&))`自行定义输出方式。

**注意**：边的输出方式每一条都是独立的！对一条边的设置仅对它自己生效。

**注意**：示例中第四条边重载输出的命名空间在一些编译器中需要指明，而一些不需要。我也不知道其中的原因。

[相关示例](../../../examples/print_edge.cpp)

[返回上级](./summary.md)

[返回目录](../../home.md)
