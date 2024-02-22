//Add

#ifdef ENABLE_SUNGMAHEE_TOWER
#include "SungmaHeeTower.h"
#endif

//Search

	DSManager dsManager;

//Add after


#ifdef ENABLE_SUNGMAHEE_TOWER
	SungmaHeeTower::SmhT SungmaHeeTower_manager;
#endif

//Search

		//xtrap
		if(bXTrapEnabled)
		{
			if (!XTManager.LoadXTrapModule())
			{
				CleanUpForEarlyExit();
				return 0;
			}
#if defined (__FreeBSD__) && defined(__FILEMONITOR__)
			//PFN_FileChangeListener pNotifyFunc = boost::bind( &CXTrapManager::NotifyMapFileChanged, CXTrapManager::instance(), _1 );
			PFN_FileChangeListener pNotifyFunc = &(CXTrapManager::NotifyMapFileChanged);

			const std::string strMap1Name = "map1.CS3";
			const std::string strMap2Name = "map2.CS3";

			FileMonitorFreeBSD::Instance().AddWatch( strMap1Name, pNotifyFunc );
			FileMonitorFreeBSD::Instance().AddWatch( strMap2Name, pNotifyFunc );
#endif
		}


//Add after

#ifdef ENABLE_SUNGMAHEE_TOWER
		SungmaHeeTower_manager.Initialize();
#endif

//Search

		if (isHackShieldEnable)
		{
			sys_log(0, "<shutdown> Releasing HackShield manager...");
			HSManager.Release();
		}

//Add after

#ifdef ENABLE_SUNGMAHEE_TOWER
		sys_log(0, "<shutdown> Destroying SungmaHeeTower_manager.");
		SungmaHeeTower_manager.Destroy();
#endif



