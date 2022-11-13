#include "heap.h"
#include <malloc.h>


void* y_alloc(size_t size) {
	return malloc(size);
}

void y_free(void* pObj) {
	free(pObj);
}

void y_copy(const void* from,void*const to,size_t size) {
	byte* dest = to;
	for (size_t i = 0; i < size; i++) {
		*dest++ = *((byte*)from)++;
	}
}

Object y_newobj(const Type*const type) {
	size_t totalSize = type->size + sizeof(Type*);
	return *(Object*)malloc(totalSize);
}

static THeap defaultHeap = { 0, y_alloc, y_free, y_copy, y_newobj };

THeap* getHeap(void* pContext) {
	return &defaultHeap;
}