// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLL_ASM_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL_ASM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLL_ASM_EXPORTS
#define DLL_ASM_API __declspec(dllexport)
#else
#define DLL_ASM_API __declspec(dllimport)
#endif

// This class is exported from the DLL_ASM.dll
class DLL_ASM_API CDLL_ASM {
public:
	CDLL_ASM(void);
	// TODO: add your methods here.
};

extern DLL_ASM_API int nDLL_ASM;

DLL_ASM_API int fnDLL_ASM(void);
