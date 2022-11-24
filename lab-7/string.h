#pragma once

char* initString(const char* text);
void destroyString(char** stringPtr);

char* readString();

int stringLength(const char* text);
void pushBackCharToString(char** string, char c);
void insertCharToString(char** string, int position, char c);
int countWordsInString(const char* string);