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
    //防止程序运行期间动态加载模块, 当一个新DLL被加载时调用  
    static void HookNewlyLoadedModule(HMODULE hModule, DWORD dwFlags);


    //跟踪当前进程加载新的DLL  
    static HMODULE WINAPI LoadLibraryA(LPCSTR lpFileName);
    static HMODULE WINAPI LoadLibraryW(LPCWSTR lpFileName);
    static HMODULE WINAPI LoadLibraryExA(LPCSTR lpFileName, HANDLE hFile, DWORD dwFlags);
    static HMODULE WINAPI LoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags);
    //防止程序运行期间动态调用API函数 对于请求已HOOK的API函数，返回用户自定义的函数地址  
    static FARPROC WINAPI GetProcess(HMODULE hModule, PCSTR pszProcName);

private:
    static void HookMsgOutPut(const std::string& msg, HMODULE hModule = NULL);
    static void HookMsgOutPutW(const std::wstring& msg);
    static void NotifyMainThreadRunning();

private:
    static CAPIHOOK* sm_pHeader; //钩子链表  
    CAPIHOOK* m_pNext;

    //要钩子的函数  
    PROC m_pfnOrig;
    PROC m_pfnHook;

    //要钩子的函数所在的dll  
    const char* m_pszModName;
    //要钩子的函数名称  
    const char* m_pszFuncName;
};