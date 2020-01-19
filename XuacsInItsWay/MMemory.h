#pragma once

#ifndef MMANAGER_H
#define MMANAGER_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <Psapi.h>
#include <stdarg.h>
#include <stdint.h>

struct Module {
	DWORD dwBase;
	DWORD dwSize;
};

class Memory {
public:
	Memory();
	~Memory();

	bool Attached;
	bool Attach(const char* pName, DWORD rights = PROCESS_ALL_ACCESS);
	void Detach();

	Module GetModule(const char* moduleName);
	MODULEENTRY32 GetModule2(char* moduleName);
	HMODULE LoadModule(const char* moduleName);

	template <class T>
	T Read(DWORD addr) {
		T _read;
		ReadProcessMemory(_process, (LPVOID)addr, &_read, sizeof(T), NULL);
		return _read;
	}
	BYTE* ReadBytes(DWORD addr, int len, BYTE* pointer = 0) {
		BYTE* _read = new BYTE[len];
		ReadProcessMemory(_process, (LPVOID)addr, _read, len, NULL);
		if (pointer != 0)
			pointer = _read;
		return _read;
	}

	template <class T>
	void Write(DWORD addr, T val) {
		WriteProcessMemory(_process, (LPVOID)addr, &val, sizeof(T), NULL);
	}

	DWORD FindPattern(DWORD start, DWORD size, const char* sig, const char* mask);

public:
	HANDLE _process;
	DWORD pID;
	bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask);
};

struct Glow {
	DWORD pEntity;
	float r;
	float g;
	float b;
	float a;
	uint8_t unk1[16];
	bool RenderWhenOccluded;
	bool RenderWhenUnoccluded;
	bool FullBloom;
	uint8_t unk2[14];
};
#endif