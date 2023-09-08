#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <memory>

template<class T, const CLSID& clsid >
class EasyComLoader 
{
public:
    EasyComLoader() {

    }

    ~EasyComLoader()
    {
        Unload();
    }

    bool Load(const wchar_t* szModule) 
    {
        if (m_hModule)
        {
            return true;
        }

        m_hModule = ::LoadLibraryW(szModule);

        if (m_hModule == nullptr)
        {
            return false;
        }

        typedef HRESULT(__stdcall* CREATE_COM_OBJECT_FUNC)(REFCLSID, REFIID, LPVOID*);
        CREATE_COM_OBJECT_FUNC pCreateFunc = reinterpret_cast<CREATE_COM_OBJECT_FUNC>(
            GetProcAddress(m_hModule, "DllGetClassObject"));

        if (pCreateFunc == nullptr)
        {
            Unload();
            return false;
        }

        HRESULT hr = pCreateFunc(
            clsid, 
            IID_IClassFactory, 
            reinterpret_cast<LPVOID*>(&m_pClassFactory));

        if (FAILED(hr))
        {
            Unload();
            return false;
        }

        return true;
    }

    void Unload() 
    {
        if (m_pClassFactory)
        {
            m_pClassFactory.Release();
            m_pClassFactory = nullptr;
        }
        
        if (m_hModule)
        {
            ::FreeLibrary(m_hModule);
            m_hModule = nullptr;
        }
    }

    bool CreateInstance(CComPtr<T>& pInstance) {
        if (m_pClassFactory == nullptr) {
            return false;
        }

        HRESULT hr = m_pClassFactory->CreateInstance(
            nullptr, 
            __uuidof(T), 
            reinterpret_cast<LPVOID*>(&pInstance));

        return SUCCEEDED(hr);
    }

private:
    EasyComLoader(const EasyComLoader&) = delete;
    EasyComLoader& operator=(const EasyComLoader&) = delete;

private:
    HMODULE m_hModule = nullptr;
    CComPtr<IClassFactory> m_pClassFactory = nullptr;
};
