#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>

#include "resource.h"

#include "interface/InfComDemo.h"

class ATL_NO_VTABLE ClsComDemo
    : public InfComDemoEx
    , public CComObjectRootEx<CComMultiThreadModel>
    , public CComCoClass<ClsComDemo, &CLSID_ClsComDemo>
{

    using ThisClass  = ClsComDemo;
    using ThisCClass = CComObject<ThisClass>;

public:
    ClsComDemo();
    ~ClsComDemo();

    // ������Ե���ģʽ����ʹ�ú�
    // DECLARE_CLASSFACTORY_SINGLETON(ThisClass);
    DECLARE_CLASSFACTORY()

    // �������Ҫע������Դ������Ϣ����ʹ�����º�
    // ��Ϊ UpdateRegistry ���� �Ǻ� OBJECT_ENTRY_AUTO �б����
    DECLARE_NO_REGISTRY()
    // ����Ӧ��ʹ�ú�
    // DECLARE_REGISTRY()
    // DECLARE_REGISTRY_XXX() 

    // Ҫ��������ɱ��ۺ�
    // DECLARE_NOT_AGGREGATABLE(ThisClass)
    // ����������Ա��ۺ�
    DECLARE_AGGREGATABLE(ThisClass)

    // �Զ����������, һ�㲻��Ҫ���� CComCoClass ����Ĭ��ʵ��
    // DECLARE_OBJECT_DESCRIPTION(XXX)

    // ������ȡ��ǰIUnknowָ��ķ�����ͨ���ھۺ�ģʽ����Ҫ
    // DECLARE_GET_CONTROLLING_UNKNOWN()

    // ������Ҫ��ע��ǰ���󴴽����ͷ�ʱ�����ں��ʵ�ʱ���ô���ʧ��
    // �����ʹ�����º� ������ ������������ HRESULT FinalConstruct()
    // ���ͷŷ��� void FinalRelease() ʹ��
    // һ�����ڱ����ڲ��ۺ�������ü���
    // DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(ThisClass)
        COM_INTERFACE_ENTRY(InfComDemo)
        COM_INTERFACE_ENTRY(InfComDemoEx)
    END_COM_MAP()

public: //InfComDemo
    virtual HRESULT STDMETHODCALLTYPE Method1() override;
    virtual HRESULT STDMETHODCALLTYPE Method2() override;
    virtual HRESULT STDMETHODCALLTYPE Method3() override;
    virtual HRESULT STDMETHODCALLTYPE Method4() override;
};

using CoClsComDemo = CComObject<ClsComDemo>;

// DllGetClassObject -> CComModule::GetClassObject ->
// AtlComModuleGetClassObject ǰ�ò���δ�ҵ�������Ҵ˴�
OBJECT_ENTRY_AUTO(CLSID_ClsComDemo, ClsComDemo);