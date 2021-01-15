/*
 * @Author: your name
 * @Date: 2020-12-30 07:17:57
 * @LastEditTime: 2020-12-30 08:39:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /workspace/cpp/algorithm/tree/BinaryTree/BinaryTree.h
 */
#ifndef __BINARYTREE_h__
#define __BINARYTREE_h__

#include <stdio.h>
#include <stdlib.h>

//https://zhuanlan.zhihu.com/p/87589946
// 定义二叉树结点
struct Node {
    int data;
    struct Node* pLChild;
    struct Node* pRChild;
} Node;

// 创建一个二叉树节点
struct Node* createNode(int value) {
    struct  Node* pNode = (struct  Node*)malloc(sizeof(struct Node));
    pNode->data = value;
    pNode->pLChild = NULL;
    pNode->pRChild = NULL;
    return pNode;
}

// 数据插入二叉树
struct Node* addNode(int value, struct Node*pNode) {
    if (pNode == NULL)
    {
         pNode = createNode(value);
    }
    else if (value < pNode->data)
    {
        if (pNode->pLChild == NULL)
        {
            pNode->pLChild = createNode(value);
        }
        else
        {
            addNode(value, pNode->pLChild);
        }  
    }
    else if (value > pNode->data)
    {
        if (pNode->pRChild == NULL)
        {
            pNode->pRChild = createNode(value);
        }
        else
        {
            addNode(value, pNode->pRChild);
        }
    }

    return pNode;
}
 
#endif