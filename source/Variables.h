#pragma once

namespace g_Variables {
	extern bool csgowindow;

	namespace Visuals {
		extern float ChamsColorEnemy[3];
		extern float ChamsColorTeam[3];
		extern bool Flash;
		extern bool Walls; 
		extern bool RainbowHands;
		extern float WallHackColorEnemy[3];
		extern float WallHackColorTeam[3];
		extern float ModelBrightness;
		extern float ChamsPlayer[3];
	}
	namespace Misc {
		extern bool FakeLag;
		extern bool Bhop;
		extern bool Radar;
	}
	namespace Aim {
		extern bool Aimbot;
		extern bool RCS;
		extern bool Trigger;
		extern int TriggerDelayMs;
		extern int TrigggerRecoveryMs;
	}
}