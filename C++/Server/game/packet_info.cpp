//Search

	Set(HEADER_CG_PASSPOD_ANSWER, sizeof(TPacketCGPasspod), "PasspodAnswer", true);

//Add after

#ifdef ENABLE_SUNGMAHEE_TOWER
	Set(HEADER_CG_SUNGMAHEE_TOWER, sizeof(TPacketCGSungmaHeeTowerEnter), "SungmaHeeTower");
#endif

