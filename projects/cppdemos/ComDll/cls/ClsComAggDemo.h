#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>

#include "resource.h"

#include "ClsComInnerDef.h"
#include "ClsComDemo.h"

class ATL_NO_VTABLE ClsComAggDemo
    : public IUnknown
    , public CComObjectRootEx<CComMultiThreadModel>
    , public CComCoClass<ClsComAggDemo, &CLSID_ClsComAggDemo>
{

public:
    using ThisClass  = ClsComAggDemo;

public:
    ClsComAggDemo();
    ~ClsComAggDemo();

    // 如果想以单例模式可以使用宏
    DECLARE_CLASSFACTORY_SINGLETON(ThisClass);
    // DECLARE_CLASSFACTORY()

    // 如果不需要注册表的资源描述信息可以使用以下宏
    // 因为 UpdateRegistry 方法 是宏 OBJECT_ENTRY_AUTO 中必须的
    // DECLARE_NO_REGISTRY()
    // 否则应该使用宏
    // DECLARE_REGISTRY()
    // DECLARE_REGISTRY_XXX() 
    DECLARE_REGISTRY_RESOURCEID(IDR_CLSCOMAGGDEMO)

    // 要求组件不可被聚合
    DECLARE_NOT_AGGREGATABLE(ThisClass)
    // 声明组件可以被聚合
    // DECLARE_AGGREGATABLE(ThisClass)

    // 自定义类的描述, 一般不需要，在 CComCoClass 中有默认实现
    // DECLARE_OBJECT_DESCRIPTION(XXX)

    // 声明获取当前IUnknow指针的方法，通常在聚合模式中需要
    DECLARE_GET_CONTROLLING_UNKNOWN()

    // 当你需要关注当前对象创建和释放时，并在合适的时候让创建失败
    // 你可以使用如下宏 并搭配 创建创建方法 HRESULT FinalConstruct()
    // 和释放方法 void FinalRelease() 使用
    // 一般用于保护内部聚合组件引用计数
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(ThisClass)
        COM_INTERFACE_ENTRY(IUnknown)
        // 这里是聚合组件, 需要注意的是, 你应该将工程类聚合进去，而不是直接将对象接口聚合进去。
        // 如果直接聚合接口对象, 那在创建聚合对象实例后, 会首先获取被聚合类的工程实例, 然再查询该工程实例支持的接口, 再通过工厂实例获取对应的接口对象。
        // 如果直接聚合可能会出现死循环。
        COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(IDispatch), m_pInnerComDemo)
        COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(InfComDemo), m_pInnerComDemo)
        COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(InfComDemoEx), m_pInnerComDemo)
    END_COM_MAP()

    // 这个方法是 用了创建完整的类的 对象，而不是面向接口的对象
    // DECLARE_COM_MY_INSTANCE_CREATER(ThisClass)

    HRESULT FinalConstruct();
    void FinalRelease();

private:
    CComPtr<IUnknown> m_pInnerComDemo = nullptr;
};

// DllGetClassObject -> CComModule::GetClassObject ->
// AtlComModuleGetClassObject 前置步骤未找到，则查找此处
OBJECT_ENTRY_AUTO(CLSID_ClsComAggDemo, ClsComAggDemo);