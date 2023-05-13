#include "binary-tree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

BinaryTreeNode* initBinaryNode() 
{
	BinaryTreeNode* node = calloc(1, sizeof(BinaryTreeNode));

	if (node == NULL)
		perror("Memory allocation failed");

	return node;
}

bool destroyBinaryBranchRecursive(BinaryTreeNode* rootNode)
{
	if (rootNode->lhs != NULL)
		destroyBinaryBranchRecursive(rootNode->lhs);
	if (rootNode->rhs != NULL)
		destroyBinaryBranchRecursive(rootNode->rhs);

	if (rootNode->value)
		free(rootNode->value);

	free(rootNode);

	return true;
}

bool destroyBinaryBranch(BinaryTreeNode** rootNode) 
{
	assert(rootNode != NULL);
	assert(*rootNode != NULL);

	destroyBinaryBranchRecursive(*rootNode);

	*rootNode = NULL;

	return true;
}

char* duplicateString(const char* string) 
{
	if (string == NULL)
		return NULL;

	unsigned int length = strnlen_s(string, MAX_NODE_STRING_LENGTH);

	char* result = malloc(length + 1);

	if (result == NULL)
	{
		perror("String allocation failed");

		return result;
	}

	for (int i = 0; i < length; i++)
		result[i] = string[i];

	result[length] = '\0';

	return result;
}

bool attachNode(BinaryTreeNode** parentNode, const char* attachNodeValue, NodeSide sideToAttach, NodeSide sideToRerouteBranch) 
{
	assert(parentNode != NULL);
	assert(sideToAttach != 0 && sideToAttach != NS_BOTH);
	assert(sideToRerouteBranch != NS_BOTH);

	BinaryTreeNode* newNode = initBinaryNode();

	if (newNode == NULL)
		return false;

	newNode->value = duplicateString(attachNodeValue);

	if (*parentNode == NULL)
	{
		*parentNode = newNode;
		return;
	}

	BinaryTreeNode* tempNode = NULL;

	switch (sideToAttach)
	{
	case NS_LEFT:
		tempNode = (*parentNode)->lhs;
		(*parentNode)->lhs = newNode;
		break;
	case NS_RIGHT:
		tempNode = (*parentNode)->rhs;
		(*parentNode)->rhs = newNode;
		break;
	default:
		assert(0);
		break;
	}
	
	if (tempNode != NULL)
		switch (sideToRerouteBranch)
		{
		case NS_LEFT:
			newNode->lhs = tempNode;
			break;
		case NS_RIGHT:
			newNode->rhs = tempNode;
			break;
		default:
			assert(0);
			break;
		}

	return true;
}

BinaryTreeIteratorFunc iteratorFunc;

void iterateBinaryTreeByCycle(BinaryTreeNode* rootNode) 
{
	BinaryTreeNode* temp = rootNode;

	while (temp != NULL)
	{
		switch(iteratorFunc(temp)) 
		{
		case NS_LEFT:
			temp = temp->lhs;
			break;
		case 0:
			temp = NULL;
			break;
		case NS_RIGHT:
			temp = temp->rhs;
			break;
		default:
			assert(0);
			break;
		}
	}
}

void iterateBinaryTreeRecursive(BinaryTreeNode* node)
{
	if (node != NULL)
		return;

	switch (iteratorFunc(node))
	{
	case NS_LEFT:
		iterateBinaryTreeRecursive(node->lhs);
		break;
	case 0:
		return;
	case NS_RIGHT:
		iterateBinaryTreeRecursive(node->rhs);
		break;
	case NS_BOTH:
		iterateBinaryTreeRecursive(node->lhs);
		iterateBinaryTreeRecursive(node->rhs);
		break;
	default:
		assert(0);
		break;
	}
}

void iterateBinaryTree(BinaryTreeNode* rootNode, BinaryTreeIteratorFunc iteratorFunction)
{
	assert(iteratorFunction != NULL);

	iteratorFunc = iteratorFunction;

	iterateBinaryTreeRecursive(rootNode);
}
