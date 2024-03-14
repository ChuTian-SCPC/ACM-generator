## unweight::SonTree

生成一棵有根树，树上每个结点的儿子数不会超过限制。

构造函数是：
```cpp
DegreeTree(
    int node = 1, 
    int begin_node = 1, 
    bool is_rooted = false, 
    int root = 1, 
    int max_son = -1
);
```

**注意** ： 这种树必须为有根树。

`void set_is_rooted(bool is_rooted)` 函数被禁用。

可以通过函数`void set_max_son(int max_son)`来设置儿子数的限制。

如果不设置，`max_son`的默认值为 $-1$ ，儿子数的限制将随机生成。

除此之外，也可以在随机生成的时候指定儿子数的限制：

1. `void gen(int max_son);`

    生成儿子数限制为max_son的树。

2. `void gen(T l_limit, U r_limit);`

    生成儿子数限制在[l_limit, r_limit]之间的树。

3. `void gen(const char *format, ...);`

    生成儿子数限制在指定范围内的函数。
    指定范围的格式详细看[rand_int](../rand/rand_int.md)中的格式。

[示例代码](./unweight_degree_tree.md)

生成一棵 $10$ 个结点的有根树，每个结点的儿子数不超过 $2$ ,即一棵二叉树。

[返回上级](./summary.md)

[返回目录](../../home.md)