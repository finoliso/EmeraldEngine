#include "miniwin.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <vector>
#include <algorithm>
#include "logs.h"
using namespace miniwin;
using namespace std;

struct GEdata{
	vector<int> x, y;
	vector<int> r, g, b;
	string sprite_path;
}GED[100];

struct FEdata{
	string name;
	struct FEUdata{
		int width, high;
		char sign;
		vector<int> x, y;
	}FEUD[100];
}FED[10];
// solo se pueden tener 10 fonts

struct text_cache{
	string text;
	vector<int> letters;
}text_cache[10];
int text_cache_point_replace = 0;

void Plot_Points(float OBJx, float OBJy, float Pointx, float Pointy, float scale_x, float scale_y, int rgb[3], bool static_mode = false);
void graph_object(float OBJx, float OBJy, float ScaleX, float ScaleY, int IdGED, bool static_mode = false);
//void graph_text(string text, float OBJx, float OBJy, float ScaleX, float ScaleY, string font_name, int color[3], bool static_mode = false);
void graph_letter(int font, int letter, float OBJx, float OBJy, float ScaleX, float ScaleY, int color[3]);
int find_letter(int font, char letter);
bool Mause_position(float& X, float& Y);
void Coordinates_to_points(float &x1, float &y1);
void Points_to_coordinates(float &x1, float &y1);
void Add_text_cache(vector<int> vec, string text);

float camara_x = 0, camara_y = 0;
float size = 5; // en este caso un cuadrado que mida 1X1 mide 50X50
float unidad_basica = 10 * size;

void Plot_Points(float OBJx, float OBJy, float Pointx, float Pointy, float scale_x, float scale_y, int rgb[3], bool static_mode){ 
	// dibuja cada punto de la estrura GEdata adaptando la escala y el size de la camara;
	float UBEs_x, UBEs_y;
	if (!static_mode){
		UBEs_x = unidad_basica * scale_x * 0.2f; //ej: 50*0,1f*0,2f=1px
		UBEs_y = unidad_basica * scale_y * 0.2f; // es la relacion de la escala con la unidad basica.
	}else{
		UBEs_x = scale_x * 10;
		UBEs_y = scale_y * 10;
	}
	
	float x_paint = OBJx + (Pointx * UBEs_x);
	float y_paint = OBJy + (Pointy * UBEs_y);
	float x_paint2 = OBJx + (Pointx+1) * UBEs_x;
	float y_paint2 = OBJy + (Pointy+1) * UBEs_y;
	
	color_rgb(rgb[0], rgb[1], rgb[2]);
	rectangulo_lleno(x_paint, y_paint, x_paint2, y_paint2);
}

void graph_object(float OBJx, float OBJy, float ScaleX, float ScaleY, int IdGED, bool static_mode){
	// 0,1f * 0,1f de escala es un pixel por un pixel. (mientras no cambies los valores de size camara)
	float objeto_cache_x, objeto_cache_y;
	if (!static_mode){
		objeto_cache_x = (OBJx - camara_x) * unidad_basica;
		objeto_cache_y = (OBJy - camara_y) * unidad_basica;
	}else{
		objeto_cache_x = OBJx * 50;
		objeto_cache_y = OBJy * 50;
	}
	
	
	for(int i = 0; i < GED[IdGED].x.size(); i++){
		int colors[3] = {GED[IdGED].r[i], GED[IdGED].g[i], GED[IdGED].b[i]};
		Plot_Points(objeto_cache_x, objeto_cache_y, GED[IdGED].x[i], GED[IdGED].y[i], ScaleX, 
		ScaleY, colors, static_mode);
	}
}

void graph_text(string text, float OBJx, float OBJy, float ScaleX, float ScaleY, string font_name, int color[3], bool static_mode){
	float UBEs_x = unidad_basica * ScaleX * 0.2f;
	float UBEs_y = unidad_basica * ScaleY * 0.2f;
	
	int font = 0;
	for(int i = 0; i < 10; i++){
		if(FED[i].name == font_name){
			font = i; break;
		}
	}
	float x1 = 0, y1 = 0;
	int letter_distance = find_letter(font, '~');    //
	
	bool initial_mode_static = static_mode;
	if (static_mode){
		int text_cache_point = 256;
		for (int i = 0; i < 10; i++){
			if(text_cache[i].text == text){
				text_cache_point = i;
			}
		}
		
		if(text_cache_point == 256){
			static_mode = false;
		}else{
			for(int i = 0; i < text_cache[text_cache_point].letters.size(); i++){
				int letter = text_cache[text_cache_point].letters[i];
				graph_letter(font, letter, OBJx + x1, OBJy + y1, ScaleX, ScaleY, color);
				float p1 = UBEs_x * (FED[font].FEUD[letter_distance].width + FED[font].FEUD[letter].width);
				float p2 = (p1 / unidad_basica);
				x1 += p2;
			}
		}
		
	}if (!static_mode){
		int text_size = text.length();
		char text_char[text_size + 1];
		strcpy(text_char, text.c_str());
		vector<int> letter_vec;
		
		for(int i = 0; i < text_size; i++){
			int letter = find_letter(font, text_char[i]);
			letter_vec.push_back(letter);
			graph_letter(font, letter, OBJx + x1, OBJy + y1, ScaleX, ScaleY, color);
			float p1 = UBEs_x * (FED[font].FEUD[letter_distance].width + FED[font].FEUD[letter].width);
			float p2 = (p1 / unidad_basica);
			x1 += p2;
		}
		
		if(initial_mode_static){
			Add_text_cache(letter_vec, text);
		}
	}
}

void graph_letter(int font, int letter, float OBJx, float OBJy, float ScaleX, float ScaleY, int color[3]){
	float objeto_cache_x = (OBJx - camara_x) * unidad_basica;
	float objeto_cache_y = (OBJy - camara_y) * unidad_basica;
	
	for(int i = 0; i < FED[font].FEUD[letter].y.size(); i++){
		Plot_Points(objeto_cache_x, objeto_cache_y, FED[font].FEUD[letter].x[i],
		FED[font].FEUD[letter].y[i], ScaleX, ScaleY, color);
	}
}

int find_letter(int font, char letter){
	//write_log("busca", "Camera_settings");
	for (int j = 0; FED[font].FEUD[j].sign != 0; j++){
		if(FED[font].FEUD[j].sign == letter){
			return j;
		}
	}
	return 0;
}

bool Mause_position(float& X, float& Y){
	float mause_cache_x = (raton_x() / unidad_basica) + camara_x;
	float mause_cache_y = (raton_y() / unidad_basica) + camara_y;
	
	X = mause_cache_x;
	Y = mause_cache_y;
	
	return raton_dentro();
}

void Coordinates_to_points(float &x1, float &y1){
	x1 = (x1-camara_x)*unidad_basica;
	y1 = (y1-camara_y)*unidad_basica;
}

void Points_to_coordinates(float &x1, float &y1){
	x1 = (x1 / unidad_basica) + camara_x;
	y1 = (y1 / unidad_basica) + camara_y;
}

void Add_text_cache(vector<int> vec, string text){
	text_cache[text_cache_point_replace].letters = vec;
	text_cache[text_cache_point_replace].text = text;
	text_cache_point_replace++;
	
	if(text_cache_point_replace >= 10)
		text_cache_point_replace = 0;
	else
		text_cache_point_replace++;
}