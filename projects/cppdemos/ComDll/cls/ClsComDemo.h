#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>

#include "resource.h"

#include "interface/InfComDemo.h"

class ATL_NO_VTABLE ClsComDemo
    : public InfComDemo
    , public CComObjectRootEx<CComMultiThreadModel>
    , public CComCoClass<ClsComDemo, &CLSID_ClsComDemo>
{

    using ThisClass  = ClsComDemo;
    using ThisCClass = CComObject<ThisClass>;

public:
    ClsComDemo();
    ~ClsComDemo();

    DECLARE_NO_REGISTRY()
    DECLARE_NOT_AGGREGATABLE(ThisClass)
    BEGIN_COM_MAP(ThisClass)
        COM_INTERFACE_ENTRY(InfComDemo)
    END_COM_MAP()

public: //InfComDemo
    virtual HRESULT STDMETHODCALLTYPE Method1() override;
    virtual HRESULT STDMETHODCALLTYPE Method2() override;
};

using CoClsComDemo = CComObject<ClsComDemo>;
