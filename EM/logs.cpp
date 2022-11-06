#include "logs.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
using namespace std;

string log_name = "";

bool create_log(){
	ofstream archivo; bool func = true;
	
	// nombre con fecha
	char tt[100];
    time_t now = time(nullptr);
    auto tm_info = localtime(&now);
    strftime(tt, 100, "logs\\%d-%m-%Y %H-%M-%S.txt", tm_info);
	
	archivo.open(tt, ios::out);
	
	if(archivo.fail()){
		func = false;
	}
	archivo.close();
	
	for (int i = 0; i < 35; i++){
		log_name += tt[i];
	}
	return func;
}

bool write_log(string info, string script_name){
	ofstream archivo; bool func = true;
	archivo.open(log_name.c_str(), ios::app);
	if (archivo.fail()){
		func = false;
	}
	
	// nombre con fecha
	char tt[100];
    time_t now = time(nullptr);
    auto tm_info = localtime(&now);
    strftime(tt, 100, "[%H:%M:%S] ", tm_info);
	
	string row = "";
	for (int i = 0; i < 11; i++){
		row += tt[i];
	}
	
	row += script_name + ": " + info;
	
	archivo << row << endl;
	archivo.close();
	return func;
}