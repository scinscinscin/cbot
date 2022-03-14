#include "Vector.h"

ArrayPtr createGenericArray(int initialLength, size_t elementSize){
	ArrayPtr newArr = malloc(sizeof(struct GenericArray));
	
	newArr->elementSize = elementSize;
	newArr->length = 0;
	newArr->capacity = initialLength;
	newArr->arr = malloc(elementSize * initialLength);

	#ifdef PRINT_NEW_VECTOR_MSG
	printf("Created a new generic array with %d element(s) and whose elementSize is %ld\n", initialLength, elementSize);
	#endif
	
	return newArr;
}
void ArrPush(ArrayPtr array, void *newElement){
	if(array->length == array->capacity){
		// increase the capacity of the array
		array->capacity *= 2;
		array->arr = realloc(array->arr, array->capacity * array->elementSize);
	}
	
	memcpy(array->arr + (array->length * array->elementSize), newElement, array->elementSize);
	array->length++;
}

void *GetArray(ArrayPtr array, int idx){
	if(idx >= array->length || idx < 0) return NULL;
	return array->arr + (idx * array->elementSize);
}