// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "cls/ClsComDemo.h"

CComModule _AtlModule;

BEGIN_OBJECT_MAP(ObjMap)
    OBJECT_ENTRY(CLSID_ClsComDemo, ClsComDemo)
END_OBJECT_MAP()

_Use_decl_annotations_
STDAPI DllCanUnloadNow()
{
    return _AtlModule.DllCanUnloadNow();
}

_Use_decl_annotations_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        // Initialize the COM module
        _AtlModule.Init(ObjMap, hModule);
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        // Uninitialize the COM module
        _AtlModule.Term();
    }

    return TRUE;
}