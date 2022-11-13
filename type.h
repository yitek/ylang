#pragma once

#ifndef __CTYPE_H__
#define __CTYPE_H__

#include "def.h"

#define TypeKind_Object 0
#define TypeKind_Int 1

#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct TStr;
	typedef struct TArray;



	typedef struct CType {
		size_t size;
		int kind;
		struct TStr* name;
	} Type;
	typedef union TValue {
		byte members[0];
		byte buffer[0];
		byte byteValue;
		int intValue;
		boolean boolValue;
		float floatValue;
		double doubleValue;
		void* pointerValue;
	} Value;

	typedef struct TObject {
		const Type* type;
		/// <summary>
		/// 这部分就是(必须)拷贝 TValue
		/// </summary>
		union {
			byte members[0];
			byte buffer[0];
			byte byteValue;
			int intValue;
			float floatValue;
			double doubleValue;
			void* pointerValue;
		};
	} Object;

	typedef struct TArgument {
		const Type* type;
		void* value;
	}Argument;

	inline Object* handle(void* obj) {
		return (Object*)obj - 1;
	}

	inline const Type* const type(void* obj) {
		return handle(obj)->type;
	}

	void* invoke(const void*const fn, const Object**const args,unsigned int argc,const Type*const retType);

	extern const Type* const typeInt;

#ifdef __cplusplus 
} // end extern "C"
#endif

#endif // !__CTYPE_H__