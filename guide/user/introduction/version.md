[返回目录](../../home.md)

### development

- 重构：
  - 让`rand_int`,`rand_odd`,`rand_even`,`rand_real`和`rand_abs`用`format`的函数支持泛型。
  - 将头文件拆分。
  - 取消接口`make_inputs_exe`,`make_inputs_seed`,`fill_inputs_exe`,`fill_inputs_seed`,`make_outputs_exe`,`fill_outputs_seed`。所有相关接口统一成`make_inputs`,`fill_inputs`,`make_outputs`,`fill_outputs`。
  - 命令行参数不直接作为函数传参，而是通过构造CommandPath,CommandFunc类。
  - 树，图，几何生成器采用策略模式。
  - 重构`Link`,`TreeLink`和`Forest`。
  - 重命名`DegreeTree`为`MaxDegreeTree`。
  - 重命名`SonTree`为`MaxSonTree`。
- 新增功能：
  - 用`rand_bool`取代`rnd.next(2)`和`rnd.next(0,1)`。
  - 给`rand_even`和`rand_odd`添加泛型支持。
  - Line：直线，Segment：线段。
  - Triangle：三角形。
  - 支持Windows log颜色。
  - 对多次相同的warn log只输出一次。
  - 支持validator验证生成数据合法性。
  - 支持hack。
  - SimplePolygon : 简单多边形。
- 修复bugs：
  - 修复树、图输出时存在多余空格的问题。
  - 修复了多次打开文件可能出现的问题。
  - 修复了判断字符串是否是小数形式的错误。
  - 修正一些格式错误。

### version 0.8.0

2024-8-7

- 修复bugs：
  - 禁用二分图相关set函数。
  - 修复有权图在设置`connect = true`的会出现生成错误。
  - 修正用户手册中一些错误。
  - 实现用户手册中提及但忘记实现的函数`Edge<T>::edge()`。
  - 修复`rand_even`使参数为负数的时候正确。
  - 修复`rand_int(format)`在有传入浮点数形式的时候正确。
- 重构：
  - 采用`node_indices`表示结点输出时的值。
  - 变量名返回值更改为const常量，原数据引用采用`ref()`。
  - `rand_int(T, U)`采用`is_convertible<T(U), long long>`。
  - `rand_int`支持`testlib`不支持的`unsigned`整型。`rand_odd`和`rand_even`也一并支持。
- 新增功能：
  - 有根树添加重定根函数`reroot`。
  - 给make inputs, make outputs添加一些函数，compare测试所有的样例。
  - 树/图的合并：merge（不额外加边），link（额外加边）。
  - FlowerChain：菊花带链。
  - Forest：森林。
  - compare可以设置无限时间。
  - rand_bracket_seq：随机合法括号序列。
  - 计算几何：
    - Point：点。
    - ConvexHull：凸包。

### version 0.7.0

2024-5-15

- 修复bugs：
  - 修复制作输出的bug。
  - 修复用户手册中的部分bug。
- 重构：
  - 重构`msg`和`io`模块，将它们合并成一个`namespace io`。
  - Windows对拍抛弃使用python。
  - 重构`Path`，统一内部的路径参数类型。
  - 重构`make_inputs_exe`,`fill_inputs_exe`,`make_outputs_exe`,`fill_outputs_exe`部分，使文件路径为可传入的参数。
  - 重构`msg`中输出颜色部分。
  - 修改默认checker的路径。
  - 修改`rand_vector`，让生成函数传参由模板类`FUNC`改成`std::function<T()>`，增加类型检查。
  - 修改`make_inputs`,`make_outputs`,`fill_inputs`,`fill_outputs`,将他们由宏修改成函数。
- 新增功能：
  - 已经支持Linux对拍。
  - 带权树和图的生成函数。
  - rand_palindrome：随机回文串。

### version 0.5.1

2024-3-14

- 修复bugs：
  - 修正在Linux上生成input数据的相关bug。
  - 修正compare中标准输出和程序输出位值相反的bug。
  - 修正了VS编译时类型错误。
  - 修正`rand_real`的精度范围问题。
  - 修正`rand_abs(T, U)`函数在`T`和`U`均为整型的时候返回整数。 
  - 有根树 `set_root` 时不正确的warning。
  - 修复了二分图初始化存在的一些问题。
- 其他 ：
  - 用户文档更新完成。
  - 给一些函数加上新的边界判定。
  - 修改warning。
- 重构：
  - 重构一个rand_sum函数，使他生成的数列更加均衡。
- 新增功能：
  - rand_int：新增可以通过解析字符串随机。
  - fill_inputs / fill_inputs_exe：生成未出现的若干个.in
  - fill_outputs / fill_outputs_exe：生成所有.out
  - rand_odd：生成随机奇数。
  - rand_even：生成随机偶数。
  - shuffle_index：根据传入的数列 $a$ ，生成一个整数数列 $b$ ，满足数字 $i$ 出现的次数为 $a_{i-offset}$ 。
  - 新增无权树和图的生成函数，支持：
  
    - Tree：随机的树。
    - Chain：链。
    - Flower：菊花。
    - HeightTree：限制树的高度为height的树。
    - DegreeTree：限制树的每个结点度数不超过max degree的树。
    - SonTree：限制有根树的每个结点的儿子数不超过max son的树。
    - Graph：随机的图。
    - BipartiteGraph：二分图。
    - DAG：有向无环图。
    - CycleGraph：环图。
    - WheelGraph：轮图。
    - GridGraph：网格图。
    - PseudoTree：基环树。
    - PseudoInTree：基环内向树。
    - PseudoOutTree：基环外向树。
    - Cactus：仙人掌。
  

### version 0.3.0
2023-9-25
- 项目雏形，支持以下功能：
  - rand_int：生成随机整数。
  - rand_real：生成随机浮点数。
  - rand_abs：生成随机数字 $x$ 满足 $|x|$ 在给定的约束范围内。
  - rand_char：生成随机字符。
  - rand_string：生成随机字符串。
  - rand_p：生成随机排列。
  - rand_sum：生成随机整数数列值的和为sum。
  - rand_vector：生成随机数列。
  - rand_prob：生成一个值，每种值出现的概率由传入决定。
  - make_inputs：根据std函数，生成编号从start到end的.in。
  - make_inputs_exe：根据std的exe文件，成编号从start到end的.in。
  - make_outputs：根据data函数，生成编号从start到end的.out。
  - make_outputs_exe：根据data的exe文件，生成编号从start到end的.out。
  - init_gen：testlib相关初始化。
  - compare：对拍（只支持Windows）。

[返回目录](../../home.md)