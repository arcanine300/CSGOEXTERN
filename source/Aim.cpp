#include <xmmintrin.h>
#include <list>
#include "Includes.h"
#include "Aim.h"

Aim* pAim = new Aim();

constexpr float M_PI = 3.1415927f;

Vector3 operator + (Vector3 const& lhs, Vector3 const& rhs) {
	return Vector3{ lhs.x + rhs.x,lhs.y + rhs.y,lhs.z + rhs.z };
}
Vector3 operator - (Vector3 const& lhs, Vector3 const& rhs) {
	return Vector3{ lhs.x - rhs.x,lhs.y - rhs.y,lhs.z - rhs.z };
}
Vector3 operator / (Vector3 const& lhs, int const& rhs) {
	return Vector3{ lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
}
Vector3 operator * (Vector3 const& lhs, float const& rhs) {
	return Vector3{ lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}
std::ostream& operator<<(std::ostream& os, Vector3 const& pos) {
	os << "xyz: " << pos.x << "," << pos.y << "," << pos.z;
	return os;
}
Vector3& Vector3::operator *= (float number) {
	this->x *= number; this->y *= number; this->z *= number;
	return *this;
}
Vector3& Vector3::operator-=(Vector3 const& rhs) {
	this->x -= rhs.x; this->y -= rhs.y; this->z -= rhs.z;
	return *this;
}
Vector3& Vector3::operator+=(Vector3 const& rhs) {
	this->x += rhs.x; this->y += rhs.y; this->z += rhs.z;
	return *this;
}

Vector3 ClampAngle(Vector3 angle) {
	while (angle.x < -180.0f)
		angle.x += 360.0f;

	while (angle.x > 180.0f)
		angle.x -= 360.0f;

	if (angle.x > 89.0f)
		angle.x = 89.0f;

	if (angle.x < -89.0f)
		angle.x = -89.0f;

	while (angle.y < -180.0f)
		angle.y += 360.0f;

	while (angle.y > 180.0f)
		angle.y -= 360.0f;

	angle.z = 0.0f;

	return angle;
}

Vector3 CalcVector(const Vector3& src, const Vector3& dst) {
	//square root func faster than normal func youd use
	const auto sqrtss = [](float in)
	{
		__m128 reg = _mm_load_ss(&in);
		return _mm_mul_ss(reg, _mm_rsqrt_ss(reg)).m128_f32[0];
	};
	Vector3 angles;
	//getting delta between source and destination vectors
	Vector3 delta = src - dst;
	//finding the hypoteneuse using pythagoras theorem a squared + b squared = c squared
	//this gives us the vector to our enemy
	float hyp = sqrtss(delta.x * delta.x + delta.y * delta.y);
	//now we need to find the angle needed to aim at the vector (aim angles)
	angles.x = asinf(delta.z / hyp) * (180 / M_PI);
	angles.y = atanf(delta.y / delta.x) * (180 / M_PI) + !((*(DWORD*)&delta.x) >> 31 & 1) * 180;
	angles.z = 0;
	return angles;
}

Vector3	getEntBonePos(uintptr_t playerBase, int boneID) {
	uintptr_t EntBoneMatrix = m->ReadMem<uintptr_t>(playerBase + g_Offsets::Netvars::m_dwBoneMatrix);
	Matrix3x4_t boneMatrix = m->ReadMem<Matrix3x4_t>(EntBoneMatrix + boneID * 0x30);
	return {
		boneMatrix.Matrix[0][3],
		boneMatrix.Matrix[1][3],
		boneMatrix.Matrix[2][3]
	};
}

void Aim::TriggerBot() {
	while (true) {
		if (g_Variables::Aim::Trigger) {
			int inCross = m->ReadMem<int>(pLocal->getLocalPlayer() + g_Offsets::Netvars::m_iCrosshairId) - 1;
			uintptr_t tEntityBase = m->ReadMem<uintptr_t>(m->moduleBase + g_Offsets::Signatures::dwEntityList + ((inCross) * 0x10));
			int currentTeam = pLocal->getTeamNum();
			int tEntityTeam = pEntity->getEntityTeamNum(tEntityBase);
			bool dormant = pEntity->getEntityDormantStatus(tEntityBase);

			if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			{
				if (GetAsyncKeyState(VK_MENU) & 0x8000)
				{
					if ((inCross > 0 && inCross < 64) && (tEntityBase != NULL) && (tEntityTeam != currentTeam) && (!dormant))
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(g_Variables::Aim::TriggerDelayMs)); //fake reaction time / delay before shot
						m->WriteMem<int>(m->moduleBase + g_Offsets::Signatures::dwForceAttack, 6);
						int recovery = pLocal->getDistanceFromPlayer(tEntityBase) * 3.3;
						std::this_thread::sleep_for(std::chrono::milliseconds(recovery)); //dynamic delay based on distance
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::nanoseconds(600));
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}

void Aim::RCS() {
	float RCSscale = 1.4f;
	Vector3 OldAimPunch;
	while (true) {
		uintptr_t clientState = m->ReadMem<uintptr_t>(m->engine + g_Offsets::Signatures::dwClientState);
		auto in_game = m->ReadMem<uintptr_t>(clientState + g_Offsets::Signatures::dwClientState_State); //only aimbot if in game
		if (g_Variables::Aim::RCS && in_game == 6) {
			Vector3 ViewAngle = m->ReadMem<Vector3>(clientState + g_Offsets::Signatures::dwClientState_ViewAngles); // get local view angle
			Vector3 localAimPunch = pLocal->getAimPunchAngle();
			int shotsFired = pLocal->getShotsFired();
			if (shotsFired > 1) {
				Vector3 NewAimPunch;
				NewAimPunch.x = ((ViewAngle.x + OldAimPunch.x) - (localAimPunch.x * RCSscale));
				NewAimPunch.y = ((ViewAngle.y + OldAimPunch.y) - (localAimPunch.y * RCSscale));
				ViewAngle = ClampAngle(NewAimPunch);
				m->WriteMem<Vector3>(clientState + g_Offsets::Signatures::dwClientState_ViewAngles, ViewAngle);
			}
			OldAimPunch.x = localAimPunch.x * RCSscale; //Set Previous Punch To Current
			OldAimPunch.y = localAimPunch.y * RCSscale;
			std::this_thread::sleep_for(std::chrono::nanoseconds(600));
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}

void Aim::aimbot() {
	//user config vars
	int AimSmoothing = 60;
	int AimBone = 8;
	float AimFov = 1.2f;
	float AimbotRCSscale = 1.6f;

	//private vars
	int EntityTeam;
	int EnmHealth;
	bool Dormant;
	float AimFovMem = AimFov;
	Vector3 ViewVector;
	Vector3 eyePos;
	Vector3 lookVector;
	Vector3 cVector;
	Vector3 sAngles;
	float fovCalc;
	uintptr_t bestTarget = NULL;

	while (true) {
		uintptr_t clientState = m->ReadMem<uintptr_t>(m->engine + g_Offsets::Signatures::dwClientState);
		auto in_game = m->ReadMem<uintptr_t>(clientState + g_Offsets::Signatures::dwClientState_State); //only aimbot if in game

		if (g_Variables::Aim::Aimbot && in_game == 6) {
			//loop through all entities in g_Offsets::Signatures::dwEntityList and find closest target
			for (int i = 1; i < 32; i++) {
				std::this_thread::sleep_for(std::chrono::nanoseconds(100));
				uintptr_t dwEntity = m->ReadMem<uintptr_t>(m->moduleBase + g_Offsets::Signatures::dwEntityList + i * 0x10); //extract single entity

				EntityTeam = m->ReadMem<int>(dwEntity + g_Offsets::Netvars::m_iTeamNum);
				EnmHealth = m->ReadMem<int>(dwEntity + g_Offsets::Netvars::m_iHealth);
				Dormant = pEntity->getEntityDormantStatus(dwEntity);
				//do checks, if ent doesn't exist, or is dead, or is teammate, or is dormant, skip
				if (!dwEntity || pLocal->getTeamNum() == EntityTeam || (EnmHealth < 1 || EnmHealth > 100) || Dormant)
					continue;

				//simulate looking at ent, check if its within AimFov
				//int shotsFired = pLocal->getShotsFired(); (shotsFired > 1) ? AimBone = 6 : AimBone = 8;
				ViewVector = m->ReadMem<Vector3>(clientState + g_Offsets::Signatures::dwClientState_ViewAngles); // get local view angle
				eyePos = pLocal->getLocalEyePos(); //world location of player head
				lookVector = CalcVector(eyePos, getEntBonePos(dwEntity, AimBone)); //simulate player head looking at enemy bone
				cVector = ClampAngle(lookVector - ViewVector); //dont get vac'd, clamp view angles https://www.unknowncheats.me/forum/counterstrike-global-offensive/291395-clamp-angles-question.html
				fovCalc = sqrt(cVector.x * cVector.x + cVector.y * cVector.y);

				//always set at least 1 ent to bestTarget or else aimbot wont have a target on fresh start
				if (fovCalc < AimFov) { //TODO: distance check, store multiple bestTargets
					AimFov = fovCalc;
					bestTarget = dwEntity;
				}
			}
			AimFov = AimFovMem;

			while (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) {
				std::this_thread::sleep_for(std::chrono::nanoseconds(100));

				EnmHealth = m->ReadMem<int>(bestTarget + g_Offsets::Netvars::m_iHealth);
				Dormant = pEntity->getEntityDormantStatus(bestTarget);
				//do checks, if ent doesn't exist, or is dead, or is teammate, or is dormant, skip
				if (!bestTarget || (EnmHealth < 1 || EnmHealth > 100) || Dormant)
					break;

				//int shotsFired = pLocal->getShotsFired(); (shotsFired > 1) ? AimBone = 6 : AimBone = 8;
				eyePos = pLocal->getLocalEyePos(); //world location of player head
				ViewVector = m->ReadMem<Vector3>(clientState + g_Offsets::Signatures::dwClientState_ViewAngles); //current player aim vector
				lookVector = CalcVector(eyePos, getEntBonePos(bestTarget, AimBone)); //vector from current players head to enemy bone
				cVector = ClampAngle(lookVector - ViewVector); //vector difference, Players ViewAngle - Angle to Enemy
				fovCalc = sqrt(cVector.x * cVector.x + cVector.y * cVector.y);
				//std::cout << "fovCalc: " << fovCalc << " || Max Angle: " << AimFov <<  " || lAngle-vAngle: " << cVector << "\n"; //debug output

				if (fovCalc < AimFov) { //dont shoot if enemy is out of aimbot fov
					lookVector -= pLocal->getAimPunchAngle() * AimbotRCSscale;
					Vector3 delta = ClampAngle(lookVector - ViewVector);
					sAngles = ClampAngle(ViewVector + (delta / AimSmoothing));
					m->WriteMem<Vector3>(clientState + g_Offsets::Signatures::dwClientState_ViewAngles, sAngles); //set view angles
					m->WriteMem<int>(m->moduleBase + g_Offsets::Signatures::dwForceAttack, 6); //force attack, gg
				} 
				else {
					break;
				}
			}
			std::this_thread::sleep_for(std::chrono::nanoseconds(2500));
		} 
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}