#include "EM\\logs.h"
#include "EM\\Emerald_class.h"
using namespace miniwin;

int main(){
	
	// Initial Parameters:
	logs_active = false;
	show_colissions = true;
	windows_sound = 0;
	
	Start_EM();
	
	int Player = AddObject();
	AllObjects[Player]->start_object("Player", 1, 4, 1, 1, "images\\nave.png", "Player");
	AllObjects[Player]->change_HitBox(0.2f, 0.1f, 1.1f, 0.9f);
	
	while(true){
		chrono::system_clock::time_point start = chrono::system_clock::now();
		if (i_cache == 0 && j_cache == 0){
			espera(1);
			// color del fondo.
			color_rgb(0, 3, 22);
			rectangulo_lleno(0,0,vancho(),valto());
		}
		
		
		
		// Actualiza las imagenes en pantalla
		Update_EM4(start);
	}
	
	return 0;
}