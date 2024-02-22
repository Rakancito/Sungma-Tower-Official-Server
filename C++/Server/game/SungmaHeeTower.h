#include "../../common/service.h"


#include "../../common/length.h"
#include "../../common/item_length.h"
#include "../../common/tables.h"
#include "char_manager.h"
#include "sectree_manager.h"
#include "group_text_parse_tree.h"

namespace SungmaHeeTower
{
	enum eSungmaHeeTowerConfig
	{
		MAP_INDEX = 143,
		SUBMAP_INDEX = 142,
		TIME_LIMIT_DUNGEON = 3600,
		MAX_REWARD = 5,

		RANGE_MIN_VNUM_MONSTERS	= 7550,
		RANGE_MAX_VNUM_MONSTERS = 7620,


		TIME_PER_FLOOR_1 = 4,
		TIME_PER_FLOOR_2 = 4,


		MONSTER_KILL_FLOOR_1 = 20,
		MONSTER_KILL_FLOOR_2 = 50,

		PORTAL_VNUM = 4020,
		HELL_TOWER_SEAL = 4021,

		DROP_PERCENT_ITEM_VNUMS = 50,

		KEY_LEVEL1_SUNGMAHEE = 70404,

		SMH_INF_STONE_1 = 7594,
		SMH_INF_ARCHER_1 = 7595,

		BOSS1 = 7609,
	};

	enum eElementTower
	{
		FIRE,
		ICE,
		ELEC,
		WIND,
		EARTH,
		DARK,
		ALL
	};

	typedef struct SRewardTowerItem
	{
		DWORD dwVnum;
		BYTE bCount;
	} TRewardTowerItem;

	typedef struct SRewardTower
	{
		BYTE bFloor;
		BYTE bElement;
		TRewardTowerItem Item[MAX_REWARD];
	} TRewardTower;

	extern int SealCoordinates_1[8][2];
	extern int SealCoordinates_2[8][2];
	extern int SealCoordinates_3[8][2];
	extern int SealCoordinates_4[8][2];

	class SmhTMap
	{
		public:
			SmhTMap(long lMapIndex);
			~SmhTMap();

			void Destroy();

			void	SetDungeonStep(BYTE bStep);
			BYTE	GetDungeonStep()		{return bdungeon_Step;};

			void	SetDungeonSubStep(BYTE bSubStep);
			BYTE	GetDungeonSubStep()		{return bdungeon_SubStep;};

			void	SetIsNextStep(bool bIsNext)	{bIsNextFloor = bIsNext;};
			bool	GetIsNextStep()			{return bIsNextFloor;};

			void 	SetMapSectree(LPSECTREE_MAP pkSectree) {pkSectreeMap = pkSectree;};
			LPSECTREE_MAP & GetMapSectree()		{return pkSectreeMap;};

			void 	SetPortal(LPCHARACTER pPortal) {pkPortal = pPortal;};
			LPCHARACTER GetPortal()			{return pkPortal;};

			void	SetMapIndex(long lMapIndex)	{map_index=lMapIndex;};
			long	GetMapIndex()			{return map_index;};

			void	SetCountKillMonsters(int iCountMonst)	{iCountMonsters = iCountMonst;};
			int	GetCountKillMonsters()		{return iCountMonsters;};

			void	SetDungeonStage();
			BYTE	GetDungeonStage()		{return bdungeon_stage;};

			void	SetDungeonLimitTime(DWORD dwLimitTime);
			DWORD	GetDungeonLimitTime()		{return dwStage_Limit_Time;};

			void	SetSeal1(LPCHARACTER pkSeal)	{pkSeal1 = pkSeal;};
			LPCHARACTER GetSeal1()			{return pkSeal1;};

			void	SetSeal2(LPCHARACTER pkSeal)	{pkSeal2 = pkSeal;};
			LPCHARACTER GetSeal2()			{return pkSeal2;};

			void	SetSeal3(LPCHARACTER pkSeal)	{pkSeal3 = pkSeal;};
			LPCHARACTER GetSeal3()			{return pkSeal3;};

			void	SetSeal4(LPCHARACTER pkSeal)	{pkSeal4 = pkSeal;};
			LPCHARACTER GetSeal4()			{return pkSeal4;};

			void	SetSeal5(LPCHARACTER pkSeal)	{pkSeal5 = pkSeal;};
			LPCHARACTER GetSeal5()			{return pkSeal5;};

			void	SetSeal6(LPCHARACTER pkSeal)	{pkSeal6 = pkSeal;};
			LPCHARACTER GetSeal6()			{return pkSeal6;};

			void	SetSeal7(LPCHARACTER pkSeal)	{pkSeal7 = pkSeal;};
			LPCHARACTER GetSeal7()			{return pkSeal7;};

			void	SetSeal8(LPCHARACTER pkSeal)	{pkSeal8 = pkSeal;};
			LPCHARACTER GetSeal8()			{return pkSeal8;};

			void	EndDungeonWarp();
			LPCHARACTER Spawn(DWORD dwVnum, int iX, int iY, int iDir, bool bSpawnMotion = false);

			void	OnSetItemMonster(LPCHARACTER pkChar, LPITEM pkItem, LPCHARACTER pkMonster);
			void	OnKill(LPCHARACTER pkMonster, LPCHARACTER pKiller);
			void	GoToNextRoom(LPCHARACTER pkChar);

			void	SendClientPacketInformationBoard(LPCHARACTER pkChar, BYTE BSubHeader);

		protected:
			long map_index;
			LPSECTREE_MAP pkSectreeMap;

			LPCHARACTER pkPortal, pkSeal1, pkSeal2, pkSeal3, pkSeal4, pkSeal5, pkSeal6, pkSeal7, pkSeal8;

			LPEVENT	e_SpawnEvent, e_LimitSpawnEvent, e_SkillEvent;

			BYTE bdungeon_Step, bdungeon_SubStep, bdungeon_stage;

			DWORD	dwStage_Limit_Time;
			int	iCountMonsters;

			bool bIsNextFloor;

	};

	class SmhT : public singleton<SmhT>
	{
		public:
			bool Access(LPCHARACTER pChar);

			bool ReadSungmaRewards();
			void Open(LPCHARACTER pkChar);
			void OpenInformationBoard(LPCHARACTER pkChar);

			bool IsSungmaTowerMap(long lMapIndex);
			void OnKill(LPCHARACTER pkMonster, LPCHARACTER pKiller);

			void GoToNextRoom(LPCHARACTER pkChar);

			void OnSetItemMonster(LPCHARACTER pkChar, LPITEM pkItem, LPCHARACTER pkMonster);

			//Set
			void	SetSubXYZ(long lX, long lY, long lZ)	{lSubMapPos.x = lX, lSubMapPos.y = lY, lSubMapPos.z = lZ;}
			void	SetXYZ(long lX, long lY, long lZ)	{lMapCenterPos.x = lX, lMapCenterPos.y = lY, lMapCenterPos.z = lZ;}
			
			//Get
			const PIXEL_POSITION &	GetSubXYZ() const	{return lSubMapPos;}
			const PIXEL_POSITION &	GetXYZ() const	{return lMapCenterPos;}

			void Remove(long lMapIndex);

			void Initialize();
			void Destroy();

		private:
			std::vector<TRewardTower>		m_vec_mapRewardTower;
			std::map<long, SmhTMap*> m_dwRegGroups;
			PIXEL_POSITION lMapCenterPos, lSubMapPos;
	};
};