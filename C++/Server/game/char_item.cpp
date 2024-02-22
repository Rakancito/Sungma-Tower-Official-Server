//Add

#ifdef ENABLE_SUNGMAHEE_TOWER
#include "SungmaHeeTower.h"
#endif

//Search

bool CHARACTER::CanReceiveItem(LPCHARACTER from, LPITEM item) const
{
	if (IsPC())
		return false;

	// TOO_LONG_DISTANCE_EXCHANGE_BUG_FIX
	if (DISTANCE_APPROX(GetX() - from->GetX(), GetY() - from->GetY()) > 2000)
		return false;
	// END_OF_TOO_LONG_DISTANCE_EXCHANGE_BUG_FIX

	switch (GetRaceNum())
	{

//Add after


#ifdef ENABLE_SUNGMAHEE_TOWER
		case SungmaHeeTower::HELL_TOWER_SEAL:
			if (SungmaHeeTower::SmhT::instance().IsSungmaTowerMap(GetMapIndex()))
			{
				if (item->GetVnum() == SungmaHeeTower::KEY_LEVEL1_SUNGMAHEE)
					return true;
				else
					return false;
			}
#endif


//Search

void CHARACTER::ReceiveItem(LPCHARACTER from, LPITEM item)
{
	if (IsPC())
		return;

	switch (GetRaceNum())
	{
		case fishing::CAMPFIRE_MOB:
			if (item->GetType() == ITEM_FISH && (item->GetSubType() == FISH_ALIVE || item->GetSubType() == FISH_DEAD))
				fishing::Grill(from, item);
			else
			{
				// TAKE_ITEM_BUG_FIX
				from->SetQuestNPCID(GetVID());
				// END_OF_TAKE_ITEM_BUG_FIX
				quest::CQuestManager::instance().TakeItem(from->GetPlayerID(), GetRaceNum(), item);
			}
			break;

//Add after

#ifdef ENABLE_SUNGMAHEE_TOWER
		case SungmaHeeTower::HELL_TOWER_SEAL:
			{
				if (from->IsPC())
				{
					if (SungmaHeeTower::SmhT::instance().IsSungmaTowerMap(from->GetMapIndex()))
						SungmaHeeTower::SmhT::instance().OnSetItemMonster(from, item, this);
				}
			}
			break;
#endif