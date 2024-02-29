#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node *node;
int n;
struct node *A;

struct node
{
    int v;                  //              0  
    struct node *child;     //            (   )
    struct node *brother;   //  child -> 0     0 <- brother
};

int ptr2loc(node v, node A)
{
    int r;
    r = -1;
    if (NULL != v)
        r = ((size_t)v - (size_t)A) / sizeof(struct node); // size_t retrieves the size of an array // this computation retrieves the byte difference between the node being tested and A
    return (int)r;
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

void showList(node current) // 'P' When called is supposed to display the child and all the brothers, if any.
{
    /*The showList function that gives a description of the brother
list at a node, i.e., it calls showNode for the current node and then
recursively calls showList on the brother node if the current v value is
not negative.*/
    
    showNode(current);
    if (current->v >= 0)
    {
        current->v -= 1;
        showList(current);
    }
}

void Set(node current, int new_v) // 'V' if node is a root, sets it to NULL. Else brother->root.
{
    /*The Set function that changes the v field of the current node.
Note that this function can only be executed when the node is a heap
by itself, i.e., its child and brother fields are all NULL. The function
must receive a positive value v, but stores its negative correspondent.*/

    // Base case: empty tree is a valid min-heap
    if (current == NULL)
    {
        current->v = new_v;
        return true;
    }

    // Recursively check subtrees
    return Set(current->child) && Set(current->brother);
}

/*
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
    int index, new_v;

    while (scanf(" %c", &input) && input != 'X')
    { // _%c tells scanf to skip any whitespace characters before reading a character.
        switch (input)
        {
        case 'S':
            scanf("%d", &index);
            showNode(&A[index]); // &A[index] provides the address of the index-th element in the array A of struct node. You need to use the address (or the pointer to the node) when you want to pass a node to a function and allow that function to modify the original node in the array.
            break;
        case 'V':
            scanf("%d %d", &index, &new_v);
            Set(A[index], new_v);
            break;
        case 'P':
            scanf("%d", &index);
            showList(index);

        // Add cases for other commands: P, U, R, M, E
        default:
            // Handle unknown command or consume extra characters
            break;
        }
    }
    // Cleanup
    for (int i = 0; i < n; i++)
    {
        free(&A[i]); // Free each node
    }
    free(A); // Free the array of pointers

    return 0;
}
