// Cheats.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <Psapi.h>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <string>
#include <atomic>
 
struct Status
{
	int maxHP, currentHP, maxSP, currentSP;
	Status()
	{
		maxHP = 0;
		currentHP = 0;
		maxSP = 0;
		currentSP = 0;
	}
};

struct Hotkeys
{
	int whitePotion;
	int bluePotion;
}hotkeys;

struct Values
{
	float HpToWhites;
	float HpToBlues;
	float SpToBlues;
	float EmergencyBlue;
	int sleepBetweenPot;
}values;

class Client
{
	HANDLE processHandle = 0;
	DWORD processID = 0;
	HWND windowHandle = 0;
	struct Status status = Status();

private:
	int readIntAtAdress(unsigned char *address)
	{
		SIZE_T nRead;
		int value = 0;
		ReadProcessMemory(processHandle, address, &value, 4, &nRead);
		return value;
	}

	void read_HP_SP_Values()
	{
		status.maxHP = readIntAtAdress((unsigned char*)0x0083E1B8);
		status.currentHP = readIntAtAdress((unsigned char*)0x0083E1B4);
		status.maxSP = readIntAtAdress((unsigned char*)0x0083E1C0);
		status.currentSP = readIntAtAdress((unsigned char*)0x0083E1BC);
	}

	void postKey(int keyCode)
	{
		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = 0;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;

		input.ki.wVk = keyCode;
		input.ki.dwFlags = 0;
		SendInput(1, &input, sizeof(INPUT));
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	void checkEmergencyBlues()
	{
		if (status.currentSP <= status.maxSP * values.EmergencyBlue)
			postKey(hotkeys.bluePotion);
	}

	void checkWhitePotions()
	{
		if (status.currentHP <= status.maxHP * values.HpToWhites)
			postKey(hotkeys.whitePotion);
	}

	void checkBluePotions()
	{
		if (status.currentSP <= status.maxSP * values.SpToBlues && status.currentHP >= status.maxHP * values.HpToBlues)
			postKey(hotkeys.bluePotion);
	}

public:
	Client(HANDLE pHandle, DWORD pID, HWND wHandle)
	{
		processHandle = pHandle;
		processID = pID;
		windowHandle = wHandle;
		read_HP_SP_Values();
	}

	std::thread start_auto_pot(int sleep, std::atomic<bool> *stopThread, std::atomic<bool> *pauseAutopot)
	{
		return std::thread([this,sleep, stopThread, pauseAutopot] 
		{
			while(!stopThread->load()) // loop until we get the signal to stop autopotting
			{
				if (pauseAutopot->load() || GetForegroundWindow() != windowHandle) // deactivate while not on the window or told to pause
				{
					Sleep(100);
					continue;
				}
				//reads hp and sp values and pots accordingly. the order is in terms of importance.
				this->read_HP_SP_Values();
				this->checkEmergencyBlues();
				this->checkWhitePotions();
				this->checkBluePotions();

				Sleep(values.sleepBetweenPot);
			}
		});		
	}		

	DWORD getPID() { return processID; }

	struct Status getStatus() { return status; }
};


BOOL CALLBACK processWindowHandle(HWND windowHandle, LPARAM saveIn)  // saveIn contains the address of the caller's windowHandle variable, which has the processID until we rewrite it into the window handle
{
	DWORD pID = 0; 
	GetWindowThreadProcessId(windowHandle, &pID);
	if (pID == *reinterpret_cast<DWORD*>(saveIn))
	{
		*reinterpret_cast<HWND*>(saveIn) = windowHandle;
		return false;
	}
	return true;
}

std::vector<Client> getClientsByName(wchar_t* name)
{
	DWORD processIds[256];
	DWORD nProcessesRead;
	LPTSTR processPath = new TCHAR[512];

	EnumProcesses(processIds, sizeof(processIds), &nProcessesRead); 
	std::vector<Client> processes;
	for (auto processID : processIds) //For each processID, we will attempt to open it, check it's name, and if it's what we want, look for it's window and create a client class wit that information
	{
		HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processID);
		HMODULE moduleHandles[50];
		DWORD nModules;
		if (processHandle == NULL)
			continue;
		if (EnumProcessModules(processHandle, moduleHandles, sizeof(moduleHandles), &nModules))
		{
			wchar_t processName[64];
			if (GetModuleBaseName(processHandle, moduleHandles[0], processName, sizeof(processName) / sizeof(TCHAR)))
			{
				if (wcscmp(processName, name) == 0)
				{
					HWND windowHandle = reinterpret_cast<HWND>(processID);
					EnumWindows(&processWindowHandle, reinterpret_cast<LPARAM>(&windowHandle));
					processes.push_back(Client(processHandle,processID, windowHandle));
				}
			}
		}
	}
	return processes;
}

void startAutopot(wchar_t* name)
{
	std::vector<Client> clients = getClientsByName(name);
	if (clients.empty())
	{
		std::cout << "No clients found, please run as administrator\n";
		return;
	}
	int i = 0;

	std::cout << "Found clients: \n";
	for (Client client : clients) // print all the clients found
	{
		struct Status status = client.getStatus();
		std::cout << i << " -> " << "PID:" << client.getPID() << "\t" << "\tHP: " << status.currentHP << "/" << status.maxHP << "\tSP: " << status.currentSP << "/" << status.maxSP << "\n";
	}

	std::cout << "Pick client (-1 to go back): ";
	int client;
	std::cin >> client;
	if (client >= clients.size())
	{
		std::cout << "Invalid client\n";
		return;
	}
	if (client == -1)
		return;

	enum { PAUSE_KEY = 1, STOP_KEY};
	RegisterHotKey(NULL, PAUSE_KEY, NULL, VK_PAUSE); // registers the hotkeys to control this program while on the background
	RegisterHotKey(NULL, STOP_KEY, MOD_SHIFT, VK_PAUSE);

	std::atomic<bool> stopThread = false, pauseAutopot = false; 
	std::thread t = clients[client].start_auto_pot(20, &stopThread, &pauseAutopot);

	std::cout << "\t\t*****Press Pause to toggle autopot and Ctrl+Pause to stop at any time*****\n";
	while(true) // runs and reports the state of the autopot as well as handling hotkey presses
	{
		if (pauseAutopot.load())
			std::cout << "Autopot paused\n";
		else
			std::cout << "Autopot running for client " << client << "\n";
		MSG msg;
		GetMessage(&msg, NULL, 0, 0);
		if (msg.message == WM_HOTKEY)
		{
			if (msg.wParam == PAUSE_KEY)
				pauseAutopot.store(!pauseAutopot.load());
			else if (msg.wParam == STOP_KEY)
			{
				stopThread.store(true);
				t.join();
				std::cout << "Stopped autopot\n";
				return;
			}
		}
	}
}

void printConfiguration()
{
	std::cout << "Pot whites for <= " << values.HpToWhites*100 << "% HP\n";
	std::cout << "Pot blues for <= " << values.SpToBlues*100 << "% SP and >= " << values.HpToBlues * 100 << "% HP\n";
	std::cout << "Pot emergency blues for <= " << values.EmergencyBlue * 100 << "% SP\n";
	std::cout << "Sleeping " << values.sleepBetweenPot << " ms per loop\n";
}

void loadConfiguration()
{
	wchar_t currentDir[220];
	GetCurrentDirectory(200, currentDir);
	std::wstring fullPath(currentDir);
	fullPath.append(TEXT("\\hotkeys.ini"));
	hotkeys.whitePotion = GetPrivateProfileInt(TEXT("hotkeys"), TEXT("WhitePotion"),56, fullPath.c_str());
	hotkeys.bluePotion = GetPrivateProfileInt(TEXT("hotkeys"), TEXT("BluePotion"),55, fullPath.c_str());
	values.sleepBetweenPot = GetPrivateProfileInt(TEXT("values"), TEXT("Sleep"), 30, fullPath.c_str());
	values.HpToWhites = (float)GetPrivateProfileInt(TEXT("values"), TEXT("HPtoWhite"), 99, fullPath.c_str()) / 100;
	values.HpToBlues = (float)GetPrivateProfileInt(TEXT("values"), TEXT("HPtoBlue"), 70, fullPath.c_str()) / 100;
	values.SpToBlues = (float)GetPrivateProfileInt(TEXT("values"), TEXT("SPtoBlue"), 30, fullPath.c_str()) / 100;
	values.EmergencyBlue = (float)GetPrivateProfileInt(TEXT("values"), TEXT("EmergencyBlue"), 3, fullPath.c_str()) / 100;
}

int _tmain(int argc, _TCHAR* argv[])
{	
	loadConfiguration();
	do
	{
		printConfiguration();
		std::cout << "\n\n\t0 - Start Autopot\n\t1 - Reload Configurations\n\t-1 - Exit\n\n";
		int option;
		std::cout << "Option: ";
		std::cin >> option;
		system("cls");
		switch (option)
		{
			case 0: 
			{
				startAutopot(TEXT("Ragnaclan.exe"));
			}break;
			case 1: 
			{
				loadConfiguration();
				std::cout << "Configurations reloaded\n";
			}break;
			default: {return 0;}
		}
	} while (true);
	
}
