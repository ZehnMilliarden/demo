#include "CAPIHookMapModuleToName.h"

#include <atlconv.h>
#include <codecvt>

const wchar_t* strNull = L"Null";

CAPIHookMapModuleToName::~CAPIHookMapModuleToName()
{

}

CAPIHookMapModuleToName& CAPIHookMapModuleToName::Instance()
{
    static CAPIHookMapModuleToName ins;
    return ins;
}

void CAPIHookMapModuleToName::Insert(HMODULE hModule, LPCSTR lpszName)
{
    std::lock_guard<std::mutex> lck(m_lckOfModuleName);
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    m_moduleName.insert(std::make_pair(hModule, converter.from_bytes(lpszName)));
}

void CAPIHookMapModuleToName::Insert(HMODULE hModule, LPCWSTR lpszName)
{
    std::lock_guard<std::mutex> lck(m_lckOfModuleName);
    m_moduleName.insert(std::make_pair(hModule, lpszName));
}

LPCWSTR CAPIHookMapModuleToName::GetName(HMODULE hModule)
{
    std::lock_guard<std::mutex> lck(m_lckOfModuleName);
    if (m_moduleName.find(hModule) != m_moduleName.end())
    {
        return m_moduleName[hModule].c_str();
    }
    return strNull;
}

CAPIHookMapModuleToName::CAPIHookMapModuleToName()
{

}
