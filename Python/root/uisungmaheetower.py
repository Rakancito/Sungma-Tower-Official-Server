import app
import net
import player
import uiToolTip
import chr
import uiScriptLocale
import localeInfo
import ui
import dbg

from _weakref import proxy

BUTTON_IMG_HEIGHT = 28
BUTTON_IMG_WIDTH = 126

MAX_REWARD_ELEMENT = 45
	
BUTTON_IMG_PATH = "d:/ymir work/ui/game/sungmahee_tower/"

class SungmaheeTowerInformationBoard(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		
		self.Floor = 0
		self.Time = 0
		self.ActualTime = 0
		self.TextTime = ""
		self.Stage = 0
		
		self.IsDungeon = False
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)

		self.Floor = 0		
		self.Time = 0
		self.ActualTime = 0
		self.TextTime = ""
		self.Stage = 0
		
		#self.stage_clear_icon_1 = 0
		#self.stage_clear_icon_2 = 0
		#self.stage_clear_icon_3 = 0
		
		self.IsDungeon = False
		
	def Show(self):
		self.__LoadWindow()
		
		ui.ScriptWindow.Show(self)

	def __LoadWindow(self):
		try:
			ui.PythonScriptLoader().LoadScriptFile(self, "UIScript/sungmaheetowerinformationboard.py")
		except:
			import exception
			exception.Abort("sungmaheetowerinformationboard.__LoadWindow.__LoadScript")
			
		try:
			self.__BindObject()
		except:
			import exception
			exception.Abort("sungmaheetowerinformationboard.__LoadWindow.__BindObject")
			
		try:
			self.__BindEvent()
		except:
			import exception
			exception.Abort("sungmaheetowerinformationboard.__LoadWindow.__BindEvent")
	
	def __BindObject(self):
		self.floor_text		= self.GetChild("floor_text")
		self.alarm_text		= self.GetChild("alarm_text")
		self.time_gauge		= self.GetChild("time_gauge")
		self.time_text		= self.GetChild("time_text")
		
		self.stage_clear_icon_1 = self.GetChild("1stage_clear_icon")
		self.stage_clear_icon_2 = self.GetChild("2stage_clear_icon")
		self.stage_clear_icon_3 = self.GetChild("3stage_clear_icon")
		
		self.stage_clear_icon_1.Hide()
		self.stage_clear_icon_2.Hide()
		self.stage_clear_icon_3.Hide()
		
	def __BindEvent(self):
		pass
	
	def __SetTimeGauge(self, cur_gauge, max_gauge):
		self.time_gauge.SetPercentage(cur_gauge, max_gauge)
		
	def __TimeTextUpdate(self):
		self.time_text.SetText(str(self.TextTime))

	def __SetTimeText(self, stage_limit_time, stage_clear_time):
		time = stage_limit_time - stage_clear_time
		m, s = divmod(time, 60)
		h, m = divmod(m, 60)
		return "%1d:%02d:%02d" % (h, m, s)
		
	def __SetFloorText(self, floor):
		self.floor_text.SetText("N"+str(floor))

	def __SetMissionText(self, mision_type):
		pass

	def SungmaheeTowerInfoBoardPacketProcess(self, type, data):
		#return
		if type == player.SUB_HEADER_INFORMATION_OPEN_BOARD:
			self.Show()
			(floor, stage, stage_time) = data
			self.Floor = floor	
			self.ActualTime = app.GetGlobalTimeStamp()
			self.Time = stage_time
			self.Stage = stage

			self.__SetFloorText(self.Floor)

			self.IsDungeon = True

			if self.Stage == 1:
				self.stage_clear_icon_1.Show()
			elif self.Stage == 2:
				self.stage_clear_icon_1.Show()
				self.stage_clear_icon_2.Show()
			elif self.Stage == 3:
				self.stage_clear_icon_1.Show()
				self.stage_clear_icon_2.Show()			
				self.stage_clear_icon_3.Show()

		elif type == player.SUB_HEADER_INFORMATION_SEND_BOARD:
			(floor, stage, stage_time) = data
			self.Floor = floor	
			self.ActualTime = app.GetGlobalTimeStamp()
			self.Time = stage_time
			self.Stage = stage

			self.__SetFloorText(self.Floor)

			self.IsDungeon = True

			if self.Stage == 1:
				self.stage_clear_icon_1.Show()
			elif self.Stage == 2:
				self.stage_clear_icon_2.Show()
			elif self.Stage == 3:
				self.stage_clear_icon_3.Show()
				
	def OnUpdate(self):
		if self.IsDungeon:
			if self.Time > 0:
				if self.Time >= app.GetGlobalTimeStamp():
					#Time
					self.__SetTimeGauge(self.Time - app.GetGlobalTimeStamp(), self.Time - self.ActualTime)			
					#Time Text
					self.TextTime = self.__SetTimeText(self.Time, app.GetGlobalTimeStamp())
					self.__TimeTextUpdate()
			
class SungmaheeTowerFloorButton:
	def __init__(self, floor_index, parent, number, parent_object):
		self.floor_index		= floor_index
		self.parent				= parent
		self.lvlWindow			= None
		self.parent_object		= ui.__mem_func__(parent_object)
		
		self.__CreateFloorButton(floor_index, parent, number)
		self.Show()

	def __del__(self):
		self.floor_index		= -1
		self.parent				= None
		self.OnClickButton		= None
		self.lvlWindow			= None
		self.parent_object		= None
		
	def __CreateFloorButton(self, floor_index, parent, number):
		self.button = ui.Button()
		self.button.SetParent(proxy(parent))
		self.button.SetUpVisual(BUTTON_IMG_PATH+"floor_button_default.sub")
		self.button.SetDownVisual(BUTTON_IMG_PATH+"floor_button_down.sub")
		self.button.SetOverVisual(BUTTON_IMG_PATH+"floor_button_over.sub")

		self.button.SetEvent( ui.__mem_func__(self.__OnClickButton), floor_index )
		pos_y = 30 * (5-number)
		self.button.SetPosition( 0, pos_y )
		self.button.Show()
			
		self.text = ui.TextLine()
		self.text.SetParent( self.button )
		self.text.SetPosition( 0, 0 )
		self.text.SetHorizontalAlignCenter()
		self.text.SetVerticalAlignCenter()
		self.text.SetWindowHorizontalAlignCenter()
		self.text.SetWindowVerticalAlignCenter()
		self.text.SetText("Level: "+str(floor_index))
		self.text.AddFlag("attach")
		self.text.AddFlag("not_pick")
		self.text.Show()
		

	def __OnClickButton(self, select):
		apply( self.parent_object, [self.floor_index] )
		
	def Show(self):
		if self.button:
			self.button.Show()
			
class SungmaheeTowerEnter(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		
		self.board						= None
		self.element_text_window		= None
		self.floor_list_window			= None
		self.floor_scrollbar			= None
		self.reward_slot				= None
		self.tooltipItem				= None
		self.element_text				= None
		
		self.enter_button				= None
		
		self.floor_actual_index			= 1
		self.floor_list					= []
		self.floor_reward_data_list		= []
		self.floor_element_data_list	= []
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
		self.board						= None
		self.element_text_window		= None
		self.floor_list_window			= None
		self.floor_scrollbar			= None
		self.reward_slot				= None
		self.tooltipItem				= None
		self.element_text				= None
		
		self.enter_button				= None
		
		self.floor_actual_index			= 0
		self.floor_list					= []
		self.floor_reward_data_list		= []
		self.floor_element_data_list	= []
		
	def Show(self):
	
		(self.StartPosX, self.StartPosY, z) = player.GetMainCharacterPosition()
		self.__LoadWindow()
		
		self.SetCenterPosition()

		ui.ScriptWindow.Show(self)
		
	def __LoadWindow(self):
		try:
			ui.PythonScriptLoader().LoadScriptFile(self, "UIScript/sungmaheetowerenter.py")
		except:
			import exception
			exception.Abort("sungmaheetowerenter.__LoadWindow.__LoadScript")
			
		try:
			self.__BindObject()
		except:
			import exception
			exception.Abort("sungmaheetowerenter.__LoadWindow.__BindObject")
			
		try:
			self.__BindEvent()
		except:
			import exception
			exception.Abort("sungmaheetowerenter.__LoadWindow.__BindEvent")
			
		self.__RefreshFloorList(MAX_REWARD_ELEMENT)
		self.__LoadDataFile()
		self.__OnClickButton(1)

	def __BindObject(self):
		self.board					= self.GetChild("board")
		self.element_text_window	= self.GetChild("element_text_window")
		self.floor_list_window		= self.GetChild("floor_list_window")
		self.floor_scrollbar		= self.GetChild("floor_scrollbar")
		self.reward_slot			= self.GetChild("reward_slot")
		self.element_text			= self.GetChild("element_text")
		self.enter_button			= self.GetChild("enter_button")
		
		self.tooltipItem			= uiToolTip.ItemToolTip()
		
	def __BindEvent(self):
		if self.board:
			self.board.SetCloseEvent(ui.__mem_func__(self.Close))
		
		if self.floor_scrollbar:
			self.floor_scrollbar.SetScrollEvent(self.__CalculScrollPos)
			self.floor_scrollbar.SetPos(MAX_REWARD_ELEMENT)
		
		if self.reward_slot:
			self.reward_slot.SetOverInItemEvent(ui.__mem_func__(self.__OverInRewardSlot))
			self.reward_slot.SetOverOutItemEvent(ui.__mem_func__(self.__OverOutRewardSlot))
		
		if self.enter_button:
			self.enter_button.SetEvent(ui.__mem_func__(self.__OnClickEnterButton))

	def __OnClickEnterButton(self):
		player.SendSungmaHeeTowerEnter(1)
		
	def __RefreshFloorList(self, pos):
		self.number = 1
		for floor_index in xrange(5):
			self.floor_list.append(SungmaheeTowerFloorButton(floor_index+(MAX_REWARD_ELEMENT-pos)+1, self.floor_list_window, self.number, self.__OnClickButton))
			self.number += 1
			
	def __CalculScrollPos(self):
		self.pos = int(self.floor_scrollbar.GetPos() * (MAX_REWARD_ELEMENT))
		self.__RefreshFloorList(self.pos)
	
	def __LoadDataFile(self):
		self.__LoadRewardFile()
		self.__LoadElementFile()
	
	def __LoadElementFile(self):
		handle = app.OpenTextFile(app.GetLocalePath() + "/sungmahee_tower/standard/sungmahee_tower_element.txt")
		count = app.GetTextFileLineCount(handle)
		for i in xrange(3,count-1):
			line = app.GetTextFileLine(handle, i)
			tokens = line.split("\t")

			TOKEN_FLOOR		= 1
			TOKEN_ELEMENT	= 2

			
			floor			= int(tokens[TOKEN_FLOOR])
			element			= str(tokens[TOKEN_ELEMENT])
			
			if element == "FIRE":
				element		= uiScriptLocale.SUNGMAHEE_TOWER_ELEMENT_FIRE
			elif element == "ICE":
				element		= uiScriptLocale.SUNGMAHEE_TOWER_ELEMENT_ICE
			elif element == "ELEC":
				element		= uiScriptLocale.SUNGMAHEE_TOWER_ELEMENT_ELEC
			elif element == "WIND":
				element		= uiScriptLocale.SUNGMAHEE_TOWER_ELEMENT_WIND
			elif element == "EARTH":
				element		= uiScriptLocale.SUNGMAHEE_TOWER_ELEMENT_EARTH
			elif element == "DARK":
				element		= uiScriptLocale.SUNGMAHEE_TOWER_ELEMENT_DARK
			elif element == "ALL":
				element		= uiScriptLocale.SUNGMAHEE_TOWER_ELEMENT_ALL

			appendingData = { 
							  "FLOOR" : floor,
							  "ELEMENT" : element,
							}
	
			self.floor_element_data_list.append(appendingData)
			
	def __LoadRewardFile(self):
		handle = app.OpenTextFile(app.GetLocalePath() + "/sungmahee_tower/standard/sungmahee_tower_reward.txt")
		count = app.GetTextFileLineCount(handle)
		for i in xrange(3,count-1):
			line = app.GetTextFileLine(handle, i)
			tokens = line.split("\t")

			TOKEN_FLOOR		= 1
			TOKEN_VNUM1		= 2
			TOKEN_COUNT1	= 3
			TOKEN_VNUM2		= 4
			TOKEN_COUNT2	= 5
			TOKEN_VNUM3		= 6
			TOKEN_COUNT3	= 7
			TOKEN_VNUM4		= 8
			TOKEN_COUNT4	= 9
			TOKEN_VNUM5		= 10
			TOKEN_COUNT5	= 11
			
			floor			= int(tokens[TOKEN_FLOOR])
			vnum1			= int(tokens[TOKEN_VNUM1])
			count1			= int(tokens[TOKEN_COUNT1])
			vnum2			= int(tokens[TOKEN_VNUM2])
			count2			= int(tokens[TOKEN_COUNT2])
			vnum3			= int(tokens[TOKEN_VNUM3])
			count3			= int(tokens[TOKEN_COUNT3])
			vnum4			= int(tokens[TOKEN_VNUM4])
			count4			= int(tokens[TOKEN_COUNT4])
			vnum5			= int(tokens[TOKEN_VNUM5])
			count5			= int(tokens[TOKEN_COUNT5])
			
			appendingData = { 
							  "FLOOR" : floor,
							  "VNUM1" : vnum1,
							  "COUNT1" : count1,
							  "VNUM2" : vnum2,
							  "COUNT2" : count2,
							  "VNUM3" : vnum3,
							  "COUNT3" : count3,
							  "VNUM4" : vnum4,
							  "COUNT4" : count4,
							  "VNUM5" : vnum5,
							  "COUNT5" : count5
							}

			self.floor_reward_data_list.append(appendingData)

	def __OnClickButton(self, floor_index):
		self.floor_actual_index = floor_index
		for data in self.floor_reward_data_list:
			if self.floor_actual_index == data["FLOOR"]:
				self.reward_slot.SetItemSlot(0, data["VNUM1"], data["COUNT1"])
				self.reward_slot.SetItemSlot(1, data["VNUM2"], data["COUNT2"])
				self.reward_slot.SetItemSlot(2, data["VNUM3"], data["COUNT3"])
				self.reward_slot.SetItemSlot(3, data["VNUM4"], data["COUNT4"])
				self.reward_slot.SetItemSlot(4, data["VNUM5"], data["COUNT5"])	
		for element in self.floor_element_data_list:
			if self.floor_actual_index == element["FLOOR"]:
				self.element_text.SetText(element["ELEMENT"])
	
	def __OverInRewardSlot(self, slot_index):
		if self.tooltipItem:
			for data in self.floor_reward_data_list:
				if self.floor_actual_index == data["FLOOR"]:
					self.tooltipItem.HideToolTip()
					self.tooltipItem.ClearToolTip()
					if (slot_index == 0):
						self.tooltipItem.AddItemData(data["VNUM1"], 0, 0)
					elif (slot_index == 1):
						self.tooltipItem.AddItemData(data["VNUM2"], 0, 0)
					elif (slot_index == 2):
						self.tooltipItem.AddItemData(data["VNUM3"], 0, 0)
					elif (slot_index == 3):
						self.tooltipItem.AddItemData(data["VNUM4"], 0, 0)
					elif (slot_index == 4):
						self.tooltipItem.AddItemData(data["VNUM5"], 0, 0)
						
	def __OverOutRewardSlot(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
			self.tooltipItem.ClearToolTip()

	def Close(self):
		self.Hide()
		
		