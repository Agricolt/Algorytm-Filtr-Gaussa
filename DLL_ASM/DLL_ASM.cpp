// DLL_ASM.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DLL_ASM.h"


// This is an example of an exported variable
DLL_ASM_API int nDLL_ASM=0;

// This is an example of an exported function.
DLL_ASM_API int fnDLL_ASM(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see DLL_ASM.h for the class definition
CDLL_ASM::CDLL_ASM()
{
    return;
}
