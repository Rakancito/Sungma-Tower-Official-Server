#Search

	def __PressNumKey(self,num):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):

			if num >= 1 and num <= 9:
				if(chrmgr.IsPossibleEmoticon(-1)):
					chrmgr.SetEmoticon(-1,int(num)-1)
					net.SendEmoticon(int(num)-1)
		else:
			if num >= 1 and num <= 4:
				self.pressNumber(num-1)

#Add before

	if app.ENABLE_SUNGMAHEE_TOWER:
		def BINARY_OpenSungmaHeeTower(self):
			if self.interface:
				self.interface.OpenSungmaHeeTowerEnter()
				
		def BINARY_SungmaHeeTowerSendInformationBoard(self, type, data):
			if self.interface:
				self.interface.SungmaHeeTowerSendInformationBoard(type, data)