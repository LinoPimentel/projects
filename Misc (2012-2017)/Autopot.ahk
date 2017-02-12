 ; ********************************************** THIS IS FOR THE GUI CONFIG ************************
IniFile=%A_WorkingDir%\DanyPots.ini   
IniRead, FileWhitePotion, %IniFile% , KeyConfig, WhitePotion
IniRead, FileBluePotion, %IniFile% , KeyConfig, BluePotion
IniRead, FileYggBerry, %IniFile% , KeyConfig, YggBerry
IniRead, FileYggSeed, %IniFile% , KeyConfig, YggSeed
IniRead, FileSilenceItem, %IniFile% , KeyConfig, SilenceItem
IniRead, FileCurseItem, %IniFile% , KeyConfig, CurseItem
IniRead, FileBlindItem, %IniFile% , KeyConfig, BlindItem
IniRead, FileAutoRestock, %IniFile% , KeyConfig, AutoRestock
IniRead, FileHPtoPotWhites, %IniFile% , Values, HPtoPotWhites
IniRead, FileSPtoPotBlues, %IniFile% , Values, SPtoPotBLues
IniRead, FileMinimumHPtoPotBlues, %IniFile% , Values, MinimumHPtoPotBlues
IniRead, FileHPtoPotBerry, %IniFile% , Values, HPtoPotBerry
IniRead, FileHPtoPotSeed, %IniFile% , Values, HPtoPotSeed
IniRead, FileRestockTimer, %IniFile% , Values, RestockTimer
IniRead, FileMinimumSPforEmergencyBlues, %IniFile% , Values, MinimumSPforEmergencyBlues
IniRead, FileTimetoWaitAfterDeath, %IniFile% , Values, TimetoWaitAfterDeath
IniRead, FileDontUseSeed, %IniFile% , CheckBoxes, DontUseSeed
IniRead, FileDontUseBerry, %IniFile% , CheckBoxes, DontUseBerry
IniRead, FileDontDetectSilence, %IniFile% , CheckBoxes, DontDetectSilence
IniRead, FileDontDetectCurse, %IniFile% , CheckBoxes, DontDetectCurse
IniRead, FileDontDetectBlind, %IniFile% , CheckBoxes, DontDetectBlind
IniRead, FileUseAutoRestock, %IniFile% , CheckBoxes, UseAutoRestock
IniRead, FileUseEmergencyBlues, %IniFile% , CheckBoxes, UseEmergencyBlues
IniRead, FileWaitAfterDeath, %IniFile% , CheckBoxes, WaitAfterDeath

Gui, Add, Text, x12 y20 w100 h20 , White Potion:
Gui, Add, Hotkey, x12 y40 w70 h20 vWhitePotion, %FileWhitePotion%
Gui, Add, Text, x12 y70 w100 h20 , Blue Potion: 
Gui, Add, Hotkey, x12 y90 w70 h20 vBluePotion, %FileBluePotion%
Gui, Add, Text, x12 y120 w100 h20 , Yggdrasill Seed:
Gui, Add, Hotkey, x12 y140 w70 h20 vYggdrasillSeed, %FileYggSeed%
Gui, Add, Text, x12 y170 w100 h20 , Yggdrasill Berry:
Gui, Add, Hotkey, x12 y190 w70 h20 vYggdrasillBerry, %FileYggBerry%
Gui, Add, Text, x12 y240 w100 h20 , Silence Item:
Gui, Add, Hotkey, x12 y260 w70 h20 vGreenPotion, %FileSilenceItem%
Gui, Add, Text, x12 y290 w100 h20 , Curse Item:
Gui, Add, Hotkey, x12 y310 w70 h20 vCursePotion, %FileCurseItem%
Gui, Add, Text, x12 y340 w100 h20 , Blind Item:
Gui, Add, Hotkey, x12 y360 w70 h20 vBlindPotion, %FileBlindItem%
Gui, Add, Text, x12 y390 w100 h20 , Auto Restock:
Gui, Add, Hotkey, x12 y410 w70 h20 vAutoRestock, %FileAutoRestock%
Gui, Add, Text, x142 y20 w90 h20 , HP to pot Whites:
Gui, Add, Edit, x142 y40 w70 h20 vHpWhite, %FileHPtoPotWhites%
Gui, Add, Text, x142 y70 w90 h20 , SP to Pot Blues:
Gui, Add, Edit, x142 y90 w70 h20 vSpBlue, %FileSPtoPotBlues%
Gui, Add, Text, x262 y70 w140 h20 , Minimum HP to pot Blues:
Gui, Add, Edit, x262 y90 w70 h20 vMinHP, %FileMinimumHPtoPotBlues%
Gui, Add, Text, x142 y120 w90 h20 , HP to pot Seed:
Gui, Add, Edit, x142 y140 w70 h20 vHpSeed, %FileHPtoPotSeed%
Gui, Add, Text, x142 y170 w90 h20 , HP to pot Berry:
Gui, Add, Edit, x142 y190 w70 h20 vHpBerry, %FileHPtoPotBerry%
Gui, Add, CheckBox, Checked%FileDontUseSeed% x262 y130 w100 h20 vDontUseSeed, Don't Use Seed
Gui, Add, CheckBox, Checked%FileDontBerry% x262 y180 w100 h20 vDontUseBerry, Don't Use Berry
Gui, Add, CheckBox, Checked%FileDontDetectSilence% x132 y250 w120 h20 vDontDetectSilence, Don't Detect Silence
Gui, Add, CheckBox, Checked%FileDontDetectCurse% x132 y300 w120 h20 vDontDetectCurse, Don't Detect Curse
Gui, Add, CheckBox, Checked%FileDontDetectBlind% x132 y350 w120 h20 vDontDetectBlind, Don't Detect Blind
Gui, Add, CheckBox, Checked%FileUseAutoRestock% x132 y400 w120 h20 vUseAutoRestock, Use Auto Restock
Gui, Add, Text, x12 y440 w90 h20 , Restock Timer:
Gui, Add, Edit, x132 y440 w70 h20 vRestockTimer, %FileRestockTimer%
Gui, Add, Text, x12 y510 w190 h20 , Minimum SP for emergency Blues:
Gui, Add, Edit, x12 y530 w70 h20 vMinEmSP, %FileMinimumSPforEmergencyBlues%
Gui, Add, CheckBox, Checked%FileUseEmergencyBlues% x202 y520 w140 h20 vUseEmergencyBlue, Use Emergency Blues
Gui, Add, Text, x12 y560 w170 h20 , Time to Wait After Death:
Gui, Add, Edit, x12 y580 w70 h20 vWoeModeTime, %FileTimetoWaitAfterDeath%
Gui, Add, CheckBox, Checked%FileWaitAfterDeath% x202 y570 w150 h20 vWoeMode, Wait after Death
Gui, Add, Button, x30 y620 w120 h20 default, Submit
Gui, Add, Button, x180 y620 w120 h20 , Submit And Save
Gui, Add, Button, x340 y620 w50 h20 , Exit
Gui, Add, GroupBox, x2 y0 w410 h220 , Potion Setup:
Gui, Add, GroupBox, x2 y220 w410 h270 , Extra Options:
Gui, Add, GroupBox, x2 y490 w410 h120 , Other Options:

Gui, Show, x626 y205 h650 w421
Return

ButtonExit:
ExitApp,0

ButtonSubmit:
Gui,Submit

ButtonSubmitAndSave:
{
    Gui,Submit
	IniWrite, %WhitePotion%, %IniFile% , KeyConfig, WhitePotion
	IniWrite, %BluePotion%, %IniFile% , KeyConfig, BluePotion
	IniWrite, %YggdrasillBerry%, %IniFile% , KeyConfig, YggBerry
	IniWrite, %YggdrasillSeed%, %IniFile% , KeyConfig, YggSeed
	IniWrite, %GreenPotion%, %IniFile% , KeyConfig, SilenceItem
	IniWrite, %CursePotion%, %IniFile% , KeyConfig, CurseItem
	IniWrite, %BlindPotion%, %IniFile% , KeyConfig, BlindItem
	IniWrite, %AutoRestock%, %IniFile% , KeyConfig, AutoRestock
	IniWrite, %HpWhite%, %IniFile% , Values, HPtoPotWhites
	IniWrite, %SpBlue%, %IniFile% , Values, SPtoPotBLues
	IniWrite, %MinHP%, %IniFile% , Values, MinimumHPtoPotBlues
	IniWrite, %HpBerry%, %IniFile% , Values, HPtoPotBerry
	IniWrite, %HpSeed%, %IniFile% , Values, HPtoPotSeed
	IniWrite, %MinEmSP%, %IniFile% , Values, MinimumSPforEmergencyBlues
	IniWrite, %RestockTimer%, %IniFile%, Values, RestockTimer
	IniWrite, %WoeModeTime%, %IniFile% , Values, TimetoWaitAfterDeath
	IniWrite, %DontUseSeed%, %IniFile% , CheckBoxes, DontUseSeed
	IniWrite, %DontUseBerry%, %IniFile% , CheckBoxes, DontUseBerry
	IniWrite, %DontDetectSilence%, %IniFile% , CheckBoxes, DontDetectSilence
	IniWrite, %DontDetectCurse%, %IniFile% , CheckBoxes, DontDetectCurse
	IniWrite, %DontDetectBlind%, %IniFile% , CheckBoxes, DontDetectBlind
	IniWrite, %UseAutoRestock%, %IniFile% , CheckBoxes, UseAutoRestock
	IniWrite, %UseEmergencyBlue%, %IniFile% , CheckBoxes, UseEmergencyBlues
	IniWrite, %WoeMode%, %IniFile% , CheckBoxes, WaitAfterDeath
}

 ; ********************************************** THIS IS FOR GETTING CLIENT INFO ************************
msgbox, Use "Break" a.k.a "Pause" to Pause and resume the script!!!

SendMode Input

winget,pid,PID,EstlandRO MR [estlandro.cc] | Gepard Shield 2.0 (^-_-^)

ProcessHandle := DllCall("OpenProcess", "Int", 24, "Char", 0, "UInt", pid, "UInt")

 ; ********************************************** THIS IS FOR DECLARING VARIABLES ************************
VarSetCapacity(MaxHP,4,0)
VarSetCapacity(ActualHP,4,0)
VarSetCapacity(MaxSP,4,0)
VarSetCapacity(ActualSP,4,0)
VarSetCapacity(Silence,1,0)
VarSetCapacity(Curse,1,0)
VarSetCapacity(Blind,1,0)

useberry = 1
useseed = 1
usewhite = 1
useblue = 1
UseEmBlue = 1
WoeModeTime *= 1000
restockcounter = 0
usegreensil = 1
usegreencurs = 1
usegreenblin = 1
RestockTimer *= 1000
StartTime := A_TickCount

 ; ********************************************** THIS IS FOR THE MAIN LOOP ************************
While 1
{
	WinGet, active_pid, PID, A
	if(active_pid != pid)
	{
		Sleep 30
		continue
	}

	if GetKeyState("LAlt", "P")
	{
		Sleep 1
		continue
	}

	if GetKeyState("LShift", "P")
	{
		Sleep 1
		continue
	}
	
	if GetKeyState("RAlt", "P")
	{
		Sleep 1
		continue
	}

	if GetKeyState("RShift", "P")
	{
		Sleep 1
		continue
	}
	
 ; ********************************************** THIS IS FOR EMERGENCY BLUES ************************

	if(UseEmergencyBlue = 1 && UseEmBlue = 1)
	{
		DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083E1C0,"Str",MaxSP,"UInt",4,"UInt *",0)
		result = 0
		Loop 4		
			result2 += *(&MaxSP + A_Index-1) << 8*(A_Index-1)
		
		DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083E1BC,"Str",ActualSP,"UInt",4,"UInt *",0)
		result3 = 0
		Loop 4
			result3 += *(&ActualSP + A_Index-1) << 8*(A_Index-1)		
		
		if(result3 <= result2*(MinEmSP/100))
		{
			if GetKeyState("LAlt", "P")
				continue
			if GetKeyState("LShift", "P")
				continue			
			Send, {%BluePotion%}  
		}
		if(result3 >= result2*0.30)
		{
			UseEmBlue = 0
			EmBlueCounter = 0
		}
	}
 ; ********************************************** THIS IS FOR GREENS ************************
 ; old address of silence 00838A88
	if(DontDetectSilence = 0 && usegreensil = 1)
	{
		DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083BE10,"Str",Silence,"UInt",1,"UInt *",0)
		sil = 0
		sil += *(&Silence)
		if(sil = 4)
		{
			if GetKeyState("LAlt", "P")
				continue
			if GetKeyState("LShift", "P")
				continue			
			Send, {%GreenPotion%}
			sil1 = %sil%
			DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083BE108,"Str",Silence,"UInt",1,"UInt *",0) 
			sil = 0
			sil += *(&Silence)
			if(sil = sil1)
			{
				usegreensil = 0
				greensilcounter = 0
			}
		}
	}

 ; old address of curse 0x008391BD
	if(DontDetectCurse =0 && usegreencurs = 1)
	{
		DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083C545,"Str",Curse,"UInt",1,"UInt *",0)
		curs = 0
		curs += *(&Curse)
		if(curs = 2)
		{
			if GetKeyState("LAlt", "P")
				continue
			if GetKeyState("LShift", "P")
				continue
			Send, {%CursePotion%}
			curs1 = %curs%
			DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083C545,"Str",Curse,"UInt",1,"UInt *",0)
			curs = 0
			curs += *(&Curse)
			if(curs = curs1)
			{ 
				usegreencurs = 0
				greencurscounter = 0
			}
		}
	}

 ; old address of blind 
	if(DontDetectBlind =0 && usegreenblin = 1)
	{
		DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083BE10,"Str",Blind,"UInt",1,"UInt *",0)
		blin = 0
		blin += *(&Blind)
		if(blin = 16)
		{
			if GetKeyState("LAlt", "P")
				continue
			if GetKeyState("LShift", "P")
				continue
			Send, {%BlindPotion%}
			blin1 = %blin%
			DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083BE10,"Str",Blind,"UInt",1,"UInt *",0)
			blin = 0
			blin += *(&Blind)
			if(blin = blin1)
			{ 
				usegreenblin = 0
				greenblincounter = 0
			}
		}
	}
 ; ********************************************** THIS IS FOR HP ************************
	DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083E1B8,"Str",MaxHP,"UInt",4,"UInt *",0)
	result = 0
	Loop 4
		result += *(&MaxHP + A_Index-1) << 8*(A_Index-1)

	DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083E1B4,"Str",ActualHP,"UInt",4,"UInt *",0)
	result1 = 0
	Loop 4
		result1 += *(&ActualHP + A_Index-1) << 8*(A_Index-1)

	if(result1 = 0)
	{
		if(WoeMode = 1)
			Sleep %WoeModeTime%
		continue
	}
	if (result1 < result)
	{
		if(result1 < result*(HpBerry/100) && useberry = 1 && DontUseBerry = 0)
		{
			HP = %result1%
			if GetKeyState("LAlt", "P")
				continue
			if GetKeyState("LShift", "P")
				continue
			Send, {%YggdrasillBerry%}  
			DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083E1B4,"Str",ActualHP,"UInt",4,"UInt *",0)
			result1 = 0
			Loop 4
				result1 += *(&ActualHP + A_Index-1) << 8*(A_Index-1)
			
			if(HP = result1)
			{
				useberry = 0
				berrycounter = 0	    
			}
		}
		if (result1 < result*(HpSeed/100) && useseed = 1 && DontUseSeed = 0)
		{
			HP = %result1%
			if GetKeyState("LAlt", "P")
				continue
			if GetKeyState("LShift", "P")
				continue
			Send, {%YggdrasillSeed%}
			DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083E1B4,"Str",ActualHP,"UInt",4,"UInt *",0)
			result1 = 0
			Loop 4
				result1 += *(&ActualHP + A_Index-1) << 8*(A_Index-1)
			
			if(HP = result1)
			{
				useseed = 0
				seedcounter = 0
			}
		}

		if(result1 < result*(HpWhite/100) && usewhite = 1)
		{
			HP = %result1%
			if GetKeyState("LAlt", "P")
				continue
			if GetKeyState("LShift", "P")
				continue
			Send, {%WhitePotion%}
			DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083E1B4,"Str",ActualHP,"UInt",4,"UInt *",0)
			result1 = 0
			Loop 4
				result1 += *(&ActualHP + A_Index-1) << 8*(A_Index-1)
			
			if(HP = result1)
			{
				if(restockcounter >= 80000 && UseAutoRestock = 1)
				{
					Send, {%AutoRestock%}
					restockcounter = 0
				}
				usewhite = 0
				whitecounter = 0
			}
		}
	} 
 ; ********************************************** THIS IS FOR SP ************************
	if(result1 > result*(MinHP/100) && useblue = 1)
	{
		DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083E1C0,"Str",MaxSP,"UInt",4,"UInt *",0)
		result2 = 0
		Loop 4
			result2 += *(&MaxSP + A_Index-1) << 8*(A_Index-1)

		DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083E1BC,"Str",ActualSP,"UInt",4,"UInt *",0)
		result3 = 0
		Loop 4
			result3 += *(&ActualSP + A_Index-1) << 8*(A_Index-1)

		if(result3 < result2*(SpBlue/100))
		{
			SP = %result3%
			if GetKeyState("LAlt", "P")
				continue
			if GetKeyState("LShift", "P")
				continue
			Send, {%BluePotion%}
			DllCall("ReadProcessMemory","UInt",ProcessHandle,"UInt",0x0083E1BC,"Str",ActualSP,"UInt",4,"UInt *",0)
			result3 = 0
			Loop 4
				result3 += *(&ActualSP + A_Index-1) << 8*(A_Index-1)

			if(SP = result3)
			{			 
				if(restockcounter >= 20000 && UseAutoRestock = 1)
				{
					if GetKeyState("LAlt", "P")
						continue
					if GetKeyState("LShift", "P")
						continue
					Send, {%AutoRestock%}
					restockcounter = 0
				}
				useblue = 0
				bluecounter = 0			  
			}
		}
	}

 ; ********************************************** THIS IS FOR COUNTERS ************************
	if(useberry = 0 && berrycounter < 30)
		berrycounter += 1
	if(useberry = 0 && berrycounter = 30)
		useberry = 1

	if(useseed = 0 && seedcounter < 30)
		seedcounter += 1
	if(useseed = 0 && seedcounter = 30)
		useseed = 1

	if(usewhite = 0 && whitecounter <30)
		whitecounter += 1
	if(usewhite = 0 && whitecounter =30)
		usewhite = 1

	if(useblue = 0 && bluecounter <30)
		bluecounter += 1
	if(useblue = 0 && bluecounter = 30)
		useblue = 1

	if(UseEmBlue = 0 && EmBlueCounter <30)
		EmBlueCounter += 1
	if(UseEmBlue = 0 && EmBlueCounter = 30)
		UseEmBlue = 1

	if(UseAutoRestock = 1)
	{
		restockcounter += 1
	}
	
	ElapsedTime := A_TickCount-StartTime
	if(ElapsedTime >= RestockTimer && UseAutoRestock = 1)
	{
		Send, {%AutoRestock%}
	    StartTime := A_TickCount
	}

	if(usegreensil = 0 && greensilcounter <30)
		greensilcounter += 1
	if(usegreensil = 0 && greensilcounter = 30)
		usegreensil = 1

	if(usegreencurs = 0 && greencurscounter <30)
		greencurscounter += 1
	if(usegreencurs = 0 && greencurscounter = 30)
		usegreencurs = 1

	if(usegreenblin = 0 && greenblincounter <30)
		greenblincounter += 1
	if(usegreenblin = 0 && greenblincounter = 30)
		usegreenblin = 1
}

Pause::Pause
