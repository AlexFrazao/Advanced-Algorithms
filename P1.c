#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct node *node;
typedef struct node *heap;
int n;
struct node *A;

struct node
{
    int v;                //              0
    struct node *child;   //            (   )
    struct node *brother; //  child -> 0     0 <- brother
};

int ptr2loc(node v, node A)
{
    int r;
    r = -1;
    if (NULL != v)
        r = ((size_t)v - (size_t)A) / sizeof(struct node); // size_t retrieves the size of an array // this computation retrieves the byte difference between the node being tested and A
    return (int)r;
}

void vizShow(FILE *f, int n)
{
    int i;
    fprintf(f, "digraph {\n");
    for (i = 0; i < n; i++)
    {
        fprintf(f, "A%d [label=\"A%d.v=%d\"]\n",
                i, i, A[i].v);
    }
    for (i = 0; i < n; i++)
    {
        if (NULL != A[i].child)
            fprintf(f, "A%d -> A%d [label=\"ch\"]\n",
                    i, ptr2loc(A[i].child, A));
        if (NULL != A[i].brother)
            fprintf(f, "A%d -> A%d [label=\"%s\"]\n",
                    i, ptr2loc(A[i].brother, A),
                    (0 < A[i].v) ? "br" : "f");
    }
    fprintf(f, "}\n");
}

node getHook(node root, node n)
{
    /*
    4: getHook traverses the connected
    nodes until it finds the link that points to the desired node, so we know what
    to cut.
    */
    node pre_n = n;
    while (n != pre_n->brother)
    {
        pre_n = pre_n->brother;
        if (pre_n == root)
        {
            if (n == root->child)
            {
                break;
            }
            pre_n = root->child;
        }
    }
    return pre_n;
}

void showNode(node v) // 'S'
{
    if (NULL == v)
        printf("NULL\n");
    else
    {
        printf("node: %d ", ptr2loc(v, A));
        printf("v: %d ", v->v);
        printf("child: %d ", ptr2loc(v->child, A));
        printf("brother: %d ", ptr2loc(v->brother, A));
        printf("\n");
    }
}

void showList(node current) // 'P'
{
    // When called is supposed to display the child and all the brothers, if any.
    while (current != NULL)
    {
        showNode(current);
        current = current->brother;
    }
}

void Set(node p, int x) // 'V'
{
    // if p is a root, sets it to NULL. Else brother->root.
    if (p != NULL && p->child == NULL && p->brother == NULL)
    {
        p->v = -x;
    }
    else
    {
        p->brother = p;
    }
    printf("set A[%d] to %d\n", ptr2loc(p, A), x);
}

static void link(node f, node c)
{

    if (f->child == NULL)
    {
        f->child = c;
        c->brother = f;
    }
    else
    {
        c->brother = f->child;
        f->child = c;
        f->child->v = abs(c->v);
    }
    printf("link A[%d] as child of A[%d]\n", ptr2loc(c, A), ptr2loc(f, A));
}

int Meld(heap q1, heap q2) // 'U'
{
    /*The Meld function is used to join two heaps, i.e., merge
the two 'ROOTS' into a single tree. This function returns the root of the
resulting tree.*/
    int root_index;
    if (abs(q1->v) > abs(q2->v))
    {
        link(q2, q1);
        printf("Swap A[%d] and A[%d]\n", ptr2loc(q1, A), ptr2loc(q2, A));
        printf("Meld A[%d] A[%d]\n", ptr2loc(q2, A), ptr2loc(q1, A));
        root_index = ptr2loc(q2, A);
    }
    else
    {
        link(q1, q2);
        printf("Meld A[%d] A[%d]\n", ptr2loc(q1, A), ptr2loc(q2, A));
        root_index = ptr2loc(q1, A);
    }
    return root_index;
}

int decreaseKey(node root, node n, int v) // 'R'
{
    int p;
    if (root->v == n->v)
    {
        root->v = -v;
        p = ptr2loc(root, A);
        return p;
    }
    else if (n == root->child && root == n->brother)
    {
        root->child = NULL;
        n->v = v;
        p = ptr2loc(root, A);
    }
    else
    {
        node pre_n = getHook(root, n);
        pre_n->brother = n->brother;
        n->v = v;
        p = ptr2loc(root, A);
    }
    Meld(root, n);
    return p;
}

/*
void M(node)
{ /*The Min function returns the absolute value of v for the cur-
rent node. When the argument node is a root the result is the heap
minimum value.
}*/

int extractMin(node root) // 'E'
{   
    if (root == NULL || root->child == NULL) {
        printf("Root has no children.\n");
        return -1; 
    }
    printf("kiiiiiiiikkkkkkkkiiiiiiiii");

    node fstNode = root->child;
    node sndNode = fstNode->brother;
    int fst_root_index, root_index, flag=0;
    
    while (fstNode != root || sndNode != NULL || sndNode != root)
    {
        if (0==flag){
            fst_root_index = Meld(fstNode, sndNode);
        }
        flag += 1;

        if(2<=flag){
            root_index = Meld(fstNode, sndNode);
            Meld(&A[fst_root_index], &A[root_index]);
        }
        fstNode = sndNode->brother;
        sndNode = fstNode->brother;
    }
    root->child = NULL;
    root->v = -1;
    return fst_root_index;
}

int main()
{
    FILE *f = fopen("heap_graph.dot", "w");
    if (!f)
    {
        fprintf(stderr, "Failed to open graph file for writing.\n");
        return 1;
    }

    scanf("%d", &n);
    getchar();

    A = (struct node *)calloc(n, sizeof(struct node)); // A is a pointer to the struct nodes
    if (!A)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        A[i].v = -1; // Initialize each struct node
        A[i].child = NULL;
        A[i].brother = NULL;
    }

    char input;
    int index, index1, index2, index3, new_v;

    while (scanf(" %c", &input) && input != 'X')
    { // _%c tells scanf to skip any whitespace characters before reading a character.
        switch (input)
        {
        case 'S': // showNode
            scanf("%d", &index);
            showNode(&A[index]); //&A so we can handle the actual node
            break;
        case 'V': // Set
            scanf("%d %d", &index, &new_v);
            Set(&A[index], new_v);
            break;
        case 'P': // showList
            scanf("%d", &index);
            showList(&A[index]);
            break;
        case 'U': // Meld
            scanf("%d %d", &index1, &index2);
            int root = Meld(&A[index1], &A[index2]);
            printf("The root of the resulting tree is A[%d]\n", root);
            break;
        case 'R': // decreaseKey
            scanf("%d %d %d", &index1, &index2, &new_v);
            int p = decreaseKey(&A[index1], &A[index2], new_v);
            printf("decKey A[%d] to %d\n", p, new_v);
            break;
        case 'E': // extractMin
            if (scanf("%d", &index3) != 1){
                printf("Error");
            }
            printf("extractMin A[%d]\n", extractMin(&A[index]));
            break;
        case 'M': // Min
            break;
        default:
            // Handle unknown command or consume extra characters
            break;
        }
    }

    vizShow(f, n);
    fclose(f);
    free(A); // Free the array of pointers

    system("dot -Tpng heap_graph.dot -o heap_graph.png");
    system("xdg-open heap_graph.png");

    return 0;
}