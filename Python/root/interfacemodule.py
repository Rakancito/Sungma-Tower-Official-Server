#Add 

if app.ENABLE_SUNGMAHEE_TOWER:
	import uiSungmaheeTower

#Search

		event.SetInterfaceWindow(self)

#Add after

		if app.ENABLE_SUNGMAHEE_TOWER:
			self.wndSungmaHeeTowerEnter = None
			self.wndSungmaHeeTowerInformationBoard = None


#Search

		self.wndChatLog = wndChatLog

#Add after

		if app.ENABLE_SUNGMAHEE_TOWER:
			self.wndSungmaHeeTowerEnter = uiSungmaheeTower.SungmaheeTowerEnter()
			self.wndSungmaHeeTowerInformationBoard = uiSungmaheeTower.SungmaheeTowerInformationBoard()

#Search

		# ACCESSORY_REFINE_ADD_METIN_STONE
		if self.wndItemSelect:
			self.wndItemSelect.Destroy()

#Add before

		if app.ENABLE_SUNGMAHEE_TOWER:
			if self.wndSungmaHeeTowerEnter:
				del self.wndSungmaHeeTowerEnter
				
			if self.wndSungmaHeeTowerInformationBoard:
				del self.wndSungmaHeeTowerInformationBoard

#Add in this file

	if app.ENABLE_SUNGMAHEE_TOWER:
		def OpenSungmaHeeTowerEnter(self):
			if self.wndSungmaHeeTowerEnter:
				self.wndSungmaHeeTowerEnter.Show()
				
		def SungmaHeeTowerSendInformationBoard(self, type, data):
			if self.wndSungmaHeeTowerInformationBoard:
				self.wndSungmaHeeTowerInformationBoard.SungmaheeTowerInfoBoardPacketProcess(type, data)

