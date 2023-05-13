#include "serialization.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LEFT_SIDE_OFFSET	0
#define NULL_VALUE_OFFSET	1
#define RIGHT_SIDE_OFFSET	2

FILE* outputStreamContext = NULL;
FILE* inputStreamContext = NULL;

const char binaryTreeFilePrefix[3] = "btf";

bool serializeBinaryTreeRecursive(const BinaryTreeNode* node);

inline bool serializeSideNodeIfNeccesary(const BinaryTreeNode* node, NodeSide* nodeSidePtr) 
{
	if (node != NULL)
	{
		fwrite(nodeSidePtr, sizeof(char), 1, outputStreamContext);

		if (serializeBinaryTreeRecursive(node) == false)
			return false;
	}

	return true;
}

bool serializeBinaryTreeRecursive(const BinaryTreeNode* node) 
{
	static const NodeSide sides[] = { NS_LEFT, 0, NS_RIGHT };

	if (node->value)
	{
		unsigned short valueLength = strnlen_s(node->value, MAX_NODE_STRING_LENGTH);

		fwrite(&valueLength, sizeof(unsigned short), 1, outputStreamContext);
		fwrite(node->value, sizeof(char), valueLength, outputStreamContext);
	}
	else
	{
		fwrite(sides + NULL_VALUE_OFFSET, sizeof(char), 1, outputStreamContext);
	}

	if (serializeSideNodeIfNeccesary(node->lhs, sides + LEFT_SIDE_OFFSET) == false)
		return false;

	if (serializeSideNodeIfNeccesary(node->rhs, sides + RIGHT_SIDE_OFFSET) == false)
		return false;

	if (isHasChildNodes(node) == false)
		fwrite(sides + NULL_VALUE_OFFSET, sizeof(char), 1, outputStreamContext);

	if (ferror(outputStreamContext))
	{
		perror("Binary tree serialization failed");
		return false;
	}

	return true;
}

bool serializeBinaryTree(const BinaryTree* root, FILE* binaryFile) 
{
	assert(root != NULL);
	assert(binaryFile != NULL);

	outputStreamContext = binaryFile;

	fwrite(binaryTreeFilePrefix, sizeof(char), sizeof(binaryTreeFilePrefix) / sizeof(char), binaryFile);

	if (ferror(binaryFile))
	{
		perror("Binary tree serialization failed");
		return false;
	}

	return serializeBinaryTreeRecursive(root);
}

bool isPrefixCorrect(const char prefix[3]) 
{
	for (int i = 0; i < (sizeof(binaryTreeFilePrefix) / sizeof(char)); i++)
		if (prefix[i] != binaryTreeFilePrefix[i])
			return false;

	return true;
}

inline char* initString(unsigned short length) 
{
	char* result = malloc(length + 1);

	if (result == NULL)
		perror("Memory allocation failed");
	else
		result[length] = '\0';

	return result;
}

BinaryTree* deserializeBinaryTreeRecursive() 
{
	BinaryTree* node = initBinaryNode();

	if (node == NULL)
		return NULL;

	unsigned short valueLength;

	fread(&valueLength, sizeof(valueLength), 1, inputStreamContext);

	if (ferror(inputStreamContext) || feof(inputStreamContext)) 
	{
		perror("Binary tree deserialization failed");
		free(node);
		return NULL;
	}

	node->value = (valueLength > 0 ? initString(valueLength) : NULL);

	if (node->value != NULL)
		fread(node->value, sizeof(char), valueLength, inputStreamContext);
	
	char tempNodeSide;

	if (fread(&tempNodeSide, sizeof(char), 1, inputStreamContext) < sizeof(char))
	{
		perror("Binary tree deserialization failed");

		if (node->value)
			free(node->value);

		free(node);

		return NULL;
	}

	NodeSide nextNodeSide = tempNodeSide;

	switch (nextNodeSide)
	{
	case NS_LEFT:
		node->lhs = deserializeBinaryTreeRecursive();

		fread(&tempNodeSide, sizeof(char), 1, inputStreamContext);

		nextNodeSide = tempNodeSide;

	case NS_RIGHT:
		node->rhs = deserializeBinaryTreeRecursive();
		break;
	default:
		break;
	}

	return node;
}

BinaryTree* deserializeBinaryTree(FILE* binaryFile) 
{
	assert(binaryFile != NULL);

	inputStreamContext = binaryFile;

	char prefix[3];

	fread(prefix, sizeof(char), sizeof(binaryTreeFilePrefix) / sizeof(char), binaryFile);

	if (ferror(binaryFile))
	{
		perror("Binary tree deserialization failed");
		return NULL;
	}

	if (isPrefixCorrect(prefix) == false)
	{
		printf("Binary tree file is empty or damaged\n");
		return NULL;
	}

	return deserializeBinaryTreeRecursive();
}
