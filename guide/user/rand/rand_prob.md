## rand_prob

1. `Con::key_type rand_prob(const Con& map)`
   传入一个类型为`Con`的参数, `Con`需要为`map`或者`unordered_map` , `Con`中的值必须为整型。

   返回一个值类型为`Con`中键的类型。

   `Con`中的值均需要大于等于 $0$ , 所有值的总和需要大于 $0$ 。

   `Con`中的键出现的概率为对应的值比上所有值的总和。

   比如对于：
   ```cpp
    map<char, int> m = {
      {'a', 3},
      {'b', 1},
      {'c', 6}
    };
   ```
   其中 $\text{a}$ 出现的概率为 $\frac{3}{10}$ ; $\text{b}$ 出现的概率为 $\frac{1}{10}$ ; $\text{c}$ 出现的概率为 $\frac{6}{10}$ 。

[示例代码](../../../examples/rand_prob.cpp)

[返回目录](../../home.md)
   