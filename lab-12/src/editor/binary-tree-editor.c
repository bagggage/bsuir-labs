#include "binary-tree-editor.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>

#include "../application.h"
#include "../input.h"

#define MAX_DEPTH 2
#define MAX_COMMAND_BUFFER 64

void redrawEditor(BinaryTreeNode* root) 
{
	system("cls");

	drawBinaryTree(root);

	rewind(stdin);
}

void convertStringToLowerCase(char* string) 
{
	int i = 0;

	while (string[i] != '\0')
	{
		string[i] = tolower(string[i]);

		i++;
	}
}

void drawCommandsList()
{
	printf("\nCommands:\n");
	printf("\n\t[ select ]\n\t[ edit ]\n\t[ delete ]\n\t[ insert ]\n\t[ enter ]\n\t[ help ]\n\t[ back ]\n\t[ exit ]\n");
}

CommandCode readCommand(BinaryTreeNode* root, bool shouldDrawCommandsList) 
{
	char commandBuffer[MAX_COMMAND_BUFFER] = { '\0' };

	bool shouldDisplayUnknownCommandMessage = false;

	CommandCode code = CC_UNKNOWN;
	
	while (code == CC_UNKNOWN)
	{
		redrawEditor(root);

		if (shouldDrawCommandsList)
		{
			drawCommandsList();

			shouldDrawCommandsList = false;
		}
		else if (shouldDisplayUnknownCommandMessage)
		{
			printf("\nUnknown command, use \'help\' to see full list of supported commands\n");
		}

		printf("\nEnter command: ");

		while (gets_s(commandBuffer, sizeof(commandBuffer)) == 0)
		{
			redrawEditor(root);

			printf("\nEnter command: ");
		}

		convertStringToLowerCase(commandBuffer);

		if (strcmp(commandBuffer, "select") == 0)
			code = CC_SELECT;
		else if (strcmp(commandBuffer, "delete") == 0)
			code = CC_DELETE;
		else if (strcmp(commandBuffer, "edit") == 0)
			code = CC_EDIT;
		else if (strcmp(commandBuffer, "insert") == 0)
			code = CC_INSERT;
		else if (strcmp(commandBuffer, "enter") == 0)
			code = CC_ENTER;
		else if (strcmp(commandBuffer, "back") == 0)
			code = CC_BACK;
		else if (strcmp(commandBuffer, "help") == 0)
			code = CC_HELP;
		else if (strcmp(commandBuffer, "exit") == 0)
			code = CC_EXIT;
		else
			shouldDisplayUnknownCommandMessage = true;	
	}

	return code;
}


BinaryTreeNode* selectNodeRecursive(BinaryTreeNode* node, int depth, int* id, int targetId) 
{
	if (depth > MAX_DEPTH) 
	{
		if (*id == targetId)
			return node;

		return NULL;
	}

	if (*id == targetId)
		return node;

	BinaryTreeNode* temp = NULL;

	if (node->lhs)
	{
		(*id)++;

		temp = selectNodeRecursive(node->lhs, depth + 1, id, targetId);
	}

	if (temp)
	{
		return temp;
	}
	else if (node->rhs)
	{
		(*id)++;

		return selectNodeRecursive(node->rhs, depth + 1, id, targetId);
	}

	return NULL;
}

BinaryTreeNode* selectNodeById(BinaryTreeNode* root, int id) 
{
	int currentId = 0;

	return selectNodeRecursive(root, 0, &currentId, id);
}

BinaryTreeNode* selectCmd(BinaryTreeNode* root)
{
	printf("Enter node ID: ");

	int id = INT_MAX;

	BinaryTreeNode* temp = NULL;

	while (temp == NULL)
	{
		if (id != INT_MAX)
			printf("Node with this id does not exist, try again: ");

		while (scanf_s("%d", &id) < 1 || id < 0 || id > powf(2, MAX_DEPTH + 2))
		{
			printf("Incorrect input, ID should be a positive number, max value is ID of the last bottom node: ");

			rewind(stdin);
		}

		temp = selectNodeById(root, id);
	}

	return temp;
}

void deleteCmd(BinaryTreeNode* node)
{
	assert(node != NULL);

	BinaryTreeNode* branchToDelete;

	if (node->lhs == NULL || node->rhs == NULL)
	{
		branchToDelete = (size_t)node->lhs | (size_t)node->rhs;

		node->lhs = NULL;
		node->rhs = NULL;
	}
	else
	{
		NodeSide side = readNodeSide("Choose branch to delete, left or right?");

		if (side == NS_LEFT)
		{
			branchToDelete = node->lhs;
			node->lhs = NULL;
		}
		else
		{
			branchToDelete = node->rhs;
			node->rhs = NULL;
		}
	}

	printf("Are you sure you want to delete the \"%s\" branch?\n(y/n): ", branchToDelete->value);

	bool answer = readAnswer();

	if (answer)
		destroyBinaryBranch(&branchToDelete);
}

void editCmd(BinaryTreeNode* node)
{
	assert(node != NULL);

	printf("Enter new value for \"%s\" node: ", node->value);

	char* newValue = readString();

	if (newValue == NULL)
	{
		waitForAnyKeyToContinue();
		return;
	}

	if (node->value)
		free(node->value);


	node->value = newValue;
}

NodeSide readNodeSide(const char* message) 
{
	printf("%s\n (L|R): ", message);

	char answer;

	while (scanf_s("%c", &answer, 1) < 1 || (tolower(answer) != 'l' && tolower(answer) != 'r'))
	{
		printf("Incorrect input (L|R): ");
		rewind(stdin);
	}

	if (tolower(answer) == 'l')
		return NS_LEFT;
	else
		return NS_RIGHT;
}

void insertCmd(BinaryTreeNode** node)
{
	assert(node != NULL);

	printf("Enter value for new node: ");

	char* newValue = readString();

	if (newValue == NULL)
	{
		waitForAnyKeyToContinue();
		return;
	}

	NodeSide sideToAttach = readNodeSide("Choose a side to attach Left(Yes) or Right(No)");
	NodeSide sideToReroute = 0;

	if ((sideToAttach == NS_LEFT && (*node)->lhs != NULL) || (sideToAttach == NS_RIGHT && (*node)->rhs != NULL))
		sideToReroute = readNodeSide("Choose a side to reroute currently attached branch, Left(Yes) or Right(No)");

	attachNode(node, newValue, sideToAttach, sideToReroute);
}

void processEditor() 
{
	BinaryTreeNode* selectedNode = NULL;
	BinaryTreeNode* currentRoot = getApplication()->dataBase;

	bool shouldDrawCommandsList = false;

	while (true)
	{
		CommandCode command = readCommand(currentRoot, shouldDrawCommandsList);

		shouldDrawCommandsList = false;

		switch (command)
		{
		case CC_SELECT:
			selectedNode = selectCmd(currentRoot);
			break;
		case CC_DELETE:
			if (selectedNode == NULL) {
				printf("You have to select node before");
				waitForAnyKeyToContinue();
				break;
			}

			if (isHasChildNodes(selectedNode) == false)
			{
				printf("Can't apply delete command, because this node doesn't have children branches");
				waitForAnyKeyToContinue();
				break;
			}

			deleteCmd(selectedNode);

			break;
		case CC_EDIT:
			if (selectedNode == NULL) {
				printf("You have to select node before");
				waitForAnyKeyToContinue();
				break;
			}

			editCmd(selectedNode);

			break;
		case CC_INSERT:
			if (selectedNode == NULL) {
				printf("You have to select node before");
				waitForAnyKeyToContinue();
				break;
			}

			insertCmd(&selectedNode);

			break;
		case CC_ENTER:
			if (selectedNode != NULL)
				currentRoot = selectedNode;
			break;
		case CC_HELP:
			shouldDrawCommandsList = true;
			break;
		case CC_BACK:
			currentRoot = getApplication()->dataBase;

			selectedNode = NULL;
			break;
		case CC_EXIT:
			return;
			break;
		default:
			break;
		}
	}
}

void spacing(int length) 
{
	for (int i = 0; i < length; i++)
	{
		if (i > 0)
			printf(".");

		printf("\t");
	}
}

void horizontalLine(int lenght) 
{
	for (int i = 0; i < lenght; i++)
		printf(". ");

	printf("\\\n");
}

void drawNodeInsideRect(const BinaryTreeNode* node, int depth, int* sequenceLength, int id, bool isHasParent, NodeSide nodeSide)
{
	if (isHasParent && nodeSide != 0 && nodeSide != NS_BOTH)
	{
		spacing(depth);

		printf("%s\n", (nodeSide == NS_LEFT ? "Yes" : "No"));

		spacing(depth);

		printf("|\n");
	}

	spacing(depth);

	int stringLength = (node->value ? strlen(node->value) : sizeof("(null)"));

	printf("#");

	for (int i = 0; i < stringLength; i++)
		printf("-");

	printf("#\n");

	spacing(depth);

	printf("|%s| [%d]\n ", node->value, id);

	spacing(depth);

	printf("#");

	for (int i = 0; i < stringLength; i++)
		printf("-");

	printf("#\n");

	*sequenceLength += stringLength + 3;
}

void printSubtree(const BinaryTreeNode* node, int depth, int* maxDepth, int* id, bool isHasParent, NodeSide nodeSide)
{
	if (depth > MAX_DEPTH)
	{
		spacing(depth);
		
		if (nodeSide == NS_LEFT)
			printf("Yes: ");
		else
			printf("No: ");

		printf("(%s) [%d]", node->value, *id);

		return;
	}

	drawNodeInsideRect(node, depth, maxDepth, *id, isHasParent, nodeSide);

	if (node->lhs != NULL)
	{
		int leftSequenceLength = 0;

		(*id)++;

		printSubtree(node->lhs, depth + 1, &leftSequenceLength, id, true, NS_LEFT);

		if (depth + 1 <= MAX_DEPTH)
		{
			if (isHasChildNodes(node->lhs))
			{
				spacing(depth + 1);
				horizontalLine(leftSequenceLength);
			}
		}
		else 
		{
			if (isHasChildNodes(node->lhs))
				printf(" [+]");

			printf("\n");
		}
	}

	if (node->rhs != NULL)
	{
		int rigthSequenceLength = 0;

		(*id)++;

		printSubtree(node->rhs, depth + 1, &rigthSequenceLength, id, true, NS_RIGHT);

		if (depth + 1 <= MAX_DEPTH)
		{
			if (isHasChildNodes(node->rhs))
			{
				spacing(depth + 1);
				horizontalLine(rigthSequenceLength);
			}
		}
		else 
		{
			if (isHasChildNodes(node->rhs))
				printf(" [+]");

			printf("\n");
		}
	}
}

void drawBinaryTree(const BinaryTree* root)
{
	assert(root != NULL);

	int maxSequenceLengthTemp = 0;
	int id = 0;

	printSubtree(root, 0, &maxSequenceLengthTemp, &id, false, 0);
}