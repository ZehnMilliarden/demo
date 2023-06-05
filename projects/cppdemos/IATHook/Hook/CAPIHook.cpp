
#include "CAPIHOOK.h"  
#include <Tlhelp32.h>  
#include <codecvt>
#include <sstream>

#include "CAPIHookMapModuleToName.h"

CAPIHOOK* CAPIHOOK::sm_pHeader = NULL;

CAPIHOOK::CAPIHOOK(
    const char* lpszModName, 
    const char* pszFuncName, 
    PROC pfnHook, 
    BOOL bExcludeAPIHookMod)
{
    //初始化变量  
    m_pszModName = lpszModName;
    m_pszFuncName = pszFuncName;
    m_pfnOrig = ::GetProcAddress(::GetModuleHandleA(lpszModName), pszFuncName);
    m_pfnHook = pfnHook;

    //将此对象加入链表中  
    m_pNext = sm_pHeader;
    sm_pHeader = this;

    //在当前已加载的模块中HOOK这个函数  
    ReplaceIATEntryInAllMods(lpszModName, m_pfnOrig, m_pfnHook, bExcludeAPIHookMod);
}

CAPIHOOK::~CAPIHOOK(void)
{
    //取消对函数的HOOK  
    ReplaceIATEntryInAllMods(m_pszModName, m_pfnHook, m_pfnOrig, TRUE);

    //把自己从链表中删除  
    CAPIHOOK* p = sm_pHeader;
    if (p == this)
    {
        sm_pHeader = this->m_pNext;
    }
    else
    {
        while (p != NULL)
        {
            if (p->m_pNext == this)
            {
                p->m_pNext = this->m_pNext;
                break;
            }
            p = p->m_pNext;
        }
    }
}

DWORD WINAPI CAPIHOOK::ThreadHook(LPVOID lpParamer)
{
    HookImpl();
    NotifyMainThreadRunning();
    return 0;
}

BOOL CAPIHOOK::HookImpl()
{
    ::MessageBox(NULL, L"Hook Start", L"Notice", NULL);

    try
    {
        static CAPIHOOK sm_LoadLibraryA("kernel32.dll", "LoadLibraryA", (PROC)CAPIHOOK::LoadLibraryA, TRUE);
        static CAPIHOOK sm_LoadLibraryW("kernel32.dll", "LoadLibraryW", (PROC)CAPIHOOK::LoadLibraryW, TRUE);
        static CAPIHOOK sm_LoadLibraryExA("kernel32.dll", "LoadLibraryExA", (PROC)CAPIHOOK::LoadLibraryExA, TRUE);
        static CAPIHOOK sm_LoadLibraryExW("kernel32.dll", "LoadLibraryExW", (PROC)CAPIHOOK::LoadLibraryExW, TRUE);
        static CAPIHOOK sm_GetProcAddress("kernel32.dll", "GetProcAddress", (PROC)CAPIHOOK::GetProcess, TRUE);
    }
    catch (...)
    {

    }

    return TRUE;
}

//防止程序运行期间动态加载模块  
void CAPIHOOK::HookNewlyLoadedModule(HMODULE hModule, DWORD dwFlags)
{
    if (hModule != NULL && (dwFlags & LOAD_LIBRARY_AS_DATAFILE) == 0)
    {
        CAPIHOOK* p = sm_pHeader;  //循环遍历链表，对每个CAPIHOOK进入HOOK  
        if (p != NULL)
        {
            ReplaceIATEntryInOneMod(p->m_pszModName, p->m_pfnOrig, p->m_pfnHook, hModule);
            p = p->m_pNext;
        }
    }

}

//防止程序运行期间动态调用API函数  
FARPROC WINAPI CAPIHOOK::GetProcess(HMODULE hModule, PCSTR pszProcName)
{
    //得到函数的真实地址
    HookMsgOutPut(pszProcName, hModule);

    FARPROC pfn = ::GetProcAddress(hModule, pszProcName);
    //遍历列表 看是不是要HOOK的函数  
    CAPIHOOK* p = sm_pHeader;
    while (p != NULL)
    {
        if (p->m_pfnOrig == pfn) //是要HOOK的函数  
        {
            pfn = p->m_pfnHook; //HOOK掉  
            break;
        }
        p = p->m_pNext;
    }
    return pfn;
}

void CAPIHOOK::HookMsgOutPut(
    const std::string& msg,
    HMODULE hModule)
{
    std::wstringstream ss; 
    ss << L"Hook Msg: ";

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    ss << converter.from_bytes(msg);
    if (hModule)
    {
        ss << L" ["  << CAPIHookMapModuleToName::Instance().GetName(hModule) << L"]";
    }
    
    ::OutputDebugStringW(ss.str().c_str());
}

void CAPIHOOK::HookMsgOutPutW(const std::wstring& msg)
{
    wchar_t szDbg[MAX_PATH] = { 0 };
    wsprintf(szDbg, L"Hook Msg: %s", msg.c_str());
    ::OutputDebugStringW(szDbg);
}

void CAPIHOOK::NotifyMainThreadRunning()
{
    HANDLE hEvent = ::OpenEventW(
        EVENT_MODIFY_STATE, FALSE, L"Global\\{e84eb5ff-9840-4fc4-bc97-650d259a27d8}");
    if (hEvent)
    {
        ::SetEvent(hEvent);
        ::CloseHandle(hEvent);
    }
}

void CAPIHOOK::ReplaceIATEntryInOneMod(LPCSTR pszExportMod, PROC pfnCurrent, PROC pfnNewFunc, HMODULE hModCaller)
{

    IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)hModCaller;
    IMAGE_OPTIONAL_HEADER* pOpNtHeader = (IMAGE_OPTIONAL_HEADER*)((BYTE*)hModCaller + pDosHeader->e_lfanew + 24); //这里加24  
    IMAGE_IMPORT_DESCRIPTOR* pImportDesc = (IMAGE_IMPORT_DESCRIPTOR*)((BYTE*)hModCaller + pOpNtHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    BOOL bFindDll = FALSE;
    while (pImportDesc->FirstThunk)
    {
        char* pszDllName = (char*)((BYTE*)hModCaller + pImportDesc->Name);

        if (_stricmp(pszDllName, pszExportMod) == 0)//如果找到pszExportMod模块,相当于hook messageboxa时的“user32.dll”  
        {
            bFindDll = TRUE;
            break;
        }
        pImportDesc++;
    }

    if (bFindDll)
    {
        DWORD n = 0;
        //一个IMAGE_THUNK_DATA就是一个导入函数  
        IMAGE_THUNK_DATA* pThunk = (IMAGE_THUNK_DATA*)((BYTE*)hModCaller + pImportDesc->OriginalFirstThunk);
        while (pThunk->u1.Function)
        {
            //取得函数名称  
            IMAGE_IMPORT_BY_NAME* pImportByName = (IMAGE_IMPORT_BY_NAME*)((BYTE*)hModCaller + pThunk->u1.AddressOfData);
            char* pszFuncName = (char*)pImportByName->Name; //函数名在结构体的Name字段中  
            //printf("function name:%-25s,  ", pszFuncName);  
            //取得函数地址  
            PDWORD lpAddr = (DWORD*)((BYTE*)hModCaller + pImportDesc->FirstThunk) + n; //从第一个函数的地址，以后每次+4字节  
            //printf("addrss:%X\n", lpAddr);  
            //在这里是比较的函数地址  
            if (*lpAddr == (DWORD)pfnCurrent)  //找到iat中的函数地址  
            {
                DWORD dwNewProc = (DWORD)pfnNewFunc;
                MEMORY_BASIC_INFORMATION mbi;
                DWORD dwOldProtect;
                //修改内存页的保护属性  
                ::VirtualQuery(lpAddr, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
                ::VirtualProtect(lpAddr, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
                ::WriteProcessMemory(GetCurrentProcess(), lpAddr, &dwNewProc, sizeof(DWORD), NULL);
                ::VirtualProtect(lpAddr, sizeof(DWORD), dwOldProtect, NULL);
                return;
            }
            n++; //每次增加一个DWORD  
            pThunk++; //指向下一个IMAGE_THUNK_DATA结构体  
        }
    }
}


void CAPIHOOK::ReplaceIATEntryInAllMods(LPCSTR pszExportMod, PROC pfnCurrent, PROC pfnNewFunc, BOOL bExcludeAPIHookMod)
{
    //取得当前模块句柄  
    HMODULE hModThis = NULL;
    if (bExcludeAPIHookMod)
    {
        MEMORY_BASIC_INFORMATION mbi;
        if (0 != ::VirtualQuery(ReplaceIATEntryInAllMods, &mbi, sizeof(MEMORY_BASIC_INFORMATION))) //ReplaceIATEntryInAllMods必须为类的static函数  
        {
            hModThis = (HMODULE)mbi.AllocationBase;
        }
    }
    //取得本进程的模块列表  
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me32;
    hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (INVALID_HANDLE_VALUE == hModuleSnap)
    {
        return;
    }
    me32.dwSize = sizeof(MODULEENTRY32);
    if (!Module32First(hModuleSnap, &me32))
    {
        return;
    }
    do
    { //对每一个模块  
        if (me32.hModule != hModThis)
        {
            CAPIHookMapModuleToName::Instance().Insert(me32.hModule, me32.szModule);
            ReplaceIATEntryInOneMod(pszExportMod, pfnCurrent, pfnNewFunc, me32.hModule);
        }
    } while (Module32Next(hModuleSnap, &me32));

    ::CloseHandle(hModuleSnap); //配对写  
}

//防止自动加载  
HMODULE WINAPI CAPIHOOK::LoadLibraryA(LPCSTR lpFileName)
{
    HookMsgOutPut(lpFileName);
    HMODULE hModule = ::LoadLibraryA(lpFileName);
    if (hModule)
    {
        CAPIHookMapModuleToName::Instance().Insert(hModule, lpFileName);
        HookNewlyLoadedModule(hModule, 0); //这个函数中忆检测hModule 了  
    }
    return hModule;
}
HMODULE WINAPI CAPIHOOK::LoadLibraryW(LPCWSTR lpFileName)
{
    HookMsgOutPutW(lpFileName);
    HMODULE hModule = ::LoadLibraryW(lpFileName);
    if (hModule)
    {
        CAPIHookMapModuleToName::Instance().Insert(hModule, lpFileName);
        HookNewlyLoadedModule(hModule, 0); //这个函数中忆检测hModule 了  
    }
    return hModule;
}
HMODULE WINAPI CAPIHOOK::LoadLibraryExA(LPCSTR lpFileName, HANDLE hFile, DWORD dwFlags)
{
    HookMsgOutPut(lpFileName);
    HMODULE hModule = ::LoadLibraryExA(lpFileName, hFile, dwFlags);

    if (hModule)
    {
        CAPIHookMapModuleToName::Instance().Insert(hModule, lpFileName);
        HookNewlyLoadedModule(hModule, dwFlags); //这个函数中忆检测hModule 了  
    }

    return hModule;
}
HMODULE WINAPI CAPIHOOK::LoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags)
{
    HookMsgOutPutW(lpFileName);
    HMODULE hModule = ::LoadLibraryExW(lpFileName, hFile, dwFlags);

    if (hModule)
    {
        CAPIHookMapModuleToName::Instance().Insert(hModule, lpFileName);
        HookNewlyLoadedModule(hModule, dwFlags); //这个函数中忆检测hModule 了  
    }

    return hModule;
}