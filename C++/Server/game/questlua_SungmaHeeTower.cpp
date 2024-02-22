#include "stdafx.h"

#ifdef ENABLE_SUNGMAHEE_TOWER
#undef sys_err
#ifndef __WIN32__
	#define sys_err(fmt, args...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, ##args)
#else
	#define sys_err(fmt, ...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#endif

#include "questlua.h"
#include "questmanager.h"
#include "SungmaHeeTower.h"
#include "char.h"
#include "party.h"

namespace quest
{
	ALUA(SungmaHeeTower_Open)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		if (pkChar)
		{
			SungmaHeeTower::SmhT::instance().Open(pkChar);
			lua_pushboolean(L, true);
		}
		else
		{
			lua_pushboolean(L, false);
		}
		
		return 2;
	}

	void RegisterSungmaHeeTowerFunctionTable()
	{
		luaL_reg functions[] =
		{
			{"Open", SungmaHeeTower_Open},
			{NULL, NULL}
		};
		
		CQuestManager::instance().AddLuaFunctionTable("SungmaHeeTower", functions);
	}

};

#endif