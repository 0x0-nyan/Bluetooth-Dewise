#pragma once
//typedef struct IUnknown IUnknown;
#include <Windows.h>
#include <iostream>
#include <comdef.h>
#include <wincred.h>
//  Include the task header file.
#include <taskschd.h>

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "credui.lib")

//int WINAPI InitSecurity();
int WINAPI RegisterTask(LPCWSTR pTaskName, PCWSTR TaskFolder, PCWSTR ExePath, PCWSTR Args, PCWSTR Delay, PCWSTR UserName);