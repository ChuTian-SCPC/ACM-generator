## unweight::DAG

生成一个有向无环图(DAG)。

构造函数是：
```cpp
DAG(
    int node = 1, 
    int side = 0, 
    int begin_node = 1
);
```

DAG是有向无环的图，所以以下函数被禁用：
```cpp
void set_direction(bool direction);
void set_self_loop(bool self_loop);
```

[示例代码](../../../examples/unweight_dag.cpp)：
生成一棵 $5$ 个结点, $6$ 条边的DAG。

[返回上级](./summary.md)

[返回目录](../../home.md)