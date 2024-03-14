### 无权图

图在设置好各个属性后，使用函数`void gen()`完成生成一个图。

可以通过下面函数拿到边：
```cpp
std::vector <Edge> edge();
```

图的通用属性是：
```cpp
int node;                    //图的结点数
int side;                    //图的边数
int begin_node;              //图结点的起始编号，默认是1
std::vector <Edge> edge;     //边
bool direction;              //是否有向，默认无向(false)
bool multiply_edge;          //是否存在重边，默认不存在(false)
bool self_loop;              //是否存在自环，默认不存在(false)
bool connect;                //是否一定连通，默认不一定(false)
bool swap_node;              //边的两个结点是否可能交换前后位置,默认对有向图是false，对无向图true
bool output_node;            //是否输出结点数，默认是true
bool output_side;            //是否输出边数，默认是true
```

结点的起始编号是指第一个结点的编号。比如一个大小为 $node$ 的图，结点编号是从  `begin_node`  到    `begin_node + node - 1`  。默认值都是 $1$ 。

`swap_node`的值是与边储存的相关。

如果是`false`，边永远是`from to`，而`true`则随机采用`from to`和`to from`的格式。

由于它影响的是边存储本身，所以如果在`gen()`函数之后修改它的值将不会影响数据和输出。



`output_node`和`output_side`都是输出相关概念。

图的输出重载了运算符`ostream &operator<<`，它输出的格式是：

```
结点数node 边数side
边1
边2
……
边side
```

`output_node`控制是否输出结点数。

`output_side`控制是否输出边数。

**注意** ：输出的最后一行没有换行。也可以通过函数`println()`实现输出该树并换行。

图的所有属性都可以通过`set_xxx()`实现修改。
```cpp
void set_node(int node);
void set_side(int side);
void set_begin_node(int begin_node);
void set_direction(bool direction);
void set_multiply_edge(bool multiply_edge);
void set_self_loop(bool self_loop);
void set_connect(bool connect);
void set_swap_node(bool swap_node) ;
void set_output_node(bool output_node);
void set_output_side(bool output_side);

//和点数相关的也可以用
void set_node_count(int count);
void set_output_node_count(bool output_node_count);

//和边数相关的也可以用
void set_side_count(int count);
void set_edge_count(int count);
void set_output_side_count(bool output_side_count);
void set_output_edge_count(bool output_edge_count);

```

[返回上级](./summary.md)

[返回目录](../../home.md)
