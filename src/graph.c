#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "btree.h"
#include <stdio_ext.h>
#include "jrb.h"
#include "dllist.h"

#define INFINITIVE 120000
#define TRUE 1
#define FALSE 0
#define MAX_CHOSE 2

typedef struct _graphics
{
    JRB edges;
    JRB vertices;
    JRB id;
} * Graph;

// JRB vertices (id,Ben xe Gia Lam)
// JRB edges(id,JRB(id,JRB(03A,trongso))) 6-7
// JRB id(Ben xe Gia Lam,id)

Graph createGraph();
int addVertex(Graph, int, char *);
char *getVertex(Graph, int);
void addEdge(Graph, int, int, char *);
int hasEdge(Graph, int, int);
int indegree(Graph, int, int *);
int outdegree(Graph, int, int *);
int DAG(Graph);
void dropGraph(Graph);

Graph createGraph()
{
    Graph graph;
    graph = (Graph)malloc(sizeof(struct _graphics));
    graph->edges = make_jrb();
    graph->vertices = make_jrb();
    graph->id = make_jrb();
    return graph;
}
int addVertex(Graph graph, int id, char *name)
{
    if (!jrb_find_str(graph->id, name))
    {
        JRB node = make_jrb();
        jrb_insert_str(graph->id, name, new_jval_i(id));
        jrb_insert_int(graph->vertices, id, new_jval_s(name));
        jrb_insert_int(graph->edges, id, new_jval_v(node));
        return 1;
    }
    return 0;
}
char *getVertex(Graph graph, int id)
{
    if (id == -1)
    {
        printf("err");
        return NULL;
    }
    return (char *)jval_v(jrb_find_int(graph->vertices, id)->val);
}
void addEdge(Graph graph, int v1, int v2, char *tuyen)
{
    JRB node = (JRB)jval_v(jrb_find_int(graph->edges, v1)->val);
    JRB node1 = jrb_find_int(node, v2);
    if (node1)
    {
        if (jrb_find_str((JRB)jval_v(node1->val), tuyen))
            return;
        jrb_insert_str((JRB)jval_v(node1->val), tuyen, new_jval_i(1));
    }
    else
    {
        JRB node2 = make_jrb();
        jrb_insert_str(node2, tuyen, new_jval_i(1));
        jrb_insert_int(node, v2, new_jval_v(node2));
    }
}
int hasEdge(Graph graph, int v1, int v2)
{
    JRB node = jrb_find_int(graph->edges, v1);
    if (!node)
    {
        return 0;
    }
    else
    {
        JRB tree = jrb_find_int((JRB)jval_v(node->val), v2);
        if (!tree)
        {
            return 0;
        }
        return 1;
    }
}
int indegree(Graph graph, int v, int *output)
{
    JRB root;
    JRB find;
    int total = 0;
    jrb_traverse(root, graph->edges)
    {
        find = jrb_find_int((JRB)jval_v(root->val), v);
        if (find)
            *(output + total++) = jval_i(root->key);
    }
    return total;
}
int outdegree(Graph graph, int v, int *output)
{
    JRB root, find;
    int total = 0;
    find = jrb_find_int(graph->edges, v);
    if (find)
    {
        JRB tree = (JRB)jval_v(find->val);
        jrb_traverse(root, tree)
        {
            *(output + total++) = jval_i(root->key);
        }
    }
    return total;
}
int DAG(Graph graph)
{
    int visited[1000];
    int n, output[100], i, u, v, start;
    Dllist node, stack;
    JRB vertex;

    jrb_traverse(vertex, graph->vertices)
    {
        memset(visited, 0, sizeof(visited));

        start = jval_i(vertex->key);
        stack = new_dllist();
        dll_append(stack, new_jval_i(start));

        while (!dll_empty(stack))
        {
            node = dll_last(stack);
            u = jval_i(node->val);
            dll_delete_node(node);
            if (!visited[u])
            {
                visited[u] = 1;
                n = outdegree(graph, u, output);
                for (i = 0; i < n; i++)
                {
                    v = output[i];
                    if (v == start)
                        return 0;
                    if (!visited[v])
                        dll_append(stack, new_jval_i(v));
                }
            }
        }
    }
    return 1;
}
void dropGraph(Graph graph)
{
    JRB root, root1, node1;
    JRB node = graph->edges;
    jrb_traverse(root, node)
    {
        node1 = (JRB)jval_v(root->val);
        jrb_traverse(root1, node1)
            jrb_free_tree((JRB)jval_v(root1->val));
    }
    jrb_free_tree(node);
    node = graph->vertices;
    jrb_traverse(root, node)
    {
        free(jval_s(root->val));
    }
    jrb_free_tree(graph->vertices);
    jrb_free_tree(graph->id);
}
