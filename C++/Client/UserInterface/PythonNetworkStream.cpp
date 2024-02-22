//Search

			Set(HEADER_GC_CHANGE_NAME, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCChangeName), STATIC_SIZE_PACKET));

//Add after

#ifdef ENABLE_SUNGMAHEE_TOWER
			Set(HEADER_GC_SUNGMAHEE_TOWER, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSungmaHeeTowerEnter), STATIC_SIZE_PACKET));
#endif