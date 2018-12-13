#ifdef PROCESSORINFO_EXPORTS
#define PROCESSORINFO_API __declspec(dllexport) 
#else
#define PROCESSORINFO_API __declspec(dllimport) 
#endif

#include <Windows.h>

#pragma comment(linker, "/export:GetAuthor=_GetAuthor@12")
#pragma comment(linker, "/export:GetDescription=_GetDescription@12")
#pragma comment(linker, "/export:Execute=_Execute@0")

#ifdef __cplusplus
extern "C" {
#endif

	PROCESSORINFO_API BOOLEAN __stdcall GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);

	PROCESSORINFO_API BOOLEAN __stdcall GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);

	PROCESSORINFO_API VOID __stdcall Execute();

#ifdef __cplusplus
}
#endif