### 树

#### 基本用法

有点权的树点权类型为`NodeType`，有边权的树边权类型为`EdgeType`，均为泛型。

四种类型分别为：

```cpp
unweight::Tree;                         //无权
node_weight::Tree<NodeType>;            //带点权
edge_weight::Tree<EdgeType>;            //带边权
both_weight::Tree<NodeType,EdgeType>;   //带点权和边权
```

树在设置好各个属性后，使用函数`void gen()`完成生成一棵树。

使用`std::ostream& <<`能够按照默认方式输出一棵树，默认输出的格式符合大部分题目的格式。

#### 成员变量

##### 含义

树的通用属性是：

```cpp
int node_count;               //树的结点数
int begin_node;               //树结点的起始编号，默认是1
bool is_rooted;               //是否是有根树，默认是无根树(false)
int root;                     //根,在有根树的情况下有效
//边
std::vector <Edge<EdgeType>> edge;     
//点权，如果存在才会有
std::vector <NodeWeight<NodeType>> nodes_weight;
bool output_node_count;       //是否输出结点数，默认是true
bool output_root;             //是否输出边，默认是true
bool swap_node;               //边的两个结点输出时是否可能交换前后位置，默认对有根树是false，对无根树是true
TreeGenerator tree_generator; //树的生成方式
```

结点的起始编号是指第一个结点的编号。比如一棵大小为 $node\textunderscore count$ 的树， 结点编号是从 $ begin\textunderscore node $   到 $ begin\textunderscore node+node\textunderscore count-1 $   。默认值都是 $1$ 。



根在**设置**的时候要注意它的范围是 $[1, node\textunderscore count]$ ，表示根是第几个结点。

比如原本有一棵树的 $node\textunderscore count$  是 $5$ ，设置它的根是 $1$ ，可以正确的生成。现在将他的根改成 $6$ ，那么根就超出结点的个数了，会造成生成失败。可以看[相关示例](../../../examples/tree_root.cpp)。



`swap_node`的值是与边输出的相关。

在有根树上面比较重要，因为它在为`false`的时候，边的输出永远是`father son`的格式。而设置为`true`之后就会随机采用`father son`和`son father`的格式。

**注意**：在修改`is_rooted`的属性的时候会它会随之修改。



`output_node_count`和`output_root`都是输出相关概念，详细见输出板块。

##### 获取

变量获取分成可以修改（引用）和`const`的版本。

根`root`的情况比较特殊，由于它在类中存储的范围是`[0,node\textunderscore count - 1]`，所以函数`int& root()`的返回值就是这个值。而`int croot() const`的返回值就是根的实际编号的值。

比如一棵树有 $5$ 个结点，起始结点的编号是 $2$ ，设置根是第 $4$ 个结点。那么`int& root()`的返回值就是`3`，而`int croot() const`的返回值就是`5`。

```cpp
int& node_count();
int& root();
int& beign_node();
std::vector<Edge<EdgeType>>& edges();
std::vector<NodeWeight<NodeType>>& nodes_weight();//如果有点权的话

int cnode_count() const;
int croot() const;
int cbeign_node() const;
std::vector<Edge<EdgeType>> cedges() const;
std::vector<NodeWeight<NodeType>> cnodes_weight() const;//如果有点权的话
```



##### 修改

```cpp
void set_node_count(int node_count);
void set_is_rooted(int is_rooted);
void set_root(int root);
void set_begin_node(int begin_node);
void set_output_node(bool output_node);
void set_output_root(bool output_root);
void set_swap_node(bool swap_node);
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



#### 输出

可以通过`void set_output_default()`设置默认的输出格式。

默认的输出的格式是：

```
结点数node_count 根root（如果是有根树的才会出现）
点权1 点权2 …… 点权node_count（如果树有点权才会出现）
边1
边2
……
边node_count-1
```

`output_node_count`控制是否输出结点数。

`output_root`控制对于有根树是否输出根。

也可以通过`void set_output(std::function<void(std::ostream&, const Tree&))`自行定义输出方式。

树的输出可以通过重载的`std::ostream& <<`输出，也可以通过`println`输出。区别在于`std::ostream& <<`不会换行，而`println`会换行，即等价于`std::cout<<tree<<std::endl;`。

#### 其他函数

##### 有根树重定根(reroot)

对于所有有根树，都可以用函数`void reroot(int root)`在树的生成之后进行重定根。

使用方式可以查看[示例代码](../../../examples/reroot.cpp)。


[返回上级](./summary.md)

[返回目录](../../home.md)
