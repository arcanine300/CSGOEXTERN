#include "Includes.h"

Visuals *pVisuals = new Visuals();

struct clrRenderEnemy {
	BYTE red = g_Variables::Visuals::ChamsColorEnemy[0];
	BYTE green = g_Variables::Visuals::ChamsColorEnemy[1];
	BYTE blue = g_Variables::Visuals::ChamsColorEnemy[2];
}chamsEnm; 

struct clrRenderLocal {
	BYTE red = g_Variables::Visuals::ChamsColorTeam[0];
	BYTE green = g_Variables::Visuals::ChamsColorTeam[1];
	BYTE blue = g_Variables::Visuals::ChamsColorTeam[2];
}chamsLocal;

struct clrRenderHands {
	BYTE red = 252;
	BYTE green = 0;
	BYTE blue = 0;
}chamsHand;

struct clrRenderRandom {
	BYTE red = 252;
	BYTE green = 125;
	BYTE blue = 125;
}chamsRand;

struct glowStructEnemy {
	float red = g_Variables::Visuals::WallHackColorEnemy[0];
	float green = g_Variables::Visuals::WallHackColorEnemy[1];
	float blue = g_Variables::Visuals::WallHackColorEnemy[2];
	float alpha = 1.f;
	uint8_t padding[8];
	float unknown = 1.f;
	uint8_t padding2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = false;
	BYTE fullBloom = false;
}glowEnm;

struct glowStructLocal {
	float red = g_Variables::Visuals::WallHackColorTeam[0];
	float green = g_Variables::Visuals::WallHackColorTeam[1];
	float blue = g_Variables::Visuals::WallHackColorTeam[2];
	float alpha = 1.f;
	uint8_t padding[8];
	float unknown = 1.f;
	uint8_t padding2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = false;
	BYTE fullBloom = false;
}glowLocal;

float brightness = 0.f;
void setModelBrightness() {
	brightness = g_Variables::Visuals::ModelBrightness;
	int ptr = m->ReadMem<int>(m->engine + g_Offsets::Signatures::model_ambient_min - 0x2c);
	int xorptr = *(int*)&brightness ^ ptr;
	m->WriteMem<int>(m->engine + g_Offsets::Signatures::model_ambient_min, xorptr);
}

void unsetModelBrightness() {
	brightness = 0.f;
	int ptr = m->ReadMem<int>(m->engine + g_Offsets::Signatures::model_ambient_min - 0x2c);
	int xorptr = *(int*)&brightness ^ ptr;
	m->WriteMem<int>(m->engine + g_Offsets::Signatures::model_ambient_min, xorptr);
}

bool reloop1 = true;
bool reloop2 = true;
void SetChamColor() {
	//std::cout << "r: " << (float)chamsHand.red << " || g: " << (float)chamsHand.green << " || b: " << (float)chamsHand.blue << "\n";
	if (chamsHand.blue < 252 && reloop1) {
		chamsHand.red -= 6;
		chamsHand.blue += 6;
		if (chamsHand.blue > 251) { reloop2 = true; }
	}
	else if (chamsHand.green < 252 && reloop2) {
		reloop1 = false;
		chamsHand.blue -= 6;
		chamsHand.green += 6;
	}
	else if (chamsHand.red < 252) {
		reloop2 = false;
		chamsHand.green -= 6;
		chamsHand.red += 6;
		if (chamsHand.red > 251) { reloop1 = true; }
	}
}

void Visuals::wallsChamsLoop() {
	std::chrono::steady_clock sc; //timer obj
	auto start = sc.now(); //start timer
	std::srand(std::time(nullptr)); chamsRand.red = rand() % 255; chamsRand.green = rand() % 255; chamsRand.blue = rand() % 255;
	unsetModelBrightness();
	bool toggleWallHack = true;
	SHORT keyState;

	while (true) {
		uintptr_t clientState = m->ReadMem<uintptr_t>(m->engine + g_Offsets::Signatures::dwClientState);
		auto in_game = m->ReadMem<uintptr_t>(clientState + g_Offsets::Signatures::dwClientState_State);

		if (g_Variables::Visuals::Walls && in_game == 6) {

			if (brightness == 0) 
				setModelBrightness();  

			if (!g_Variables::Visuals::RainbowHands) {
				auto end = sc.now();  //end timer
				auto time_span = static_cast<std::chrono::duration<double>>(end - start);
				if (time_span.count() > 180) {
					start = end;
					chamsRand.red = rand() % 255; chamsRand.green = rand() % 255; chamsRand.blue = rand() % 255;
				}
				m->WriteMem<clrRenderRandom>(pLocal->getLocalViewModel() + g_Offsets::Netvars::m_clrRender, chamsRand);
			}

			if (keyState = GetAsyncKeyState(VK_END)) { //end key
				if (keyState < 0) { // key is down 
					toggleWallHack = !toggleWallHack;
					if (toggleWallHack) { Beep(800, 200); } else { Beep(500, 200); } //activation and deactivation sound
				}
			}
			uintptr_t dwGlowManager = m->ReadMem<uintptr_t>(m->moduleBase + g_Offsets::Signatures::dwGlowObjectManager);

			for (short int i = 1; i <= 32; i++) {
				uintptr_t dwEntity = m->ReadMem<uintptr_t>(m->moduleBase + g_Offsets::Signatures::dwEntityList + i * 0x10);
				int Dormant = m->ReadMem<int>(dwEntity + g_Offsets::Signatures::m_bDormant);
				int EnmHealth = m->ReadMem<int>(dwEntity + g_Offsets::Netvars::m_iHealth);
				if (Dormant || (EnmHealth < 1 || EnmHealth > 100) || dwEntity == NULL) 
					continue;

				int iGlowIndx = m->ReadMem<int>(dwEntity + g_Offsets::Netvars::m_iGlowIndex);
				int EntityTeam = m->ReadMem<int>(dwEntity + g_Offsets::Netvars::m_iTeamNum);
				int CurrTeam = pLocal->getTeamNum();

				if (CurrTeam == EntityTeam) {
					if (g_Variables::Visuals::RainbowHands) { 
						m->WriteMem<clrRenderHands>(dwEntity + g_Offsets::Netvars::m_clrRender, chamsHand); 
					}
					else { 
						m->WriteMem<clrRenderLocal>(dwEntity + g_Offsets::Netvars::m_clrRender, chamsLocal); 
					}

					if (toggleWallHack) 
						m->WriteMem<glowStructLocal>(dwGlowManager + (iGlowIndx * 0x38) + 0x8, glowLocal);
				}
				else if (CurrTeam != EntityTeam) {
					m->WriteMem<clrRenderEnemy>(dwEntity + g_Offsets::Netvars::m_clrRender, chamsEnm);

					if (toggleWallHack) 
						m->WriteMem<glowStructEnemy>(dwGlowManager + (iGlowIndx * 0x38) + 0x8, glowEnm);
				}
				std::this_thread::sleep_for(std::chrono::nanoseconds(25));
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}

void Visuals::antiFlash() {
	while (true) {
		if (g_Variables::Visuals::Flash) {
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
			m->WriteMem<float>(pLocal->getLocalPlayer() + g_Offsets::Netvars::m_flFlashDuration, 0.f);
		}
		else {
			m->WriteMem<float>(pLocal->getLocalPlayer() + g_Offsets::Netvars::m_flFlashDuration, 255.f);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

	}
}

void Visuals::rainbowHands() {
	while (true) {
		uintptr_t clientState = m->ReadMem<uintptr_t>(m->engine + g_Offsets::Signatures::dwClientState);
		auto in_game = m->ReadMem<uintptr_t>(clientState + g_Offsets::Signatures::dwClientState_State);

		if (g_Variables::Visuals::RainbowHands && in_game == 6) {
			if (brightness == 0) { setModelBrightness(); }
			SetChamColor();
			m->WriteMem<clrRenderHands>(pLocal->getLocalViewModel() + g_Offsets::Netvars::m_clrRender, chamsHand); 
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else {
			chamsHand.red = 252; chamsHand.green = 0; chamsHand.blue = 0; reloop1 = true; reloop2 = true; //reset rgb cycle
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}

