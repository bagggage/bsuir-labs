#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <assert.h>

#include "../input.h"
#include "../application.h"
#include "../binary-tree/binary-tree.h"
#include "../binary-tree/serialization.h"

void redrawGameInterface(BinaryTreeNode* node) 
{
	system("cls");

	if (isHasChildNodes(node) == false)
		printf("\nIt is %s?\n", node->value);
	else
		printf("\n%s?\n", node->value);

	printf("Enter answer (y/n): ");
}

bool readAnswerAndDrawInterface(BinaryTreeNode* node) 
{
	char answer;

	rewind(stdin);

	while (_getch("%c", &answer, 1) < 1 || (tolower(answer) != 'y' && tolower(answer) != 'n'))
	{
		redrawGameInterface(node);

		printf("Incorrect input, answer in way (y/n): ");
		rewind(stdin);
	}

	return (tolower(answer) == 'y');
}

void readCorrectAnswerAndPushToDataBase(BinaryTreeNode* node) 
{
	printf("Who/What it was?\n");

	char* correctAnswer = readString();

	if (correctAnswer == NULL)
	{
		printf("Opss... Something went wrong :(\n");
		return;
	}

	printf("Please, enter carifying question for this person/thing: ");

	char* question = readString();

	if (question == NULL)
	{
		printf("Opss... Something went wrong (-_-') (((\n");
		return;
	}

	printf("What is answer on this quetion for %s?\n(y/n): ", correctAnswer);

	bool answer = readAnswer();

	BinaryTreeNode* wrongAnswerNode = initBinaryNode();
	BinaryTreeNode* answerNode = initBinaryNode();

	char* temp = node->value;

	node->value = question;
	wrongAnswerNode->value = temp;
	answerNode->value = correctAnswer;

	if (answer)
	{
		node->lhs = answerNode;
		node->rhs = wrongAnswerNode;
	}
	else 
	{
		node->lhs = wrongAnswerNode;
		node->rhs = answerNode;
	}
}

void processGame()
{
	bool shouldRepeat = false;

	do
	{
		BinaryTreeNode* currentNode = getApplication()->dataBase;

		while (currentNode != NULL)
		{
			redrawGameInterface(currentNode);

			bool answer = readAnswer();
			bool thereIsNoAnswer = false;

			if (isHasChildNodes(currentNode) == false)
			{
				if (answer)
				{
					printf("\nSUUU!!!!!!!\n");
					waitForAnyKeyToContinue();

					break;
				}
				else
				{
					thereIsNoAnswer = true;
				}
			}

			else if (answer)
			{
				if (currentNode->lhs)
					currentNode = currentNode->lhs;
				else
					thereIsNoAnswer = true;
			}
			else
			{
				if (currentNode->rhs)
					currentNode = currentNode->rhs;
				else
					thereIsNoAnswer = true;
			}

			if (thereIsNoAnswer) 
			{
				printf("\n:( Kak tak tooo???\n");
				readCorrectAnswerAndPushToDataBase(currentNode);

				break;
			}
		}

		system("cls");
		printf("Do you want to play again?\n(y/n): ");

		shouldRepeat = readAnswer();

	} while (shouldRepeat);
}