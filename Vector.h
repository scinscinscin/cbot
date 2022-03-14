#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct GenericArray{
	void* arr; // the actual data
	size_t elementSize; // the size of the elements that the array will contain, sizeof()
	int length; // the amount of elements that the array current has
	int capacity; // the amount of elements that the array can take
};
typedef struct GenericArray* ArrayPtr;
ArrayPtr createGenericArray(int initialLength, size_t elementSize);
void ArrPush(ArrayPtr array, void *newElement);
void *GetArray(ArrayPtr array, int idx);