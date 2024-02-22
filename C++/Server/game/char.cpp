//Add

#ifdef ENABLE_SUNGMAHEE_TOWER
#include "SungmaHeeTower.h"
#endif

//Search

void CHARACTER::OnClick(LPCHARACTER pkChrCauser)
{
	if (!pkChrCauser)
	{
		sys_err("OnClick %s by NULL", GetName());
		return;
	}

	DWORD vid = GetVID();
	sys_log(0, "OnClick %s[vnum %d ServerUniqueID %d, pid %d] by %s", GetName(), GetRaceNum(), vid, GetPlayerID(), pkChrCauser->GetName());

//Add after

#ifdef ENABLE_SUNGMAHEE_TOWER
	if ((IsNPC()) && (GetRaceNum() == (WORD)(SungmaHeeTower::PORTAL_VNUM)) && (SungmaHeeTower::SmhT::instance().IsSungmaTowerMap(pkChrCauser->GetMapIndex())))
	{
		SungmaHeeTower::SmhT::instance().GoToNextRoom(pkChrCauser);
		return;
	}

#endif

//Add at to the end file

#ifdef ENABLE_SUNGMAHEE_TOWER
void CHARACTER::SungmaHeeTowerEnter()
{
	if (!SungmaHeeTower::SmhT::instance().Access(this));
	{
		ChatPacket(CHAT_TYPE_INFO, "Error!!! This Dungeon it's over, you will need contact with the Admin.");
	}
}
#endif