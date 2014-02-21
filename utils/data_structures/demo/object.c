#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

#include "base_type.h"
#include "../object.h"

struct node_data_t {
    int depth;
};

struct node_t {
    struct object obj;
    struct node_data_t data;
};

struct node_t *random_tree(ui_t count)
{
    struct node_t *root = calloc(1, sizeof(struct node_t));
    struct node_t *fakeroot = calloc(1, sizeof(struct node_t));
    static struct node_t **nodes;
    ui_t i;

    root->obj.parent = &fakeroot->obj;
    fakeroot->obj.parent = &root->obj;
    fakeroot->obj.lchild = object_tree_l_most_node(&root->obj);
    fakeroot->obj.rchild = object_tree_r_most_node(&root->obj);
    nodes = calloc(count, sizeof(struct node_t *));
    nodes[0] = root;
    nodes[0]->obj.id = 0;
    for (i = 1; i < count; i++) {
        struct node_t *par = nodes[rand() % i];
        nodes[i] = calloc(1, sizeof(struct node_t));
        nodes[i]->obj.id = i;
        object_attach_child(OBJECT_POINTER(par), OBJECT_POINTER(nodes[i]));
    }
    free(nodes);
    return root;
}

void save_depth(struct node_t *tree)
{
    struct node_t *it;
    for (it = object_tree_l_most_node(tree->obj.parent); it;
         it = object_tree_iterator_increment(tree->obj.parent,
                                             OBJECT_POINTER(it))) {
        struct node_t *par =
            (struct node_t *)object_parent(OBJECT_POINTER(it));
        if (par) {
            it->data.depth = par->data.depth + 1;
        }
        else {
            it->data.depth = 0;
        }
    }
}

void print_node(struct node_t *node)
{
    int i;
    for (i = 0; i < node->data.depth; i++) {
        printf("\t");
    }
    printf("%ld\n", node->obj.id);
}

void print_tree(struct node_t *tree)
{
    struct node_t *it;
    save_depth(tree);
    for (it = object_tree_l_most_node(tree->obj.parent); it;
         it = object_tree_iterator_increment(tree->obj.parent,
                                             OBJECT_POINTER(it))) {
        print_node(it);
    }
}

int main()
{
    srand(time(NULL));
    while (1) {
        ui_t count;
        struct node_t *tree = NULL;
        if(scanf("%lu", &count)==EOF) {
            break;
        }
        tree = random_tree(count);
        print_tree(tree);
        object_tree_free(OBJECT_POINTER(tree));
    }
    return 0;
}
