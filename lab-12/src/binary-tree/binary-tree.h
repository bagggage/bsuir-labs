#pragma once

#include <stdbool.h>

#define MAX_NODE_STRING_LENGTH 256

typedef struct BinaryTreeNode 
{
	char* value;

	struct BinaryTreeNode* lhs;
	struct BinaryTreeNode* rhs;

} BinaryTreeNode;

typedef enum NodeSide 
{
	NS_LEFT = -1,
	NS_RIGHT = 1,
	NS_BOTH = 2

} NodeSide;

typedef struct BinaryTreeNode BinaryTree;
typedef NodeSide(*BinaryTreeIteratorFunc)(BinaryTreeNode*);

BinaryTreeNode* initBinaryNode();
bool destroyBinaryBranch(BinaryTreeNode** rootNode);

bool attachNode(BinaryTreeNode** parentNode,const char* attachNodeValue, NodeSide sideToAttach, NodeSide sideToRerouteBranch);

inline bool isHasChildNodes(const BinaryTreeNode* node)
{
	return (node->lhs != 0 || node->rhs != 0);
}

void iterateBinaryTree(BinaryTreeNode* rootNode, BinaryTreeIteratorFunc iteratorFunction);
