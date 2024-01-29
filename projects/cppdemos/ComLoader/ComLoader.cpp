// ComLoader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

#include <atlbase.h>
#include <atlcom.h>

#include "interface/InfComDemo.h"
#include "EasyComLoader.h"

int main()
{
    {
        std::shared_ptr<EasyComLoader<InfComDemo, CLSID_ClsComDemo>> loader =
            std::make_shared<EasyComLoader<InfComDemo, CLSID_ClsComDemo>>();
        loader->Load(L"ComDll.dll");

        CComPtr<InfComDemo> pInfComDemo1;
        CAdapt<CComPtr<InfComDemo>> pInfComDemo2;
        loader->CreateInstance(pInfComDemo1);
        loader->CreateInstance(pInfComDemo2);

        CComPtr<InfComDemoEx> pInfComDemo3 = nullptr;
        pInfComDemo3 = pInfComDemo1;

        pInfComDemo3->Method1();
        pInfComDemo3->Method2();
        pInfComDemo3->Method3();
        pInfComDemo3->Method4();
    }


    {
        std::shared_ptr<EasyComLoader<InfComDemo, CLSID_ClsComAggDemo>> loaderV2 =
            std::make_shared<EasyComLoader<InfComDemo, CLSID_ClsComAggDemo>>();
        loaderV2->Load(L"ComDll.dll");
        CComPtr<InfComDemo> pInfComDemo1;
        loaderV2->CreateInstance(pInfComDemo1);

        CComPtr<InfComDemoEx> pInfComDemo3 = nullptr;
        pInfComDemo3 = pInfComDemo1;

        pInfComDemo3->Method1();
        pInfComDemo3->Method2();
        pInfComDemo3->Method3();
        pInfComDemo3->Method4();
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
