#include "pch.h"
#include "ClsComAggDemo.h"

ClsComAggDemo::ClsComAggDemo()
{

}

ClsComAggDemo::~ClsComAggDemo()
{

}

HRESULT ClsComAggDemo::FinalConstruct()
{
    return ClsComDemo::CreateMyInstance(GetControllingUnknown(), m_pInnerComDemo);
}

void ClsComAggDemo::FinalRelease()
{
    m_pInnerComDemo = nullptr;
}