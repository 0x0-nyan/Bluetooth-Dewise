/********************************************************************
 * TaskScheduler.cpp
 * 2025-11-14
 * This sample schedules a task to start notepad.exe 30 seconds after
 * the system is started.
 *
 * I was bit lazy to code my own task scheduler, so I skidded MSDN example.
 * For details see https://learn.microsoft.com/ja-jp/windows/win32/taskschd/boot-trigger-example--c---
 * 
 * Honestly I was considering to omit this feature because I was thinking that if the system crashes by KeBugCheck,
 * the created tasks will go away and it makes no sense to have this feature.
 * 
 * But at least if you don't terminate the process or shutdown the system,
 * eventually the program will call the NtRaiseHardError and system will crash in user-mode,
 * which keeps registry and created tasks so I decided to keep this feature.
 * Although after a few times of testing I noticed that KeBugCheck seems to keep created tasks.
 *
********************************************************************/

#define _WIN32_DCOM
#include "TaskScheduler.h"

using namespace std;

/*
int WINAPI InitSecurity() {
    //  ------------------------------------------------------
//  Initialize COM.
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        //MessageBoxW(NULL, L"CoInitializeEx failed", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return (int)hr;
    }

    //  Set general COM security levels.
    hr = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        0,
        NULL);

    if (FAILED(hr))
    {
        //MessageBoxW(NULL, L"CoInitializeSecurity failed", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        CoUninitialize();
        return (int)hr;
    }
    return hr;
}*/


int WINAPI RegisterTask(LPCWSTR pTaskName, PCWSTR TaskFolder, PCWSTR ExePath, PCWSTR Args, PCWSTR Delay, PCWSTR UserName)
{
    //  Initialize COM.
    HRESULT hrInit = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hrInit))
    {
        //MessageBoxW(NULL, L"CoInitializeEx failed", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return (int)hrInit;
    }

    //  Set general COM security levels.
    hrInit = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        0,
        NULL);

    if (FAILED(hrInit))
    {
        //MessageBoxW(NULL, L"CoInitializeSecurity failed", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        CoUninitialize();
        return (int)hrInit;
    }
    //return hrInit;

    //  ------------------------------------------------------
    //  Create a name for the task.
    LPCWSTR wszTaskName = pTaskName;

    //  Get the Windows directory and set the path to Notepad.exe.
    wstring wstrExecutablePath = ExePath;
    wstring wstrArgs = Args;


    //  ------------------------------------------------------
    //  Create an instance of the Task Service. 
    ITaskService* pService = NULL;
    HRESULT hr = CoCreateInstance(CLSID_TaskScheduler,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_ITaskService,
        (void**)&pService);
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Failed to create an instance of ITaskService", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        CoUninitialize();
        return (int)hr;
    }

    //  Connect to the task service.
    hr = pService->Connect(_variant_t(), _variant_t(),
        _variant_t(), _variant_t());
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"ITaskService::Connect failed", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pService->Release();
        CoUninitialize();
        return (int)hr;
    }

    //  ------------------------------------------------------
    //  Get the pointer to the root task folder.  
    //  This folder will hold the new task that is registered.
    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(TaskFolder), &pRootFolder);
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Cannot get Root Folder pointer", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pService->Release();
        CoUninitialize();
        return (int)hr;
    }

    //  If the same task exists, remove it.
    pRootFolder->DeleteTask(_bstr_t(wszTaskName), 0);

    //  Create the task builder object to create the task.
    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);

    pService->Release();  // COM clean up.  Pointer is no longer used.
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Failed to create a task definition", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        CoUninitialize();
        return (int)hr;
    }


    //  ------------------------------------------------------
    //  Get the registration info for setting the identification.
    IRegistrationInfo* pRegInfo = NULL;
    hr = pTask->get_RegistrationInfo(&pRegInfo);
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Cannot get identification pointer", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return (int)hr;
    }

	hr = pRegInfo->put_Author((BSTR)L"NT AUTHORITY\\SYSTEM");
    pRegInfo->Release();
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Cannot put identification info", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return (int)hr;
    }

    //  ------------------------------------------------------
    //  Create the settings for the task
    ITaskSettings* pSettings = NULL;
    hr = pTask->get_Settings(&pSettings);
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Cannot get settings pointer", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return (int)hr;
    }

    //  Set setting values for the task. 
    hr = pSettings->put_StartWhenAvailable(VARIANT_TRUE);
    pSettings->Release();
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Cannot put setting info", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return (int)hr;
    }


    //  ------------------------------------------------------
    //  Get the trigger collection to insert the boot trigger.
    ITriggerCollection* pTriggerCollection = NULL;
    hr = pTask->get_Triggers(&pTriggerCollection);
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Cannot get trigger collection", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return (int)hr;
    }

    //  Add the boot trigger to the task.
    ITrigger* pTrigger = NULL;
    hr = pTriggerCollection->Create(TASK_TRIGGER_BOOT, &pTrigger);
    pTriggerCollection->Release();
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Cannot create a boot trigger", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return (int)hr;
    }

    IBootTrigger* pBootTrigger = NULL;
    hr = pTrigger->QueryInterface(
        IID_IBootTrigger, (void**)&pBootTrigger);
    pTrigger->Release();
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"QueryInterface call failed on IBootTrigger", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return (int)hr;
    }

    hr = pBootTrigger->put_Id(_bstr_t(L"Trigger1"));
    if (FAILED(hr))
		//MessageBoxW(NULL, L"Cannot put trigger ID", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

    //  Set the task to start at a certain time. The time 
    //  format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone).
    //  For example, the start boundary below
    //  is January 1st 2005 at 12:05
    hr = pBootTrigger->put_StartBoundary(_bstr_t(L"1995-01-01T00:00:00"));
    if (FAILED(hr))
		//MessageBoxW(NULL, L"Cannot put the start boundary", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

    //hr = pBootTrigger->put_EndBoundary(_bstr_t(L"2015-05-02T08:00:00"));
    //if (FAILED(hr))
	//	MessageBoxW(NULL, L"Cannot put the end boundary", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

    // Delay the task to start 30 seconds after system start. 
    hr = pBootTrigger->put_Delay((BSTR)Delay);
    pBootTrigger->Release();
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Cannot put boot trigger delay", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return (int)hr;
    }


    //  ------------------------------------------------------
    //  Add an Action to the task. This task will execute Notepad.exe.     
    IActionCollection* pActionCollection = NULL;

    //  Get the task action collection pointer.
    hr = pTask->get_Actions(&pActionCollection);
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Cannot get Task collection pointer", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return (int)hr;
    }

    //  Create the action, specifying it as an executable action.
    IAction* pAction = NULL;
    hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    pActionCollection->Release();
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Cannot create action", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return (int)hr;
    }

    IExecAction* pExecAction = NULL;
    //  QI for the executable task pointer.
    hr = pAction->QueryInterface(
        IID_IExecAction, (void**)&pExecAction);
    pAction->Release();
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"QueryInterface call failed for IExecAction", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return hr;
    }

    //  Set the path of the executable to ExePath and set arguments
    hr = pExecAction->put_Path(_bstr_t(wstrExecutablePath.c_str()));
    hr = pExecAction->put_Arguments(_bstr_t(wstrArgs.c_str()));
    pExecAction->Release();
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Cannot put the action executable path", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return hr;
    }


    //  ------------------------------------------------------
    //  Save the task in the root folder.
    IRegisteredTask* pRegisteredTask = NULL;
    VARIANT varPassword;
    varPassword.vt = VT_EMPTY;
    hr = pRootFolder->RegisterTaskDefinition(
        _bstr_t(wszTaskName),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(UserName),
        varPassword,
        TASK_LOGON_SERVICE_ACCOUNT,
        _variant_t(L""),
        &pRegisteredTask);
    if (FAILED(hr))
    {
		//MessageBoxW(NULL, L"Error registering the task", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return hr;
    }

	//MessageBoxW(NULL, L"\n Success! Task successfully registered.", L"Info", MB_ICONINFORMATION | MB_OK | MB_SYSTEMMODAL);

    //  Clean up.
    pRootFolder->Release();
    pTask->Release();
    pRegisteredTask->Release();
    CoUninitialize();
    return 0;
}