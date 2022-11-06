#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

int main(){
	
	system("title EMP");
	system("mode con: cols=15 lines=1");
	
	string command = "title ";
	string name = "";
	cout << ">> ";
	cin >> name;
	command += name;
	system(command.c_str());
	
	while(true){
		system ("cls");
		string sound = "";
		cout << ">> ";
		cin >> sound;
		if (sound == "con"){
			sndPlaySound(0, 0);
		}else{
			sound += ".wav";
			sndPlaySound(sound.c_str(), SND_ASYNC);
		}
	}
	
	getch();
	return 0;
}