#pragma once


#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "heap.h"

#ifdef __cplusplus 
extern "C" {
#endif

	typedef struct TArray {
		size_t length;
		byte buffer[0];
	}Array;


	inline Array* array_constructor(size_t length, size_t elemSize, size_t additionSize, THeap* heap) {
		Array* arr = (Array*)heap->alloc(sizeof(Array) + length * elemSize + additionSize);
		arr->length = length;
		return arr;
	}

	inline size_t array_length(Array*const self) {
		return self->length;
}

	inline const char* const array_cstr(Array*const self) {
		return self->buffer;
	}

	inline void* array_at(Array*const self, size_t at,size_t elemSize) {
		if (at >= self->length) return 0;
		return self->buffer + at * elemSize;
	}


	inline Array* array_concat(Array*const self, Array* other, size_t elemSize, size_t additionSize, THeap* heap) {
		if (self == 0) return other;
		if (other == 0) return self;
		size_t length = self->length + other->length;
		Array* arr = (Array*)heap->alloc(sizeof(Array) + length * elemSize + additionSize);
		arr->length = length;
		size_t selfSize = self->length * elemSize;
		heap->copy(self->buffer, arr->buffer, selfSize);
		heap->copy(other->buffer, arr->buffer + selfSize, other->length * elemSize);
		//*(byte*)(str->buffer + length) = 0;
		return arr;
	}

	inline Array* array_slice(Array*const self, size_t start, size_t length, size_t elemSize, size_t additionSize, THeap* heap) {
		if (self == 0) return 0;
		if (start + length >= self->length) return 0;
		Array* arr = (Array*)heap->alloc(sizeof(Array) + length * elemSize + additionSize);
		arr->length = length;
		size_t sliceSize = length * elemSize;
		heap->copy(self->buffer + start * elemSize, arr->buffer, length * elemSize);
		//*(char*)(arr->buffer + str->length) = 0;
		return arr;
	}

#ifdef __cplusplus 
} // end extern "C"
#endif

#endif //!__ARRAY_H__
