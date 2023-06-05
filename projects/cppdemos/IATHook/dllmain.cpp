// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "Hook/CAPIHook.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE hHookThread = 
            ::CreateThread(
                NULL, 
                0, 
                (LPTHREAD_START_ROUTINE)
                (&CAPIHOOK::ThreadHook)
                , NULL
                , 0
                , NULL);

        if (hHookThread)
        {
            ::CloseHandle(hHookThread);
        }
    }
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

