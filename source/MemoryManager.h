#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h>

#include <SDKDDKVer.h>
#include <stdio.h>
#include <optional>
#include <Psapi.h>
#include <algorithm>

#include "Variables.h"

class MemoryManager {
public:
	uintptr_t moduleBase;
	uintptr_t engine;
	MODULEENTRY32 client;
	DWORD pID;
	static HANDLE hProc;

	bool processHack(const char* process_name_cs_go) {
		HANDLE hwdProcessID_ = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		PROCESSENTRY32 processEntryVar;
		processEntryVar.dwSize = sizeof(processEntryVar);
		do {
			if (!strcmp(processEntryVar.szExeFile, process_name_cs_go)) {
				pID = processEntryVar.th32ProcessID;
				CloseHandle(hwdProcessID_);
				this->hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->pID);
				return true;
			}
		} while (Process32Next(hwdProcessID_, &processEntryVar));
		CloseHandle(hwdProcessID_);
		return false;
	}

	uintptr_t GetModuleBaseAddress(const char* modName, DWORD procId) {

		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

		if (hsnap != INVALID_HANDLE_VALUE) {

			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(modEntry);

			if (Module32First(hsnap, &modEntry)) {

				do {
					if (!strcmp(modEntry.szModule, modName)) {
						CloseHandle(hsnap);
						return (uintptr_t)modEntry.modBaseAddr;
					}
				} while (Module32Next(hsnap, &modEntry));
			}
		}
	}

	MODULEENTRY32 GetModule(const char* modName, DWORD procId) {

		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

		if (hsnap != INVALID_HANDLE_VALUE) {

			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(modEntry);

			if (Module32First(hsnap, &modEntry)) {

				do {
					if (!strcmp(modEntry.szModule, modName)) {
						CloseHandle(hsnap);
						return modEntry;
					}
				} while (Module32Next(hsnap, &modEntry));
			}
		}
		MODULEENTRY32 module = { -1 };
		return module;
	}

	template <class c>
	c ReadMem(DWORD dwAddress) {
		c val;
		ReadProcessMemory(hProc, (LPVOID)dwAddress, &val, sizeof(c), NULL);
		return val;
	}

	template <class c>
	BOOL WriteMem(DWORD dwAddress, c valToWrite) {
		return WriteProcessMemory(hProc, (LPVOID)dwAddress, &valToWrite, sizeof(c), NULL);
	}

	template <class c>
	BOOL WriteMemAfterProtect(DWORD dwAddress, c valToWrite) {
		DWORD_PTR oldProtect;
		VirtualProtectEx(hProc, (LPVOID)dwAddress, sizeof(c), PAGE_EXECUTE_READWRITE, &oldProtect);
		WriteProcessMemory(hProc, (LPVOID)dwAddress, &valToWrite, sizeof(c), NULL);
		VirtualProtectEx(hProc, (LPVOID)dwAddress, sizeof(c), oldProtect, NULL);
		return true;
	}
	MemoryManager();
	~MemoryManager();
};

