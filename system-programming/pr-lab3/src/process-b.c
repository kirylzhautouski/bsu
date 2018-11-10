#define _UNICODE

#include <tchar.h>
#include <Windows.h>
#include <stdio.h>
#include <strsafe.h>

HANDLE hMutex;

HANDLE hSlot;
LPTSTR slot = TEXT("\\\\.\\mailslot\\process_b_mailslot");
LPCSTR mutexName = "mutex";

BOOL MakeSlot(LPTSTR lpszSlotName);
BOOL ReadSlot();

int _tmain()
{
    hMutex = CreateMutexA(NULL, TRUE, mutexName);
    if (hMutex == INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("CreateMutexA failed"));
        return 1;
    }

    BOOL makeSlotResult = MakeSlot(slot);
    if (makeSlotResult == (FALSE))
    {
        _tprintf(TEXT("Can't create mailslot"));
        CloseHandle(hMutex);

        return 2;
    }
    
    while (!ReadSlot())
    {
        Sleep(3000);
    }

    CloseHandle(hMutex);
    return 0;
}

BOOL MakeSlot(LPTSTR lpszSlotName)
{
    hSlot = CreateMailslot(lpszSlotName, 0, MAILSLOT_WAIT_FOREVER, (LPSECURITY_ATTRIBUTES)NULL);

    if (hSlot == (INVALID_HANDLE_VALUE))
        return FALSE;

    return TRUE;
}

BOOL ReadSlot()
{
    DWORD cbMessage, cMessage, cbRead;
    LPVOID address; 
    BOOL fResult; 
    HANDLE hEvent;
 
    cbMessage = cMessage = cbRead = 0; 
 
    fResult = GetMailslotInfo(hSlot, // mailslot handle 
        (LPDWORD) NULL,               // no maximum message size 
        &cbMessage,                   // size of next message 
        &cMessage,                    // number of messages 
        (LPDWORD) NULL);              // no read time-out 
 
    if (!fResult) 
    { 
        printf("GetMailslotInfo failed with %d.\n", GetLastError()); 
        return FALSE; 
    } 
 
    if (cbMessage == MAILSLOT_NO_MESSAGE) 
    { 
        printf("Waiting for a message...\n"); 
        return FALSE; 
    } 

    while (cMessage != 0)  // retrieve all messages
    { 
        DWORD dwBytesRead;
        fResult = ReadFile(hSlot, 
            &address, 
            sizeof(address), 
            &dwBytesRead,													
		    (LPOVERLAPPED)NULL); 
 
        if (!fResult) 
        { 
            printf("ReadFile failed with %d.\n", GetLastError());  
            return FALSE; 
        } 
 
        // Display the message. 

        if (hMutex == NULL)
        {
            _tprintf(TEXT("CreateMutex error"));
            return FALSE;
        }

        _tprintf(TEXT("Contents of the mailslot: %s"), (TCHAR*)address); 
 
        ReleaseMutex(hMutex);

        fResult = GetMailslotInfo(hSlot,  // mailslot handle 
            (LPDWORD) NULL,               // no maximum message size 
            &cbMessage,                   // size of next message 
            &cMessage,                    // number of messages 
            (LPDWORD) NULL);              // no read time-out 
 
        if (!fResult) 
        { 
            printf("GetMailslotInfo failed (%d)\n", GetLastError());
            return FALSE; 
        } 
    } 

    return TRUE; 
}
