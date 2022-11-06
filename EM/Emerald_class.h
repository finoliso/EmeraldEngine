// Emerald Version 4
// Create by Gael Etchebest

#include "miniwin.h"
#include "Camera_Settings.h"
#include "Archive_Settings.h"
#include "Sound_Settings.h"
#include "logs.h"
#include <conio.h>
#include <ctime>
#include <chrono>
#include <windows.h>
using namespace miniwin;

bool logs_active = false;
bool show_colissions = false;
int windows_sound = 2;
float deltaTime = 0;

vector<string> PathImage_list;
vector<string> PathFont_list;

class object;
class text;
class object_sound;

object *AllObjects[100];
int iterator_AllObj = 0;
text *AllTexts[20];
int iterator_AllTxt = 0;
object_sound *AllObjSou[30];
int iterator_AllObjSou = 0;

int AddObject();
int AddText();
int AddLayer(string name);

int FindObject(string name);
int FindLayer(string name);
bool FindTag(vector<int> objs, string tag1);

void upload_images();
void upload_fonts();
void import_fonts();
void import_images();

struct Layers{
	string layer_name;
	vector<int> layers_objects;
	int points;
}Layers[10];
int iterator_AllLyr = 0;

class object{
	public:
		float X = 0, Y = 0, ScaleX = 0.1f, ScaleY = 0.1f, Rotation = 0;
		float XminCollision = 0, YminCollision = 0, XmaxCollision = 0, YmaxCollision = 0;
		string name;
		int GED_id = 0;
		string sprite_path = "";
		string Layer_name;
		int Layer_number;
		bool static_mode = false;
		string tag = "";
		
		void start_object(string name1, float X1, float Y1, float ScaleX1, float ScaleY1, string sprite_path1, string tag1, string layer1 = ""){
			name = name1;
			tag = tag1;
			X = X1; Y = Y1;
			ScaleX = ScaleX1; ScaleY = ScaleY1;
			change_sprite(sprite_path1);
			if (layer1 == ""){
				Layer_number = 0;
				Layer_name = Layers[0].layer_name;
				Layers[0].layers_objects.push_back(FindObject(name));
			}else{
				Layer_number = FindLayer(layer1);
				Layer_name = Layers[Layer_number].layer_name;
				Layers[Layer_number].layers_objects.push_back(FindObject(name));
			}
		}
		
		void change_HitBox(float x1, float y1, float x2, float y2){
			XminCollision = x1;
			YminCollision = y1;
			XmaxCollision = x2;
			YmaxCollision = y2;
		}
		
		void object_update(){
			if (ScaleX != 0 && ScaleY != 0){
				graph_object(X, Y, ScaleX, ScaleY, GED_id, static_mode);
			}
			if(static_mode){
				//XminCollision = -0.1f * size + 0.5f;
				//YminCollision = -0.1f * size + 0.5f;
				//XmaxCollision = -0.3333f * size + 3.6666f;
				//YmaxCollision = -0.3333f * size + 3.6666f;
			}
		}
		
		bool change_sprite(string path){
			bool func = false;
			for (int i = 0; i < PathImage_list.size(); i++){
				if(path == GED[i].sprite_path){
					sprite_path = path;
					GED_id = i;
					func = true;
				}
			}
			return func;
		}
		
		bool on_collision(vector<int> &objs){
			bool colission = false;
			for (int i = 0; i < iterator_AllObj; i++){
				if (AllObjects[i]->name != name){
					float Xmin, Xmax, Ymin, Ymax;
					float XminOther, XmaxOther, YminOther, YmaxOther;
					if(!static_mode){
						Xmin = XminCollision + X;
						Xmax = XmaxCollision + X;
						Ymin = YminCollision + Y;
						Ymax = YmaxCollision + Y;
						
						XminOther = AllObjects[i]->XminCollision + AllObjects[i]->X;
						XmaxOther = AllObjects[i]->XmaxCollision + AllObjects[i]->X;
						YminOther = AllObjects[i]->YminCollision + AllObjects[i]->Y;
						YmaxOther = AllObjects[i]->YmaxCollision + AllObjects[i]->Y;
					}else{
						// (size*original)/5=x
						Xmin = XminCollision * size / 5 + X;
						Xmax = XmaxCollision * size / 5 + X;
						Ymin = YminCollision * size / 5 + Y;
						Ymax = YmaxCollision * size / 5 + Y;
						
						XminOther = AllObjects[i]->XminCollision * size / 5 + AllObjects[i]->X;
						XmaxOther = AllObjects[i]->XmaxCollision * size / 5 + AllObjects[i]->X;
						YminOther = AllObjects[i]->YminCollision * size / 5 + AllObjects[i]->Y;
						YmaxOther = AllObjects[i]->YmaxCollision * size / 5 + AllObjects[i]->Y;
					}
					
					
					int tipo_colision = 0;
					if (tipo_colision == 0){
						if ((XminOther >= Xmin && XminOther <= Xmax) || (XmaxOther >= Xmin && XmaxOther <= Xmax)){
							if ((YminOther >= Ymin && YminOther <= Ymax) || (YmaxOther >= Ymin && YmaxOther <= Ymax)){
								objs.push_back(i);
								tipo_colision = 1;
							}
						}
					}if(tipo_colision == 0){
						if ((Xmin >= XminOther && Xmin <= XmaxOther) || (Xmax >= XminOther && Xmax <= XmaxOther)){
							if ((Ymin >= YminOther && Ymin <= YmaxOther) || (Ymax >= YminOther && Ymax <= YmaxOther)){
								objs.push_back(i);
								tipo_colision = 2;
							}
						}
					}
				} 
			}
			if(objs.size() != 0){
				colission = true;
			}
			return colission;
		}
		
		void show_box_colission(){
			if (!(XminCollision == 0 && YminCollision == 0 && XmaxCollision == 0 && YmaxCollision == 0)){
				vector<int> show_box_colission_cache1;
				if(this->on_collision(show_box_colission_cache1)){
					color(ROJO);
				}else{
					color(VERDE);
				}
	
				float x1 = X + XminCollision; float y1 = Y + YminCollision;
				float x2 = X + XmaxCollision; float y2 = Y + YmaxCollision;
				
				Coordinates_to_points(x1, y1);
				Coordinates_to_points(x2, y2);
	
				rectangulo(x1, y1, x2, y2);
			}
		}
		
};

class text{
	public:
		float X = 0, Y = 0;
		float ScaleX = 1, ScaleY = 1;
		string text;
		string font;
		int r = 0, g = 0, b = 0;
		
		void text_start(string text1,float x1,float y1,float ScaleX1, float ScaleY1){
			X = x1; Y = y1;
			ScaleX = ScaleX1;
			ScaleY = ScaleY1;
			text = text1;
			font = PathFont_list[0];
		}
		
		void change_color(int r1, int g1, int b1){
			r = r1; g = g1; b = b1;
		}
		
		void change_text(string text1){
			this->text = text1;
		}
		
		void text_update(){
			int rgb1[3] = {r, g, b};
			graph_text(text, X, Y, ScaleX, ScaleY, font, rgb1, false);
		}
};

class object_sound{
	public:
		string name;
		string sound = "";
		float duration_time = 0;
		float goes_time = 0;
		bool in_process = false;
		bool haveId = false;
		int IdSpeakers = 0;
		
		bool Play(string sound1, float duration_time1){
			if (!in_process){
				in_process = true;
				sound = sound1;
				duration_time = duration_time1;
				goes_time = 0;
				haveId = true;
				IdSpeakers = PlaySoundEM(sound, duration_time);
				if (IdSpeakers >= iterator_speak){
					Break();
					return false;
				}
				return true;
			}else
				return false;
		}
		
		void UpdateS(){
			if (this->in_process){
				if (goes_time >= duration_time){
					Break();
				}
				goes_time += deltaTime;
			}
		}
		
		void Break(){
			in_process = false;
			sound = "";
			duration_time = 0;
			goes_time = 0;
			if (haveId){
				BreakSoundEM(IdSpeakers);
			}
			haveId = false;
			IdSpeakers = 0;
		}
};

void Start_EM(){
	
	for (int i = 0; i < windows_sound; i++){
		initializeSpeekers("nada" + to_string(i));
	}
	
	vredimensiona(600,500);
	// Importar las Imagenes:
	PathImage_list.push_back("images\\bird.png");
	PathImage_list.push_back("images\\asteroide.png");
	PathImage_list.push_back("images\\laser.png");
	PathImage_list.push_back("images\\nave.png");
	PathImage_list.push_back("images\\estrellas.png");
	
	// Importar las Fuentes:
	PathFont_list.push_back("fonts\\normal.fe");
	
	// Añadir Layers:
	AddLayer("Default");
	
	if(logs_active){
		create_log();
		write_log("Inicio de log", "Emerald_Class");
		write_log("Emerald Log System", "Emerald_Class");
	}
	
	import_fonts();
	upload_fonts();
	import_images();
	upload_images();
	
}

int i_cache = 0, j_cache = 0;

int cont_Updates = 0;

void Update_EM4(chrono::system_clock::time_point start_time){
	
	unidad_basica = 10 * size;
	
	for (int i = 0; i < iterator_AllObjSou; i++){
		AllObjSou[i]->UpdateS();
	}
	
	int realizaciones = 0;
	bool salida = false;
	for (int i = i_cache; i < iterator_AllLyr + 1; i++){
		if (i == iterator_AllLyr){
			for (int j = j_cache; j < iterator_AllTxt; j++){
				if (1 + realizaciones > 2){
					i_cache = i; j_cache = j;
					salida = true;
					break;
				}
				AllTexts[j]->text_update();
			}
		}else{
			for (int j = j_cache; j < Layers[i].layers_objects.size(); j++){
				if (Layers[i].points + realizaciones > 3){
					i_cache = i; j_cache = j;
					salida = true;
					break;
				}
				AllObjects[Layers[i].layers_objects[j]]->object_update();
				if(show_colissions)
					AllObjects[Layers[i].layers_objects[j]]->show_box_colission();
			}
		}
		
		if (salida)
			break;
		else{
			i_cache = 0; j_cache = 0;
		}
	}
	
	if (!salida){
		refresca();
	}
	
	
	chrono::system_clock::time_point end = chrono::system_clock::now();
	chrono::duration<float,std::milli> duration = end - start_time;
	color_rgb(255, 255, 255);
	deltaTime = duration.count() / 1000;
	
}

// object system list functions

int AddObject(){
	AllObjects[iterator_AllObj] = new object();
	iterator_AllObj++;
	return iterator_AllObj - 1;
}

int FindObject (string name){
	for (int i = 0; i < iterator_AllObj; i++){
		if (AllObjects[i]->name == name){
			return i;
		}
	}
	return iterator_AllObj + 100;
}

int AddText(){
	AllTexts[iterator_AllTxt] = new text();
	iterator_AllTxt++;
	return iterator_AllTxt - 1;
}

int AddLayer(string name){
	Layers[iterator_AllLyr].layer_name = name;
	Layers[iterator_AllLyr].points = 1;
	iterator_AllLyr++;
	return iterator_AllLyr - 1;
}

int FindLayer(string name){
	for (int i = 0; i < iterator_AllLyr; i++){
		if (Layers[i].layer_name == name){
			return i;
		}
	}
	return iterator_AllLyr + 100;
}

bool FindTag(vector<int> objs, string tag1){
	for (int i = 0; i < objs.size(); i++){
		if (AllObjects[objs[i]]->tag == tag1){
			return true;
		}
	}
	return false;
}

int AddObjectSound(string name1){
	AllObjSou[iterator_AllObjSou] = new object_sound();
	AllObjSou[iterator_AllObjSou]->name = name1;
	iterator_AllObjSou++;
	return iterator_AllObjSou - 1;
}

int FindObjectSound(string name1){
	for (int i = 0; i < iterator_AllObjSou; i++){
		if (AllObjSou[i]->name == name1){
			return i;
		}
	}
	return iterator_AllObjSou + 100;
}

// import and upload functions

void import_images(){
	for (int i = 0; i < PathImage_list.size(); i++){
		/*vector<char> datos_char;
		if(read_archives(PathImage_list[i], datos_char)){
			if(!comprehension_GE(datos_char, i, PathImage_list[i])){
				write_log("No se ha podido leer el archivo \"" + PathImage_list[i] + "\"", "Emerald_class");
			}
		}*/
		
		vector<unsigned char> trol;
		int w1, h1;
		if(read_PNG(PathImage_list[i], trol, w1, h1)){
			if(comprehension_PNG(trol, i, PathImage_list[i], w1, h1)){
				
			}
		}
	}
}

void import_fonts(){
	int cont1 = 0;
	for (int i = 0; i < PathFont_list.size(); i++){
		vector<char> datos_char;
		vector<string> paths1;
		vector<char> s1;
		string name;
		
		if(read_archives(PathFont_list[i], datos_char) && comprehension_FED(datos_char, name, paths1, s1)){
			for (int j = 0; j < paths1.size(); j++){
				vector<unsigned char> datos2_img;
				int w1, h1;
				if(read_PNG(paths1[j], datos2_img, w1, h1) && comprehension_Letters(datos2_img, cont1, paths1[j], name, s1[j], w1, h1)){
					cont1++;
				}
				
			}
		}
	}
}

void upload_fonts(){
	int no_uploads = 0;
	int cont_FEUDC = 0, cont_FED = -1, cont_FEUD = 0;
	while(no_uploads <= 5){
		if(FEUD_copy[cont_FEUDC].use){
			no_uploads = 0;
			if(FEUD_copy[cont_FEUDC].fontType == FED[cont_FED].name){
				FED[cont_FED].FEUD[cont_FEUD].high = FEUD_copy[cont_FEUDC].high;
				FED[cont_FED].FEUD[cont_FEUD].width = FEUD_copy[cont_FEUDC].width;
				FED[cont_FED].FEUD[cont_FEUD].sign = FEUD_copy[cont_FEUDC].sign;
				FED[cont_FED].FEUD[cont_FEUD].x = FEUD_copy[cont_FEUDC].x;
				FED[cont_FED].FEUD[cont_FEUD].y = FEUD_copy[cont_FEUDC].y;
				cont_FEUDC++; cont_FEUD++;
			}else{
				cont_FED++;
				cont_FEUD = 0;
				FED[cont_FED].name = FEUD_copy[cont_FEUDC].fontType;
			}
		}else{
			cont_FEUDC++;
			no_uploads++;
		}
	}
}

void upload_images(){
	int cont_Ged = 0, cont_CGed = 0;
	int no_uploads = 0;
	// estara buscando y cargando las imagenes de GED_copy hasta que en algun momento no logra cargar mas de 5 seguidas.
	while(no_uploads <= 5){
		if (GED_copy[cont_CGed].use){
			GED[cont_Ged].x = GED_copy[cont_CGed].x;
			GED[cont_Ged].y = GED_copy[cont_CGed].y;
			GED[cont_Ged].r = GED_copy[cont_CGed].r;
			GED[cont_Ged].g = GED_copy[cont_CGed].g;
			GED[cont_Ged].b = GED_copy[cont_CGed].b;
			GED[cont_Ged].sprite_path = GED_copy[cont_CGed].sprite_path;
			write_log("La imagen \"" + GED_copy[cont_CGed].sprite_path + "\" a sido iniciada correctamente", "Emerald_Class");
			no_uploads = 0; cont_Ged++;
		}else{
			no_uploads++;
		}
		cont_CGed++;
	}
}

bool KeyPress(char l){
	int n1 = int(toupper(l));
	if (GetAsyncKeyState(n1))
		return true;
	else
		return false;
}
