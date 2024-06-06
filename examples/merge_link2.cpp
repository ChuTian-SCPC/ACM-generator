#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

void output_graph_setting(unweight::Graph graph) {
    cout<<boolalpha;
    cout<<"direction     :"<<graph.direction()<<endl;
    cout<<"multiply edge :"<<graph.multiply_edge()<<endl;
    cout<<"self loop     :"<<graph.self_loop()<<endl;
    cout<<"connect       :"<<graph.connect()<<endl;
}

int main()
{
    init_gen();
    unweight::Graph graph1(3,5);
    graph1.set_multiply_edge(true);
    graph1.gen();

    unweight::Graph graph2(3,6);
    graph2.set_self_loop(true);
    graph2.gen();

    unweight::Graph setting_graph;
    
    unweight::Graph merged_graph1 = unweight::merge(graph1, graph2);
    cout<<"merged graph 1 settings :"<<endl;
    output_graph_setting(merged_graph1);
    cout<<endl;

    unweight::Graph merged_graph2 = unweight::merge(graph2, graph1);
    cout<<"merged graph 2 settings :"<<endl;
    output_graph_setting(merged_graph2);
    cout<<endl;

    unweight::Graph merged_graph3 = unweight::merge(setting_graph, graph1, graph2);
    cout<<"merged graph 3 settings :"<<endl;
    output_graph_setting(merged_graph3);

    return 0;
}
/*
output:
WARN Ignore 3 edge(s) due to the graph's attribute-based conditions.
merged graph 1 settings :
direction     :false
multiply edge :true
self loop     :false
connect       :false

WARN Ignore 2 edge(s) due to the graph's attribute-based conditions.
merged graph 2 settings :
direction     :false
multiply edge :false
self loop     :true
connect       :false

WARN Ignore 5 edge(s) due to the graph's attribute-based conditions.
merged graph 3 settings :
direction     :false
multiply edge :false
self loop     :false
connect       :false
*/
