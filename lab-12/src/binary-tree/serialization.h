#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "binary-tree.h"

bool serializeBinaryTree(const BinaryTree* root, FILE* binaryFile);
BinaryTree* deserializeBinaryTree(FILE* binaryFile);
