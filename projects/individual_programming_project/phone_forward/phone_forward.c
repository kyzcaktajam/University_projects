#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 12
#define ZERO_ID 48

struct PhoneNumbers {
	int size;
	int allocSize;
	char **numbers;
};

typedef struct PhoneNumbers PhoneNumbers;

struct PhoneForward {
	struct PhoneForward *children[ALPHABET_SIZE];
	char *newPrefix;
};

typedef struct PhoneForward PhoneForward;

// Struktura przechowująca stos cyfr.
struct Stack {
	int size;
	int allocSize;
	char *digits;
};

typedef struct Stack Stack;

PhoneForward * phfwdNew(void) {
	PhoneForward *newPhfwd = NULL;
	newPhfwd = (PhoneForward *)malloc(sizeof (PhoneForward));
	if (!newPhfwd) return NULL;
    newPhfwd->newPrefix = NULL;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		newPhfwd->children[i] = NULL;
	}
	return newPhfwd;
}

void phfwdDelete(PhoneForward *pf) {
	if (!pf) return;
	if (pf->newPrefix) free(pf->newPrefix);
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		phfwdDelete(pf->children[i]);
	}
	free(pf);
}

// Funkcja zwracająca true, jeżeli znak jest cyfrą, w przeciwnym wypadku false.
static bool isDigit(char x) {
	if (x >= '0' && x <= '9') return true;
	if (x == '*' || x == '#') return true;
	return false; 
}

// Funkcja zwracająca true, jeżeli ciąg znaków jest poprawnym numerem, 
// w przeciwnym wypadku zwraca false.
static bool isCorrect(char const *num) {
	if (!num) return false;
	int i = 0;
	while (num[i] != '\0'){
		if (!isDigit(num[i])) return false;
		i++;
	}
	if (i == 0) return false;
	return true;
}

// Funkcja zwracająca wartość cyfry reprezentowanej przez znak. 
static int getId(char x) {
	if (x == '*')
	    return 10;
	if (x == '#')
	    return 11;
	return (int)x - ZERO_ID;
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
	if (!isCorrect(num1) || !isCorrect(num2) || strcmp(num1, num2) == 0) {
	    return false;
	}
	if (!pf) 
	    return false;
	PhoneForward *currentNode = pf;
	int len = (int)strlen(num1);
	PhoneForward *firstAlloc = NULL;
	int firstId = 0;
	for (int i = 0; i < len; ++i) {
		int id = getId(num1[i]);
		if (!currentNode->children[id]) {
			currentNode->children[id] = phfwdNew();
			if (!currentNode->children[id]) {
				if (firstAlloc) {
				    phfwdDelete(firstAlloc->children[firstId]);
				    firstAlloc->children[firstId] = NULL;
			    }
			    return false;
			}
			if (!firstAlloc) {
				firstAlloc = currentNode;
				firstId = id;
			}
		}
		currentNode = currentNode->children[id];
	}
	char *s = NULL;
	len = (int)strlen(num2);
	s = (char *)malloc(len + 1);
	if (!s && firstAlloc) {
		phfwdDelete(firstAlloc->children[firstId]);
		firstAlloc->children[firstId] = NULL;
	}
	if (!s) 
	    return false;
	strcpy(s, num2);
	if (currentNode->newPrefix)
	    free(currentNode->newPrefix);
	currentNode->newPrefix = s;
	return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
	if (!pf || !num) 
	    return;
	if (!isCorrect(num))
		return;

	PhoneForward *currentNode = pf;
	int len = (int)strlen(num);
	int id = 0;
	for (int i = 0; i < len; i++) {
		id = getId(num[i]);
		if(!currentNode->children[id])
		    return;
		if (i != len - 1)
		    currentNode = currentNode->children[id];
	}
		
	phfwdDelete(currentNode->children[id]);
	currentNode->children[id] = NULL;
}

void phnumDelete(PhoneNumbers *pnum) {
	if(!pnum)
	    return;
	if (pnum -> numbers) {
	  for(int i = 0; i < pnum->allocSize; i++) {
		  if (pnum->numbers[i]) free(pnum->numbers[i]);
	  }
	  free(pnum->numbers);
    }
	free(pnum);
}

// Funkcja zwracająca wskaźnik na nową zaalokowną strukturę Phonenumbers,
// zawierającą number num.
static PhoneNumbers * newPhnum (char const *num) {
	PhoneNumbers *pn = NULL;
	pn = (PhoneNumbers *)malloc(sizeof (PhoneNumbers));
	if (!pn) {
		return NULL;
	}
	if (!num) {
		pn->size = 0;
		pn->allocSize = 0;
		pn->numbers = NULL;
		return pn;
	}
	pn->size = 1;
	pn->allocSize = 1;
	int len = (int)strlen(num);
	pn->numbers = (char **)malloc(sizeof(char*));
	if (!pn->numbers) {
		phnumDelete(pn);
		return NULL;
	}
	pn->numbers[0] = (char *) malloc((len + 1) * sizeof(char));
	if (!pn->numbers[0]) {
		phnumDelete(pn);
		return NULL;
	}
	strcpy(pn->numbers[0], num);
	return pn;
}

PhoneNumbers * phfwdGet(PhoneForward const *pf, char const *num) {
	if (!pf) {
	    return NULL;
	}
	if (!isCorrect(num)) {
		return newPhnum(NULL);
	}
	PhoneForward *currentNode = (PhoneForward *)pf;
	char const * newPref = NULL;
	int prevLength = 0, len = strlen(num);

	for (int i = 0; i < len; ++i) {
		int id = getId(num[i]);
		if (!currentNode->children[id]) {
			break;
		}
		currentNode = currentNode -> children[id];
		if (currentNode->newPrefix) {
			prevLength = i + 1;
			newPref = currentNode -> newPrefix;
		}
	}
	short newLength = len;
	if (newPref)
	    newLength = strlen(num) + strlen(newPref) - prevLength;
	char * result = NULL ;
	result = (char *)malloc(newLength + 1);
	if (!result)
	    return NULL;
	if (newPref) { 
		int plen = strlen(newPref);
	    for (int i = 0; i < plen; i++) {
		    result[i] = newPref[i];
	    }
	    int shift = plen - prevLength;
	    for (int i = plen; i < newLength; i++) {
		    result[i] = num[i - shift];
	    }
	    result[newLength] = '\0';
    } 
    else {
		strcpy(result, num);
	}
	PhoneNumbers *res = newPhnum(result);
	free(result);
	return res;
}

char const * phnumGet(PhoneNumbers const *pnum, size_t idx) {
	if (!pnum || (int)idx >= (pnum->size)) return NULL;
	if ((int)idx < 0) return NULL;
	return pnum->numbers[idx];
}

// Funkcja odwrotnie przekierowująca numer num (zamienia prefiks newPref
// na stack).
static char * getForward (char *num, char *newPref, char *stack,
int numSz, int stackSz) {
	int newPrefSz = (int)strlen(newPref);
	if (newPrefSz > numSz) return NULL;
	for (int i = 0; i < newPrefSz; i++) {
		if (newPref[i] != num[i]) return NULL;
	}
	int newLen = numSz + stackSz - newPrefSz;
	char *result = NULL;
	result = (char *)malloc(newLen + 1);
	if (!result)
	    return "alloc_fail";
	for (int i = 0; i < stackSz; i++) {
		result[i] = stack[i];
	}
	int shift = newPrefSz - stackSz;
	for (int i = stackSz; i < newLen; i++) {
		result[i] = num[i + shift];
	}
	result[newLen] = '\0';
	return result;
}

// Funkcja zwracająca wskaźnik na nową zaalokowaną strukturę Stack, lub
// NULL, gdy alokacja się nie powiodła.
static Stack * newStack() {
	Stack *stack = NULL;
	stack = (Stack *)malloc(sizeof(Stack));
	if (!stack) {
		return NULL;
	}
	stack->size = 0;
	stack->allocSize = 1;
	stack->digits =(char *)malloc(sizeof(char));
	if (!stack->digits) {
		free(stack);
		return NULL;
	}
	return stack;
}

// Funkcja dodająca cyfrę x do stosu, zwraca true, jeżeli operacja się udała,
// w przeciwnym razie false.
static bool insertToStack (Stack *stack, char x) {
	if (stack->size == stack->allocSize) {
		char *newDigits = realloc(stack->digits, 2 * (stack->allocSize));
		if (!newDigits) {
		    return false;
	    }
	    stack->allocSize *= 2;
	    stack->digits = newDigits;
	}
	stack->digits[stack->size] = x;
	stack->size++;
	return true;
}

// Funkcja usuwająca pierwszą od góry cyfrę ze stosu.
static void deleteFromStack(Stack *stack) {
	if (stack->size == 0) return;
	stack->size--;
}

// Funkcja usuwająca stos.
static void deleteStack(Stack *stack) {
	if (!stack) {
		return;
	}
	if (stack->digits) free(stack->digits);
	free(stack);
}

// Funkcja dodająca numer do struktury PhoneNumbers. Zwraca true, jeżeli
// operacja udała się, w przeciwnym razie false.
static bool addToRes(PhoneNumbers *res, char *num) {
	if (res->allocSize == res->size) {
		char **newNumbers = 
		realloc(res->numbers, (2 * (res->allocSize)) * sizeof(char *));
		if (!newNumbers) {
			free(num);
			return false;
		}
		res->numbers = newNumbers;
		res->allocSize *=2;
		for (int i = res->size + 1; i < res->allocSize; i++) {
			newNumbers[i] = NULL;
		}
	}
	res->numbers[res->size] = num;
	res->size++;
	return true;
}

// Funkcja implementująca algorytm dfs przechodządy całe drzewo reprezentowane
// przez strukturę PhoneForward, w każdym wierzchołku wyznacza przekierowanie
// numeru num.
static void dfs(PhoneForward *pf, Stack *stack, PhoneNumbers* res, 
char const *num, int numSz, bool *fail) {
	if (pf->newPrefix) {
		char *rev = getForward((char *)num, pf->newPrefix, 
		stack->digits, numSz, stack->size);
		if (rev && strcmp(rev, "alloc_fail") == 0) {
			*fail = true;
			return;
		}
		if (rev) addToRes(res, rev);
	}
	
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		char digit = (char)(i + '0');
		if (i == 10) digit = '*';
		if (i == 11) digit = '#';
		if (pf->children[i]) {
			if (!insertToStack(stack, digit)) {
				*fail = true;
				return;
			}
			dfs(pf->children[i], stack, res, num, numSz, fail);
			deleteFromStack(stack);
		}
	}
}

// Funkcja porórnująca dwa napisy leksykograficznie(gdy str1 > str2 zwraca 1,
// gdy str1 = str2 zwraca 0, gdy str1 < str2 zwraca -1).
static int compare(const char * str1, const char * str2) {
	int i = 0;
	while(str1[i] != '\0' && str2[i] != '\0') {
		int dif = getId(str1[i]) - getId(str2[i]);
		if (dif > 0) return 1;
		if (dif < 0) return -1;
		i++;
	}
	if (str1[i] != '\0') {
		return 1;
	}
	if (str2[i] != '\0') {
		return -1;
	}
	return 0;
}

// Funkcja pomocnicza używana przez algorytm qsort.
static int _strcmp(const void* a, const void* b)
{
  return compare( *(const char**)a, *(const char**)b );
}

// Funkcja sortująca numery zawarte w strukturze PhoneNumbers leksykograficznie
// i usuwająca duplikaty.
static void phnumSort(PhoneNumbers *pn) {
	if (!pn) return;
	if (!pn->numbers || pn->size == 0) return;
	qsort(pn->numbers, pn->size, sizeof(pn->numbers[0]), _strcmp);
	int j = 1;
	int newSize = pn->size;
	
	for (int i = 1; i < pn->size; i++) {
		if (strcmp(pn->numbers[i], pn->numbers[i-1]) == 0) {
			newSize--;
		}
		else {
			if (j != i) {
				free(pn->numbers[j]);
			}
			pn->numbers[j] = pn->numbers[i];
			if (i != j) {
				pn->numbers[i] = NULL;
			}
			j++;
		}
	}
	for (int i = newSize; i < pn->size; i++) {
		free(pn->numbers[i]);
		pn->numbers[i] = NULL;
	}
	pn->size = newSize;
}

PhoneNumbers * phfwdReverse(PhoneForward const *pf, char const *num) {
	if (!pf)
	    return NULL;
	if (!isCorrect(num))
	    return newPhnum(NULL);
	PhoneNumbers *result = NULL;
	result = (PhoneNumbers*)malloc(sizeof(PhoneNumbers));
	if (!result) {
		return NULL;
	}
	result->numbers =(char **)malloc(sizeof(char *));
	if (!result->numbers) {
		free(result);
		return NULL;
	}
	result->numbers[0] = NULL;
	result->allocSize = 1;
	result->size = 0;
	Stack *stack = newStack();
	if (!stack) {
		phnumDelete(result);
		return NULL;
	}
	int numLen = (int) strlen(num);
	bool fail = 0;
	dfs((PhoneForward*)pf, stack, result, (char *)num, numLen, &fail);
	deleteStack(stack);
	if (fail) {
		phnumDelete(result);
		return NULL;
	}
	char *copy = NULL;
	copy = (char *)malloc(numLen + 1);
	if (!copy) {
		phnumDelete(result);
		return NULL;
	}
	strcpy(copy, num);
	if (addToRes(result, copy)) {
		phnumSort(result);
		return result;
	}
	phnumDelete(result);
	return NULL;
}

PhoneNumbers * phfwdGetReverse(PhoneForward const *pf, char const *num) {
	if (!pf) {
		return NULL;
	}
	if (!num || !isCorrect(num)) {
		return newPhnum(NULL);
	}
	PhoneNumbers *result = NULL;
	result = phfwdReverse(pf, num);
	if (!result) {
		return NULL;
	}
	if (result->size == 0) {
		return result;
	}
	int newSize = result->size;
	int j = 0;
	for (int i = 0; i < result->size; i++) {
		PhoneNumbers *forward = NULL; 
		forward = phfwdGet(pf, result->numbers[i]);
		if (!forward) {
			phnumDelete(result);
			return NULL;
		}
		if (strcmp(num, phnumGet(forward, 0)) != 0) {
			newSize--;
		}
		else {
			if (i != j) {
				free (result->numbers[j]);
			    result->numbers[j] = result->numbers[i];
				result->numbers[i] = NULL;
			}
			j++;
		}
		phnumDelete(forward);
	}
	for (int i = newSize; i < result->size; i++) {
		free (result->numbers[i]);
		result->numbers[i] = NULL;
	}
	result->size = newSize;
	return result;
}
