#include "SDK/LogitechLEDLib.h"
#include "setting.h"
#include <windows.h>
#include <iostream>
#include <fcntl.h>
#include <signal.h>

#define physicsPath TEXT("Local\\acpmf_physics")
#define staticPath  TEXT("Local\\acpmf_static")

int _break = 1;
HANDLE physicsPtr;
HANDLE staticPtr;

int createFileMap() {
	physicsPtr = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, O_RDWR, 0, 256, physicsPath);
	staticPtr = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, O_RDWR, 0, 452, staticPath);

	if (!physicsPtr || !staticPtr) return 1;
	return 0;
}

LPVOID viewMapedFile(HANDLE fd, size_t size) {
	LPVOID file = MapViewOfFile(fd, FILE_MAP_READ, 0, 0, size);
	if (!file) {
		std::cerr << "MapViewOfFile Error." << std::endl;
		exit(1);
	}

	return file;
}

void close(HANDLE h, LPVOID ptr) {
	UnmapViewOfFile(ptr);
	CloseHandle(h);
}

void endLoop(int s) {
	std::cout << "ok";
	_break = 0;
}

int main() {
	int lerror = createFileMap();
	if (lerror != 0) {
		std::cerr << "Could not load MMAP." << std::endl;
	}

	LPVOID p = viewMapedFile(physicsPtr, 256);
	LPVOID s = viewMapedFile(staticPtr, 452);

	bool LedInitialized = LogiLedInit();

	if (!LedInitialized) {
		std::cout << "Failed Logitech library init." << std::endl;
		return 1;
	}

	LogiLedSetTargetDevice(LogiLed::DeviceType::Mouse);

	int *RPM = (int*)(uintptr_t(p) + uintptr_t(20));
	int *maxRPM = (int*)(uintptr_t(s) + uintptr_t(452-40));
	int ofstMrpm = *maxRPM - RPMoffset;

	if (signal(SIGINT, endLoop) == SIG_ERR) {
		std::cerr << "Could not receive a signal." << std::endl;
	}

	while (_break) {
		if (*RPM >= RPMoffset) {
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