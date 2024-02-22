//Search

		bool SendExchangeExitPacket();

//Add after

#ifdef ENABLE_SUNGMAHEE_TOWER
		bool RecvSungmaHeeTowerEnter();
#endif

//Search

		void __BettingGuildWar_SetBettingMoney(UINT uBettingMoney);

//Add after

#ifdef ENABLE_SUNGMAHEE_TOWER
	public:
		bool	SendSungmaHeeTowerAction(BYTE bSubHeader);
#endif