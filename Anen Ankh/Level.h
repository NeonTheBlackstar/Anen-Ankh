#pragma once

//Klasa poziomu, dowolnego. Zak�adamy �e r�ni� si� tylko parametrami 
//z kt�rymi s� wywo�ywane w Game poszczeg�lne funkcje.
//Odpowiada za ca�e zarz�dzanie poziomem i 
//oddawanie do game np. informacji o zako�czeniu poziomu, �mierci itp.

class Level
{
public:
	Level() {};
	~Level() {};

	//Test
	void setTeapot();
};

void Level::setTeapot()
{
	glutWireTeapot(1);
}