//Add

#ifdef ENABLE_SUNGMAHEE_TOWER
#include "SungmaHeeTower.h"
#endif

//Search

	if (true == IsMonster() && 2493 == GetMobTable().dwVnum)
	{
		if (NULL != pkKiller && NULL != pkKiller->GetGuild())
		{
			CDragonLairManager::instance().OnDragonDead( this, pkKiller->GetGuild()->GetID() );
		}
		else
		{
			sys_err("DragonLair: Dragon killed by nobody");
		}
	}

//Add after

#ifdef ENABLE_SUNGMAHEE_TOWER
	if ((IsStone()) || (IsMonster()))
	{
		if (pkKiller && pkKiller->IsPC())
		{
			if (SungmaHeeTower::SmhT::instance().IsSungmaTowerMap(pkKiller->GetMapIndex()))
				SungmaHeeTower::SmhT::instance().OnKill(this, pkKiller);
		}
	}
#endif