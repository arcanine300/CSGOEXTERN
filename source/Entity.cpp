#include "Includes.h"
#include "Aim.h"

LocalPlayer *pLocal = new LocalPlayer();

LocalPlayer::LocalPlayer() {}
LocalPlayer::~LocalPlayer() {}

uintptr_t LocalPlayer::getLocalPlayer() {
	return m->ReadMem<uintptr_t>(m->moduleBase + g_Offsets::Signatures::dwLocalPlayer);
}

int LocalPlayer::getLocalViewModel() {
	uintptr_t localPlayer = pLocal->getLocalPlayer();
	int viewModelHandler = m->ReadMem<int>(pLocal->getLocalPlayer() + g_Offsets::Netvars::m_hViewModel);
	int currViewModelEntity = m->ReadMem<int>(m->moduleBase + g_Offsets::Signatures::dwEntityList + (((viewModelHandler & 0xFFF) - 1) * 16));
	if (currViewModelEntity != NULL) {
		return currViewModelEntity;
	}
}

Vector3 LocalPlayer::getAimPunchAngle() {
	return m->ReadMem<Vector3>(pLocal->getLocalPlayer() + g_Offsets::Netvars::m_aimPunchAngle);
}

Vector3 LocalPlayer::getLocalPlayerPos() {
	return m->ReadMem<Vector3>(pLocal->getLocalPlayer() + g_Offsets::Netvars::m_vecOrigin);
}

float LocalPlayer::getDistanceFromPlayer(uintptr_t enemy) {
	Vector3 playerPos = pLocal->getLocalPlayerPos();
	Vector3 enemyPos = m->ReadMem<Vector3>(enemy + g_Offsets::Netvars::m_vecOrigin);
	float distance = sqrt(std::pow(playerPos.x - enemyPos.x, 2) + std::pow(playerPos.y - enemyPos.y, 2) + std::pow(playerPos.z - enemyPos.z, 2)) * 0.0254;
	return distance;
}

Vector3 LocalPlayer::getLocalEyePos() {
	Vector3 localPos = pLocal->getLocalPlayerPos();
	Vector3 headOffsetPos = m->ReadMem<Vector3>(pLocal->getLocalPlayer() + g_Offsets::Netvars::m_vecViewOffset);
	localPos = localPos + headOffsetPos;
	return localPos;
}

int LocalPlayer::getCurrentWeapon() {
	int weapon = m->ReadMem<int>(pLocal->getLocalPlayer() + g_Offsets::Netvars::m_hActiveWeapon);
	int wepEnt = m->ReadMem<int>(m->moduleBase + g_Offsets::Signatures::dwEntityList + ((weapon & 0xFFF) - 1) * 0x10);
	if (wepEnt != NULL) {
		return m->ReadMem<int>(wepEnt + g_Offsets::Netvars::m_iItemDefinitionIndex);
	}
}

int LocalPlayer::getShotsFired() {
	return m->ReadMem<int>(pLocal->getLocalPlayer() + g_Offsets::Netvars::m_iShotsFired);
}

int LocalPlayer::getTeamNum() {
	return m->ReadMem<int>(pLocal->getLocalPlayer() + g_Offsets::Netvars::m_iTeamNum);
}

Entity *pEntity = new Entity();

Entity::Entity() {}
Entity::~Entity() {}

int Entity::getEntityClassID(uintptr_t dwEntity) {
	int one = m->ReadMem<int>(dwEntity + 0x8); //IClientNetworkable vtable
	int two = m->ReadMem<int>(one + 2 * 0x4); //3rd function in the vtable (GetClientClass)
	int three = m->ReadMem<int>(two + 0x1); //pointer to the ClientClass struct out of the mov eax
	int classid = m->ReadMem<int>(three + 0x14); //classid
	return classid;
}

int Entity::getEntityTeamNum(int EntityList) {
	return m->ReadMem<int>(EntityList + g_Offsets::Netvars::m_iTeamNum);
}

bool Entity::getEntityDormantStatus(int EntityList) {
	return m->ReadMem<bool>(EntityList + g_Offsets::Signatures::m_bDormant);
}
