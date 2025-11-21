#pragma once
#pragma comment(lib, "winmm.lib")
#include <Windows.h>



int WINAPI RandScreenX(int Mode);
int WINAPI RandScreenY(int Mode);
int WINAPI GetScreenSize(LPSIZE lpScreen);


class WAVE {
private:
	WAVEFORMATEX wfx;
	WAVEHDR hdr;
	BOOLEAN Closed;
public:
	WAVE();
	HWAVEOUT hWaveOut;
	int WINAPI Init(unsigned long SampleFreq);
	int WINAPI Volume(DWORD dwVolume);
	int WINAPI Play(LPSTR Buffer, DWORD BufferLen, unsigned long nLoop);
	int WINAPI Stop();
	int WINAPI Kill();
	~WAVE();
};
