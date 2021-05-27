#include "SDK/LogitechLEDLib.h"
#include "setting.h"
#include <windows.h>
#include <iostream>
#include <fcntl.h>
#include <signal.h>

#define physicsPath TEXT("Local\\acpmf_physics")
#define staticPath  TEXT("Local\\acpmf_static")
#define seekP 256
#define seekS 452

using namespace std;

int hbreak = 1;
HANDLE physicsPtr;
HANDLE staticPtr;

int createFileMap() {
	physicsPtr = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, O_RDWR, 0, seekP, physicsPath);
	staticPtr = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, O_RDWR, 0, seekS, staticPath);

	if (!physicsPtr || !staticPtr) return 1;
	return 0;
}

LPVOID viewMapedFile(HANDLE fd, size_t size) {
	LPVOID file = MapViewOfFile(fd, FILE_MAP_READ, 0, 0, size);
	if (!file) {
		cerr << "MapViewOfFile Error." << endl;
		exit(1);
	}

	return file;
}

void close(HANDLE h, LPVOID ptr) {
	UnmapViewOfFile(ptr);
	CloseHandle(h);
}

void endLoop(int s) {
	hbreak = 0;
}

int main() {
	int major, minor, build;
	int lerror = createFileMap();
	if (lerror != 0) {
		cerr << "Could not load MMAP." << endl;
	}

	LPVOID p = viewMapedFile(physicsPtr, seekP);
	LPVOID s = viewMapedFile(staticPtr, seekS);

	bool LedInitialized = LogiLedInit();

	if (!LedInitialized) {
		cerr << "Failed Logitech library init." << endl;
		return 1;
	}

	LogiLedGetSdkVersion(&major, &minor, &build);
	cout << "Done initialize.\nSDK Version:: ";
	cout << major << "." << minor << "." << build << endl;
	
	if (!LogiLedSetTargetDevice(LogiLed::DeviceType::Mouse)) {
		cout << "Could not find the target.\nPlease edit the line #67 in LogiLed::DeviceType of main.cpp." << endl;
	}

	const int *const RPM = (int*)(uintptr_t(p) + uintptr_t(20));
	const int *const maxRPM = (int*)(uintptr_t(s) + uintptr_t(412));
	int ofstMrpm = *maxRPM - RPMoffset;

	if (signal(SIGINT, endLoop) == SIG_ERR) {
		cerr << "Could not receive a signal." << endl;
	}

	while (hbreak) {
		if (*RPM >= ofstMrpm) {
			LogiLedSetLighting(OnR, OnG, OnB);
			Sleep(OnDuration);
			LogiLedSetLighting(OffR, OffG, OffB);
			Sleep(OffDuration);
		}

		Sleep(interval);
	}

	close(physicsPtr, p);
	close(staticPtr, s);
	LogiLedShutdown();

	return 0;
}