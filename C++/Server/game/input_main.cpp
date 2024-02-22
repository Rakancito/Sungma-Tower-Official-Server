//Search

int CInputMain::Analyze(LPDESC d, BYTE bHeader, const char * c_pData)
{

//Add before


#ifdef ENABLE_SUNGMAHEE_TOWER
void CInputMain::RecvSungmaHeeTower(LPCHARACTER ch, const char* data)
{

	struct SPacketCGSungmaHeeTowerEnter * pinfo = (struct SPacketCGSungmaHeeTowerEnter *) data;

	switch (pinfo->bSubHeader)
	{
		case SUB_HEADER_TOWER_ENTER:
			ch->SungmaHeeTowerEnter();
			break;
	}
}

#endif

//Search

		case HEADER_CG_REFINE:
			Refine(ch, c_pData);
			break;

//Add before

#ifdef ENABLE_SUNGMAHEE_TOWER
		case HEADER_CG_SUNGMAHEE_TOWER:
			RecvSungmaHeeTower(ch, c_pData);
			break;
#endif