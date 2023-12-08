

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rbtree.h"

#define NODE_SIZE   (128)

typedef struct {
    int key;
    struct rb_node rb;
    int val;
}Node_t;


Node_t nodes[NODE_SIZE];

int insert(Node_t *node_ptr, struct rb_root *root)
{
    struct rb_node **new = &root->rb_node, *parent = NULL;
    while(*new)
    {
        parent = *new;
        if (node_ptr->key < rb_entry(*new, Node_t, rb)->key)
        {
            new = &parent->rb_left;
        }
        else if (node_ptr->key > rb_entry(*new, Node_t, rb)->key)
        {
            new = &parent->rb_right;
        }
        else
        {
            return 0;
        }
    }

    rb_link_node(&node_ptr->rb, parent, new);
    rb_insert_color(&node_ptr->rb, root);

    return 1;
}

int erase(Node_t *node_ptr, struct rb_root *root)
{
    rb_erase(&node_ptr->rb, root);
}

Node_t* search(int key, struct rb_root *root)
{
    Node_t *node_ptr = NULL;
    struct rb_node *rbnode = root->rb_node;
    while(rbnode)
    {
        node_ptr = rb_entry(rbnode, Node_t, rb);
        if (key < node_ptr->key)
        {
            rbnode = rbnode->rb_left;
        }
        else if (key > node_ptr->key)
        {
            rbnode = rbnode->rb_right;
        }
        else
        {
            return node_ptr;
        }
    }
    return NULL;
}

void dump(struct rb_root *root)
{
    struct rb_node *rbnode = NULL;
    Node_t *node_ptr = NULL;
    for(rbnode = rb_first(root);rbnode;rbnode = rb_next(rbnode))
    {
        node_ptr = rb_entry(rbnode, Node_t, rb);
        printf("key: %d, val: %d\n", node_ptr->key, node_ptr->val);
    }
}

int main(int argc, char *argv[])
{
    int idx = 0;

    srand(0);
    for (idx = 0;idx < NODE_SIZE;idx++)
    {
        nodes[idx].key = rand();
        nodes[idx].val = rand();
    }

    struct rb_root root = RB_ROOT;

    for (idx = 0;idx < NODE_SIZE;idx++)
    {
        insert(nodes + idx, &root);
    }
    dump(&root);

    Node_t *searchNode = search(8936987, &root);
    if (searchNode)
    {
        printf("%d, val:%d\n", searchNode->key, searchNode->val);
    }
    else
    {
        printf("%d not found\n", 100669);
    }

    if (searchNode)
    {
        printf("erase node %d\n", searchNode->key);
        erase(searchNode, &root);
    }

    dump(&root);

    return 0;
}

