#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#define M 3

typedef struct tree *prt_tree;

struct tree
{
    int data;         //存储的数据
    prt_tree left;    //左子节点
    prt_tree right;   //右子节点
    prt_tree brother; //兄弟节点
};
prt_tree create_node(int data)
{
    prt_tree tree = malloc(sizeof(struct tree));
    memset(tree, 0, sizeof(struct tree));
    tree->data = data;
    return tree;
}

prt_tree insert(int data, prt_tree root, prt_tree father)
{
    if (root == NULL)
    {
        root = create_node(data);
        return root;
    }
    //在兄弟节点进行搜索,直到data夹在两个兄弟节点之中,或者相等,或者为空
    //如果相等不进行处理,直接返回
    //如果比最小的还小,插入节点后,与第一个交换返回
    //如果是夹在兄弟中,就进入第一个的右,或者第二个左节点
    //如果等于NULL的话,证明这是叶子节点,就直接在中间插入,否则进行插入操作,回到第一行
    //如果是空就插入在最后一个兄弟节点
    //插入之后继续进行循环,得到n来判断这个节点数量有没有大于m
    prt_tree tmp = root;
    int n = 0;
    while (tmp != NULL)
    {
        n += 1;
        if (data == tmp->data)
        {
            //相等的情况
            break;
        }
        else if (data < tmp->data)
        {
            //比最小还小的情况(也就是最小),往左边插入,先判断left节点是不是NULL
            //为NULL则认为是叶子节点,数量+1,然后跳出循环,新节点变为老节点
            //否则递归调用insert,往left节点继续搜索
            if (tmp->left == NULL)
            {
                prt_tree new_tree = create_node(data);
                new_tree->brother = tmp;
                n += 1;
                root = new_tree;
                break;
            }
            else
            {
                //往左节点继续搜索
                insert(data, tmp->left, tmp);
            }
        }
        else if (tmp->brother == NULL)
        {
            //等于空的情况(也就是最大的情况),往右插入,先判断right节点是不是NULL
            //为NULL则认为是叶子节点,新节点加入兄弟节点后,tmp指向兄弟节点,跳出循环
            //否则递归调用insert,往right节点继续搜索
            if (tmp->right == NULL)
            {
                prt_tree new_tree = create_node(data);
                tmp->brother = new_tree;
                tmp = tmp->brother;
                break;
            }
            else
            {
                insert(data, tmp->right, tmp);
            }
        }
        else if (data > tmp->data && data < tmp->brother->data)
        {
            //夹在中间的情况,如果right为空的就认为是叶子节点,插入在中间
            if (tmp->right == NULL)
            {
                //插入在中间,新建立一个新的节点,然后将节点插入到中间
                prt_tree new_tree = create_node(data);
                prt_tree tmpPrtTree = tmp->brother;
                tmp->brother = new_tree;
                new_tree->brother = tmpPrtTree;
                tmp = tmp->brother;
                break;
            }
            else
            {
                //往父的right插入
                insert(data, tmp->right, tmp);
            }
        }
        tmp = tmp->brother;
    }
    //接着tmp继续循环,遍历这个节点有没有超过m
    while (tmp != NULL)
    {
        tmp = tmp->brother;
        n += 1;
    }
    //判断是否超过M
    if (n > M)
    {
        //超过m,取出中间的前一个节点
        prt_tree mid_before_tree = root;
        prt_tree mid_tree = NULL;
        int sub = ceil(M / 2);
        for (int i = 0; i < sub; i++)
        {
            mid_before_tree = mid_before_tree->brother;
        }
        mid_tree = mid_before_tree->brother;
        //取出中间的后,判断父节点,是不是等于当前,如果是等于当前root节点
        //将这一整个块从mid_tree分开,中间节点提取到上一节点
        mid_before_tree->brother = NULL;
        mid_tree->left = root;
        mid_tree->right = mid_tree->brother;
        mid_tree->brother = NULL;
        //则认为这是在操作根节点,操作的是根节点的话
        //就要将现在的这个节点变为根节点
        if (root == father)
        {
            root = mid_tree;
        }
        else
        {
            //然后判断比父节点大还是小
            //比父大就是直接插入到父的兄弟节点
            //然后父的兄弟的left为空
            //比父小就进行交换,插入
            //父right为空
            if (father->data > data)
            {
                prt_tree tmp_node = father->brother;
                father->brother = mid_tree;
                mid_tree->brother = tmp_node;
            }
            else
            {
                //连接兄弟节点
                mid_tree->brother = father->brother;
                father->brother = mid_tree;
            }
        }
    }
    printf("data:%d\tn:%d\n", data, n);
    return root;
}

//排序输出
void print_tree(prt_tree root)
{
    if (root == NULL)
        return;
    print_tree(root->left);
    printf("%d\t", root->data);
    print_tree(root->brother);
    if (root->brother == NULL)
    {
        print_tree(root->right);
    }
}

int main()
{
    prt_tree root = NULL;
    root = insert(1, root, root);
    root = insert(6, root, root);
    root = insert(8, root, root);
    root = insert(11, root, root);
    root = insert(15, root, root);
    root = insert(16, root, root);
    root = insert(18, root, root);
    print_tree(root);
    return 0;
}