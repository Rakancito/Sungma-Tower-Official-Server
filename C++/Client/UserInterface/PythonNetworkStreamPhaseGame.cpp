//Search

			case HEADER_GC_SPECIFIC_EFFECT:
				ret = RecvSpecificEffect();
				break;

//Add after

#ifdef ENABLE_SUNGMAHEE_TOWER
		case HEADER_GC_SUNGMAHEE_TOWER:
			ret = RecvSungmaHeeTowerEnter();
			break;
#endif

//Add in this file

#ifdef ENABLE_SUNGMAHEE_TOWER
bool CPythonNetworkStream::RecvSungmaHeeTowerEnter()
{
	TPacketGCSungmaHeeTowerEnter kSungmaHeeTowerEnter;

	if (!Recv(sizeof(kSungmaHeeTowerEnter), &kSungmaHeeTowerEnter))
		return false;

	switch (kSungmaHeeTowerEnter.bSubHeader)
	{
		case SUB_HEADER_TOWER_OPEN_PANEL:
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OpenSungmaHeeTower", Py_BuildValue("()"));
		}
		break;
		case SUB_HEADER_INFORMATION_OPEN_BOARD:
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SungmaHeeTowerSendInformationBoard", Py_BuildValue("i(iii)", SUB_HEADER_INFORMATION_OPEN_BOARD,  kSungmaHeeTowerEnter.bFloor, kSungmaHeeTowerEnter.bStage, kSungmaHeeTowerEnter.dwStage_Limit_Time));
		}
		break;
		case SUB_HEADER_INFORMATION_SEND_BOARD:
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SungmaHeeTowerSendInformationBoard", Py_BuildValue("i(iii)", SUB_HEADER_INFORMATION_SEND_BOARD, kSungmaHeeTowerEnter.bFloor, kSungmaHeeTowerEnter.bStage, kSungmaHeeTowerEnter.dwStage_Limit_Time));
		}
		break;

	}

	return true;
}

bool CPythonNetworkStream::SendSungmaHeeTowerAction(BYTE bSubHeader)
{
	TPacketCGSungmaHeeTowerEnter packet;
	packet.bHeader = HEADER_CG_SUNGMAHEE_TOWER;
	packet.bSubHeader = bSubHeader;

	if (!Send(sizeof(TPacketCGSungmaHeeTowerEnter), &packet))
	{
		Tracef("SendSungmaHeeTowerEnter Send Packet Error\n");
		return false;
	}

	return true;
}
#endif

