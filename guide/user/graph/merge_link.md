## 合并(merge/link)

两个图和树可以进行合并操作，分成两个类型:

一个是`merge`，合并不生成额外的边。

一个是`link`，合并并且生成`extra_edges`条边。

`merge`和`link`返回值是`Graph<NodeType, EdgeType>`。

`TreeOrGraph`表示树或者图的一种。

### MergeType/LinkType

MergeType和LinkType是相同的，只是名字不同，他们包括:

``` cpp
enum class LinkType {
    Direct,
    Increase,
    Shuffle,           
    Dedupe
};
```

`Direct`表示直接合并，`node_indices`等于第一个树或图的`node_indices`后接上第二个树或图的`node_indices`。

`Increase`表示递增地合并，`node_indices`等于从`begin_node`到`beign_node + node_count - 1`。

`Shuffle`的`node_indices`与`Increase`相同，但是点的分布会被打乱。

`Dedupe`是将`node_indices`去重，重新计算点的数量，以`node_indices`每个值第一次出现的位置作为新的`node_indices`。

区别详见[相关示例](../../../examples/merge_link1.cpp)。

有如下的接口：

```cpp
Graph<NodeType, EdgeType> merge(
    Graph<NodeType, EdgeType> setting_graph, 
    TreeOrGraph1<NodeType, EdgeType> source1, 
    TreeOrGraph2<NodeType, EdgeType> source2, 
    MergeType merge_type = MergeType::Shuffle);

Graph<NodeType, EdgeType> merge(
    TreeOrGraph1<NodeType, EdgeType> source1, 
    TreeOrGraph2<NodeType, EdgeType> source2, 
    MergeType merge_type = MergeType::Shuffle);


Graph<NodeType, EdgeType> link(
    Graph<NodeType, EdgeType> setting_graph, 
    TreeOrGraph1<NodeType, EdgeType> source1, 
    TreeOrGraph2<NodeType, EdgeType> source2, 
    int extra_edges, 
    LinkType link_type = LinkType::Shuffle);


Graph<NodeType, EdgeType> link(
    TreeOrGraph1<NodeType, EdgeType> source1, 
    TreeOrGraph2<NodeType, EdgeType> source2, 
    int extra_edges, 
    LinkType link_type = LinkType::Shuffle);
```

`merge`和`link`得到的新的图的属性可以由`setting_graph`设置。

如果不设置，那么它的属性就等同于`source1`。

如果`source1`是一个树的话，那么它的`is_rooted`决定`direction`的值，`connect`，`multiply_edge`，`self_loop`将设为默认值(`false`)。

**注意**：对于存在边权的情况下，`setting_graph`记得设置边权的生成函数，因为边权的生成函数默认值为`nullptr`。

在边的合并过程中，会考虑图的属性，将不符合的边给过滤掉。

详见[相关示例](../../../examples/merge_link2.cpp)。

除此之外，两颗树有一个特殊的`link`，就是两棵树额外加一条边形成一颗新的树。

区别在于它的`TreeLinkType`没有`Dedupe`。

它的接口为：

```cpp
Tree<NodeType, EdgeType> link(
    Tree<NodeType, EdgeType> setting_tree, 
    Tree<NodeType, EdgeType> source1, 
    Tree<NodeType, EdgeType> source2, 
    TreeLinkType link_type = TreeLinkType::Shuffle);

Tree<NodeType, EdgeType> link(
    Tree<NodeType, EdgeType> source1, 
    Tree<NodeType, EdgeType> source2, 
    TreeLinkType link_type = TreeLinkType::Shuffle);
```

详见[相关示例](../../../examples/merge_link3.cpp)

[返回上级](./summary.md)

[返回目录](../../home.md)
