//Add

#ifdef ENABLE_SUNGMAHEE_TOWER
PyObject* playerSendSungmaHeeTowerEnter(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bSubHeader;
	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();

	if (!PyTuple_GetByte(poArgs, 0, &bSubHeader))
		return Py_BuildException();

	switch (bSubHeader)
	{
		case SUB_HEADER_TOWER_ENTER:
			rns.SendSungmaHeeTowerAction(bSubHeader);
			break;
	}

	return Py_BuildNone();
}
#endif

//Search

{ "GetLevel",								playerGetLevel,								METH_VARARGS },

//Add after


#ifdef ENABLE_SUNGMAHEE_TOWER
		{ "SendSungmaHeeTowerEnter",				playerSendSungmaHeeTowerEnter,				METH_VARARGS },
#endif


//Search the last } and aff before

#ifdef ENABLE_SUNGMAHEE_TOWER
	PyModule_AddIntConstant(poModule, "SUB_HEADER_INFORMATION_OPEN_BOARD", SUB_HEADER_INFORMATION_OPEN_BOARD);
	PyModule_AddIntConstant(poModule, "SUB_HEADER_INFORMATION_SEND_BOARD", SUB_HEADER_INFORMATION_SEND_BOARD);
#endif