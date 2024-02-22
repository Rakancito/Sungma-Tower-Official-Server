//Search

	HEADER_CG_CLIENT_VERSION2					= 0xf1,

//Add before

#ifdef ENABLE_SUNGMAHEE_TOWER
	HEADER_CG_SUNGMAHEE_TOWER = 96, //Remember, replace this number if you are using.
#endif

//Search

	HEADER_GC_KEY_AGREEMENT_COMPLETED			= 0xfa,

//Add before

#ifdef ENABLE_SUNGMAHEE_TOWER
	HEADER_GC_SUNGMAHEE_TOWER = 141, //Remember, replace this number if you are using.
#endif

//Add before #pragma pack()

#ifdef ENABLE_SUNGMAHEE_TOWER

enum EPacketSungmaHeeTower
{
	SUB_HEADER_TOWER_OPEN_PANEL,
	SUB_HEADER_TOWER_ENTER,
	SUB_HEADER_INFORMATION_OPEN_BOARD,
	SUB_HEADER_INFORMATION_SEND_BOARD,
};

typedef struct SPacketGCSungmaHeeTowerEnter
{
	BYTE	bHeader;
	BYTE	bSubHeader;
	BYTE	bFloor;
	BYTE	bStage;
	DWORD	dwStage_Limit_Time;
} TPacketGCSungmaHeeTowerEnter;

typedef struct SPacketCGSungmaHeeTowerEnter
{
	BYTE	bHeader;
	BYTE	bSubHeader;
} TPacketCGSungmaHeeTowerEnter;

#endif
