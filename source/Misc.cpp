#include "Includes.h"

void Misc::Bhop() {
	uintptr_t jumpBuffer;
	while (true) {
		if (g_Variables::Misc::Bhop) {
			std::this_thread::sleep_for(std::chrono::milliseconds(3));
			BYTE flags = m->ReadMem<BYTE>(pLocal->getLocalPlayer() + g_Offsets::Netvars::m_fFlags);
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				if (flags & 1) { jumpBuffer = 5; } else { jumpBuffer = 4; }
				m->WriteMem<DWORD>(m->moduleBase + g_Offsets::Signatures::dwForceJump, jumpBuffer);
			}
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}

void Misc::RadarResolve() {
	while (true) {
		if (g_Variables::Misc::Radar) {
			for (int i = 1; i < 64; i++) {
				int dwEntity = m->ReadMem<int>(m->moduleBase + g_Offsets::Signatures::dwEntityList + i * 0x10);
				bool EntityIsDormant = pEntity->getEntityDormantStatus(dwEntity);

				if (EntityIsDormant) continue;

				int pEntTeam = pEntity->getEntityTeamNum(dwEntity);

				if (pEntTeam == pLocal->getTeamNum()) continue;

				m->WriteMem<int>(dwEntity + g_Offsets::Netvars::m_bSpotted, true);
				std::this_thread::sleep_for(std::chrono::nanoseconds(600));
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		
	}
}

void Misc::FakeLag() {
	SHORT keyState;
	bool toggle = false;
	while (true) {
		if (g_Variables::Misc::FakeLag) {

			if (keyState = GetAsyncKeyState(VK_DELETE)) { //delete key
				if (keyState < 0) { // key is down 
					toggle = !toggle;
					if (toggle) { Beep(800, 200); } else { Beep(500, 200); } //activation and deactivation sound
				}
			}

			if (toggle) {
				std::this_thread::sleep_for(std::chrono::nanoseconds(600));
				if (GetAsyncKeyState(VK_LBUTTON)) //send packets if shooting
				{
					m->WriteMemAfterProtect<BYTE>(m->engine + g_Offsets::Signatures::dwbSendPackets, 1);
					std::this_thread::sleep_for(std::chrono::milliseconds(250)); //delay sleep incase you are full auto
				}
				else { //choke packets if enabled
					m->WriteMemAfterProtect<BYTE>(m->engine + g_Offsets::Signatures::dwbSendPackets, 0);
					std::this_thread::sleep_for(std::chrono::milliseconds(4)); //choke packets for less than 12ticks
					m->WriteMemAfterProtect<BYTE>(m->engine + g_Offsets::Signatures::dwbSendPackets, 1);
				}
			}
			else {
				m->WriteMemAfterProtect<BYTE>(m->engine + g_Offsets::Signatures::dwbSendPackets, 1);
				std::this_thread::sleep_for(std::chrono::milliseconds(24));
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}

Misc *pMisc = new Misc();