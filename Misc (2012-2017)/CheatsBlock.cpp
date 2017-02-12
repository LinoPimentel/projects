// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "sha256 string.h"
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>


#include <fstream>
#include <string>
#include <cerrno>

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

using std::string;

// FUNCTION PROTOTYPES

bool FileNameParser(char *orig[]);	//grf name parser
string Get_File_String(const char *filename); //Reads file contents and returns a string
unsigned int _cdecl SendHook(int lenght);	// hook on send
unsigned int _cdecl NormalGrfProt(char *buffer);	//normal grf
unsigned int _cdecl GrfContentCheckSumWoe(int dummy, int dummy1, int dummy2, int size);	
unsigned int _cdecl GrfNameDetectionWoE(int filehandle, char *buffer); //woe grfs, forces the load of other files. Only activate in woe

// hook
void hookadress(unsigned int origfuncadress, unsigned int ourfuncadress, unsigned int proxycaveadress, int nopcount, int argumentcount);
void _stdcall Unhook();

// GLOBAL VARIABLES
bool entryflag=1;  // ensure it runs a sigle time disable later
short filefound;
char bytes_to_add[2] = {0x00,0x00};
// hex codes for just the file name to track (for both woe and normal protection)
char filenames[2][25] = {	// put number of files on first [] and also change in the function grfnamedetection.
						{0xC5, 0xA9, 0xB8, 0xAE, 0xBF, 0xA1, 0xC0, 0xCC, 0xC5, 0xCD, 0x5F, 0xB3, 0xB2, 0x2E, 0x73, 0x70, 0x72},	//creator_M.spr
						{0xC5, 0xA9, 0xB8, 0xAE, 0xBF, 0xA1, 0xC0, 0xCC, 0xC5, 0xCD, 0x5F, 0xB3, 0xB2, 0x2E, 0x61, 0x63, 0x74}	//creator_M.act
						};
string fragini_checksum = "c163fc3d6a4a093db445ba856b80c7bd567310afbfa92b5b4155076d0d695cd0";	// checksum for FRAG.INI
// check_sums for each of the files
string check_sums[2] = {"e0c94b4b2b34def73ad649993005dfd0d7c4a2eacd513c7923fb03e3b004250b",	//creator_M.spr
						"0f1c15cd8b124eb211e04e9eabde79197394d3c0aa4c8b38702b403af8a8dd24"	//creator_M.act
					  };
//file paths for normal grf block
char normal_file_paths[2][78] = {"data\\sprite\\인간족\\몸통\\남\\크리에이터_남.spr",	//creator_M.spr
						        "data\\sprite\\인간족\\몸통\\남\\크리에이터_남.act"	//creator_M.act
							  };
// file paths for woe grf block
char woe_file_paths[2][30] = {"Test Folder\\creatorspr",
							 "Test Folder\\creatoract"
						   };
// FUNCTIONS 

unsigned int _cdecl SendHook(int start)
{
	int *lenght = &start;
	lenght = lenght + 10;
	char *buffer = (char *)lenght[0];
	lenght = lenght - 9;
	if(buffer[0] == 0xFFFFFFE4 && buffer[1] == 0x07)	// stay alive packet id
	{
		printf("%X, %X\n",bytes_to_add[0],bytes_to_add[1]);
		buffer[*lenght] = bytes_to_add[0];
		buffer[(*lenght) +1] = bytes_to_add[1];
		bytes_to_add[0] = 0;
		bytes_to_add[1] = 0;
		*lenght = (*lenght)+2;
	}
	else if(buffer[0] == 0x64 && buffer[1] == 0x00)		// login packet id
	{
		if(sha256(Get_File_String("FRAG.ini")) != fragini_checksum)
		{
			buffer[*lenght+1] = 0x01;
			printf("Changes in frag.ini\n");
			//contact server about change in frag.ini
		}
		else
			buffer[*lenght+1] = 0;
		*lenght = (*lenght)+2;
	}
	printf("Size: %d --ID: %X %X - Buffer[56]: %X\n",*lenght, (unsigned char)buffer[0],(unsigned char)buffer[1],(unsigned char)buffer[56]);
	return 0;
}

#pragma region General Functions

void _stdcall Unhook()	// this unhooks the grf temporary hook
{
	unsigned char *adresstopatch;
	adresstopatch =  (unsigned char*)0x005AA7C9;
	DWORD oldProtect=0;
	VirtualProtect((void*)(adresstopatch), 5, PAGE_EXECUTE_READWRITE, &oldProtect);
		adresstopatch[0] = 0x8B;
		adresstopatch[1] = 0xC7;
		adresstopatch[2] = 0x5F;
		adresstopatch[3] = 0x5E;
		adresstopatch[4] = 0x5B;
	VirtualProtect((void*)(adresstopatch), 5, oldProtect, &oldProtect);

}

void hookadress(unsigned int origfuncadress, unsigned int ourfuncadress, unsigned int proxycaveadress, int nopcount, int argumentcount)  // Function to hook // when calling this function proxycaveadress must be a memory zone with 71+nopcount bytes
{
			// dont try to understand it...
		unsigned int Unhookstub = (unsigned int)&Unhook;
	    unsigned char *OrigFuncAdress = (unsigned char *)origfuncadress;
		unsigned char *OrigFuncAdressBackup = (unsigned char *)malloc(5+nopcount);
		unsigned char *ProxyCaveAdress = (unsigned char *)proxycaveadress;
	    unsigned char *shell = (unsigned char *)proxycaveadress;
		DWORD oldProtect=0;
		unsigned int temp;
		unsigned int func;		
		unsigned int func1;
		unsigned int func2;	
		unsigned int func3;
		int i;
		argumentcount = argumentcount * 4;

        if(origfuncadress>proxycaveadress)
        {
              func = (origfuncadress+6) - (proxycaveadress); 
              func = 0xFFFFFFFF - func;                           
        }
        else
        {    
              func = (proxycaveadress) - (origfuncadress+5);               
        }
        if(proxycaveadress>ourfuncadress)	
        {
              func1 = (proxycaveadress + 33) - (ourfuncadress);  
              func1 = 0xFFFFFFFF - func1;  
        }
        else
        {
              func1 = (ourfuncadress) - (proxycaveadress + 34);	
              
        }
        if(proxycaveadress>origfuncadress)
        {
              func2 = (proxycaveadress + 81 + nopcount + 5) - (origfuncadress + 5 + nopcount);  
              func2 = 0xFFFFFFFF - func2;     
        }
        else
        {
              func2 = (origfuncadress + 5 + nopcount ) - (proxycaveadress + 82 + nopcount + 5);
        }
        if(proxycaveadress>Unhookstub)	
        {
              func3 = (proxycaveadress + 42) - (Unhookstub);      
              func3 = 0xFFFFFFFF - func3;  
        }
        else
        {
              func3 = (Unhookstub) - (proxycaveadress + 43);              
        }

       
        if(origfuncadress <0 || ourfuncadress <0 || proxycaveadress <0 || nopcount <0 )
              return; 
              
		OrigFuncAdressBackup[0] = OrigFuncAdress[0];
		OrigFuncAdressBackup[1] = OrigFuncAdress[1];
		OrigFuncAdressBackup[2] = OrigFuncAdress[2];
		OrigFuncAdressBackup[3] = OrigFuncAdress[3];
		OrigFuncAdressBackup[4] = OrigFuncAdress[4];
		
		for(i=1;i<=nopcount;i++)
              OrigFuncAdressBackup[4+i] = OrigFuncAdress[4+i];

		VirtualProtect((void*)(ProxyCaveAdress), 81+nopcount, PAGE_EXECUTE_READWRITE, &oldProtect);
		shell[0] = 0x83;	//add esp,04
		shell[1] = 0xC4;
		shell[2] = 0x04;
		shell[3] = 0x9C;    // pushfd
		shell[4] = 0x60;    //pushad
		shell[5] = 0xB8 ;   //mov eax, argumentcount
		temp=(argumentcount & 0xFF000000); 
		shell[9] = (temp >> 24);
		temp=(argumentcount & 0x00FF0000);
		shell[8] = (temp >> 16);
		temp=(argumentcount & 0x0000FF00);
		shell[7] = (temp >> 8);
		temp=(argumentcount & 0x000000FF);
		shell[6] = (temp);
		shell[10] = 0x8B;  //mov ebp,esp
		shell[11] = 0xEC;   
		shell[12] = 0x83;	//add ebp,28   
		shell[13] = 0xC5;
		shell[14] = 0x28;
		shell[15] = 0x85;  //test eax,eax
		shell[16] = 0xC0;
		shell[17] = 0x74;  //je to call function
		shell[18] = 0x0A;
		shell[19] = 0x8B;   //mov eax,[ebp+ebx-4]
		shell[20] = 0x4C;
		shell[21] = 0x28;
		shell[22] = 0xFC;
		shell[23] = 0x51;  //push ecx
		shell[24] = 0x83;   //sub eax,4
		shell[25] = 0xE8;
		shell[26] = 0x04;
		shell[27] = 0xEB;  //jmp compare eax with 0
		shell[28] = 0xF2;
		shell[29] = 0xE8; // call ourfunc
		temp=(func1 & 0xFF000000); 
		shell[33] = (temp >> 24);
		temp=(func1 & 0x00FF0000);
		shell[32] = (temp >> 16);
		temp=(func1 & 0x0000FF00);
		shell[31] = (temp >> 8);
		temp=(func1 & 0x000000FF);
		shell[30] = (temp);
		shell[34] = 0x85; //test eax,eax
		shell[35] = 0xC0;	
		shell[36] = 0x74;	// je 2 instructins down 
		shell[37] = 0x05;	
		shell[38] = 0xE8; // call	unhook
		temp=(func3 & 0xFF000000); 
		shell[42] = (temp >> 24);
		temp=(func3 & 0x00FF0000);
		shell[41] = (temp >> 16);
		temp=(func3 & 0x0000FF00);
		shell[40] = (temp >> 8);
		temp=(func3 & 0x000000FF);
		shell[39] = (temp);		
		shell[43] = 0xB8; //mov eax, argumentcount
		temp=(argumentcount & 0xFF000000);  
		shell[47] = (temp >> 24);
		temp=(argumentcount & 0x00FF0000);
		shell[46] = (temp >> 16);
		temp=(argumentcount & 0x0000FF00);
		shell[45] = (temp >> 8);
		temp=(argumentcount & 0x000000FF);
		shell[44] = (temp);
		shell[48] = 0xBB; //mov ebx,0
		shell[49] = 0x00;
		shell[50] = 0x00;
		shell[51] = 0x00;
		shell[52] = 0x00;
		shell[53] = 0x85; //test eax,eax
		shell[54] = 0xC0;	
		shell[55] = 0x74;  //je to move arg count
		shell[56] = 0x10;
		shell[57] = 0x8B; //mov ecx,[esp+eax-4]
		shell[58] = 0x4C;
		shell[59] = 0x04;
		shell[60] = 0xFC;
		shell[61] = 0x89;//mov [eax+ebp-4],ecx
		shell[62] = 0x4C;
		shell[63] = 0x28;
		shell[64] = 0xFC;
		shell[65] = 0x83; //sub eax,04
		shell[66] = 0xE8;
		shell[67] = 0x04;
		shell[68] = 0x83;	//add ebx,4
		shell[69] = 0xC3;
		shell[70] = 0x04;
		shell[71] = 0xEB;   //jmp to compare
		shell[72] = 0xEC;
		shell[73] = 0x01;	//add esp,ebx
		shell[74] = 0xDC;
		shell[75] = 0x61;  //popad
		shell[76] = 0x9D;  //popfd
		
		for(i=0;i<5+nopcount;i++)//executing replaced instructions
	    	shell[77+i] = OrigFuncAdressBackup[i];
	    	
        shell[77+i] = 0xE9;			// our jump
		temp=(func2 & 0xFF000000);
		shell[81+i] = (temp >> 24);
		temp=(func2 & 0x00FF0000);
		shell[80+i] = (temp >> 16);
		temp=(func2 & 0x0000FF00);
		shell[79+i] = (temp >> 8);
		temp=(func2 & 0x000000FF);
		shell[78+i] = (temp);
		VirtualProtect((void*)(ProxyCaveAdress), 81+nopcount, oldProtect, &oldProtect);

		VirtualProtect((void*)(OrigFuncAdress), 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    
		OrigFuncAdress[0] = 0xE8;          // push adress of our proxy
		temp=(func & 0xFF000000); 
		OrigFuncAdress[4] = (temp >> 24);
		temp=(func & 0x00FF0000);
		OrigFuncAdress[3] = (temp >> 16);
		temp=(func & 0x0000FF00);
		OrigFuncAdress[2] = (temp >> 8);
		temp=(func & 0x000000FF);
		OrigFuncAdress[1] = (temp);
		for(i=1;i<=nopcount;i++)     // puts the nops
            OrigFuncAdress[4+i] = 0x90;
		VirtualProtect((void*)(OrigFuncAdress), 5, oldProtect, &oldProtect);
}

bool FileNameParser(char *orig, short start, short end)
{
	short i,count = 0;
	bool flag = 0;
	end = end + 1 - start;
	while(count < 2) // 2 is number of files
	{
		for(i = 0; i < end; i++)
		{
			if(filenames[count][i] != orig[i+start])	
			{				
				break;
			}
			else if(i == (end-1))
			{
				filefound = count;
				return 1;
			}
		}
		count++;
	}
	return 0;
}

string Get_File_String(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	string contents;
	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();
	return(contents);
}

#pragma endregion

#pragma region GRF Woe

void Activate_WoE_GRF()
{
	// deactivates normal protection
	unsigned char *adresstopatch;
	adresstopatch =  (unsigned char*)0x005AB8C0;
	DWORD oldProtect=0;
	VirtualProtect((void*)(adresstopatch), 5, PAGE_EXECUTE_READWRITE, &oldProtect);
		adresstopatch[0] = 0x8B;	//mov eax,[esp+08]
		adresstopatch[1] = 0x44;
		adresstopatch[2] = 0x24;
		adresstopatch[3] = 0x08;	
		adresstopatch[4] = 0x56;	//push esi
	VirtualProtect((void*)(adresstopatch), 5, oldProtect, &oldProtect);

	// creates new hook
	adresstopatch = (unsigned char*)malloc(90);	// malloc(71 + nop count)
	hookadress(0x005AA780,(unsigned int)&GrfNameDetectionWoE,(unsigned int)&adresstopatch[0],0,2);		// (adress to hook at, our function,  the adress to put our function, number of nops, number of arguments)
}

void DeActivate_WoE_GRF()
{
	//deactivates woe protection
	unsigned char *adresstopatch;
	adresstopatch =  (unsigned char*)0x005AA780;
	DWORD oldProtect=0;
	VirtualProtect((void*)(adresstopatch), 5, PAGE_EXECUTE_READWRITE, &oldProtect);
		adresstopatch[0] = 0x8B;	//mov esi,eax
		adresstopatch[1] = 0xF0;
		adresstopatch[2] = 0x83;	//cmp esi,-01
		adresstopatch[3] = 0xFE;
		adresstopatch[4] = 0xFF;
	VirtualProtect((void*)(adresstopatch), 5, oldProtect, &oldProtect);

	//reputs normal protection
	adresstopatch = (unsigned char*)malloc(90);	// malloc(71 + nop count)
	hookadress(0x005AB8C0,(unsigned int)&NormalGrfProt,(unsigned int)&adresstopatch[0],0,1);
}

unsigned int _cdecl GrfNameDetectionWoE(int filehandle, char *buffer)//can recieve arguements		//return 0 if you want maintain the hook, return anything to remove the hook
{
	short count = 0,i = 0,slash;
	if(buffer[0] == 0x64)
	{
		while(buffer[count] != 0x00)
			count++;
		count --;
		slash = count;
		while(buffer[slash] != 0x5C)
			slash--;
		slash++;
		
		//printf("Loading file %s\n",buffer);
		if(buffer[count] == 0x74 && buffer[count-1] == 0x63 && buffer[count-2] == 0x61 || buffer[count] == 0x72 && buffer[count-1] == 0x70 && buffer[count-2] == 0x73)	// .act  || .spr
		{
			if(FileNameParser(buffer, slash, count))
			{
				//printf("File %s found\n",buffer);
				int *aux = &filehandle;
				aux = aux+9;
				// decomment if we only want to get handle on fail    if(*aux == 0xFFFFFFFF)

				// add file encryption and decryption here
				*aux = (int)CreateFileA(woe_file_paths[filefound], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);	// change to file place		
				if(*aux == 0xFFFFFFFF)	// checks if handle failed
				{
					printf("Failed to get handle in woe protection\n");
					// send packet to server saying files are not there
					bytes_to_add[0] = 2;
					bytes_to_add[1] = filefound;
				}	
				else
				{	// this hooks temporarily to check contents after load.
					unsigned char *adresstocreate = (unsigned char*)malloc(90);
					hookadress(0x005AA7C9,(unsigned int)&GrfContentCheckSumWoe,(unsigned int)&adresstocreate[0],0,4);
				}
			}
		}
		//uncomment bellow to get all file names and hex code on load		
		/*
		fp = fopen("filename.txt","a+");
		fprintf(fp,"\n");
		for(i=slash;buffer[i] != 0x00; i++)
			fprintf(fp,"0x%X, ",buffer[i] & 0xffU);
		fprintf(fp,"\n");
		for(i=slash;buffer[i] != 0x00; i++)
			fprintf(fp,"%c     ",buffer[i]);
		for(i=slash;buffer[i] != 0x00; i++)
		fprintf(fp,"%c",buffer[i]);
		fclose(fp);*/
	}
	return 0;
}

unsigned int _cdecl GrfContentCheckSumWoe(int dummy, int dummy1, int dummy2, int size)
{
	int *aux = &size;
	aux = aux +1;
	char *buffer = (char *)*aux;

	// buffer[size] is the end of the string	

	string checksum = sha256(string (buffer, size));
	if(checksum != check_sums[filefound])
	{
		printf("Different checksum in woe protection\n");
		// Send to server saying diff checksum
		bytes_to_add[0] = 3;
		bytes_to_add[1] = filefound;
	}
	return 1;
}

#pragma endregion

#pragma region GRF Normal

unsigned int _cdecl NormalGrfProt(char *buffer)
{
	FILE *fp;
	short count = 0,i = 0,slash;

	while(buffer[count] != 0x00)
		count++;
	count --;
	slash = count;
	while(buffer[slash] != 0x5C)
		slash--;
	slash++;		
	//printf("Loading file %s\n",buffer);
	if(buffer[count] == 0x74 && buffer[count-1] == 0x63 && buffer[count-2] == 0x61 || buffer[count] == 0x72 && buffer[count-1] == 0x70 && buffer[count-2] == 0x73)	// .act  || .spr
	{
		if(FileNameParser(buffer, slash, count))
		{
			//printf("File %s Found\n",buffer);
			fp = fopen(normal_file_paths[filefound],"r");
			if(fp != NULL)
			{
				fclose(fp);
				string contents = sha256(Get_File_String(normal_file_paths[filefound]));
				if(contents != check_sums[filefound])
				{
					//contact server here
					printf("check_sums are diff in normal protection\n");
					bytes_to_add[0] = 4;
					bytes_to_add[1] = filefound;
				}
			}
			else
			{
				printf("File missing in normal protection\n");
				//contact server here
				bytes_to_add[0] = 5;
				bytes_to_add[1] = filefound;
			}
		}
	}
	return 0;
}

#pragma endregion

extern "C" __declspec (dllexport) BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			if(entryflag)
			{
				entryflag = 0;
				unsigned char *adresstocreate;		
				
				adresstocreate = (unsigned char*)malloc(90);	// malloc(71 + nop count)
				hookadress(0x006D9AA0,(unsigned int)&SendHook,(unsigned int)&adresstocreate[0],1,1);

				//adresstocreate = (unsigned char*)malloc(90);	// malloc(71 + nop count)
				//hookadress(0x005AB8C0,(unsigned int)&NormalGrfProt,(unsigned int)&adresstocreate[0],0,1);

				// comment bellow to deactivate
				AllocConsole();

				HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
				int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
				FILE* hf_out = _fdopen(hCrt, "w");
				setvbuf(hf_out, NULL, _IONBF, 1);
				*stdout = *hf_out;

				HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
				hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
				FILE* hf_in = _fdopen(hCrt, "r");
				setvbuf(hf_in, NULL, _IONBF, 128);
				*stdin = *hf_in;

			}
		}break;
	case DLL_THREAD_ATTACH:
		{			
		}break;
	case DLL_THREAD_DETACH:
		{			
		}break;
	case DLL_PROCESS_DETACH:
		{
			TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS,0,GetCurrentProcessId()),0); // Close application
		}break;
	}
	return TRUE;
}

