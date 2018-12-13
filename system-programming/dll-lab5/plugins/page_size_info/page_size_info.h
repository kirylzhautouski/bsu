#ifdef PAGESIZEINFO_EXPORTS
#define PAGESIZEINFO_API __declspec(dllexport) 
#else
#define PAGESIZEINFO_API __declspec(dllimport) 
#endif

#include <Windows.h>

#pragma comment(linker, "/export:GetAuthor=_GetAuthor@12")
#pragma comment(linker, "/export:GetDescription=_GetDescription@12")
#pragma comment(linker, "/export:Execute=_Execute@0")

#ifdef __cplusplus
extern "C" {
#endif

	PAGESIZEINFO_API BOOLEAN __stdcall GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);

	PAGESIZEINFO_API BOOLEAN __stdcall GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);

	PAGESIZEINFO_API VOID __stdcall Execute();

#ifdef __cplusplus
}
#endif