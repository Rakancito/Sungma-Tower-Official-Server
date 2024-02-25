/*
	Author: R4kan
	Date: 31-01-2022
	Version: 0.5
	Github: https://github.com/Rakancito/
*/

#include "stdafx.h"
#include "SungmaHeeTower.h"

#include "item.h"
#include "char.h"
#include "utils.h"
#include "party.h"
#include "regen.h"
#include "config.h"
#include "packet.h"
#include "motion.h"
#include "item_manager.h"
#include "guild_manager.h"
#include "guild.h"
#include "start_position.h"
#include "locale_service.h"
#include "char_manager.h"
#include "desc.h"
#include "group_text_parse_tree.h"
#include <boost/lexical_cast.hpp> 
#include "db.h"

namespace SungmaHeeTower
{
	/*
		Struct Part
	*/

	struct FSkillOneSungmaHeeTower
	{
		FSkillOneSungmaHeeTower() {};
		void operator()(LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER pkChar = (LPCHARACTER) ent;
				if (pkChar && pkChar->IsPC())
				{
					if (!pkChar->IsDead())
					{
						pkChar->EffectPacket(SE_EFFECT_SMH_CIRCLE_THORN);
						pkChar->ComputeSnakeSkill(273, pkChar, 1);
					}
				}
			}
		}
	};

	struct FChatMap
	{
		FChatMap(const char* str, long lMapIndex) : m_str(str), m_lMapIndex(lMapIndex) {};
		void operator()(LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER pkChar = (LPCHARACTER) ent;
				if (pkChar->IsPC())
					pkChar->ChatPacket(CHAT_TYPE_INFO, "%s", m_str);
			}
		}

		const char* m_str;
		long m_lMapIndex;
	};

	struct FCountMonsters
	{
		size_t m_cnt_Monsters;

		FCountMonsters() : m_cnt_Monsters(0) {}

		void operator() (LPENTITY ent)
		{
			if ( ent->IsType(ENTITY_CHARACTER) == true )
			{
				LPCHARACTER lpChar = (LPCHARACTER)ent;

				if ( lpChar->IsMonster() == true && (lpChar->GetMobTable().dwVnum >= RANGE_MIN_VNUM_MONSTERS && lpChar->GetMobTable().dwVnum <= RANGE_MAX_VNUM_MONSTERS))
				{
					m_cnt_Monsters++;
				}
			}
		}
	};

	struct FExitAndGoTo
	{
		FExitAndGoTo() {};
		void operator()(LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER pkChar = (LPCHARACTER) ent;
				if (pkChar && pkChar->IsPC())
				{
					PIXEL_POSITION posSub = SmhT::instance().GetSubXYZ();
					if (!posSub.x)
						pkChar->WarpSet(EMPIRE_START_X(pkChar->GetEmpire()), EMPIRE_START_Y(pkChar->GetEmpire()));
					else
						pkChar->WarpSet(posSub.x, posSub.y);
				}
			}
		}
	};

	struct FSendInformationBoardMap
	{
		FSendInformationBoardMap(long lMapIndex, SmhTMap* pMap) : m_lMapIndex(lMapIndex), m_pMap(pMap) {};
		void operator()(LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER pkChar = (LPCHARACTER) ent;
				if (pkChar->IsPC())
					m_pMap->SendClientPacketInformationBoard(pkChar, SUB_HEADER_INFORMATION_SEND_BOARD);
			}
		}

		SmhTMap* m_pMap;
		long m_lMapIndex;
	};

	struct FPurgeSectree
	{
		void operator () (LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER ch = (LPCHARACTER) ent;
#ifdef NEW_PET_SYSTEM
				if (ch && !ch->IsPC() && !ch->IsPet() && !ch->IsNewPet())
#else
				if (ch && !ch->IsPC() && !ch->IsPet())
#endif
				{
					//if (ch->IsNPC())
					//	M2_DESTROY_CHARACTER(ch);
					//else
					if (ch->IsMonster())
						ch->Dead(); //fix Purge Area
				}
			}
		}
	};

	struct FPurgeSectreeAll
	{
		void operator () (LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER ch = (LPCHARACTER) ent;
#ifdef NEW_PET_SYSTEM
				if (ch && !ch->IsPC() && !ch->IsPet() && !ch->IsNewPet())
#else
				if (ch && !ch->IsPC() && !ch->IsPet())
#endif
				{
					if (ch->IsNPC())
						M2_DESTROY_CHARACTER(ch);
					else
						ch->Dead(); //fix Purge Area
				}
			}
		}
	};

	/*
		Event Part
	*/

	EVENTINFO(r_sungmahispawn_info)
	{
		SmhTMap*	pMap;
		BYTE	bStep;
		BYTE	bSubStep;
		BYTE	bCounter;
	};
	
	EVENTFUNC(r_sungmahispawn_event)
	{
		r_sungmahispawn_info* pEventInfo = dynamic_cast<r_sungmahispawn_info*>(event->info);

		if (!pEventInfo)
			return 0;
		
		SmhTMap* pMap = pEventInfo->pMap;
		if (!pMap)
			return 0;
		
		if (!pMap->GetMapSectree())
			return 0;
		
		/*
			First Floor
		*/

		if (pEventInfo->bStep == 1)
		{

			/* 
				Stage 0
			*/

			if (pEventInfo->bSubStep == 1)
			{
				char szNotice[512];
				snprintf(szNotice, sizeof(szNotice), LC_TEXT("The Battle will start in %d seconds."), pEventInfo->bCounter);

				pMap->SetIsNextStep(false);
				FChatMap f(szNotice, pMap->GetMapIndex());
				pMap->GetMapSectree()->for_each(f);

				pEventInfo->bCounter--;

				if (pEventInfo->bCounter == 0)
				{
					pMap->SetDungeonSubStep(2);
					return 0;
				}
				return PASSES_PER_SEC(1);
			}

			else if (pEventInfo->bSubStep == 2)
			{
				char szNotice[512];
				snprintf(szNotice, sizeof(szNotice), LC_TEXT("Kill all the monsters within the time limit!"));

				FChatMap f(szNotice, pMap->GetMapIndex());
				pMap->GetMapSectree()->for_each(f);

				pMap->SetDungeonLimitTime(TIME_PER_FLOOR_1);

				struct FSendInformationBoardMap f2(pMap->GetMapIndex(), pMap);
				pMap->GetMapSectree()->for_each(f2);

				pMap->SetDungeonSubStep(3);

				return 0;				
			}
			else if (pEventInfo->bSubStep == 3)
			{
				struct FCountMonsters f;
				pMap->GetMapSectree()->for_each(f);

				if (f.m_cnt_Monsters < 1)
					regen_load_in_file("data/dungeon/sungmahi_dungeon/floor_1_1.txt", pMap->GetMapIndex(), pMap->GetMapSectree()->m_setting.iBaseX, pMap->GetMapSectree()->m_setting.iBaseY);
				return PASSES_PER_SEC(5);
			}
			else if (pEventInfo->bSubStep == 4)
			{
				pMap->SetPortal(pMap->Spawn((DWORD)PORTAL_VNUM, 442, 392, 0));
				return 0;
			}

			/* 
				Stage 1
			*/

			else if (pEventInfo->bSubStep == 5)
			{
				pMap->SetDungeonLimitTime(TIME_PER_FLOOR_1);

				int iSealSort = number(1,4);

				if (iSealSort == 1)
				{
					pMap->SetSeal1(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_1[0][0], SealCoordinates_1[0][1], 0));
					pMap->SetSeal2(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_1[1][0], SealCoordinates_1[1][1], 0));
					pMap->SetSeal3(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_1[2][0], SealCoordinates_1[2][1], 0));
					pMap->SetSeal4(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_1[3][0], SealCoordinates_1[3][1], 0));
					pMap->SetSeal5(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_1[4][0], SealCoordinates_1[4][1], 0));
					pMap->SetSeal6(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_1[5][0], SealCoordinates_1[5][1], 0));
					pMap->SetSeal7(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_1[6][0], SealCoordinates_1[6][1], 0));
					pMap->SetSeal8(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_1[7][0], SealCoordinates_1[7][1], 0));
				}
				else if (iSealSort == 2)
				{
					pMap->SetSeal1(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_2[0][0], SealCoordinates_2[0][1], 0));
					pMap->SetSeal2(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_2[1][0], SealCoordinates_2[1][1], 0));
					pMap->SetSeal3(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_2[2][0], SealCoordinates_2[2][1], 0));
					pMap->SetSeal4(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_2[3][0], SealCoordinates_2[3][1], 0));
					pMap->SetSeal5(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_2[4][0], SealCoordinates_2[4][1], 0));
					pMap->SetSeal6(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_2[5][0], SealCoordinates_2[5][1], 0));
					pMap->SetSeal7(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_2[6][0], SealCoordinates_2[6][1], 0));
					pMap->SetSeal8(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_2[7][0], SealCoordinates_2[7][1], 0));
				}
				else if (iSealSort == 3)
				{
					pMap->SetSeal1(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_3[0][0], SealCoordinates_3[0][1], 0));
					pMap->SetSeal2(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_3[1][0], SealCoordinates_3[1][1], 0));
					pMap->SetSeal3(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_3[2][0], SealCoordinates_3[2][1], 0));
					pMap->SetSeal4(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_3[3][0], SealCoordinates_3[3][1], 0));
					pMap->SetSeal5(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_3[4][0], SealCoordinates_3[4][1], 0));
					pMap->SetSeal6(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_3[5][0], SealCoordinates_3[5][1], 0));
					pMap->SetSeal7(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_3[6][0], SealCoordinates_3[6][1], 0));
					pMap->SetSeal8(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_3[7][0], SealCoordinates_3[7][1], 0));
				}
				else if (iSealSort == 4)
				{
					pMap->SetSeal1(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_4[0][0], SealCoordinates_4[0][1], 0));
					pMap->SetSeal2(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_4[1][0], SealCoordinates_4[1][1], 0));
					pMap->SetSeal3(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_4[2][0], SealCoordinates_4[2][1], 0));
					pMap->SetSeal4(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_4[3][0], SealCoordinates_4[3][1], 0));
					pMap->SetSeal5(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_4[4][0], SealCoordinates_4[4][1], 0));
					pMap->SetSeal6(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_4[5][0], SealCoordinates_4[5][1], 0));
					pMap->SetSeal7(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_4[6][0], SealCoordinates_4[6][1], 0));
					pMap->SetSeal8(pMap->Spawn(HELL_TOWER_SEAL, SealCoordinates_4[7][0], SealCoordinates_4[7][1], 0));
				}

				char szNotice[512];
				snprintf(szNotice, sizeof(szNotice), LC_TEXT("Find a Sealstone to break the seal!"));

				FChatMap f(szNotice, pMap->GetMapIndex());
				pMap->GetMapSectree()->for_each(f);

				pMap->SetDungeonSubStep(6);

				return 0;
			}
			else if (pEventInfo->bSubStep == 6)
			{
				struct FCountMonsters f;
				pMap->GetMapSectree()->for_each(f);

				if (f.m_cnt_Monsters < 1)
					regen_load_in_file("data/dungeon/sungmahi_dungeon/floor_1_2.txt", pMap->GetMapIndex(), pMap->GetMapSectree()->m_setting.iBaseX, pMap->GetMapSectree()->m_setting.iBaseY);

				return PASSES_PER_SEC(5);
			}
			else if (pEventInfo->bSubStep == 7)
			{
				pMap->SetPortal(pMap->Spawn((DWORD)PORTAL_VNUM, 448, 93, 0));
				return 0;
			}

			/* 
				Stage 2
			*/

			else if (pEventInfo->bSubStep == 8)
			{

				char szNotice[512];
				snprintf(szNotice, sizeof(szNotice), LC_TEXT("Kill all the monsters within the time limit!"));

				FChatMap f(szNotice, pMap->GetMapIndex());
				pMap->GetMapSectree()->for_each(f);

				pMap->SetDungeonLimitTime(8);
				struct FSendInformationBoardMap f2(pMap->GetMapIndex(), pMap);
				pMap->GetMapSectree()->for_each(f2);

				pMap->SetDungeonSubStep(9);

				return 0;				
			}
			else if (pEventInfo->bSubStep == 9)
			{
				struct FCountMonsters f;
				pMap->GetMapSectree()->for_each(f);

				if (f.m_cnt_Monsters < 1)
					regen_load_in_file("data/dungeon/sungmahi_dungeon/floor_1_3.txt", pMap->GetMapIndex(), pMap->GetMapSectree()->m_setting.iBaseX, pMap->GetMapSectree()->m_setting.iBaseY);
				return PASSES_PER_SEC(5);
			}
			else if (pEventInfo->bSubStep == 10)
			{
				struct FCountMonsters f;
				pMap->GetMapSectree()->for_each(f);

				pMap->Spawn((DWORD)BOSS1, 411, 424, 0);
				return 0;
			}
			else if (pEventInfo->bSubStep == 11)
			{
				pMap->SetPortal(pMap->Spawn((DWORD)PORTAL_VNUM, 441, 392, 0));
				return 0;
			}
			else if (pEventInfo->bSubStep == 12)
			{
				char szNotice[512];
				snprintf(szNotice, sizeof(szNotice), LC_TEXT("Climb the steps of Sung Mahi Tower and enter the next level."));
				SendNoticeMap(szNotice, pMap->GetMapIndex(), true);

				pMap->SetIsNextStep(true);
				pMap->SetPortal(pMap->Spawn((DWORD)PORTAL_VNUM, 342, 295, 0));
				return 0;
			}

		}

		/*
			Second Floor
		*/

		else if (pEventInfo->bStep == 2)
		{

			/* 
				Stage 0
			*/

			if (pEventInfo->bSubStep == 1)
			{
				char szNotice[512];
				snprintf(szNotice, sizeof(szNotice), LC_TEXT("The Battle will start in %d seconds."), pEventInfo->bCounter);

				pMap->SetIsNextStep(false);
				FChatMap f(szNotice, pMap->GetMapIndex());
				pMap->GetMapSectree()->for_each(f);

				pEventInfo->bCounter--;

				if (pEventInfo->bCounter == 0)
				{
					pMap->SetDungeonSubStep(2);
					return 0;
				}
				return PASSES_PER_SEC(1);
			}

			else if (pEventInfo->bSubStep == 2)
			{

				char szNotice[512];
				snprintf(szNotice, sizeof(szNotice), LC_TEXT("Kill all the monsters within the time limit!"));

				FChatMap f(szNotice, pMap->GetMapIndex());
				pMap->GetMapSectree()->for_each(f);

				pMap->SetDungeonLimitTime(TIME_PER_FLOOR_2);

				struct FSendInformationBoardMap f2(pMap->GetMapIndex(), pMap);
				pMap->GetMapSectree()->for_each(f2);

				pMap->SetDungeonSubStep(3);

				return 0;				
			}
			else if (pEventInfo->bSubStep == 3)
			{
				struct FCountMonsters f;
				pMap->GetMapSectree()->for_each(f);

				if (f.m_cnt_Monsters < 1)
					regen_load_in_file("data/dungeon/sungmahi_dungeon/floor_2_1.txt", pMap->GetMapIndex(), pMap->GetMapSectree()->m_setting.iBaseX, pMap->GetMapSectree()->m_setting.iBaseY);
				return PASSES_PER_SEC(5);
			}
			else if (pEventInfo->bSubStep == 4)
			{
				pMap->SetPortal(pMap->Spawn((DWORD)PORTAL_VNUM, 442, 392, 0));
				return 0;
			}

			/* 
				Stage 2
			*/

			else if (pEventInfo->bSubStep == 5)
			{
				pMap->SetDungeonLimitTime(5);

				char szNotice[512];
				snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] Destroy the Demon Stone!"));

				FChatMap f(szNotice, pMap->GetMapIndex());
				pMap->GetMapSectree()->for_each(f);

				regen_load_in_file("data/dungeon/sungmahi_dungeon/floor_2_2.txt", pMap->GetMapIndex(), pMap->GetMapSectree()->m_setting.iBaseX, pMap->GetMapSectree()->m_setting.iBaseY);

				return 0;
			}

		}

		return 0;
	}

	EVENTINFO(r_sungmahilimitspawn_info)
	{
		SmhTMap*	pMap;
	};
	
	EVENTFUNC(r_sungmahilimitspawn_event)
	{
		r_sungmahispawn_info* pEventInfo = dynamic_cast<r_sungmahispawn_info*>(event->info);

		if (!pEventInfo)
			return 0;
		
		SmhTMap* pMap = pEventInfo->pMap;
		if (!pMap)
			return 0;
		
		if (!pMap->GetMapSectree())
			return 0;
		
		pMap->EndDungeonWarp();

		return 0;
	}

	EVENTINFO(r_sungmahee_skill_info)
	{
		SmhTMap*	pMap;
	};
	
	EVENTFUNC(r_sungmahee_skill_event)
	{
		r_sungmahee_skill_info* pEventInfo = dynamic_cast<r_sungmahee_skill_info*>(event->info);
		if (pEventInfo)
		{
			SmhTMap* pMap = pEventInfo->pMap;
			if (pMap)
			{
				if (pMap->GetDungeonStep() == 1 && pMap->GetDungeonSubStep() == 9)
				{
					FSkillOneSungmaHeeTower f;
					pMap->GetMapSectree()->for_each(f);
				}
			}
		}
		
		return PASSES_PER_SEC(20);
	}


	/*
		Global Part
	*/

	int SealCoordinates_1[8][2] = {
		{448, 150}, {456, 149}, {466, 140}, {468, 120}, {454, 107}, {431, 113}, {425, 129}, {436, 147}
	};

	int SealCoordinates_2[8][2] = {
		{456, 149}, {466, 140}, {468, 120}, {454, 107}, {431, 113}, {425, 129}, {436, 147}, {448, 150}
	};
	
	int SealCoordinates_3[8][2] = {
		{436, 147}, {448, 150}, {456, 149}, {466, 140}, {431, 113}, {425, 129}, {468, 120}, {454, 107}
	};
	
	int SealCoordinates_4[8][2] = {
		{468, 120}, {466, 140}, {431, 113}, {456, 149}, {425, 129}, {454, 107}, {436, 147}, {448, 150}
	};

	/*
		Object Part
	*/

	SmhTMap::SmhTMap(long lMapIndex)
	{
		if (e_SpawnEvent != NULL)
			event_cancel(&e_SpawnEvent);
		e_SpawnEvent = NULL;

		if (e_LimitSpawnEvent != NULL)
			event_cancel(&e_LimitSpawnEvent);
		e_LimitSpawnEvent = NULL;

		if (e_SkillEvent != NULL)
			event_cancel(&e_SkillEvent);
		e_SkillEvent = NULL;

		SetMapIndex(lMapIndex);
		SetMapSectree(SECTREE_MANAGER::instance().GetMap(map_index));
		SetDungeonStep(1);
		SetDungeonLimitTime(0);
		SetPortal(NULL);
		SetDungeonStage();
	}

	SmhTMap::~SmhTMap()
	{
		if (e_SpawnEvent != NULL)
			event_cancel(&e_SpawnEvent);
		e_SpawnEvent = NULL;

		if (e_LimitSpawnEvent != NULL)
			event_cancel(&e_LimitSpawnEvent);
		e_LimitSpawnEvent = NULL;

		if (e_SkillEvent != NULL)
			event_cancel(&e_SkillEvent);
		e_SkillEvent = NULL;

	}

	void SmhTMap::Destroy()
	{
		if (e_SpawnEvent != NULL)
			event_cancel(&e_SpawnEvent);
		e_SpawnEvent = NULL;

		if (e_LimitSpawnEvent != NULL)
			event_cancel(&e_LimitSpawnEvent);
		e_LimitSpawnEvent = NULL;

		if (e_SkillEvent != NULL)
			event_cancel(&e_SkillEvent);
		e_SkillEvent = NULL;

		SetPortal(NULL);
		SetDungeonLimitTime(0);
	}

	void SmhTMap::SetDungeonStep(BYTE bStep)
	{
		bdungeon_Step = bStep;

		if (e_SpawnEvent != NULL)
			event_cancel(&e_SpawnEvent);
		e_SpawnEvent = NULL;

		if (e_LimitSpawnEvent != NULL)
			event_cancel(&e_LimitSpawnEvent);
		e_LimitSpawnEvent = NULL;

		r_sungmahispawn_info* pEventInfo = AllocEventInfo<r_sungmahispawn_info>();
		pEventInfo->pMap = this;
		pEventInfo->bStep = bStep;
		pEventInfo->bSubStep = 1;
		pEventInfo->bCounter = 20;
		e_SpawnEvent = event_create(r_sungmahispawn_event, pEventInfo, PASSES_PER_SEC(5));			
	}

	void SmhTMap::SetDungeonSubStep(BYTE bSubStep)
	{
		bdungeon_SubStep = bSubStep;

		if (e_SpawnEvent != NULL)
			event_cancel(&e_SpawnEvent);
		e_SpawnEvent = NULL;

		if (e_SkillEvent != NULL)
			event_cancel(&e_SkillEvent);
		e_SkillEvent = NULL;

		if (GetDungeonStep() == 1 && GetDungeonSubStep() == 9)
		{
			r_sungmahee_skill_info* pEventInfo_ = AllocEventInfo<r_sungmahee_skill_info>();
			pEventInfo_->pMap = this;
			e_SkillEvent = event_create(r_sungmahee_skill_event, pEventInfo_, PASSES_PER_SEC(10));
		}

		r_sungmahispawn_info* pEventInfo = AllocEventInfo<r_sungmahispawn_info>();
		pEventInfo->pMap = this;
		pEventInfo->bStep = GetDungeonStep();
		pEventInfo->bSubStep = bSubStep;
		pEventInfo->bCounter = 0;
		e_SpawnEvent = event_create(r_sungmahispawn_event, pEventInfo, PASSES_PER_SEC(1));

	}

	void SmhTMap::SetDungeonLimitTime(DWORD dwLimitTime)
	{
		dwStage_Limit_Time = (dwLimitTime * 60) + get_global_time();

		if (e_LimitSpawnEvent != NULL)
			event_cancel(&e_LimitSpawnEvent);
		e_LimitSpawnEvent = NULL;

		r_sungmahilimitspawn_info* pEventInfo = AllocEventInfo<r_sungmahilimitspawn_info>();
		pEventInfo->pMap = this;
		e_LimitSpawnEvent = event_create(r_sungmahilimitspawn_event, pEventInfo, PASSES_PER_SEC(dwLimitTime*60));

		if (!GetMapSectree())
			return;

		struct FSendInformationBoardMap f(GetMapIndex(), this);
		GetMapSectree()->for_each(f);

	}

	LPCHARACTER SmhTMap::Spawn(DWORD dwVnum, int iX, int iY, int iDir, bool bSpawnMotion)
	{
		if (dwVnum == 0)
			return NULL;

		if (!GetMapSectree())
			return NULL;
	
		LPCHARACTER pkMob = CHARACTER_MANAGER::instance().SpawnMob(dwVnum, GetMapIndex(), GetMapSectree()->m_setting.iBaseX + iX * 100, GetMapSectree()->m_setting.iBaseY + iY * 100, 0, bSpawnMotion, iDir == 0 ? -1 : (iDir - 1) * 45);
		if (pkMob)
			sys_log(0, "<SnakeLair> Spawn: %s (map index: %d). x: %d y: %d", pkMob->GetName(), GetMapIndex(), (GetMapSectree()->m_setting.iBaseX + iX * 100), (GetMapSectree()->m_setting.iBaseY + iY * 100));
		
		return pkMob;
	}

	void SmhTMap::EndDungeonWarp()
	{
		if (GetMapSectree())
		{
			FExitAndGoTo f;
			GetMapSectree()->for_each(f);
		}

		long lMapIndex = GetMapIndex();
		SECTREE_MANAGER::instance().DestroyPrivateMap(GetMapIndex());
		Destroy();
		SmhT::instance().Remove(lMapIndex);
		M2_DELETE(this);
	}

	void SmhTMap::OnSetItemMonster(LPCHARACTER pkChar, LPITEM pkItem, LPCHARACTER pkMonster)
	{
		if (!pkChar)
			return;

		if (!pkItem)
			return;

		if (!pkMonster)
			return;

		int itemVnum = pkItem->GetVnum();
		ITEM_MANAGER::instance().RemoveItem(pkItem);

		if (itemVnum == KEY_LEVEL1_SUNGMAHEE && GetDungeonStep() == 1)
		{	
			if (GetSeal1())
			{
				if (GetSeal1()->GetVID() == pkMonster->GetVID())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] Break the seal with Sung Mahi's Sealstone!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);

					GetSeal1()->Dead();
					SetSeal1(NULL);
				}
			}
			if (GetSeal2())
			{
				if (GetSeal1())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] You have to break another seal first!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);
					
					return;
				}

				if (GetSeal2()->GetVID() == pkMonster->GetVID())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] Break the seal with Sung Mahi's Sealstone!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);

					GetSeal2()->Dead();
					SetSeal2(NULL);
				}
			}
			if (GetSeal3())
			{
				if (GetSeal2())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] You have to break another seal first!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);
					
					return;
				}

				if (GetSeal3()->GetVID() == pkMonster->GetVID())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] Break the seal with Sung Mahi's Sealstone!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);

					GetSeal3()->Dead();
					SetSeal3(NULL);
				}
			}
			if (GetSeal4())
			{
				if (GetSeal3())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] You have to break another seal first!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);
					
					return;
				}

				if (GetSeal4()->GetVID() == pkMonster->GetVID())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] Break the seal with Sung Mahi's Sealstone!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);

					GetSeal4()->Dead();
					SetSeal4(NULL);
				}
			}
			if (GetSeal5())
			{
				if (GetSeal4())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] You have to break another seal first!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);
					
					return;
				}

				if (GetSeal5()->GetVID() == pkMonster->GetVID())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] Break the seal with Sung Mahi's Sealstone!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);

					GetSeal5()->Dead();
					SetSeal5(NULL);
				}
			}
			if (GetSeal6())
			{
				if (GetSeal5())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] You have to break another seal first!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);
					
					return;
				}

				if (GetSeal6()->GetVID() == pkMonster->GetVID())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] Break the seal with Sung Mahi's Sealstone!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);

					GetSeal6()->Dead();
					SetSeal6(NULL);
				}
			}
			if (GetSeal7())
			{
				if (GetSeal6())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] You have to break another seal first!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);
					
					return;
				}

				if (GetSeal7()->GetVID() == pkMonster->GetVID())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] Break the seal with Sung Mahi's Sealstone!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);

					GetSeal7()->Dead();
					SetSeal7(NULL);
				}
			}
			if (GetSeal8())
			{
				if (GetSeal7())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] You have to break another seal first!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);
					
					return;
				}

				if (GetSeal8()->GetVID() == pkMonster->GetVID())
				{
					char szNotice[512];
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("[Sung Mahi Tower] Break the seal with Sung Mahi's Sealstone!"));

					FChatMap f(szNotice, GetMapIndex());
					GetMapSectree()->for_each(f);

					GetSeal8()->Dead();
					SetSeal8(NULL);

					SetDungeonSubStep(7);

					FPurgeSectreeAll f2;
					GetMapSectree()->for_each(f2);

				}
			}
		}
	}

	void SmhTMap::SetDungeonStage()
	{
		bdungeon_stage += 1;
		struct FSendInformationBoardMap f2(GetMapIndex(), this);
		GetMapSectree()->for_each(f2);
	}

	void SmhTMap::OnKill(LPCHARACTER pkMonster, LPCHARACTER pKiller)
	{	
		BYTE bStep = GetDungeonStep();

		if (!bStep)
			return;
		
		if (!GetMapSectree())
			return;

		if (bStep == 1)
		{
			if (GetDungeonSubStep() == 6)
			{
				if (pkMonster->IsMonster())
				{
					if(!pKiller)
						return;

					int iChance = number(1, 100);
					if (iChance < DROP_PERCENT_ITEM_VNUMS)
					{
						LPITEM pkItem = ITEM_MANAGER::instance().CreateItem(KEY_LEVEL1_SUNGMAHEE);
						if (!pkItem)
							return;
			
						PIXEL_POSITION mPos;
						mPos.x = pkMonster->GetX();
						mPos.y = pkMonster->GetY();
			
						pkItem->AddToGround(GetMapIndex(), mPos);
						pkItem->StartDestroyEvent();
						pkItem->SetOwnership(pKiller, 60);
					}
				}

				return;
			}

			if (pkMonster->GetMobTable().dwVnum == BOSS1)
			{
				char szNotice[512];
				snprintf(szNotice, sizeof(szNotice), LC_TEXT("The portal to the next room is open."));
				SendNoticeMap(szNotice, GetMapIndex(), true);

				SetDungeonSubStep(11);

				return;
			}

			SetCountKillMonsters(GetCountKillMonsters()+1);
			if (GetCountKillMonsters() == MONSTER_KILL_FLOOR_1)
			{
				if (e_SpawnEvent != NULL)
					event_cancel(&e_SpawnEvent);
				e_SpawnEvent = NULL;

				if (e_LimitSpawnEvent != NULL)
					event_cancel(&e_LimitSpawnEvent);
				e_LimitSpawnEvent = NULL;
	
				SetCountKillMonsters(0);

				char szNotice[512];
				if (GetDungeonSubStep() == 9)
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("The portal to the next room is open."));
				else
					snprintf(szNotice, sizeof(szNotice), LC_TEXT("Destroy the enemy of this room."));
				SendNoticeMap(szNotice, GetMapIndex(), true);

				SetDungeonStage();

				if (GetDungeonSubStep() == 3)
					SetDungeonSubStep(4);
				else if (GetDungeonSubStep() == 9)
					SetDungeonSubStep(10);
			}
		}

		else if (bStep == 2)
		{
			SetCountKillMonsters(GetCountKillMonsters()+1);
			if (GetCountKillMonsters() == MONSTER_KILL_FLOOR_2)
			{
				if (e_SpawnEvent != NULL)
					event_cancel(&e_SpawnEvent);
				e_SpawnEvent = NULL;

				if (e_LimitSpawnEvent != NULL)
					event_cancel(&e_LimitSpawnEvent);
				e_LimitSpawnEvent = NULL;
	
				SetCountKillMonsters(0);

				char szNotice[512];
				snprintf(szNotice, sizeof(szNotice), LC_TEXT("The portal to the next room is open."));
				SendNoticeMap(szNotice, GetMapIndex(), true);

				if (GetDungeonSubStep() == 3)
					SetDungeonSubStep(4);
				else if (GetDungeonSubStep() == 9)
					SetDungeonSubStep(10);
			}
		}
	}

	void SmhTMap::GoToNextRoom(LPCHARACTER pkChar)
	{
		if (!pkChar)
			return;

		if (!GetMapSectree())
			return;

		if (!GetPortal())
			return;

		FPurgeSectreeAll f;
		GetMapSectree()->for_each(f);

		SetPortal(NULL);

		if (GetIsNextStep())
			SetDungeonStep(GetDungeonStep()+1);
		else
			SetDungeonSubStep(GetDungeonSubStep()+1);

		PIXEL_POSITION pos = SmhT::instance().GetXYZ();
		if (!pos.x)
			return;
		else
		{
			if (GetDungeonStep() == 1)
			{
				if (GetDungeonSubStep() == 5)
					pkChar->WarpSet(pos.x+444*100, pos.y+147*100, GetMapIndex());
				else if (GetDungeonSubStep() == 8)
					pkChar->WarpSet(pos.x+403*100, pos.y+458*100, GetMapIndex());
				else if (GetDungeonSubStep() == 12)
					pkChar->WarpSet(pos.x+321*100, pos.y+342*100, GetMapIndex());
			}

		}
	}

	void SmhTMap::SendClientPacketInformationBoard(LPCHARACTER pkChar, BYTE bSubHeader)
	{
		TPacketGCSungmaHeeTowerEnter packet;
		packet.bHeader = HEADER_GC_SUNGMAHEE_TOWER;
		packet.bSubHeader = bSubHeader;

		if (bSubHeader == SUB_HEADER_INFORMATION_SEND_BOARD)
		{
			packet.bFloor = GetDungeonStep();
			packet.bStage = GetDungeonStage();
			packet.dwStage_Limit_Time = GetDungeonLimitTime();
		}
		else if (bSubHeader == SUB_HEADER_INFORMATION_OPEN_BOARD)
		{
			packet.bFloor = GetDungeonStep();
			packet.bStage = GetDungeonStage();
			packet.dwStage_Limit_Time = GetDungeonLimitTime();
		}
		else
		{
			packet.bFloor = 1;
			packet.bStage = 0;
			packet.dwStage_Limit_Time = 0;
		}

		pkChar->GetDesc()->Packet(&packet, sizeof(packet));
	}

	/*
		Global Part
	*/

	bool SmhT::Access(LPCHARACTER pChar)
	{
		if (!pChar)
			return false;

		long lNormalMapIndex = (long)MAP_INDEX;
		PIXEL_POSITION pos = GetXYZ(), posSub = GetSubXYZ();

		if (!pos.x)
		{
			LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap((long)(SungmaHeeTower::MAP_INDEX));
			if (pkSectreeMap)
				SetXYZ(pkSectreeMap->m_setting.iBaseX, pkSectreeMap->m_setting.iBaseY, 0);
		}
			
		if (!posSub.x)
		{
			LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap((long)(SungmaHeeTower::SUBMAP_INDEX));
			if (pkSectreeMap)
				SetSubXYZ(pkSectreeMap->m_setting.iBaseX, pkSectreeMap->m_setting.iBaseY, 0);
		}

		long lMapIndex = SECTREE_MANAGER::instance().CreatePrivateMap(lNormalMapIndex);

		if (!lMapIndex)
		{
			pChar->ChatPacket(CHAT_TYPE_INFO, "An error ocurred during map creation.");
			return false;
		}

		SmhTMap *pMap;

		pMap = M2_NEW SmhTMap(lMapIndex);

		if (pMap)
		{
			m_dwRegGroups.insert(std::make_pair(lMapIndex, pMap));

			PIXEL_POSITION mPos;
			if (!SECTREE_MANAGER::instance().GetRecallPositionByEmpire((int)(MAP_INDEX), 0, mPos))
			{
				pChar->ChatPacket(CHAT_TYPE_INFO, "Sectree Error get recall position.");
				return true;
			}

			if(pChar)
			{
				pChar->SaveExitLocation();
				pChar->WarpSet(mPos.x+403*100, mPos.y+458*100, pMap->GetMapIndex());
			}

			return true;
		}
	}

	bool SmhT::ReadSungmaRewards()
	{
		DWORD dwVnum1 = 0, dwVnum2 = 0, dwVnum3 = 0, dwVnum4 = 0, dwVnum5 = 0;
		BYTE bCount1 = 0, bCount2 = 0, bCount3 = 0, bCount4 = 0, bCount5 = 0, bFloor = 0, bElement = 0;

		TRewardTower tRewardTower;

		char szQuery[1024];
		snprintf(szQuery, sizeof(szQuery), "SELECT id, element, vnum1, count1, vnum2, count2, vnum3, count3, vnum4, count4, vnum5, count5 FROM sungmahee_tower");
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));

		if (pMsg->Get()->uiNumRows == 0)
			return false;
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
		{
			str_to_number(bFloor, row[0]); 
			str_to_number(bElement, row[1]);
			str_to_number(dwVnum1, row[2]);
			str_to_number(bCount1, row[3]);
			str_to_number(dwVnum2, row[4]);
			str_to_number(bCount2, row[5]);
			str_to_number(dwVnum3, row[6]);
			str_to_number(bCount3, row[7]);
			str_to_number(dwVnum4, row[8]);
			str_to_number(bCount4, row[9]);
			str_to_number(dwVnum5, row[10]);
			str_to_number(bCount5, row[11]);

			tRewardTower.bFloor = bFloor;
			tRewardTower.bElement = bElement;

			tRewardTower.Item[0].dwVnum = dwVnum1;
			tRewardTower.Item[0].bCount = bCount1;
			tRewardTower.Item[1].dwVnum = dwVnum2;
			tRewardTower.Item[1].bCount = bCount2;
			tRewardTower.Item[2].dwVnum = dwVnum3;
			tRewardTower.Item[2].bCount = bCount3;
			tRewardTower.Item[3].dwVnum = dwVnum4;
			tRewardTower.Item[3].bCount = bCount4;
			tRewardTower.Item[4].dwVnum = dwVnum5;
			tRewardTower.Item[4].bCount = bCount5;

			m_vec_mapRewardTower.push_back(tRewardTower);

		}					
		return true;
	}

	void SmhT::Open(LPCHARACTER pkChar)
	{
		if (!pkChar)
			return;

		TPacketGCSungmaHeeTowerEnter packet;

		packet.bHeader	= HEADER_GC_SUNGMAHEE_TOWER;
		packet.bSubHeader	= SUB_HEADER_TOWER_OPEN_PANEL;

		packet.bFloor = 0;
		packet.bStage = 0;
		packet.dwStage_Limit_Time = 0;

		pkChar->GetDesc()->Packet(&packet, sizeof(packet));
	
	}


	void SmhT::OpenInformationBoard(LPCHARACTER pkChar)
	{
		if (!pkChar)
			return;

		long lMapIndex = pkChar->GetMapIndex();
		
		if (lMapIndex < 1000)
			return;
	
		SmhTMap* pMap = m_dwRegGroups.find(lMapIndex)->second;
		if (!pMap)
			return;

		pMap->SendClientPacketInformationBoard(pkChar, SUB_HEADER_INFORMATION_OPEN_BOARD);
	}

	bool SmhT::IsSungmaTowerMap(long lMapIndex)
	{
		long lMinIndex = (long)(MAP_INDEX) * 10000, lMaxIndex = (long)(MAP_INDEX) * 10000 + 10000;
		if (((lMapIndex >= lMinIndex) && (lMapIndex <= lMaxIndex)) || (lMapIndex == (long)(MAP_INDEX)))
			return true;
		
		return false;
	}


	void SmhT::OnKill(LPCHARACTER pkMonster, LPCHARACTER pKiller)
	{
		if ((!pkMonster) || (!pKiller))
			return;

		long lMapIndex = pKiller->GetMapIndex();
		
		if (lMapIndex < 1000)
			return;
	
		SmhTMap* pMap = m_dwRegGroups.find(lMapIndex)->second;
		if (!pMap)
			return;
		
		pMap->OnKill(pkMonster, pKiller);

		return;
	}

	void SmhT::GoToNextRoom(LPCHARACTER pkChar)
	{
		if (!pkChar)
			return;

		long lMapIndex = pkChar->GetMapIndex();
		
		if (lMapIndex < 1000)
			return;
	
		SmhTMap* pMap = m_dwRegGroups.find(lMapIndex)->second;

		if (!pMap)
			return;
		
		pMap->GoToNextRoom(pkChar);

		return;
	}

	void SmhT::OnSetItemMonster(LPCHARACTER pkChar, LPITEM pkItem, LPCHARACTER pkMonster)
	{
		if (!pkChar)
			return;

		if (!pkItem)
			return;

		if (!pkMonster)
			return;

		long lMapIndex = pkChar->GetMapIndex();
		
		if (lMapIndex < 1000)
			return;
	
		itertype(m_dwRegGroups) iter = m_dwRegGroups.find(lMapIndex), iterEnd = m_dwRegGroups.end();
		if (iter == iterEnd)
			return;
		
		SmhTMap* pMap = m_dwRegGroups.find(lMapIndex)->second;
		if (pMap)
		{
			pMap->OnSetItemMonster(pkChar, pkItem, pkMonster);
		}
	}	

	void SmhT::Remove(long lMapIndex)
	{
		itertype(m_dwRegGroups) iter = m_dwRegGroups.find(lMapIndex);

		if (iter != m_dwRegGroups.end())
		{
			m_dwRegGroups.erase(iter);
		}

		return;
	}

	void SmhT::Initialize()
	{
		m_vec_mapRewardTower.clear();
		ReadSungmaRewards();
	}

	void SmhT::Destroy()
	{
		m_vec_mapRewardTower.clear();
	}

}