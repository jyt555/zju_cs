#include <stdio.h>
#include <stdlib.h>
#define MAX 35
typedef struct TreeNode *Tree;
struct TreeNode {
    int value;  /* the value of this node */
    int flag;   /* if the flag == 1, it's a black node, else it's a red node */
    int count;  /* to count the number of black nodes from leaves to this node */
    Tree  Left;
    Tree  Right;
};
Tree T;     /* the root of binary tree */
int i;      /* the number of the trees */
char *c[MAX];   /* result of judge */
int EN;     /* flag for condition 2 */

void createTree(); /* function: create the binary tree with N numbers given */
Tree addNode(Tree T, Tree node); /* function: add new node into the binary tree */
void JudgeRBT(); /* function: judge whether it's a red-black tree and store the result */
int BlackSon(Tree T); /* condition 1: for each red node, both its children are black */
int SameBN(Tree T); /* condition 2: For each node, all paths from it to leaves contain the same number of black nodes.*/

int main(void)
{
    int K;      /* the total number of cases, K <= 30 */
    scanf("%d",&K);
    for(i = 0;i < K;i++){
        T = NULL;       /* initialize the head pointer */
        EN = 1;
        createTree(); /* create the binary tree */
        JudgeRBT();     /* judge whether it's a red-black tree and store the result */
    }
    for(i = 0;i < K;i++)/* print the result */
        printf("%s\n",c[i]);
    return 0;
}
void createTree() /* function: create the binary tree with N numbers given */
{
    int j, number;
    int N;      /* the total number of nodes in binary tree, N <= 30 */

    scanf("%d",&N);
    // printf("N:%d\n",N);//    all codes for test are marked by '//'
    for(j = 0;j < N;j++){
        Tree node = (Tree)malloc(sizeof(struct TreeNode));
        node->flag = 0;     /* assume it's a red node */
        node->Left = NULL;  /* initialization */
        node->Right = NULL;
        node->count = 0;
        scanf("%d",&number);/* put in the number */
        // printf("put in:%d\n",number);//
        if(number > 0)  node->flag = 1;     /* flag == 1 means it's the black node */
        else            number = -number;   /* keep 'number' positive */
        node->value = number;
        T = addNode(T, node);   /* add this node into the binary tree */
    }
}
Tree addNode(Tree T, Tree node) /* function: add new node into the binary tree */
{
    if(T == NULL)   /* this position is empty, and add node here */
        return T = node;
    else{   /* search the position for the number */
        if(node->value < T->value) T->Left = addNode(T->Left, node);
        else    T->Right = addNode(T->Right, node);
    }
    return T;
}
void JudgeRBT() /* function: judge whether it's a red-black tree and store the result */
{
    if(!BlackSon(T)){
        c[i] = "No";
        return;
    }
    // printf("condition 1: done.\n");//
    if(!(SameBN(T) && EN)){
        c[i] = "No";
        return;
    }
    if(!T->flag){   /* condition 3: the root is black node */
        c[i] = "No";
        return;
    }
    c[i] = "Yes";
}
int BlackSon(Tree T) /* condition 1: for each red node, both its children are black */
{
    if(T == NULL) return 1;
    if(!T->flag){
        // printf("judge:%d\n",T->value);//
        if(T->Left == NULL)     return 1;   /* leaf(NULL) is black node */
        if(!T->Left->flag) return 0;
        if(T->Right == NULL)    return 1;
        if(!T->Right->flag)return 0;
    }//else printf("non_judge:%d\n",T->value);//
    if(BlackSon(T->Left) && BlackSon(T->Right)) return 1;
    return 0;
}
int SameBN(Tree T) /* condition 2: For each node, all paths from it to leaves contain the same number of black nodes.*/
{
    if(T->Left == NULL) T->count = T->flag + 1;
    else                T->count = T->flag + SameBN(T->Left);
    if(T->Right == NULL){
        if(T->count != T->flag + 1) EN = 0; /* once count_left != count_right, EN = 0 */
    }else if(T->count != T->flag + SameBN(T->Right)) EN = 0;
    // printf("value:%d,count:%d\n",T->value,T->count);//
    return T->count;
}