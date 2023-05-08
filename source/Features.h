#pragma once

class Misc {
public:
	static void FakeLag();
	static void Bhop();
	static void RadarResolve();
}; 
class Visuals {
public:
	static void antiFlash();
	static void wallsChamsLoop();
	static void rainbowHands();
};
class Aim {
public:
	static void aimbot();
	static void RCS();
	static void TriggerBot();
};

extern Aim* pAim;
extern Visuals *pVisuals;
extern Misc *pMisc;