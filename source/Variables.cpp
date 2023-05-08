#include "Variables.h"

namespace g_Variables {
	bool csgowindow = false;

	namespace Visuals {
		float ChamsPlayer[3] = { 248.f, 0.f, 0.f };
		float ChamsColorEnemy[3] = { 10.f, 205.f, 205.f };
		float ChamsColorTeam[3] = { 225.f, 0.f, 164.f };
		bool Flash = false;
		bool Walls = false;
		bool RainbowHands = false;
		float WallHackColorEnemy[3] = { .250f, .03f, .250f };
		float WallHackColorTeam[3] = { .245f, .225f, .0f };
		float ModelBrightness = 6.f;
		
	}
	namespace Misc {
		bool FakeLag = false;
		bool Bhop = false;
		bool Radar = false;
	}
	namespace Aim {
		bool Aimbot = false;
		bool RCS = false;
		bool Trigger = false;
		int TriggerDelayMs = 20;
		int TrigggerRecoveryMs = 175;
	}
}