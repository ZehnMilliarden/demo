#include "pch.h"
#include "ClsComDemo.h"

#include <iostream>
#include <sstream>

ClsComDemo::ClsComDemo()
{

}

ClsComDemo::~ClsComDemo()
{

}

HRESULT STDMETHODCALLTYPE ClsComDemo::Method1()
{
    std::wstringstream ss;
    ss << this << L" Method1\r\n";
    ::OutputDebugString(ss.str().c_str());
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ClsComDemo::Method2()
{
    std::wstringstream ss;
    ss << this << L" Method2\r\n";
    ::OutputDebugString(ss.str().c_str());
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ClsComDemo::Method3()
{
    std::wstringstream ss;
    ss << this << L" Method3\r\n";
    ::OutputDebugString(ss.str().c_str());
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ClsComDemo::Method4()
{
    std::wstringstream ss;
    ss << this << L" Method4\r\n";
    ::OutputDebugString(ss.str().c_str());
    return S_OK;
}
