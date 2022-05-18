#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "dllist.h"
#define INFINITIVE 1200
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
//JRB edges(id,JRB(id,JRB(03A,trongso))) 6-7
// JRB id(Ben xe Gia Lam,id)
int dem = -1;
Graph createGraph();
int addVertex(Graph, int, char *);
char *getVertex(Graph, int);
void addEdge(Graph, int, int, char *);
int hasEdge(Graph, int, int);
int indegree(Graph, int, int *);
int outdegree(Graph, int, int *);
int DAG(Graph);
void dropGraph(Graph);
void addVertexFromFile(Graph g, char *filename);
void addEdge_str(Graph, char *, char *, char *);
int getId(Graph g, char *name);
int BFS(Graph g, char *s, char *f, int *rank);
int getPath(Graph g, int end, int rank[], int *paths[]);
int toiUu(Graph g, int path[], int size);
JRB getWeight(Graph g, int v1, int v2);
int getValue(JRB tree, char *key);
int getMin(JRB tree);
void printPath(Graph, int *, int);
char *getBusNameMin(JRB, char *);
int hasVertex(Graph g, char *name);

void menu(void)
{
    printf("\t\t\t|======================================================|\n");
    printf("\t\t\t|%25sMENU%25s|\n", " ", " ");
    printf("\t\t\t|------------------------------------------------------|\n");
    printf("\t\t\t|  1  |   %-45s|\n", "Tim duong di");
    printf("\t\t\t|  2  |   %-45s|\n", "Ket thuc chuong trinh");
    printf("\t\t\t|======================================================|\n");
}
int main()
{
    int chose;
    int rank[INFINITIVE], end;
    int i, total, output[20];
    int *paths[10], sum, *toiuu, min;
    char str1[50], str2[50];
    Graph graph = createGraph();
    JRB root;
    do
    {
        dem = -1;
        addVertexFromFile(graph, "station.txt");
        menu();
        printf("Hay nhap lua chon cua ban: ");
        while (!scanf("%d", &chose))
        {
            printf("Nhap sai: Khong duoc nhap chu!\nChi duoc nhap so tu %d - %d\n", 1, MAX_CHOSE);
            menu();
            printf("Hay nhap lua chon cua ban: ");
            __fpurge(stdin);
        }
        __fpurge(stdin);
        switch (chose)
        {
        case 1:
            printf("Nhap ten cua diem dau: ");
            fgets(str1, 50, stdin);
            str1[strlen(str1) - 1] = 0;
            if (!hasVertex(graph, str1))
            {
                printf("Khong ton tai diem buyt %s\n", str1);
                goto ext;
            }
            printf("Nhap ten cua diem den: ");
            fgets(str2, 50, stdin);
            str2[strlen(str2) - 1] = 0;
            if (!hasVertex(graph, str2))
            {
                printf("Khong ton tai diem buyt %s\n", str2);
                goto ext;
            }
            end = BFS(graph, str1, str2, rank); // duyet cay theo chieu rong
            if (rank[end] == INFINITIVE)
            {
                printf("khong ton tai duong di");
                goto ext;
            }
            // printf("ID: %d\nName: %s\nRank: %d\n", end, getVertex(graph, end), rank[end]);
            for (i = 0; i < 10; i++)
            {
                paths[i] = (int *)malloc(sizeof(int) * (rank[end] + 1));
                for (int j = 0; j < rank[end]; j++)
                {
                    paths[i][j] = -1;
                }
                paths[i][rank[end]] = end;
            }
            sum = getPath(graph, end, rank, paths); //lay duong di it diem nhat
            toiuu = (int *)malloc(sizeof(int) * (sum + 1));

            for (i = 0; i <= sum; i++)
            {
                for (int j = 0; j <= rank[end]; j++)
                {
                    printf("%s%c", getVertex(graph, paths[i][j]), j == rank[end] ? '\n' : '-');
                }
            }

            for (i = 0; i <= sum; i++)
            {
                toiuu[i] = toiUu(graph, paths[i], rank[end]); // chon it tuyen duong nhat tren tuyen duong vuaw lay
                // printf("Lan doi: %d\n", toiuu[i]);
                if (!i)
                    min = i;
                else if (toiuu[i] < toiuu[min])
                    min = i;
            }
            toiUu(graph, paths[min], rank[end]);
            printPath(graph, paths[min], rank[end]);
            for (i = 0; i < 10; i++)
                free(paths[i]);
            free(toiuu);
        ext:
            break;
        case 2:
            printf("\tKet thuc chuong trinh\n");
            dropGraph(graph);
            chose = 0;
            break;
        default:
            printf("Chi duoc chon cac so tu %d - %d!!!\n", 1, MAX_CHOSE);
            chose = 1;
            break;
        }
    } while (chose);
    return 0;
}
int hasVertex(Graph g, char *name)
{
    int find = getId(g, name);
    if (find != INFINITIVE)
    {
        return 1;
    }
    return 0;
}
void printPath(Graph g, int path[], int size)
{
    JRB node;
    char name[50] = "";
    for (int i = 0; i < size; i++)
    {
        node = getWeight(g, path[i], path[i + 1]);
        strcpy(name, getBusNameMin(node, name));
        printf("Ben: \t\t%-45sBAT Tuyen buyt:   %s\n", getVertex(g, path[i]), name);
    }
    printf("Ben cuoi: \t%-30s\n", getVertex(g, path[size]));
}
char *getBusNameMin(JRB tree, char *key)
{
    int min = 1000, i;
    char *rt;
    JRB root;
    jrb_traverse(root, tree)
    {
        if (strcmp(jval_s(root->key), key) == 0)
        {
            rt = key;
            goto trave;
        }
        i = jval_i(root->val);
        if (min > i)
        {
            min = i;
            rt = jval_s(root->key);
        }
    }
trave:
    return rt;
}
JRB getWeight(Graph g, int v1, int v2) /*  */
{
    JRB node = (JRB)(jval_v(jrb_find_int(g->edges, v1)->val));
    return (JRB)(jval_v(jrb_find_int(node, v2)->val));
}
int getValue(JRB tree, char *key)
{
    JRB find = jrb_find_str(tree, key);
    if (!find)
        return 0;
    return jval_i(find->val);
}
int getMin(JRB tree)
{
    JRB root;
    int min = 100;
    int n;
    jrb_traverse(root, tree)
    {
        n = jval_i(root->val);
        if (min > n)
            min = n;
    }
    return min;
}
int toiUu(Graph g, int path[], int size)
{
    JRB root, node, next;
    int val;
    for (int i = size - 2; i >= 0; i--)
    {
        node = getWeight(g, path[i], path[i + 1]); //cac tuyen xe qua 2 diem i i+1
        // cac tuyen xe giua 2 diem tiep theo
        next = getWeight(g, path[i + 1], path[i + 2]);
        jrb_traverse(root, node)
        {
            val = getValue(next, jval_s(root->key));
            if (val)
                (root->val).i = val;
            else
                (root->val).i = getMin(next) + 1;
        }
    }
    return getMin(node);
}
int getPath(Graph g, int end, int rank[], int *paths[])
{
    int input[20];
    int i;
    if (!rank[end])
    {
        paths[dem + 1][rank[end]] = end; //p[0][0]=a
        return ++dem;
    }
    int total = indegree(g, end, input);
    for (int j = 0; j < total; j++)
    {
        if (rank[input[j]] == rank[end] - 1)
        {
            i = getPath(g, input[j], rank, paths);
            for (int k = i; k >= 0; k--)
            {
                if (paths[k][rank[input[j]]] >= 0)
                    break;
                paths[k][rank[input[j]]] = input[j];
            }
        }
    }
    return dem;
}
int BFS(Graph g, char *s, char *f, int *rank)
{
    Dllist list;
    list = new_dllist();
    int b = getId(g, s);
    int e = getId(g, f);
    int r, total, n;
    char visited[INFINITIVE];
    int output[100];
    memset(visited, 0, sizeof(visited));
    rank[b] = 0;
    rank[e] = INFINITIVE;
    dll_append(list, new_jval_i(b));
    visited[b] = TRUE;
    while (!dll_empty(list))
    {
        r = jval_i(dll_first(list)->val);
        dll_delete_node(dll_first(list));
        if (rank[r] > rank[e])
        {
            goto here;
        }
        total = outdegree(g, r, output);
        for (int i = 0; i < total; i++)
        {
            n = output[i];
            if (visited[n])
                continue;
            dll_append(list, new_jval_i(n));
            rank[n] = rank[r] + 1;
            visited[output[i]] = TRUE;
        }
    }
here:
    return e;
}
int getId(Graph g, char *name)
{
    JRB node = jrb_find_str(g->id, name);
    if (node)
    {
        return jval_i(node->val);
    }
    else
        return INFINITIVE;
}
void addEdge_str(Graph g, char *s1, char *s2, char *tuyen)
{
    int id1 = getId(g, s1);
    int id2 = getId(g, s2);
    addEdge(g, id1, id2, tuyen);
}
void addVertexFromFile(Graph g, char *filename)
{
    char str[1024];
    char so[20], so1[20], so2[20];
    char *ten;
    FILE *f;
    int i = 0, j, z = 10;
    JRB root;
    f = fopen(filename, "r");
    char ten1[1024];
    while (!feof(f) && fgets(str, 1024, f))
    {
        sscanf(str + 5, "%s", so);
        strcpy(so1, so);
        strcat(so1, " Chieu di");
        strcpy(so2, so);
        strcat(so2, " Chieu ve");
        fgets(str, 1024, f);
        ten = strtok(str, "\t");
        ten = strtok(NULL, "-");
        strcpy(ten1, ten);
        i += addVertex(g, i, strdup(ten));
        while (1)
        {
            ten = strtok(NULL, "-\n");
            if (!ten)
                break;
            i += addVertex(g, i, strdup(ten));
            addEdge_str(g, ten1, ten, strdup(so1));
            strcpy(ten1, ten);
        }
        fgets(str, 1024, f);
        ten = strtok(str, "\t");
        ten = strtok(NULL, "-");
        strcpy(ten1, ten);
        i += addVertex(g, i, strdup(ten));
        while (1)
        {
            ten = strtok(NULL, "-\n");
            if (!ten)
                break;
            i += addVertex(g, i, strdup(ten));
            addEdge_str(g, ten1, ten, strdup(so2));
            strcpy(ten1, ten);
        }
    }
    free(ten);
    fclose(f);
    // printf("Co %d diem buyt\n", i);
}
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

/* 
cau truc cac cay 

phan doc file va  insert data

thuat toan tim duong di ngan nhat  ->mangr 2 chiều gồm mỗi dòng là 1 đường đi ngắn nhất 
mỗi oo là 1 đỉnh ví dụ 


thuat toan tim tuyen xe it phai chuyen nhat

 */