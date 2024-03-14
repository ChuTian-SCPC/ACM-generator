### 无权树

树在设置好各个属性后，使用函数`void gen()`完成生成一棵树。

可以通过下面函数拿到边和根：

```cpp
int root();
std::vector <Edge> edge();
```

树的通用属性是：

```cpp
int node;                     //树的结点数
int begin_node;               //树结点的起始编号，默认是1
bool is_rooted;               //是否是有根树，默认是无根树(false)
int root;                     //根,在有根树的情况下有效
std::vector <Edge> edge;      //边
bool output_node;             //是否输出结点数，默认是true
bool output_root;             //是否输出边，默认是true
bool swap_node;               //边的两个结点是否可能交换前后位置，默认对有根树是false，对无根树是true
TreeGenerator tree_generator; //树的生成方式
```

结点的起始编号是指第一个结点的编号。比如一棵大小为 $node$ 的树， 结点编号是从  $begin\_node$  到    $begin\_node + node - 1$  。默认值都是 $1$ 。



根在**设置**的时候要注意它的范围是在结点编号之内的，尤其是在结点数和起始结点编号变化之后，需要考虑根是否在一个合法范围内。

比如原本有一棵树的起始编号是 $1$ ，根也是 $1$ ，现在将他的起始编号改成 $2$ ，那么根就不在树结点编号的范围内了，会造成生成失败。可以看[相关示例](../../../examples/tree_root.cpp)。



`swap_node`的值是与边储存的相关。

在有根树上面比较重要，因为它在为`false`的时候，存储的边永远是`father son`的格式。而设置为`true`之后就会随机采用`father son`和`son father`的格式。

由于它影响的是边存储本身，所以如果在`gen()`函数之后修改它的值将不会影响数据和输出。



`output_node`和`output_root`都是输出相关概念。

树的输出重载了运算符`ostream &operator<<`，它输出的格式是：

```
结点数node 根root（如果是有根树的才会出现）
边1
边2
……
边node-1
```

`output_node`控制是否输出结点数。

`output_root`控制对于有根树是否输出根。

**注意** ：输出的最后一行没有换行。也可以通过函数`println()`实现输出该树并换行。

树的所有属性都可以通过`set_xxx()`实现修改。

```cpp
void set_node(int node);
void set_is_rooted(int is_rooted);
void set_root(int root);
void set_begin_node(int begin_node);
void set_output_node(bool output_node);
void set_output_root(bool output_root);
void set_swap_node(bool swap_node);
```


[返回上级](./summary.md)

[返回目录](../../home.md)
