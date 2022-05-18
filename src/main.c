#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <curses.h>
#include <string.h>
#include "jrb.h"
#include "dllist.h"
#include "graph.h"
int dem = -1;
int temp = 0, temp1 = 0, temp2 = 0, temp3 = 0 /* , temp4 = 0, temp5 = 0 */;
char list2[2][25] = {"Tim duong di", "Thoat"};
char item[30], s[250];
WINDOW *w, *titlew, *miw, *empty, *plist;

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

void initialise_colors()
{
    start_color();
    init_pair(9, COLOR_BLACK, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(11, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(12, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(13, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    init_pair(14, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    init_pair(15, COLOR_WHITE, COLOR_BLACK);
    init_pair(6, COLOR_BLACK, COLOR_GREEN);
    init_pair(16, COLOR_GREEN, COLOR_BLACK);
    init_pair(7, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(17, COLOR_MAGENTA, COLOR_BLACK);
}
void xoa(char s[90], int vitrixoa)
{
    int n = strlen(s);                 //gán n= độ dài của chuỗi
    for (int i = vitrixoa; i < n; i++) //duyệt chuỗi từ vị trí cần xóa
        s[i] = s[i + 1];
    s[n - 1] = '\0'; //kết thúc
}
void chuanHoaTen(char *s)
{
    for (int i = 0; i < (int)strlen(s); i++)
        if (s[i] == ' ' && s[i + 1] == ' ')
        {
            xoa(s, i);
            i--;
        }
    if (s[0] == ' ')
        xoa(s, 0);
    if (s[strlen(s) - 1] == ' ')
        xoa(s, strlen(s) - 1);
}
char nhap(char *s, char *str, int x, int y, WINDOW *xyz)
{
    char ch;
    int letter_count = 0;
    for (int i = 0; i < 50; i++)
        s[i] = ' ';
    s[50] = '\0';
    do
    {
        box(xyz, ACS_VLINE, ACS_HLINE); // sets default borders for the window
        ch = wgetch(xyz);
        s[letter_count] = ch;
        if (ch == 27)
            return ch;
        if (ch == '-')
        {
            s[letter_count] = ' ';
            (letter_count > 0) ? (letter_count = letter_count - 2) : letter_count;
            s[letter_count + 1] = ' ';
        }
        letter_count++;
        mvwprintw(xyz, x, y + strlen(str), "%s", s);
    } while ((ch != 10) && (letter_count < 50) && ch != 9);
    if (ch == 10 || ch == 9)
        s[letter_count - 1] = ' ';
    chuanHoaTen(s);
    return ch;
}
void BOX(WINDOW *x) // in ra window menu
{
    box(x, ACS_VLINE, ACS_HLINE); // sets default borders for the window
    mvwprintw(x, 9, 1, "Press <ENTER> to see the option selected");
    mvwprintw(x, 10, 1, "Up and Down arrow keys to naviage (ESC to Exit)");
    titlew = newwin(3, 70, 1, 1);
    wattrset(titlew, COLOR_PAIR(2));
    box(titlew, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    mvwprintw(titlew, 1, 1, "%27sTIM TUYEN XE BUS%28s", " ", " ");
    wattroff(titlew, COLOR_PAIR(2));
    wrefresh(titlew);
    wrefresh(x);
}

void BOXE(int a) //in ra window function
{
    empty = newwin(20, 70, 1, 1);
    box(empty, ACS_VLINE, ACS_HLINE);
    wclear(empty);
    wrefresh(empty);
    wattron(empty, A_STANDOUT);
    if (a == 2)
        sprintf(item, "%-23s", list2[temp2]);
    mvwprintw(empty, 1, 2, "%s", item);
    wattroff(empty, A_STANDOUT);
    wrefresh(empty);
}
int main()
{
    initscr(); // initialize Ncurses
    initialise_colors();
    int rank[INFINITIVE];
    long end;
    int i;
    int *paths[10], sum, *toiuu, min;
    char str1[50], str2[50];
    Graph graph = createGraph();
    dem = -1;
    addVertexFromFile(graph, "station.txt");

    int ch;
    miw = newwin(12, 70, 4, 1); // create a new window
    BOX(miw);
    // now print all the menu items and highlight the first one
    for (temp2 = 0; temp2 < 2; temp2++)
    {
        if (temp2 == 0)
            wattron(miw, A_STANDOUT); // highlights the first item.
        else
            wattroff(miw, A_STANDOUT);
        sprintf(item, "%-15s", list2[temp2]);
        mvwprintw(miw, temp2 + 1, 2, "%s", item);
    }
    wrefresh(miw); // update the terminal screen
    temp2 = 0;
    noecho();          // disable echoing of characters on the screen
    keypad(miw, TRUE); // enable keyboard input for the window.
    curs_set(0);       // hide the default screen cursor.
    while ((ch = wgetch(miw)) != 27)
    {
        // right pad with spaces to make the items appear with even width.
        sprintf(item, "%-15s", list2[temp2]);
        mvwprintw(miw, temp2 + 1, 2, "%s", item);
        // use a variable to increment or decrement the value based on the input.
        switch (ch)
        {
        case KEY_UP:
            temp2--;
            temp2 = (temp2 < 0) ? 1 : temp2;
            break;
        case KEY_DOWN:
            temp2++;
            temp2 = (temp2 > 1) ? 0 : temp2;
            break;
        case 10:
            if (temp2 == 0) //function 1:   khai_bao_covid();
            {
                // khai_bao_covid();
                do
                {
                    BOXE(2);
                    wattrset(empty, COLOR_PAIR(12));
                    mvwprintw(empty, 3, 2, "Nhap ten cua diem dau: ");
                    wattroff(empty, COLOR_PAIR(12));

                    ch = nhap(str1, "Nhap ten cua diem dau: ", 3, 2, empty);
                    if (ch == 27)
                        break;
                    if (!hasVertex(graph, str1))
                    {
                        mvwprintw(empty, 10, 2, "khong ton tai diem bus %s.\n  nhan ESC de thoat", str1);
                        ch = nhap(str1, "Nhap ten cua diem dau: ", 10, 2, empty);
                        if (ch == 27)
                            break;
                    }
                    box(empty, ACS_VLINE, ACS_HLINE);
                    wattrset(empty, COLOR_PAIR(12));
                    mvwprintw(empty, 6, 2, "Nhap ten cua diem cuoi: ");
                    wattroff(empty, COLOR_PAIR(12));

                    ch = nhap(str2, "Nhap ten cua diem cuoi: ", 6, 2, empty);
                    if (ch == 27)
                        break;
                    if (!hasVertex(graph, str2))
                    {
                        mvwprintw(empty, 10, 2, "khong ton tai diem bus %s\n  nhan ESC de thoat", str2);
                        ch = nhap(str2, "Nhap ten cua diem dau: ", 10, 2, empty);
                        if (ch == 27)
                            break;
                    }
                    end = BFS(graph, str1, str2, rank); // duyet cay theo chieu rong
                    if (rank[end] == INFINITIVE)
                    {
                        mvwprintw(empty, 10, 2, "khong ton tai duong di tu %sden %s\n  nhan ESC de thoat", str1, str2);
                        ch = nhap(str2, ": ", 10, 2, empty);
                        if (ch == 27)
                            break;
                        exit(0);
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

                    // for (i = 0; i <= sum; i++)
                    // {
                    //     for (int j = 0; j <= rank[end]; j++)
                    //     {
                    //         printf("%s%c", getVertex(graph, paths[i][j]), j == rank[end] ? '\n' : '-');
                    //     }
                    // }

                    for (i = 0; i <= sum; i++)
                    {
                        toiuu[i] = toiUu(graph, paths[i], rank[end]); // chon it tuyen duong nhat tren tuyen duong vuaw lay
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

                    box(empty, ACS_VLINE, ACS_HLINE);
                } while (wgetch(empty) != 27);
                wclear(plist);
                wrefresh(plist);
                wclear(empty);
                wrefresh(empty);
            }
            if (temp2 == 1) // function 2:
            {
                dropGraph(graph);
                delwin(w);
                endwin();
                exit(0);
            }
            break;
        }
        BOX(miw);
        wattron(miw, A_STANDOUT);
        sprintf(item, "%-15s", list2[temp2]);
        mvwprintw(miw, temp2 + 1, 2, "%s", item);
        wattroff(miw, A_STANDOUT);
    }
    delwin(miw);
    endwin();
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
    plist = newwin(20, 120, 1, 71);
    box(plist, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    int line = 2;

    JRB node;
    char name[50] = "";
    for (int i = 0; i < size; i++)
    {
        node = getWeight(g, path[i], path[i + 1]);
        strcpy(name, getBusNameMin(node, name));
        mvwprintw(plist, line++, 2, "Ben:\t\t%-45sBAT Tuyen buyt:   %s", getVertex(g, path[i]), name);
    }
    mvwprintw(plist, line++, 2, "Ben cuoi: \t%-30s\n", getVertex(g, path[size]));
    box(plist, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    wrefresh(plist);
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
    int i = 0;
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