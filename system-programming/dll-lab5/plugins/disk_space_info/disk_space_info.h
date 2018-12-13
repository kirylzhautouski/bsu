#ifdef DISKSPACEINFO_EXPORTS
#define DISKSPACEINFO_API __declspec(dllexport) 
#else
#define DISKSPACEINFO_API __declspec(dllimport) 
#endif

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	DISKSPACEINFO_API BOOLEAN __stdcall GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);

	DISKSPACEINFO_API BOOLEAN __stdcall GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);

	DISKSPACEINFO_API VOID __stdcall Execute();

#ifdef __cplusplus
}
#endif
