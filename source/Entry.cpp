#include <iostream>
#include "Includes.h"

MemoryManager *m = new MemoryManager();

int main() {
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)pMisc->Bhop, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)pVisuals->antiFlash, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)pVisuals->wallsChamsLoop, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)pVisuals->rainbowHands, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)pMisc->RadarResolve, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)pAim->RCS, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)pAim->aimbot, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)pAim->TriggerBot, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)pMisc->FakeLag, 0, 0, 0);

	DWORD dwMode;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hOutput, &dwMode); 
	dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING; 
	SetConsoleMode(hOutput, dwMode);
	HWND console = GetConsoleWindow(); 
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 700, 300, TRUE); //set width and height 
	char input;

	while (g_Variables::csgowindow) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(300));
		std::cout << "\033[92m[RELEASE]\033[0m \033[96mCSGOEXTERN.CPP \033[95m(\033[92m+REP \033[95mhttps://www.unknowncheats.me/) \n\033[0m";
		std::cout << "Toggle a feature by typing the associated number \n";
		std::cout << "[1] WallHack & Enemy Chams (Toggle Key: \"END Key\"): " << ((g_Variables::Visuals::Walls) ? "\033[92mActivated" : "\033[31mOffline") << "\n\033[0m";
		std::cout << "[2] BunnyHop (Hold Space): " << ((g_Variables::Misc::Bhop) ? "\033[92mActivated" : "\033[31mOffline") << "\n\033[0m";
		std::cout << "[3] TriggerBot (Hold Left Alt): " << ((g_Variables::Aim::Trigger) ? "\033[92mActivated" : "\033[31mOffline") << "\n\033[0m";
		std::cout << "[4] NoFlash: " << ((g_Variables::Visuals::Flash) ? "\033[92mActivated" : "\033[31mOffline") << "\n\033[0m";
		std::cout << "[5] RadarHack: " << ((g_Variables::Misc::Radar) ? "\033[92mActivated" : "\033[31mOffline") << "\n\033[0m";
		std::cout << "[6] Recoil Control System: " << ((g_Variables::Aim::RCS) ? "\033[92mActivated" : "\033[31mOffline") << "\n\033[0m";
		std::cout << "[7] Low FOV Legit Aimbot (AIM KEY: side mouse btn): " << ((g_Variables::Aim::Aimbot) ? "\033[92mActivated" : "\033[31mOffline") << "\n\033[0m";
		std::cout << "[8] FakeLag (Toggle Key: \"DELETE / DEL Key\"): " << ((g_Variables::Misc::FakeLag) ? "\033[92mActivated" : "\033[31mOffline") << "\n\033[0m";
		std::cout << "[9] \033[31mRA\033[33mIN\033[92mBO\033[36mWH\033[95mAN\033[35mDS: " << ((g_Variables::Visuals::RainbowHands) ? "\033[92mActivated" : "\033[31mOffline") << "\n\033[0m";
		std::cin >> input;
		std::cin.ignore(256, '\n');
		system("cls");
		if (input == '1') {
			g_Variables::Visuals::Walls = !g_Variables::Visuals::Walls;
		}else if (input == '2') {
			g_Variables::Misc::Bhop = !g_Variables::Misc::Bhop;
		}else if (input == '3') {
			g_Variables::Aim::Trigger = !g_Variables::Aim::Trigger;
		}else if (input == '4') {
			g_Variables::Visuals::Flash = !g_Variables::Visuals::Flash;
		}else if (input == '5') {
			g_Variables::Misc::Radar = !g_Variables::Misc::Radar;
		}else if (input == '6') {
			g_Variables::Aim::RCS = !g_Variables::Aim::RCS;
		}else if (input == '7') {
			g_Variables::Aim::Aimbot = !g_Variables::Aim::Aimbot;
		}else if (input == '8') {
			g_Variables::Misc::FakeLag = !g_Variables::Misc::FakeLag;
		}else if (input == '9') {
			g_Variables::Visuals::RainbowHands = !g_Variables::Visuals::RainbowHands;
		}else {
			std::cout << "Invalid Input \n";
		}
	}
	return 0;
}