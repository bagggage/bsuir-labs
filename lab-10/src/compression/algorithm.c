#include "algorithm.h"

#include <string.h>
#include <assert.h>

#include "../exception.h"

typedef struct WordCount 
{
	STACK_ELEMENT_BODY

	const char* word;
	unsigned int cout;

	size_t length;
	size_t size;
} WordCount;

typedef struct FindPairIterationData 
{
	Stack* stack;
	Dictionary* dictionary;
	WordCount* currentWord;
	WordCount* pair;
	int profit;

} FindPairIterationData;

BOOL isLetter(char c) 
{
	return 
		(
			(c >= 'A' && c <= 'Z') ||
			(c >= 'a' && c <= 'z') ||
			(c >= 'À' && c <= 'ÿ')
		);
}

BOOL isWord(const char* string) 
{
	for (int i = 0; string[i] != '\0'; i++)
		if (isLetter(string[i]) == FALSE)
			return FALSE;

	return TRUE;
}

int increaseCountOfWordInStack(WordCount* element, const char* word) 
{
	if (strcmp(element->word, word) == 0)
	{
		element->cout++;
		element->size += element->length;

		return 0;
	}

	return 1;
}

inline char* stringCopy(const char* source, size_t size) 
{
	char* copy = calloc(size + 1, sizeof(char));

	if (copy == NULL)
		throwException(1, "Memory allocation failed");

	for (int i = 0; source[i] != '\0'; i++)
		copy[i] = source[i];

	return copy;
}

inline void coutWordsInStringWithStack(Stack* stack, const char* string) 
{
	static const char separators[] = " ,.?!\"\'/:;-*+=(){}[]_\\|";

	char* lexem = strtok(string, separators);

	while (lexem != NULL)
	{
		if (isWord(lexem))
			if (stackIterate(stack, lexem, &increaseCountOfWordInStack))
			{
				WordCount wordCount = { .cout = 1, .length = strlen(lexem) };
				wordCount.word = stringCopy(lexem, wordCount.length);
				wordCount.size = wordCount.length;

				stackPush(stack, &wordCount, sizeof(WordCount));
			}

		lexem = strtok(NULL, separators);
	}
}

void countWordsInFileWithStack(Stack* stack, FILE* file) 
{
	char currentString[2048] = { '\0' };
	int error = 0;

	while (fgets(currentString, sizeof(currentString), file))
		coutWordsInStringWithStack(stack, currentString);

	if ((error = ferror(file)) != 0)
		throwException(error, "File reading failed");
}

static int resultProfit = 0;

int searchForMaxProfitPair(WordCount* wordCount, FindPairIterationData* context) 
{
	if (context->currentWord == wordCount || dictionaryLookUp(context->dictionary, wordCount->word) != NULL)
		return 1;

	int currentProfit =
		(context->currentWord->size +
		wordCount->size) -
		(wordCount->cout * context->currentWord->length +
		context->currentWord->cout * wordCount->length);

	if (currentProfit > context->profit)
	{
		context->profit = currentProfit;
		context->pair = wordCount;
	}

	return 1;
}

int searchForReplacementPairs(WordCount* wordCount, FindPairIterationData* context) 
{
	if (dictionaryLookUp(context->dictionary, wordCount->word) != NULL)
		return 1;

	context->currentWord = wordCount;
	context->profit = 0;
	context->pair = NULL;

	stackIterate(context->stack, context, &searchForMaxProfitPair);

	if (context->profit >= 16 && context->pair != NULL)
	{
		context->currentWord = context->pair;
		context->pair = NULL;

		stackIterate(context->stack, context, &searchForMaxProfitPair);

		if (context->pair == NULL)
		{
			context->pair = context->currentWord;
			context->currentWord = wordCount;
		}

		dictionaryInsert(context->dictionary, context->currentWord->word, context->pair->word);
		dictionaryInsert(context->dictionary, context->pair->word, context->currentWord->word);

		resultProfit += context->profit;
	}

	return 1;
}

void makeReplacementDictionary(Dictionary* dictionary, FILE* file) 
{
	assert(dictionary != NULL);
	assert(file != NULL);

	fseek(file, 0, SEEK_SET);

	Stack words = stackInit();

	countWordsInFileWithStack(&words, file);

	FindPairIterationData context;

	context.stack = &words;
	context.dictionary = dictionary;
	context.profit = 0;

	stackIterate(&words, &context, &searchForReplacementPairs);
	stackClear(&words);

	printf("Profit: %d\n", resultProfit);
}

const char* getCurrentWord(const char* string, const char* separators) 
{
	const char* lexem = string;

	while (lexem != NULL && isWord(lexem) == FALSE)
		lexem = strtok(string, separators);

	return lexem;
}

BOOL isSeparator(char c) 
{
	static const char separators[] = " ,.?!\"\'/:;-*+=(){}[]_\\|";

	for (int i = 0; i < sizeof(separators) / sizeof(char) - 1; i++)
		if (c == separators[i])
			return TRUE;

	return FALSE;
}

char* getLexem(const char* string) 
{
	char* lexemEnd = string;

	while (*lexemEnd != '\0' && isSeparator(*lexemEnd) == FALSE)
		lexemEnd++;

	return lexemEnd;
}

char* getWordFromLexem(const char* lexemBegin, const char* lexemEnd) 
{
	size_t size = lexemEnd - lexemBegin;
	char* lexem = calloc(size + 1, sizeof(char));

	if (lexem == NULL) 
		throwException(1, "Memory allocation failed");

	strncpy(lexem, lexemBegin, size);

	if (isWord(lexem))
	{
		return lexem;
	}
	else 
	{
		free(lexem);
		return NULL;
	}
}

void fileWriteDictionary(const Dictionary* dictionary, FILE* file) 
{
	Dictionary tempPairs = dictionaryInit();

	for (int i = 0; i < HASH_TABLE_SIZE; i++) 
	{
		Pair* pair = dictionary->hashTable[i];

		while (pair != NULL)
		{
			if (dictionaryLookUp(&tempPairs, pair->key) == NULL && dictionaryLookUp(&tempPairs, pair->value) == NULL)
			{
				fprintf(file, "%s/%s\n", pair->key, pair->value);

				dictionaryInsert(&tempPairs, pair->key, pair->value);
				dictionaryInsert(&tempPairs, pair->value, pair->key);
			}

			pair = pair->next;
		}
	}

	dictionaryDestroy(&tempPairs);

	fprintf(file, "$\n");
}

void fileReadDictionary(Dictionary* dictionary, FILE* file) 
{
	char string[1024] = { '\0' };

	while (fgets(string, 1024, file)) 
	{
		if (*string == '$')
			break;

		int i = 0;

		//TODO Issues handling

		while (string[i] != '/')
			i++;

		char* wordLhs = calloc(i + 1, sizeof(char));
		strncpy(wordLhs, string, i);

		int wordRhsStart = (++i);

		while (string[i] != '\n')
			i++;

		char* wordRhs = calloc(i - wordRhsStart + 1, sizeof(char));
		strncpy(wordRhs, string + wordRhsStart, i - wordRhsStart);

		dictionaryInsert(dictionary, wordLhs, wordRhs);
		dictionaryInsert(dictionary, wordRhs, wordLhs);
	}

	int error = 0;

	if ((error = ferror(file)) != 0)
		throwException(error, "File reading error");
}

void fillFileAndReplaceWords(Dictionary* dictionary, FILE* source, FILE* file) 
{
	char string[2048] = { '\0' };

	while (fgets(string, sizeof(string), source))
	{
		int i = 0;

		while (string[i] != '\0')
		{
			int startPos = i;

			while (isSeparator(string[i]))
				i++;

			if (i > 0)
				fwrite(string + startPos, sizeof(char), i - startPos, file);

			if (string[i] != '\0')
			{
				const char* lexemBegin = string + i;
				const char* lexemEnd = getLexem(lexemBegin);
				char* word;

				if ((word = getWordFromLexem(lexemBegin, lexemEnd)) != NULL)
				{
					const char* replacement;

					if ((replacement = dictionaryLookUp(dictionary, word)) != NULL)
						fwrite(replacement, sizeof(char), strlen(replacement), file);
					else
						fwrite(lexemBegin, sizeof(char), lexemEnd - lexemBegin, file);

					free(word);
				}
				else 
				{
					fwrite(lexemBegin, sizeof(char), lexemEnd - lexemBegin, file);
				}

				i += lexemEnd - lexemBegin;
			}
		}
	}

	int error = 0;

	if ((error = ferror(source)) != 0)
		throwException(error, "File reading error");
}
