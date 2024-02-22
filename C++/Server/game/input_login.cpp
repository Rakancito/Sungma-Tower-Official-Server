//Add

#ifdef ENABLE_SUNGMAHEE_TOWER
#include "SungmaHeeTower.h"
#endif

//Search

}

void CInputLogin::Empire(LPDESC d, const char * c_pData)

//Add before and under the function void CInputLogin::Entergame(LPDESC d, const char * data)


#ifdef ENABLE_SUNGMAHEE_TOWER
	if (SungmaHeeTower::SmhT::instance().IsSungmaTowerMap(ch->GetMapIndex()) && ch->IsPC())
	{
		SungmaHeeTower::SmhT::instance().OpenInformationBoard(ch);
	}
#endif