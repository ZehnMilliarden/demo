#pragma once

#include <map>
#include <mutex>
#include <string>
#include <Windows.h>

class CAPIHookMapModuleToName
{
public:
    ~CAPIHookMapModuleToName();
    static CAPIHookMapModuleToName& Instance();

public:
    void Insert(HMODULE hModule, LPCSTR lpszName);
    void Insert(HMODULE hModule, LPCWSTR lpszName);
    LPCWSTR GetName(HMODULE hModule);

protected:
    CAPIHookMapModuleToName();

private:
    std::mutex m_lckOfModuleName;
    std::map<HMODULE, std::wstring> m_moduleName;
};