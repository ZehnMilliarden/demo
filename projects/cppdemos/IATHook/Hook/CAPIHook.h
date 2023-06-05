#pragma once  

#include <Windows.h>  
#include <string>

class CAPIHOOK
{
public:
    CAPIHOOK(const char* lpszModName, const char* pszFuncName, PROC pfnHook, BOOL bExcludeAPIHookMod = TRUE);
    ~CAPIHOOK(void);

public:
    static DWORD WINAPI ThreadHook(LPVOID lpParamer);
    static BOOL HookImpl();

private:
    static void ReplaceIATEntryInOneMod(LPCSTR pszExportMod, PROC pfnCurrent, PROC pfnNewFunc, HMODULE hModCaller);
    static void ReplaceIATEntryInAllMods(LPCSTR pszExportMod, PROC pfnCurrent, PROC pfnNewFunc, BOOL bExcludeAPIHookMod);
    //��ֹ���������ڼ䶯̬����ģ��, ��һ����DLL������ʱ����  
    static void HookNewlyLoadedModule(HMODULE hModule, DWORD dwFlags);


    //���ٵ�ǰ���̼����µ�DLL  
    static HMODULE WINAPI LoadLibraryA(LPCSTR lpFileName);
    static HMODULE WINAPI LoadLibraryW(LPCWSTR lpFileName);
    static HMODULE WINAPI LoadLibraryExA(LPCSTR lpFileName, HANDLE hFile, DWORD dwFlags);
    static HMODULE WINAPI LoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags);
    //��ֹ���������ڼ䶯̬����API���� ����������HOOK��API�����������û��Զ���ĺ�����ַ  
    static FARPROC WINAPI GetProcess(HMODULE hModule, PCSTR pszProcName);

private:
    static void HookMsgOutPut(const std::string& msg, HMODULE hModule = NULL);
    static void HookMsgOutPutW(const std::wstring& msg);
    static void NotifyMainThreadRunning();

private:
    static CAPIHOOK* sm_pHeader; //��������  
    CAPIHOOK* m_pNext;

    //Ҫ���ӵĺ���  
    PROC m_pfnOrig;
    PROC m_pfnHook;

    //Ҫ���ӵĺ������ڵ�dll  
    const char* m_pszModName;
    //Ҫ���ӵĺ�������  
    const char* m_pszFuncName;
};