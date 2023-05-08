#include "MemoryManager.h"
HANDLE MemoryManager::hProc;
MemoryManager::MemoryManager() {
	DWORD bytesRead;
	this->hProc = NULL;
	this->pID = NULL;
	try {
		if (!processHack("csgo.exe")) 
			throw 1;

		this->moduleBase = GetModuleBaseAddress("client.dll", pID);
		this->engine = GetModuleBaseAddress("engine.dll", pID);
		this->client = GetModule("client.dll", pID);
		g_Variables::csgowindow = true;
	}
	catch (...) {
		MessageBoxA(NULL, "Couldn't attach to CS:GO.", "'Hacking isnt Cheating'", MB_ICONSTOP | MB_OK);
		g_Variables::csgowindow = false;
	}
}

MemoryManager::~MemoryManager() {
	CloseHandle(this->hProc);
}