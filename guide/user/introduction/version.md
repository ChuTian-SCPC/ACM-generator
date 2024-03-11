[返回目录](../../home.md)

### version 0.5.0
2024-3-5
- 修复bugs：
  - 修正在Linux上生成input数据的相关bug。
  - 修正compare中标准输出和程序输出位值相反的bug。
  - 修正了VS编译时类型错误。
- 重构：
  - 重构一个rand_sum函数，使他生成的数列更加均衡。
- 新增功能：
  - rand_int：新增可以通过解析字符串随机。
  - fill_inputs / fill_inputs_exe：生成未出现的num个.in
  - fill_outputs / fill_outputs_exe：生成所有.out
  - rand_odd：生成随机奇数。
  - rand_even：生成随机偶数。
  - shuffle_index：根据传入的数列$a$，生成一个整数数列$b$，满足数字$i$出现的次数为$a_{i-offset}$。
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
    - PseudoOutTree：基环树。
    - Cactus：仙人掌。
  

### version 0.3.0
2023-9-25
- 项目雏形，支持以下功能：
  - rand_int：生成随机整数。
  - rand_real：生成随机浮点数。
  - rand_abs：生成随机数字x满足|x|在给定的约束范围内。
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