#include <stdio.h>
#include <stdlib.h>

#include <string.h>
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
