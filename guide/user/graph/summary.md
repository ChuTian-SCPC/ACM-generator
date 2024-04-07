## 简介

树和图都在命名空间`generator::graph`中。在`generator::all`中已经被包括。

树和图分成四种，无权，带点权，带边权，带点权和边权。

分别在以下命名空间中：

1. `generator::graph::unweight`：无权
2. `generator::graph::node_weight`：带点权
3. `generator::graph::edge_weight`：带边权
4. `generator::graph::both_weight`：带点权和边权

对于点和边详见：

- [点简介](./node_summary.md)

- [边简介](./edge_summary.md)

- [树简介](./tree_summary.md)

    - [树(Tree)](./unweight_tree.md)
    - [链(Chain)](./unweight_chain.md)
    - [菊花图(Flower)](./unweight_flower.md)
    - [指定高度的树(HeightTree)](./unweight_height_tree.md)
    - [限制最大度数的树(DegreeTree)](./unweight_degree_tree.md)
    - [限制最大儿子数的树(SonTree)](./unweight_son_tree.md)

- [无权图简介](./unweight_graph_summary.md)
    - [图(Graph)](./unweight_graph.md)
    - [二分图(BipartiteGraph)](./unweight_bipartite_graph.md)
    - [有向无环图(DAG)](./unweight_dag.md)
    - [环图(CycleGraph)](./unweight_cycle_graph.md)
    - [轮图(WheelGraph)](./unweight_wheel_graph.md)
    - [网格图(GridGraph)](./unweight_grid_graph.md)
    - [基环树(PseudoTree)](./unweight_pseudo_tree.md)
    - [基环内向树(PseudoInTree)](./unweight_pseudo_in_tree.md)
    - [基环外向树(PseudoOutTree)](./unweight_pseudo_out_tree.md)
    - [仙人掌(Cactus)](./unweight_cactus.md)

[返回目录](../../home.md)

