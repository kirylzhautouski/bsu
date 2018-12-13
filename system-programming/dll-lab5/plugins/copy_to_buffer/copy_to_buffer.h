#ifdef COPYTOBUFFER_EXPORTS
#define COPYTOBUFFER_API __declspec(dllexport) 
#else
#define COPYTOBUFFER_API __declspec(dllimport) 
#endif

#include <Windows.h>

#pragma comment(linker, "/export:CopyToBuffer=_CopyToBuffer@16")

#ifdef __cplusplus
extern "C" {
#endif
	
	COPYTOBUFFER_API BOOLEAN __stdcall CopyToBuffer(LPSTR buffer, const char* info, DWORD dwBufferSize, DWORD* pdwBytesWritten);

#ifdef __cplusplus
}
#endif