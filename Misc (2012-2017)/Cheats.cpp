// Cheats.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")

//***************** GLOBALS ***************************
unsigned char targetON[6] = {0xE9, 0x0D, 0x02, 0x00, 0x00, 0x90}, targetOFF[6] = {0x0F, 0x85, 0x0C, 0x02, 0x00, 0x00};
unsigned char groundON[2] = {0xEB, 0x46}, groundOFF[2] = {0x75, 0x46};
unsigned char moveFix[11] = {0x85, 0xF6, 0x8B, 0x97, 0xB4, 0x00, 0x00, 0x00, 0x8B, 0x4A, 0x40};
unsigned char FreeWalkOn[5] = {0x90,0x90,0x90,0x90,0x90},FreeWalkOff[5] = {0xEB,0x98,0x90,0x74,0xDC};
unsigned char atkMoveeOn[6] = {0xE9,0x0A,0x03,0x00,0x00,0x90}, atkMoveeOff[6] = {0x0F,0x85,0x09,0x03,0x00,0x00};
unsigned char *target_adress = (unsigned char*)0x0062ADFE;
unsigned char *groundAdress = (unsigned char*)0x0062B114;
unsigned char *moveFix = (unsigned char*)0x005A716E;
unsigned char *freewalk = (unsigned char*)0x005A723A;
unsigned char *atkMove = (unsigned char*)0x0062A926;
HANDLE hConsole;

//***************** PROTOTYPES ***********************
bool PrintProcessNameAndID ( DWORD processID, int i);
bool ToggleTarget(int input,HANDLE Process);
bool ToggleGround(int input,HANDLE Process);
HANDLE ProcesstoAttach();
void ShowValues(HANDLE hConsole, bool target, bool ground);
void FixMoveFunc(HANDLE Process);
bool FreeWalkToggle(int input, HANDLE Process);
bool ToggleatkMovee(int input,HANDLE Process);

//***************** FUNCTIONS ***********************
bool PrintProcessNameAndID ( DWORD processID, int i)
{
    wchar_t szProcessName[50];
    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, processID );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
        }

   		if(wcscmp(szProcessName,TEXT("frag.exe")) == 0 || wcscmp(szProcessName,TEXT("woero.exe")) == 0 || wcscmp(szProcessName,TEXT("xRO.exe")) == 0) 
		{
			SetConsoleTextAttribute(hConsole, 10);
			printf("%d",i);
			SetConsoleTextAttribute(hConsole, 15);
			printf(" - %ls  (PID: %u ) \n",szProcessName, processID );		
			CloseHandle( hProcess );
			return 1;
		}
	}
    // Release the handle to the process.

    CloseHandle( hProcess );
	return 0;
}

HANDLE ProcesstoAttach()
{
	int input,i;
	DWORD cbNeeded, cProcesses;
	DWORD ProcessIDs[200];
	HANDLE process;
	bool clients[200]={0};

	EnumProcesses( ProcessIDs, sizeof(ProcessIDs), &cbNeeded );   // Get the list of process identifiers.

    cProcesses = cbNeeded / sizeof(DWORD);	 // Calculate how many process identifiers were returned.    

    for ( i = 0; i < cProcesses; i++ )
    {
        if( ProcessIDs[i] != 0 )
        {
            clients[i] = PrintProcessNameAndID( ProcessIDs[i],i);	// Print the name and process identifier for each process.
        }
    }

	do{
		printf("\nType the number of the process to attach to or -1 to exit : ");
		do{
			scanf("%d",&input);
		}while(input >=200);
		if(clients[input] == 1 && input >=0)
		{
			process = OpenProcess( PROCESS_ALL_ACCESS , FALSE, ProcessIDs[input] );
			if(process == NULL)
			{
				printf("Error, exiting\n");
				system("pause");
				exit(0);
			}
			else
			{
				ToggleTarget(0,process);
				ToggleGround(0,process);
				FixMoveFunc(process);
				FreeWalkToggle(0,process);
				return process;
			}
			break;

		}
		if(input == -1)
		{
			exit(0);
		}
	}while(true);
}

bool ToggleTarget(int input,HANDLE Process)
{
	SIZE_T *number = new SIZE_T;
	DWORD oldProtect;
	if(input == 1)		// make it on
		{
			
			VirtualProtectEx(Process,target_adress, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
			WriteProcessMemory(Process,target_adress, targetON, 6,number);
			VirtualProtectEx(Process,target_adress, 6, oldProtect, &oldProtect);
			
			return 1;
		}
		if(input == 0)		// make it off
		{
			VirtualProtectEx(Process,target_adress, 2, PAGE_EXECUTE_READWRITE, &oldProtect);
			WriteProcessMemory(Process,target_adress, targetOFF, 2,number);
			VirtualProtectEx(Process,target_adress, 2, oldProtect, &oldProtect);
			return 0;
		}
}

bool ToggleGround(int input,HANDLE Process)
{
	SIZE_T *number = new SIZE_T;
	DWORD oldProtect = 0;

	if(input == 1)
		{
			
			VirtualProtectEx(Process,groundAdress, 2, PAGE_EXECUTE_READWRITE, &oldProtect);
			WriteProcessMemory(Process,groundAdress, groundON, 2,number);
			VirtualProtectEx(Process,groundAdress, 2, oldProtect, &oldProtect);
			
			return 1;
		}
		if(input == 0)
		{
			VirtualProtectEx(Process,groundAdress, 2, PAGE_EXECUTE_READWRITE, &oldProtect);
			WriteProcessMemory(Process,groundAdress, groundOFF, 2,number);
			VirtualProtectEx(Process,groundAdress, 2, oldProtect, &oldProtect);
			return 0;
		}
}

void FixMoveFunc(HANDLE Process)
{
	SIZE_T *number = new SIZE_T;
	DWORD oldProtect;

	VirtualProtectEx(Process, moveFix, 11, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(Process, moveFix, moveFix, 11,number);
	VirtualProtectEx(Process, moveFix, 11, oldProtect, &oldProtect);
}

bool FreeWalkToggle(int input, HANDLE Process)
{
	SIZE_T *number = new SIZE_T;
	DWORD oldProtect;

	if(input == 1)
	{
		VirtualProtectEx(Process, freewalk, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
		WriteProcessMemory(Process, freewalk, FreeWalkOn, 5,number);
		VirtualProtectEx(Process, freewalk, 5, oldProtect, &oldProtect);
		return 1;
	}
	else
	{
		VirtualProtectEx(Process, freewalk, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
		WriteProcessMemory(Process, freewalk, FreeWalkOff, 5,number);
		VirtualProtectEx(Process, freewalk, 5, oldProtect, &oldProtect);
		return 0;
	}
}

bool ToggleatkMovee(int input,HANDLE Process)
{
	SIZE_T *number = new SIZE_T;
	DWORD oldProtect;

	if(input == 1)
	{
		VirtualProtectEx(Process, atkMove, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
		WriteProcessMemory(Process, atkMove, atkMoveeOn, 6,number);
		VirtualProtectEx(Process, atkMove, 6, oldProtect, &oldProtect);
		return 1;
	}
	else
	{
		VirtualProtectEx(Process, atkMove, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
		WriteProcessMemory(Process, atkMove, atkMoveeOff, 6,number);
		VirtualProtectEx(Process, atkMove, 6, oldProtect, &oldProtect);
		return 0;
	}
}

void ShowValues(bool target, bool ground, bool walk, bool moveatk)
{
	system("cls");

	SetConsoleTextAttribute(hConsole, 15);
	printf("Type the corresponding number to type the one you want\n\n");	
	printf("    1 - Target Skills anti-move is: ");
		
	if(target == 1)
	{
		SetConsoleTextAttribute(hConsole, 10);
		printf("ON\n");
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);
		printf("OFF\n");
	}
	SetConsoleTextAttribute(hConsole, 15);
	printf("    2 - Ground Skills anti-move is: ");
		
	if(ground == 1)
	{
		SetConsoleTextAttribute(hConsole, 10);
		printf("ON\n");
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);
		printf("OFF\n");
	}
	SetConsoleTextAttribute(hConsole, 15);

	printf("    3 - Free Walk is: ");
	if(walk == 1)
	{
		SetConsoleTextAttribute(hConsole, 10);
		printf("ON\n");
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);
		printf("OFF\n");
	}
	SetConsoleTextAttribute(hConsole, 15);

	printf("    4 - Move on Attack Cursor Click is: ");
	if(moveatk == 1)
	{
		SetConsoleTextAttribute(hConsole, 10);
		printf("ON\n");
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);
		printf("OFF\n");
	}

	SetConsoleTextAttribute(hConsole, 15);
	printf("\n    9 - Put All  ");
	SetConsoleTextAttribute(hConsole, 10);
	printf("ON\n");
	SetConsoleTextAttribute(hConsole, 15);
	SetConsoleTextAttribute(hConsole, 15);
	printf("    0 - Put All  ");
	SetConsoleTextAttribute(hConsole, 12);
	printf("OFF\n");
	SetConsoleTextAttribute(hConsole, 15);
	printf("\n\n\n -1 to exit\n\n\nInput : ");

}


//  ********************** MAIN ***************************
int _tmain(int argc, _TCHAR* argv[])
{

	HANDLE Process;

    int i,input;
	bool target = 0,ground = 0, walk = 0, moveatk = 0;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	Process = ProcesstoAttach();

	do{
		ShowValues(target,ground,walk,moveatk);
		scanf("%d",&input);

		switch(input)
		{
			case -1:{
						CloseHandle( Process );
						CloseHandle( hConsole);
						exit(0);
					}break;
			case 1: target = ToggleTarget(!target,Process);break;
			case 2: ground = ToggleGround(!ground,Process);break;
			case 3: walk = FreeWalkToggle(!walk,Process);break;
			case 4: moveatk = ToggleatkMovee(!moveatk,Process);break;
			case 0: {
						target = ToggleTarget(0,Process);
						ground = ToggleGround(0,Process);
						walk = FreeWalkToggle(0,Process);
						moveatk = ToggleatkMovee(0,Process);
					}break;
			case 9:{
						target = ToggleTarget(1,Process);
						ground = ToggleGround(1,Process);
						walk = FreeWalkToggle(1,Process);
						moveatk = ToggleatkMovee(1,Process);
					}break;

			default: break;
		}

	}while(true);

	CloseHandle( Process );
	CloseHandle( hConsole);
    system("pause");
}
