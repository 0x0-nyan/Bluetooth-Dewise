/*
*
* common.cpp : Commonly used functions and class
* Created in 2025-11-16
* Created by @0x0-nyan
*
*/
#include "common.h"

int WINAPI RandScreenX(int Mode) {
    if (Mode >= 1) {
        SYSTEMTIME st;
        GetLocalTime(&st);
        srand((unsigned int)(st.wMilliseconds + st.wSecond * 1000 + st.wMinute * 60000));
    }
    int ScrX = GetSystemMetrics(SM_CXSCREEN);
    int rVal = rand() % ScrX;
    //if (rVal < 0) rVal = 0;
    if (rVal > ScrX - 200) rVal = ScrX - (rand() % 300);
    return rVal;
}

int WINAPI RandScreenY(int Mode) {
    if (Mode >= 1) {
        SYSTEMTIME st;
        GetLocalTime(&st);
        srand((unsigned int)(st.wMilliseconds + st.wSecond * 1000 + st.wMinute * 60000));
    }
    int ScrY = GetSystemMetrics(SM_CYSCREEN);
    int rVal = rand() % ScrY;
    //if (rVal < 0) rVal = 0;
    if (rVal > ScrY - 300) rVal = ScrY - (rand() % 400);
    return rVal;
}

int WINAPI GetScreenSize(LPSIZE lpScreen) {
    lpScreen->cx = GetSystemMetrics(SM_CXSCREEN);
    lpScreen->cy = GetSystemMetrics(SM_CYSCREEN);
    return 0;
}



WAVE::WAVE() {
    wfx = { 0 };
    hdr = { 0 };
    Closed = TRUE;
    hWaveOut = nullptr;
}

    int WINAPI WAVE::Init(unsigned long SampleFreq) {
        wfx = {
            WAVE_FORMAT_PCM,
            1,
            SampleFreq,
            SampleFreq,
            1,
            8,
            0
        };
        if (waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
            Closed = FALSE;
            return 0;
        }
    }
    int WINAPI WAVE::Volume(DWORD dwVolume) {
        return waveOutSetVolume(hWaveOut, dwVolume);
    }
    int WINAPI WAVE::Play(LPSTR Buffer, DWORD BufferLen, unsigned long nLoop) {
        hdr = { Buffer, BufferLen, 0, 0, WHDR_BEGINLOOP | WHDR_ENDLOOP, nLoop, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &hdr, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &hdr, sizeof(WAVEHDR));
        return 0;
    }
    int WINAPI WAVE::Stop() {
        return waveOutReset(hWaveOut);
    }
    int WINAPI WAVE::Kill() {
        waveOutReset(hWaveOut);
        waveOutSetVolume(hWaveOut, 0xFFFFFFFF);
        waveOutUnprepareHeader(hWaveOut, &hdr, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
        Closed = TRUE;
        return 0;
    }
    WAVE::~WAVE() {   //Destructor
        if (Closed == FALSE) {
            waveOutReset(hWaveOut);
            waveOutSetVolume(hWaveOut, 0xFFFFFFFF);
            waveOutUnprepareHeader(hWaveOut, &hdr, sizeof(WAVEHDR));
            waveOutClose(hWaveOut);
        }
    }
