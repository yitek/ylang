#pragma once
#ifndef __STR_H__
#define __STR_H__
#include "array.h"

#ifdef __cplusplus 
extern "C" {
#endif

typedef struct TStr{
	size_t length;
	unichar buffer[0];
}Str;

Str* str_constructor(const char* const cstr, THeap* heap) {
	char* p = (char*)cstr;
	size_t length = 0;
	while (*p++ != 0) length++;
	Str* str = heap->alloc(sizeof(Str) + (length + 1) * sizeof(unichar));
	heap->copy(cstr, str->buffer, length * sizeof(unichar));
	*(str->buffer + length) = 0;
	return str;
}

size_t str_length(Str* self) {
	return self->length;
}

const char* const str_cstr(Str* self) {
	return self->buffer;
}

boolean str_equal(Str* self, Str* other) {
	if (self == 0) {
		if (other == 0) {
			return true;
		}
		else return false;
	}
	else {
		if (other == 0) return false;
		size_t len = self->length;
		if (len != other->length) return false;
		for (size_t i = 0; i < len; i++)
			if (self->buffer[i] != other->buffer[i]) return false;
		return true;
	}
}

int str_compare(Str* self, Str* other) {
	if (self == 0) {
		if (other == 0) {
			return 0;
		}
		else return -1;
	}
	else {
		if (other == 0) return 1;
		size_t len0 = self->length;
		size_t len1 = other->length;
		for (size_t i = 0; i < len0; i++) {
			if (i == len1) return 1;
			unichar c1 = self->buffer[i];
			unichar c2 = other->buffer[i];
			if (c1 > c2) return 1;
			if (c1 < c2) return -1;
		}
		if (len0 == len1) return 0;
		return -1;
	}
}

Str* str_concat(Str* self, Str* other, THeap* heap) {
	Str* str = (Str*)array_concat((Array*)self, (Array*)other, sizeof(unichar), sizeof(unichar), heap);
	*(str->buffer + str->length) = 0;
	return str;
}

Str* str_slice(Str* self, size_t start, size_t length, THeap* heap) {
	Str* str = (Str*)array_slice((Array*)self, start, length, sizeof(unichar), sizeof(unichar), heap);
	*(str->buffer + str->length) = 0;
	return str;
}


#ifdef __cplusplus 
} // end extern "C"
#endif

#endif //!__STR_H__
