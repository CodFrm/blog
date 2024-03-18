#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct tree tree;

struct tree {
    tree* left;
    tree* right;
    int val;
};

tree* new_tree() {
    tree* t = malloc(sizeof(tree));
    memset(t, 0, sizeof(tree));
    return t;
}

int add_tree(tree* root) {
    int val = 0;
    printf("value:");
    scanf("%d", &val);
    root->val = val;
    int flag = 0;
    while (1) {
        printf("left(1) or right(2) or upper(3) or quit(any):");
        scanf("%d", &flag);
        if (flag == 1) {
            if (root->left == NULL) {
                root->left = new_tree();
            }
            if (!add_tree(root->left)) break;
        } else if (flag == 2) {
            if (root->right == NULL) {
                root->right = new_tree();
            }
            if (!add_tree(root->right)) break;
        } else if (flag == 3) {
            return 1;
        } else {
            break;
        }
    }
    return 0;
}

//前;中;后序遍历 dfs
void print_tree_before(tree* root) {
    if (root == NULL) {
        return;
    }
    printf("%d\t", root->val);
    print_tree_before(root->left);
    print_tree_before(root->right);
}

void print_tree_stack(tree* root) {
    //使用栈来实现前序(不是递归)
    tree* stack[100] = {NULL};
    int start = 0, end = 0;
    stack[0] = root;
    while (end >= 0) {
        //弹出栈顶
        int top = end--;
        tree * tmp=stack[top];
        printf("%d\t", tmp->val);
        //右入
        if (tmp->right != NULL) {
            stack[++end] = tmp->right;
        }
        //左入
        if (tmp->left != NULL) {
            stack[++end] = tmp->left;
        }
    }
}

void print_tree_mid(tree* root) {
    if (root == NULL) {
        return;
    }
    print_tree_mid(root->left);
    printf("%d\t", root->val);
    print_tree_mid(root->right);
}

void print_tree_after(tree* root) {
    if (root == NULL) {
        return;
    }
    print_tree_after(root->left);
    print_tree_after(root->right);
    printf("%d\t", root->val);
}

//层次遍历 bfs
void print_tree_cengci(tree* root) {
    tree* queue[100] = {NULL};
    int pos = 0;  //队首
    int end = 0;  //队尾
    queue[0] = root;
    while (queue[pos] != NULL) {
        if (queue[pos]->left != NULL) {
            //入队
            queue[++end] = queue[pos]->left;
        }
        if (queue[pos] != NULL) {
            queue[++end] = queue[pos]->right;
        }
        //输出(出队)
        printf("%d\t", queue[pos++]->val);
    }
}

int main() {
    tree* root = new_tree();
    root->left = new_tree();
    root->right = new_tree();
    root->left->left = new_tree();
    root->left->right = new_tree();
    root->left->left->val = 4;
    root->left->right->val = 5;
    root->val = 1;
    root->left->val = 2;
    root->right->val = 3;
    // add_tree(root);
    print_tree_before(root);
    printf("\n");
    print_tree_mid(root);
    printf("\n");
    print_tree_after(root);
    printf("\n");
    print_tree_cengci(root);
    printf("\n");
    print_tree_stack(root);
    printf("\n");
    system("pause");
    return 0;
}
