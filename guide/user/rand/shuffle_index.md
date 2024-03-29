## shuffle_index

1. `std::vector<int> shuffle_index(Iter begin, Iter end, int offset = 0)`
   传入两个类型为`Iter`的参数 $be$ 和一个类型为 `int` 的参数 $offset$ , 默认值为 $0$ 。

   `Iter`的类型为迭代器，且需要满足：
    
    - `Iter` 必须是输入迭代器。
    - 两个迭代器必须指向同一容器内的合法元素范围。
    - `Iter` 所指的值需为整型。

   返回一个数组类型为`vector<int>`，长度为 $begin$ 到 $end$ 的值的总和。

   在位置 $pos$ 的值表示 $pos - begin + offset$ 在数组中出现的次数。

   比如对于：
   ```cpp
    vector<int> a = {3, 1, 2};
    vector<int> b = shuffle_index(a);
    ```
    在数组 $b$ 中， $0$ 会出现 $3$ 次； $1$ 会出现 $1$ 次； $2$ 会出现 $2$ 次。

2. `vector<int> shuffle_index(vector<int> v, int offset = 0)`
   传入一个类型为`vector<int>`的数组 $v$ 和一个类型为 `int` 的参数 $offset$ , 默认值为 $0$ 。

   等同于函数：`shuffle_index(v.begin(), v.end(), offset)` 。
   
[示例代码](../../../examples/shuffle_index.cpp)

[返回目录](../../home.md)


