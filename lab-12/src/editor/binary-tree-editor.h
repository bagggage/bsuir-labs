#pragma once

#include "../binary-tree/binary-tree.h"

typedef enum CommandCode
{
	CC_UNKNOWN,
	CC_EXIT,
	CC_SELECT,
	CC_EDIT,
	CC_DELETE,
	CC_INSERT,
	CC_ENTER,
	CC_HELP,
	CC_BACK

} CommandCode;

void processEditor();

void drawBinaryTree(const BinaryTree* root);
