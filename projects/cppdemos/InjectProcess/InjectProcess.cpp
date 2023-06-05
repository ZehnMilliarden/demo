// InjectProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

#include "cmdline.h"

bool AdjustPrivilege()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // 获取当前进程的令牌
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        return false;
    }

    // 获取Debug权限的LUID
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid)) {
        return false;
    }

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // 提升进程权限
    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
        return false;
    }

    return true;
}

void InjectDllToProc(
    const std::string& proc, 
    const std::string& dll)
{

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (!::CreateProcessA(
        proc.c_str(), 
        NULL, 
        NULL, 
        NULL, 
        FALSE, 
        CREATE_SUSPENDED,
        NULL, 
        NULL, 
        &si, 
        &pi))
    {
        std::cout << "Error creating process" << std::endl;
        return;
    }

    LPVOID lpRemoteDllPath = VirtualAllocEx(pi.hProcess, NULL, dll.length(), MEM_COMMIT, PAGE_READWRITE);
    if (!lpRemoteDllPath)
    {
        std::cout << "Error Alloc Remote Memory" << std::endl;
        return;
    }

    if (!WriteProcessMemory(pi.hProcess, lpRemoteDllPath, dll.c_str(), dll.length(), NULL))
    {
        std::cout << "Error Write Remote Memory" << std::endl;
        return;
    }

    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    if (!kernel32)
    {
        std::cout << "Error Get Kernel32 Module Fail" << std::endl;
        return;
    }

    LPVOID loadLibrary = (LPVOID)::GetProcAddress(kernel32, "LoadLibraryA");
    if (!loadLibrary)
    {
        std::cout << "Error Get API LoadLibraryA Fail" << std::endl;
        return;
    }

    HANDLE hResumeEvent = ::CreateEvent(NULL, FALSE, FALSE, L"Global\\{e84eb5ff-9840-4fc4-bc97-650d259a27d8}");

    HANDLE hRemoteThread = ::CreateRemoteThread(
        pi.hProcess, 
        NULL, 
        0, 
        (LPTHREAD_START_ROUTINE)loadLibrary, 
        lpRemoteDllPath,
        0, 
        NULL);

    if (!hRemoteThread)
    {
        std::cout << "Error CreateRemoteThread, GetLastError: " << ::GetLastError() <<std::endl;
        if (hResumeEvent)
        {
            ::CloseHandle(hResumeEvent);
        }
        ::ResumeThread(pi.hThread);
        return;
    }

    ::WaitForSingleObject(hRemoteThread, INFINITE);

    if (hResumeEvent)
    {
        ::WaitForSingleObject(hResumeEvent, INFINITE);
        ::CloseHandle(hResumeEvent);
    }

    ::ResumeThread(pi.hThread);
}


int main(int argc, char* argv[])
{
    cmdline::parser param;
    param.add<std::string>("proc", 'p', "Process Path", true);
    param.add<std::string>("dll", 'd', "Dynamic Link Library Path", true);

    bool bOk = param.parse(argc, argv);
    if (!bOk)
    {
        std::cerr << param.error() << std::endl << param.usage();
        return 0;
    }

    std::string proc = param.get<std::string>("proc");
    std::string dll  = param.get<std::string>("dll");

    AdjustPrivilege();
    InjectDllToProc(proc, dll);

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
