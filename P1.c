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

void Set(node node, int new_v) // 'V'
{
    // if node is a root, sets it to NULL. Else brother->root.
    if (node != NULL && node->child == NULL && node->brother == NULL)
    {
        node->v = -new_v;
    }
    else
    {
        node->brother = node;
    }
}

int Meld(heap q1, heap q2) // 'U'
{ 
    /*The Meld function is used to join two heaps, i.e., merge
the two roots into a single tree. This function returns the root of the
resulting tree.*/
    int root_index;
    if (q1->v < q2->v){
        q1->brother = q2->child;
        q2->child = q1; 
        root_index = ptr2loc(q2, A);

        printf("Swap A[%d] and A[%d]\n", ptr2loc(q1,A), ptr2loc(q2,A));
        printf("Meld A[%d] A[%d]\n", ptr2loc(q2,A), ptr2loc(q1,A));

    } else{
        q2->brother = q1->child;
        q1->child = q2;
        root_index = ptr2loc(q1, A);

        printf("Meld A[%d] A[%d]\n", ptr2loc(q1,A), ptr2loc(q2,A));
        
    }
    return root_index;
}

/*
void R(root, node, v)
{ /*The DecreaseKey function is used to decrease the v
field of the current node. This node may be part of a meldable heap
tree and therefore this operation might need modify this tree. The
root of the corresponding tree must be given as the first argument to
this function.
}

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
        case 'S':
            scanf("%d", &index);
            showNode(&A[index]); //&A so we can handle the actual node
            break;
        case 'V':
            scanf("%d %d", &index, &new_v);
            Set(&A[index], new_v);
            break;
        case 'P':
            scanf("%d", &index);
            showList(&A[index]);
            break;
        case 'U':
            scanf("%d %d", &index1, &index2);
            Meld(&A[index1], &A[index2]);
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

    return 0;
}
