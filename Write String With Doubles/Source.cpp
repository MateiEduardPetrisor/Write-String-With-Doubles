#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#pragma warning(disable:4996)

union convertUnion
{
	long long longLongValue;
	double doubleValue;
};

char* convertStringToHex(char* stringToConvert)
{
	char* convertedString = (char*)malloc(sizeof(char) * 2 * strlen(stringToConvert) + 1);
	size_t len = strlen(stringToConvert);
	for (size_t i = 0; i < len; i++) {
		sprintf(convertedString + i * 2, "%02X", stringToConvert[i]);
	}
	return convertedString;
}

void reverseString(char* &stringToReverse)
{
	_strrev(stringToReverse);
}

double convertHexToDouble(char* hexStringToConvert)
{
	convertUnion converter;
	converter.longLongValue = strtoll(hexStringToConvert, NULL, 16);
	return converter.doubleValue;
}

int checkMachineEndianess()
{
	unsigned int testInteger = 1;
	char *charTest = (char*)&testInteger;
	if (*charTest)
	{
		return 1; // little - endian
	}
	else
	{
		return 0; // big - endian
	}
}

size_t computeNumberOfTokens(char* stringToSplit)
{
	size_t nrTokens = 0;
	size_t stringLength = strlen(stringToSplit) + 1;
	if (stringLength % 8 == 0)
	{
		nrTokens = stringLength / 8;
	}
	else
	{
		nrTokens = stringLength / 8 + 1;
	}
	return nrTokens;
}

char** splitStringInTokens(char* stringToSplit)
{
	size_t numberOfTokens = computeNumberOfTokens(stringToSplit);
	char** tokensArray = (char**)malloc(sizeof(char*) * numberOfTokens);
	for (size_t i = 0; i < numberOfTokens; i++)
	{
		tokensArray[i] = (char*)malloc(sizeof(char) * 8 + 1);
		memcpy_s(tokensArray[i], 8, stringToSplit, 8);
		char* tokenPtr = tokensArray[i];
		tokenPtr += 8;
		*tokenPtr = '\0';
		stringToSplit += 8;
	}
	return tokensArray;
}

char** convertTokensToHex(size_t numberOfTokens, char** tokensArray)
{
	char** tokensHexArray = (char**)malloc(sizeof(char*) * numberOfTokens);
	for (int i = 0; i < numberOfTokens; i++)
	{
		tokensHexArray[i] = convertStringToHex(tokensArray[i]);
	}
	return tokensHexArray;
}

double* convertTokensToDouble(size_t numberOfTokens, char** tokensHexArray)
{
	double* resultTokensDouble = (double*)malloc(sizeof(double) * numberOfTokens);
	for (int i = 0; i < numberOfTokens; i++)
	{
		resultTokensDouble[i] = convertHexToDouble(tokensHexArray[i]);
	}
	return resultTokensDouble;
}

size_t computeMemoryBlockLength(size_t numberOfTokens, char** tokensArray)
{
	size_t memoryBlockLength = 0;
	for (size_t i = 0; i < numberOfTokens; i++)
	{
		memoryBlockLength += strlen(tokensArray[i]);
	}
	memoryBlockLength = memoryBlockLength + 1;
	return memoryBlockLength;
}

void copyTokensToMemoryBlock(size_t numberOfTokens, char** tokensArray, double* resultTokensDouble, void* memoryBlock)
{
	memcpy_s(memoryBlock, strlen(tokensArray[0]), (void*)&resultTokensDouble[0], strlen(tokensArray[0]));
	char* memoryBlockPtr = (char*)memoryBlock;
	memoryBlockPtr += strlen(tokensArray[0]);
	for (size_t i = 1; i < numberOfTokens; i++)
	{
		memcpy_s(memoryBlockPtr, strlen(tokensArray[i]), (void*)&resultTokensDouble[i], strlen(tokensArray[i]));
		memoryBlockPtr += strlen(tokensArray[i]);
	}
	*memoryBlockPtr = '\0';
}

void printStringTokens(size_t numberOfTokens, char** tokensArray)
{
	for (size_t i = 0; i < numberOfTokens; i++)
	{
		printf("Token[%zu] %s\n", i, tokensArray[i]);
	}
	printf("\n");
}

void printDoubleTokens(size_t numberOfTokens, double* doublesTokensArray)
{
	for (size_t i = 0; i < numberOfTokens; i++)
	{
		printf("Token Double[%zu] %.*e \n", i, DECIMAL_DIG, doublesTokensArray[i]);
	}
}

void freeMemoryBlock(void* *memoryBlock)
{
	free(memoryBlock);
	memoryBlock = NULL;
}

void freeArray(size_t arraySize, void*** arrayTofree)
{
	for (size_t i = 0; i < arraySize; i++)
	{
		freeMemoryBlock(arrayTofree[i]);
	}
	free(arrayTofree);
	arrayTofree = NULL;
}

int main()
{

	char stringToConvert[] = "The quick brown fox jumps over the lazy dog";
	size_t numberOfTokens = computeNumberOfTokens(stringToConvert);
	char** tokensArray = splitStringInTokens(stringToConvert);

	printf("~~~ String Tokens ~~~\n");
	printStringTokens(numberOfTokens, tokensArray);

	if (checkMachineEndianess())
	{
		for (int i = 0; i < numberOfTokens; i++)
		{
			reverseString(tokensArray[i]);
		}
	}

	printf("~~~ String Tokens After Endianness ~~~\n");
	printStringTokens(numberOfTokens, tokensArray);

	printf("~~~ String Tokens In Hex ~~~\n");
	char** tokensHexArray = convertTokensToHex(numberOfTokens, tokensArray);
	printStringTokens(numberOfTokens, tokensHexArray);

	printf("~~~ String Tokens In Double ~~~\n");
	double* resultTokensDouble = convertTokensToDouble(numberOfTokens, tokensHexArray);
	printDoubleTokens(numberOfTokens, resultTokensDouble);

	size_t memoryBlockLength = computeMemoryBlockLength(numberOfTokens, tokensArray);
	void* memoryBlock = (void*)malloc(memoryBlockLength);
	copyTokensToMemoryBlock(numberOfTokens, tokensArray, resultTokensDouble, memoryBlock);

	printf("~~~ The String Written With Doubles ~~~\n");
	puts((char*)memoryBlock);

	freeArray(numberOfTokens, (void***)tokensArray);
	freeArray(numberOfTokens, (void***)tokensHexArray);
	freeMemoryBlock((void**)resultTokensDouble);
	freeMemoryBlock((void**)memoryBlock);

	getchar();

	return 0;
}