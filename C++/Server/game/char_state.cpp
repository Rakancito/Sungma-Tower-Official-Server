//Add

#ifdef ENABLE_SUNGMAHEE_TOWER
#include "SungmaHeeTower.h"
#endif

//Search

void CHARACTER::__StateIdle_Monster()
{

	if (IsStun())
		return;

	if (!CanMove())
		return;

	[...]

	if (!victim || victim->IsDead())
	{
		SetVictim(NULL);
		victim = NULL;
		m_dwStateDuration = PASSES_PER_SEC(1);
	}

//Add after

#ifdef ENABLE_SUNGMAHEE_TOWER
	if (GetMobTable().dwVnum == SungmaHeeTower::SMH_INF_STONE_1)
	{

		int iPercent = 0; // @fixme136
		if (GetMaxHP() >= 0)
			iPercent = (GetHP() * 100) / GetMaxHP();

		if (iPercent <= 100 && GetMaxSP() != 1)
		{
			SetMaxSP(1);

			CHARACTER_MANAGER::instance().SelectStone(this);
			for (BYTE bIndex = 0; bIndex < 20; bIndex++)
			{
				CHARACTER_MANAGER::instance().SpawnGroup(SungmaHeeTower::SMH_INF_ARCHER_1, GetMapIndex(), GetX() - 3000, GetY() - 3000, GetX() + 3000, GetY() + 3000);
				CHARACTER_MANAGER::instance().SpawnGroup(SungmaHeeTower::SMH_INF_ARCHER_1, GetMapIndex(), GetX() - 4000, GetY() - 4000, GetX() + 4000, GetY() + 4000);
				CHARACTER_MANAGER::instance().SpawnGroup(SungmaHeeTower::SMH_INF_ARCHER_1, GetMapIndex(), GetX() - 5500, GetY() - 5500, GetX() + 5500, GetY() + 5500);
			}
		CHARACTER_MANAGER::instance().SelectStone(NULL);
		UpdatePacket();
		}

        	SetVictim(NULL);
        	victim = NULL;
        	m_dwStateDuration = PASSES_PER_SEC(1);

	}
#endif

//Search

		if (DISTANCE_APPROX(GetX() - pkChrProtege->GetX(), GetY() - pkChrProtege->GetY()) > 1000)
		{
			if (Follow(pkChrProtege, number(150, 400)))
			{
				[...]
			}
		}

//Add before

#ifdef ENABLE_SUNGMAHEE_TOWER
		if (GetMobTable().dwVnum == SungmaHeeTower::SMH_INF_ARCHER_1)
		{
			if (DISTANCE_APPROX(GetX() - pkChrProtege->GetX(), GetY() - pkChrProtege->GetY()) > 5500)
			{
				if (Follow(pkChrProtege, number(150, 400)))
					return;
			}
		}
		else
#endif


