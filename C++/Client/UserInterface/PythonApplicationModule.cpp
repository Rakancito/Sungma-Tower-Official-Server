//Add before the last }


#ifdef ENABLE_SUNGMAHEE_TOWER
	PyModule_AddIntConstant(poModule, "ENABLE_SUNGMAHEE_TOWER", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_SUNGMAHEE_TOWER", 0);
#endif

