
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
    //��ʼ������  
    m_pszModName = lpszModName;
    m_pszFuncName = pszFuncName;
    m_pfnOrig = ::GetProcAddress(::GetModuleHandleA(lpszModName), pszFuncName);
    m_pfnHook = pfnHook;

    //���˶������������  
    m_pNext = sm_pHeader;
    sm_pHeader = this;

    //�ڵ�ǰ�Ѽ��ص�ģ����HOOK�������  
    ReplaceIATEntryInAllMods(lpszModName, m_pfnOrig, m_pfnHook, bExcludeAPIHookMod);
}

CAPIHOOK::~CAPIHOOK(void)
{
    //ȡ���Ժ�����HOOK  
    ReplaceIATEntryInAllMods(m_pszModName, m_pfnHook, m_pfnOrig, TRUE);

    //���Լ���������ɾ��  
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

//��ֹ���������ڼ䶯̬����ģ��  
void CAPIHOOK::HookNewlyLoadedModule(HMODULE hModule, DWORD dwFlags)
{
    if (hModule != NULL && (dwFlags & LOAD_LIBRARY_AS_DATAFILE) == 0)
    {
        CAPIHOOK* p = sm_pHeader;  //ѭ������������ÿ��CAPIHOOK����HOOK  
        if (p != NULL)
        {
            ReplaceIATEntryInOneMod(p->m_pszModName, p->m_pfnOrig, p->m_pfnHook, hModule);
            p = p->m_pNext;
        }
    }

}

//��ֹ���������ڼ䶯̬����API����  
FARPROC WINAPI CAPIHOOK::GetProcess(HMODULE hModule, PCSTR pszProcName)
{
    //�õ���������ʵ��ַ
    HookMsgOutPut(pszProcName, hModule);

    FARPROC pfn = ::GetProcAddress(hModule, pszProcName);
    //�����б� ���ǲ���ҪHOOK�ĺ���  
    CAPIHOOK* p = sm_pHeader;
    while (p != NULL)
    {
        if (p->m_pfnOrig == pfn) //��ҪHOOK�ĺ���  
        {
            pfn = p->m_pfnHook; //HOOK��  
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
    IMAGE_OPTIONAL_HEADER* pOpNtHeader = (IMAGE_OPTIONAL_HEADER*)((BYTE*)hModCaller + pDosHeader->e_lfanew + 24); //�����24  
    IMAGE_IMPORT_DESCRIPTOR* pImportDesc = (IMAGE_IMPORT_DESCRIPTOR*)((BYTE*)hModCaller + pOpNtHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    BOOL bFindDll = FALSE;
    while (pImportDesc->FirstThunk)
    {
        char* pszDllName = (char*)((BYTE*)hModCaller + pImportDesc->Name);

        if (_stricmp(pszDllName, pszExportMod) == 0)//����ҵ�pszExportModģ��,�൱��hook messageboxaʱ�ġ�user32.dll��  
        {
            bFindDll = TRUE;
            break;
        }
        pImportDesc++;
    }

    if (bFindDll)
    {
        DWORD n = 0;
        //һ��IMAGE_THUNK_DATA����һ�����뺯��  
        IMAGE_THUNK_DATA* pThunk = (IMAGE_THUNK_DATA*)((BYTE*)hModCaller + pImportDesc->OriginalFirstThunk);
        while (pThunk->u1.Function)
        {
            //ȡ�ú�������  
            IMAGE_IMPORT_BY_NAME* pImportByName = (IMAGE_IMPORT_BY_NAME*)((BYTE*)hModCaller + pThunk->u1.AddressOfData);
            char* pszFuncName = (char*)pImportByName->Name; //�������ڽṹ���Name�ֶ���  
            //printf("function name:%-25s,  ", pszFuncName);  
            //ȡ�ú�����ַ  
            PDWORD lpAddr = (DWORD*)((BYTE*)hModCaller + pImportDesc->FirstThunk) + n; //�ӵ�һ�������ĵ�ַ���Ժ�ÿ��+4�ֽ�  
            //printf("addrss:%X\n", lpAddr);  
            //�������ǱȽϵĺ�����ַ  
            if (*lpAddr == (DWORD)pfnCurrent)  //�ҵ�iat�еĺ�����ַ  
            {
                DWORD dwNewProc = (DWORD)pfnNewFunc;
                MEMORY_BASIC_INFORMATION mbi;
                DWORD dwOldProtect;
                //�޸��ڴ�ҳ�ı�������  
                ::VirtualQuery(lpAddr, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
                ::VirtualProtect(lpAddr, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
                ::WriteProcessMemory(GetCurrentProcess(), lpAddr, &dwNewProc, sizeof(DWORD), NULL);
                ::VirtualProtect(lpAddr, sizeof(DWORD), dwOldProtect, NULL);
                return;
            }
            n++; //ÿ������һ��DWORD  
            pThunk++; //ָ����һ��IMAGE_THUNK_DATA�ṹ��  
        }
    }
}


void CAPIHOOK::ReplaceIATEntryInAllMods(LPCSTR pszExportMod, PROC pfnCurrent, PROC pfnNewFunc, BOOL bExcludeAPIHookMod)
{
    //ȡ�õ�ǰģ����  
    HMODULE hModThis = NULL;
    if (bExcludeAPIHookMod)
    {
        MEMORY_BASIC_INFORMATION mbi;
        if (0 != ::VirtualQuery(ReplaceIATEntryInAllMods, &mbi, sizeof(MEMORY_BASIC_INFORMATION))) //ReplaceIATEntryInAllMods����Ϊ���static����  
        {
            hModThis = (HMODULE)mbi.AllocationBase;
        }
    }
    //ȡ�ñ����̵�ģ���б�  
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
    { //��ÿһ��ģ��  
        if (me32.hModule != hModThis)
        {
            CAPIHookMapModuleToName::Instance().Insert(me32.hModule, me32.szModule);
            ReplaceIATEntryInOneMod(pszExportMod, pfnCurrent, pfnNewFunc, me32.hModule);
        }
    } while (Module32Next(hModuleSnap, &me32));

    ::CloseHandle(hModuleSnap); //���д  
}

//��ֹ�Զ�����  
HMODULE WINAPI CAPIHOOK::LoadLibraryA(LPCSTR lpFileName)
{
    HookMsgOutPut(lpFileName);
    HMODULE hModule = ::LoadLibraryA(lpFileName);
    if (hModule)
    {
        CAPIHookMapModuleToName::Instance().Insert(hModule, lpFileName);
        HookNewlyLoadedModule(hModule, 0); //�������������hModule ��  
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
        HookNewlyLoadedModule(hModule, 0); //�������������hModule ��  
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
        HookNewlyLoadedModule(hModule, dwFlags); //�������������hModule ��  
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
        HookNewlyLoadedModule(hModule, dwFlags); //�������������hModule ��  
    }

    return hModule;
}