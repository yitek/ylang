#pragma once
#ifndef __DEF_H__
#define __DEF_H__

#ifdef __cplusplus 
extern "C" {
#else
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#endif
	typedef unsigned long long size_t;
	typedef int boolean;

	typedef char byte;
	typedef char unichar;

#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__HDEF_H__

