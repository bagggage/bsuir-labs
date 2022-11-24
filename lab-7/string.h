#pragma once

char* initString(const char* text);
void destroyString(char** stringPtr);

char* readString();

int stringLength(char* text);
void pushBackCharToString(char** string, char c);
int countWordsInString(char* string);