#include <iostream>
#include <windows.h>
#include "miniwin.h"
#include "logs.h"
using namespace std;

void SendSignal(string windows1, string message);
void initializeSpeekers(string WN);
int PlaySoundEM(string file_name, float time);
void ChangeSpeakers(int id, bool func1, string path1);

struct speakers{
	string windows_name;
	string path_sound;
	bool func;
}Speakers[10];
int iterator_speak = 0;

void initializeSpeekers(string WN){
	HWND wind = FindWindow(0, WN.c_str());
	if (!wind){
		system ("start sounds\\parlante.exe");
		Sleep(300);
		SendSignal("EMP", WN);
	}
	Speakers[iterator_speak].windows_name = WN;
	ChangeSpeakers(iterator_speak, false, "");
	iterator_speak++;
}

void SendSignal(string windows1, string message){
	HWND wind = FindWindow(0, windows1.c_str());
	int length1 = message.length();
	char cosa[length1 + 1];
	strcpy(cosa, message.c_str());
	for (int i = 0; i < length1; i++){
		PostMessage(wind,WM_KEYDOWN,int(toupper(cosa[i])),1);
	}
	PostMessage(wind,WM_KEYDOWN,0x0D,1);
}

int PlaySoundEM(string file_name, float time){
	for (int i = 0; i < iterator_speak; i++){
		if (!Speakers[i].func){
			SendSignal(Speakers[i].windows_name, file_name);
			ChangeSpeakers(i, true, file_name);
			return i;
		}
	}
	return iterator_speak;
}

void BreakSoundEM(int Id){
	SendSignal(Speakers[Id].windows_name, "con");
	ChangeSpeakers(Id, false, "");
}

void ChangeSpeakers(int id, bool func1, string path1){
	Speakers[id].func = func1;
	Speakers[id].path_sound = path1;
}