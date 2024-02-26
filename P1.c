#include <stdio.h>
#include <stdlib.h>

typedef struct node* node;

struct node {
    int v;
    node child;
    node brother;
};

node new_node(){
    node new_node = (node)malloc(sizeof(struct node));
    new_node->v = 1;
    new_node->child = NULL;
    new_node->brother = NULL;
    return new_node;
}

void V(node node_to_be_changed, int new_v){
    if(new_v >= 0){
        node_to_be_changed->v = new_v;
    }else{
        node_to_be_changed->v = abs(new_v);

    }
}

int main() {
    node root = new_node();
    root->child = new_node();
    root->child->brother = new_node();

    V(root, 1);
    V(root->child, 2);
    V(root->child->brother, 3);

    printf("Root node value: %d\n", root->v);
    printf("Child node value: %d\n", root->child->v);
    printf("Brother node value: %d\n", root->child->brother->v);

    return 0;
}