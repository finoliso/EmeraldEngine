#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "logs.h"
#include "lodepng.h"
using namespace std;

bool is_number(char n);
bool comprehension_PNG(vector<unsigned char> image, int IdGed, string sprite_path, int w, int h);
bool read_PNG(string path, vector<unsigned char>& image, int& w1, int& h1);
bool comprehension_row_GE(vector<char> input, vector<int>& xyrgb);
bool read_archives(string path, vector<char>& output);

struct GEdata_copy{
	vector<int> x, y;
	vector<int> r, g, b;
	string sprite_path;
	bool use;
}GED_copy[100];

struct FEUdata_copy{
	vector<int> x, y;
	char sign;
	int width, high;
	string fontType;
	bool use;
}FEUD_copy[100];


bool read_archives(string path, vector<char>& output){
	// lee los archivos y saca un vector de tipo char con todo lo almacenado en ellos.
	FILE *scr;
	bool Func = false;
	char path_char[path.length() + 1];
	strcpy(path_char, path.c_str());
	scr = fopen(path_char, "r");
	if(scr == NULL){
		Func = false;
	}else{
		Func = true;
		while(!feof(scr)){
			output.push_back(fgetc(scr));
		}
	}
	fclose(scr);
	return Func;
}

bool read_PNG(string path, vector<unsigned char>& image, int& w1, int& h1){
	vector<unsigned char> buffer;
	
	unsigned w, h;
	
	lodepng::load_file(buffer, path);
	lodepng::State state;
	unsigned error = lodepng::decode(image, w, h, state, buffer);
	
	w1 = w; h1 = h;
	
	if(error)
		return false;
	else
		return true;
}

bool comprehension_PNG(vector<unsigned char> image, int IdGed, string sprite_path, int w, int h){
	GED_copy[IdGed].use = true;
	GED_copy[IdGed].sprite_path = sprite_path;
	int cont1 = 0;
	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			if(image[cont1+3] == 255){
				GED_copy[IdGed].x.push_back(x);
				GED_copy[IdGed].y.push_back(y);
				GED_copy[IdGed].r.push_back(image[cont1]);
				GED_copy[IdGed].g.push_back(image[cont1 + 1]);
				GED_copy[IdGed].b.push_back(image[cont1 + 2]);
			}
			cont1+=4;
		}
		//cont1+=4;
	}
	return true;
}

bool comprehension_FED(vector<char> input, string& name, vector<string>& paths, vector<char>& s1){
	string fases = "name";
	bool func = true;
	int cont1 = 0;
	string dato1 = "";
	char dato3;
	
	for(int i = 0; i < input.size(); i++){
		if(fases == "name"){
			
			if(input[i] == '='){
				if(input[i-1] == 'e' && input[i-2] == 'm' && input[i-3] == 'a' && input[i-4] == 'n'){
					fases = "lec_name";
				}else
					func = false;
			}
			
		}else if(fases == "lec_name"){
			
			if(input[i] == '\"'){
				cont1++;
			}else if(cont1 == 1){
				dato1 += input[i];
			}else if(cont1 == 2){
				name = dato1;
				dato1 = ""; cont1 = 0;
				fases = "sign";
			}
			
		}else if (fases == "sign"){
			if(input[i] == '='){
				if (input[i-1] == 'n' && input[i-2] == 'g' && input[i-3] == 'i' && input[i-4] == 's'){
					fases = "lec_sign";
				}else{
					func = false;
				}
			}
		}else if(fases == "lec_sign"){
			if(input[i] == '\''){
				cont1++;
			}else if (cont1 == 1){
				dato3 = input[i];
			}else if (cont1 == 2){
				s1.push_back(dato3);
				dato3 = ' '; cont1 = 0;
				fases = "paths";
			}
		}else if(fases == "paths"){
			
			if(input[i] == '='){
				if(input[i-1] == 'h' && input[i-2] == 't' && input[i-3] == 'a' && input[i-4] == 'p'){
					
					fases = "lec_paths";
				}else
					func = false;
			}
			
		}else if(fases == "lec_paths"){
			
			if(input[i] == '\"'){
				cont1++;
			}else if(cont1 == 1){
				dato1 += input[i];
			}else if(cont1 == 2){
				paths.push_back(dato1);
				dato1 = ""; cont1 = 0;
				fases = "sign";
			}
			
		}
		
		if(!func){
			break;
		}
	}
	return func;
}

bool comprehension_Letters(vector<unsigned char> image, int IdFeud, string path, string font_name, char sign, int w1, int h1){
	FEUD_copy[IdFeud].use = true;
	FEUD_copy[IdFeud].fontType = font_name;
	FEUD_copy[IdFeud].width = w1,
	FEUD_copy[IdFeud].high = h1;
	FEUD_copy[IdFeud].sign = sign;
	int cont1 = 0;
	for (int y = 0; y < h1; y++){
		for (int x = 0; x < w1; x++){
			if(image[cont1+3] == 255){
				FEUD_copy[IdFeud].x.push_back(x);
				FEUD_copy[IdFeud].y.push_back(y);
			}
			cont1+=4;
		}
	}
	return true;
}

bool is_number(char n){
	if(n == '1' || n == '2' || n == '3' || n == '4' || n == '5' || n == '6' || n == '7'
	|| n == '8' || n == '9' || n == '0'){
		return true;
	}else{
		return false;
	}
}
