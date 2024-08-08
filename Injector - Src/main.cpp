#include "base-resources.h"
#include <iostream>

int main()
{
    DWORD procID = 0;
    char dllPath[MAX_PATH];
    const char* dllName = "inject.dll";
    const char* windowTitle = "AssaultCube";

    if (!FileExists(dllName))
    {
        LogError("FileExists", "File wasn't found during runtime.");
    }

    if (!GetFullPathName(dllName, MAX_PATH, dllPath, nullptr))
    {
        LogError("GetFullPathName", "Failed to get full path of the file during runtime.");
    }

    GetProcId(windowTitle, procID);
    if (procID == 0)
    {
        LogError("GetProcId", "Target process (AssaultCube) wasn't found during runtime.");
    }

    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, procID);
    if (!processHandle)
    {
        LogError("OpenProcess", "Failed to open a handle to process (AssaultCube) during runtime.");
    }

    void* allocated_mem = VirtualAllocEx(processHandle, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!allocated_mem)
    {
        LogError("VirtualAllocEx", "Failed to allocate memory in process (AssaultCube) during runtime.");
    }

    if (!WriteProcessMemory(processHandle, allocated_mem, dllPath, MAX_PATH, nullptr))
    {
        LogError("WriteProcessMemory", "Failed to write to process memory during runtime.");
    }

    HANDLE threadHandle = CreateRemoteThread(processHandle, nullptr, 0, LPTHREAD_START_ROUTINE(LoadLibraryA), allocated_mem, 0, nullptr);
    if (!threadHandle)
    {
        LogError("CreateRemoteThread", "Failed to create the remote thread during runtime.");
    }

    CloseHandle(threadHandle); // Close thread handle after creation
    CloseHandle(processHandle);
    VirtualFreeEx(processHandle, allocated_mem, 0, MEM_RELEASE);
    MessageBoxA(0, "Successfully injected!", "Injected", MB_OK | MB_ICONINFORMATION);

    return 0;
}
