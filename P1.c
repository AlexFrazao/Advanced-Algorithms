#include <stdio.h>
#include <stdlib.h>

typedef struct node *node;
int n;  // Declare the number of nodes
node A; // Declare A as a pointer to an array of struct node

struct node
{
    int v;
    node child;
    node brother;
};

node A_creation(node *A, int n)
{
    for (int i = 0; i < n; i++)
    {
        A[i] = (node)calloc(1, sizeof(struct node)); // Allocate and initialize each node
        A[i]->v = -1;                                // As per your document, initial v should be -1
        A[i]->child = NULL;
        A[i]->brother = NULL;
    }
}

int ptr2loc(node v, node A)
{
    int r;
    r = -1;
    if (NULL != v)
        r = ((size_t)v - (size_t)A) / sizeof(struct node); // size_t retrieves the size of an array // this computation retrieves the byte difference between the node being tested and A
    return (int)r;
}

void S(node v, node A)
{
    int f;
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

/*void P(node)
{
    /*The showList function that gives a description of the brother
list at a node, i.e., it calls showNode for the current node and then
recursively calls showList on the brother node if the current v value is
not negative.
}

void V(node node_to_be_changed, int new_v)
{

    if (new_v >= 0)
    {
        node_to_be_changed->v = new_v;
    }
    else
    {
        node_to_be_changed->v = abs(new_v);
    }
}

void U(heap, heap)
{ /*The Meld function is used to join two heaps, i.e., merge
the two roots into a single tree. This function returns the root of the
resulting tree.
}

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
    scanf("%d", &n);
    getchar();

    node *A = (node *)calloc(n, sizeof(struct node));
    if(!A){
        fprintf(stderr, "memory allocation failed\n");
        return 1;
    }

    printf(A);
    A_creation(A, n);

    char input;
    int index, new_v;

    while (scanf(" %c", &input) && input != 'X') { // _%c tells scanf to skip any whitespace characters before reading a character.  
        switch (input) {
            case 'S':
                scanf("%d", &index);    
                S(A[index], A);
                break;
            case 'V':
                scanf("%d %d", &index, &new_v);
                V(A[index], new_v);
                break;
            // Add cases for other commands: P, U, R, M, E
            default:
                // Handle unknown command or consume extra characters
                break;
        }
    }
        // Cleanup
    for (int i = 0; i < n; i++) {
        free(A[i]); // Free each node
    }
    free(A); // Free the array of pointers

    return 0;
}


