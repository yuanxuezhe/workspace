/*
 * @Author: your name
 * @Date: 2020-12-30 07:51:22
 * @LastEditTime: 2020-12-30 08:37:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /workspace/cpp/algorithm/tree/BinaryTree/main.cpp
 */

#include "stdio.h"
#include "BinaryTree.h"

int main(int argc, char const *argv[])
{
    struct Node* pnode;
    int a[] = {9,1,2,22,65,32,53,3,4,5,6};
    for (size_t i = 0; i < sizeof(a)/sizeof(int); i++)
    {
        pnode = addNode(a[i],NULL);
    }
    printf("%d \n",pnode->data );
    return 0;
}
