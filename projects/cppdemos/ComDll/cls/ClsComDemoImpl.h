#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <sstream>

#include "interface/InfComDemo.h"
#include "ClsComInnerDef.h"

class ClsComDemoImpl
    : public InfComDemoEx
    , public CComObjectRootEx<CComMultiThreadModel>
{
public:
    ClsComDemoImpl();
    ~ClsComDemoImpl();

public: //InfComDemo
    virtual HRESULT STDMETHODCALLTYPE Method1() override;
    virtual HRESULT STDMETHODCALLTYPE Method2() override;
public: //InfComDemoEx
    virtual HRESULT STDMETHODCALLTYPE Method3() override;
    virtual HRESULT STDMETHODCALLTYPE Method4() override;
private:
    void DumpInfo(const std::wstringstream& ss);
};