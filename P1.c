#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

static void link(node f, node c){
    f->child = c;
    c->brother = f;
    printf("link A[%d] as child of A[%d]\n", ptr2loc(c, A), ptr2loc(f, A));
}

/*void getHook(node root, node node){
    
    int target = node;
    while (target != node->brother){ 
        node = node->brother;
    }
    node->brother = root;
}*/

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
    printf("set A[%d] to %d\n", ptr2loc(p,A), x);
}

int Meld(heap q1, heap q2) // 'U'
{ 
    /*The Meld function is used to join two heaps, i.e., merge
the two roots into a single tree. This function returns the root of the
resulting tree.*/
    int root_index;
    if (q1->v > q2->v){
        link(q2, q1);
        printf("Swap A[%d] and A[%d]\n", ptr2loc(q1,A), ptr2loc(q2,A));
        printf("Meld A[%d] A[%d]\n", ptr2loc(q2,A), ptr2loc(q1,A));
        root_index = ptr2loc(q1, A);
    } else{
        link(q1, q2);
        printf("Meld A[%d] A[%d]\n", ptr2loc(q1,A), ptr2loc(q2,A));
        root_index = ptr2loc(q2, A);
    }
    return root_index;
}

void decreaseKey(node root, node node, int v) // 'R'
{ 
/*  1:assumes that the first argument is the root of
the tree that contains the second argument.
    
    2: assumes that
the argument value of v is indeed smaller that the represented value in the
argument node.
    
    3: If both the node and root arguments are equal then we are
decreasing the value at the root, which is a simple matter of changing the
value.
    
    4: Otherwise getHook function is called. getHook traverses the connected 
    nodes until it finds the link that points to the desired node, so we know what
    to cut.
    
    5: The respective pointer is altered, so that the
sub-tree rooted at the node becomes separate from the original tree.
    
    6: Then the v value is decreased

    7: In calling the Meld function the original root is used
as first argument and the node (now also a root) as the second argument.
    
    8: In both cases the return value is the root of the resulting heap tree.
    
    9: An important detail to consider is when the argument node is the only
child of its parent node. In this case the respective child pointer should
be set to NULL.     */

    if (root == node){
        root->v = v;
    }else{
        /*getHook(root, node);*/
    }
}

/*
void M(node)
{ /*The Min function returns the absolute value of v for the cur-
rent node. When the argument node is a root the result is the heap
minimum value.
}

void E(node root)
{ /*The ExtractMin function removes the root node from the current
heap. The function returns the, possibly new, identification of the
resulting tree.
}
*/

int main()
{
    FILE *f = fopen("heap_graph.dot", "w");
    if (!f) {
        fprintf(stderr, "Failed to open graph file for writing.\n");
        return 1;
    }

    scanf("%d", &n);
    getchar();

    A = (struct node *)calloc(n, sizeof(struct node)); // A is a pointer to the struct nodes
    if (!A)
    {
        fprintf(stderr, "memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        A[i].v = -1; // Initialize each struct node
        A[i].child = NULL;
        A[i].brother = NULL;
    }

    char input;
    int index, index1, index2, new_v;

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
            printf("The root of the resulting tree is A[%d]", root);
            break;
        case 'R': // decreaseKey
            break;
        case 'M': // Min
            break;
        case 'E': // ExtractMin
            break;
            

        // Add cases for other commands: P, U, R, M, E
        default:
            // Handle unknown command or consume extra characters
            break;
        }
    }

    vizShow(f,n);
    fclose(f);
    free(A); // Free the array of pointers

    system("dot -Tpng heap_graph.dot -o heap_graph.png");
    system("xdg-open heap_graph.png");

    return 0;
}
