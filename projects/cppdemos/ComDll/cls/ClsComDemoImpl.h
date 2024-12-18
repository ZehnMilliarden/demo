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
    using ThisClass = ClsComDemoImpl;
    using ThisCoClass = CComObject<ThisClass>;
    using ThisCoAggClass = CComAggObject<ThisClass>;

public:
    ClsComDemoImpl();
    ~ClsComDemoImpl();

    BEGIN_COM_MAP(ClsComDemoImpl)
        COM_INTERFACE_ENTRY(InfComDemo)
        COM_INTERFACE_ENTRY(InfComDemoEx)
    END_COM_MAP()
    DECLARE_COM_MY_INSTANCE_CREATER(ThisClass)

public: //InfComDemo
    virtual HRESULT STDMETHODCALLTYPE Method1() override;
    virtual HRESULT STDMETHODCALLTYPE Method2() override;
public: //InfComDemoEx
    virtual HRESULT STDMETHODCALLTYPE Method3() override;
    virtual HRESULT STDMETHODCALLTYPE Method4() override;
private:
    void DumpInfo(const std::wstringstream& ss);
};