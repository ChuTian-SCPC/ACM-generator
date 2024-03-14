## unweight::HeightTree

生成一个指定高度的树。

构造函数是：
```cpp
HeightTree(
    int node = 1, 
    int begin_node = 1, 
    int root = 1, 
    int height = -1
);
```
**注意** ： 这种树必须为有根树，树的高度以根节点为 $1$ 开始计算。

`void set_is_rooted(bool is_rooted)` 函数被禁用。

可以通过函数`void set_height(int height)`来设置树的高度。

如果不设置，`height`的默认值为 $-1$ ，树的高度采取随机一个树高。

除此之外，也可以在随机生成的时候指定树高：

1. `void gen(int height);`

    生成高度为hegiht的树。

2. `void gen(T l_limit, U r_limit);`

    生成高度在[l_limit, r_limit]之间的树。

3. `void gen(const char *format, ...);`

    生成高度在指定范围内的函数。
    指定范围的格式详细看[rand_int](../rand/rand_int.md)中的格式。

[示例代码1](../../../examples/unweight_height_tree1.cpp)

生成一棵 $5$ 个结点的有根树，结点起始编号为 $1$ ，根为 $2$ , 高度为 $3$ 。

[示例代码2](../../../examples/unweight_height_tree2.cpp)

生成一棵 $5$ 个结点的有根树， 高度在 $[3,4]$ 之间。