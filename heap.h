#pragma once
#ifndef __HEAP_H__
#define __HEAP_H__

#include "def.h"
#include "type.h"

#ifdef __cplusplus 
extern "C"{ 
#endif



typedef void* (*FHeapAlloc)(size_t size);
typedef void (*FHeapFree)(void* pObj);
typedef void (*FHeapCopy)(const void* from,void*const to,size_t size);
typedef Object (*FHeapNewObj)(const Type*const type);

typedef struct YHeap {
	void* pContext;
	FHeapAlloc alloc;
	FHeapFree free;
	FHeapCopy copy;
	FHeapNewObj newobj;
} THeap;


THeap* getHeap(void* pContext);

#ifdef __cplusplus 
} // end extern "C"
#endif

#endif //!__HEAP_H__