#include "type.h"
#include <ffi.h>
#include <malloc.h>



const Type intType = {
    .kind = TypeKind_Int,
    .size = sizeof(int)
};

const Type* const typeInt = &intType;


void* invoke(const void*const fn, const Object** const args, unsigned int argc, const Type*const retType) {
    ffi_cif cif;
    ffi_type** argTypes = (ffi_type**)_malloca(sizeof(ffi_type*) * argc);
    void** argValues = (void**)_malloca(sizeof(void*) * argc);
    ffi_arg rc;
    /* Initialize the argument info vectors */
    for (unsigned int i = 0; i < argc; i++) {
        Object* arg = args[i];
        if (arg->type == typeInt) {
            argTypes[i] = &ffi_type_sint;
            //argValues[i] = &arg.intValue;
        }
        else {
            argTypes[i] = &ffi_type_pointer;
            //argValues[i] = arg.members;
        }
        argValues[i] = arg->members;
    }
    
    
    /* Initialize the cif */
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, argc,
        &ffi_type_sint, argTypes) == FFI_OK)
    {
        ffi_call(&cif, (void(*)())fn, &rc, argValues);
        /* rc now holds the result of the call to puts */
        //ÄÃµ½·µ»ØÖµ
        return rc;
        /* values holds a pointer to the function's arg, so to
           call puts() again all we need to do is change the
           value of s */
        // ffi_call(&cif, (void(*)())fn, &rc, argValues);
    }
    return 0;
}