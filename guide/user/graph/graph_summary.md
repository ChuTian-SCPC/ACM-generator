### 图

#### 基本用法

有点权的图点权类型为`NodeType`，有边权的图边权类型为`EdgeType`，均为泛型。

四种类型分别为：

```cpp
unweight::Graph;                         //无权
node_weight::Graph<NodeType>;            //带点权
edge_weight::Graph<EdgeType>;            //带边权
both_weight::Graph<NodeType,EdgeType>;   //带点权和边权
```

图在设置好各个属性后，使用函数`void gen()`完成生成一个图。

使用`std::ostream& <<`能够按照默认方式输出一个图，默认输出的格式符合大部分题目的格式。

#### 成员变量

##### 含义

图的通用属性是：
```cpp
int node_count;              //图的结点数
int edge_count;              //图的边数
int begin_node;              //图结点的起始编号，默认是1
//边
std::vector <Edge<EdgeType>> edge;     
//点权，如果存在才会有
std::vector <NodeWeight<NodeType>> nodes_weight;
bool direction;              //是否有向，默认无向(false)
bool multiply_edge;          //是否存在重边，默认不存在(false)
bool self_loop;              //是否存在自环，默认不存在(false)
bool connect;                //是否一定连通，默认不一定(false)
bool swap_node;              //边的两个结点输出时是否可能交换前后位置,默认对有向图是false，对无向图true
bool output_node_count;      //是否输出结点数，默认是true
bool output_edge_count;      //是否输出边数，默认是true
```

结点的起始编号是指第一个结点的编号。比如一棵大小为 $node\textunderscore count$ 的树， 结点编号是从   $begin\textunderscore node$   到     $begin\textunderscore node + node\textunderscore count - 1$   。默认值都是 $1$ 。

注意对于有向图，`connect`指的是弱连通。

`swap_node`的值是与边输出的相关。

如果是`false`，边输出永远是`from to`，而`true`则随机采用`from to`和`to from`的格式输出。

**注意**：在修改`direction`的属性的时候会它会随之修改。



`output_node_count`和`output_edge_count`都是输出相关概念，详细见输出板块。


`node_indices`表示的含义是每个结点对应输出和返回边中点的编号。

默认的情况都是从 `begin_node` 到 ` begin_node + node_count-1`，由这两个属性决定的。

**注意**：在一般情况下并不需要关注此变量。

它也可以自定义，不过优先级较低，在`node_count`和`begin_node`改变的到时候都会重新初始化。

##### 获取

变量获取分成可以修改（引用）和`const`的版本。

```cpp
int node_count() const;
int edge_count() const;
bool swap_node() const;
int beign_node() const;
bool direction() const;
bool multiply_edge() const;
bool self_loop() const;
bool connect() const;
std::vector<int> node_indices() const;
std::vector<Edge<EdgeType>> edges() const;
std::vector<NodeWeight<NodeType>> nodes_weight() const;//如果有点权的话

int& node_count_ref();
int& edge_count_ref();
bool& swap_node_ref();
int& beign_node_ref();
bool& direction_ref();
bool& multiply_edge_ref();
bool& self_loop_ref();
bool& connect_ref();
std::vector<int>& node_indices_ref();
std::vector<Edge<EdgeType>>& edges_ref();
std::vector<NodeWeight<NodeType>>& nodes_weight_ref();//如果有点权的话
```

##### 修改

```cpp
void set_node_count(int node_count);
void set_edge_count(int edge_count);
void set_begin_node(int begin_node);
void set_direction(bool direction);
void set_multiply_edge(bool multiply_edge);
void set_self_loop(bool self_loop);
void set_connect(bool connect);
void set_swap_node(bool swap_node) ;
void set_output_node_count(bool output_node_count);
void set_output_edge_count(bool output_side_count);
void set_node_indices(std::vector<int> node_indices);
void set_node_indices(int index, int number);
```



#### 权值随机函数

`NodeGenFunction`和`EdgeGenFunction`分别是点权和边权的随机生成函数。

使用方式可以查看[示例代码](../../../examples/tree1.cpp)。

函数是无传参，返回值为`NodeType`和`EdgeType`的。

```cpp
typedef std::function<NodeType()> NodeGenFunction; 
typedef std::function<EdgeType()> EdgeGenFunction;
```

可以通过以下函数设置：

```cpp
void set_nodes_weight_function(NodeGenFunction nodes_weight_function);
void set_edges_weight_function(EdgeGenFunction edges_weight_function);
```

可以通过以下函数获取：

```cpp
NodeGenFunction nodes_weight_function();
EdgeGenFunction edges_weight_function();
```


#### 输出

可以通过`void set_output_default()`设置默认的输出格式。

默认的输出的格式是：

```
结点数node_count 边数edge_count
点权1 点权2 …… 点权node_count（如果树有点权才会出现）
边1
边2
……
边edge_count
```

`output_node_count`控制是否输出结点数。

`output_edge_count`控制是否输出边数。

也可以通过`void set_output(std::function<void(std::ostream&, const Graph<NodeType, EdgeType>&)>)`自行定义输出方式。

树的输出可以通过重载的`std::ostream& <<`输出，也可以通过`println`输出。区别在于`std::ostream& <<`不会换行，而`println`会换行，即等价于`std::cout<<graph<<std::endl;`。



[返回上级](./summary.md)

[返回目录](../../home.md)
