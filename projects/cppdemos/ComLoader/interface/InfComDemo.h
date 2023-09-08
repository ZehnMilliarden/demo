#pragma once

#include <Windows.h>

// 2312b16a-7166-4838-8628-a914d96bef9a
extern "C" const __declspec(selectany) CLSID CLSID_ClsComDemo =
{ 0x2312b16a, 0x7166, 0x4838, { 0x86, 0x28, 0xa9, 0x14, 0xd9, 0x6b, 0xef, 0x9a } };

MIDL_INTERFACE("eabd8132-1ac2-4cf2-99aa-e94d1189ea2b")
InfComDemo : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE Method1() = 0;
    virtual HRESULT STDMETHODCALLTYPE Method2() = 0;
};