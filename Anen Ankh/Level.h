#pragma once

#include "Labirynth.h"

//Klasa poziomu, dowolnego. Zak�adamy �e r�ni� si� tylko parametrami 
//z kt�rymi s� wywo�ywane w Game poszczeg�lne funkcje.
//Odpowiada za ca�e zarz�dzanie poziomem i 
//oddawanie do game np. informacji o zako�czeniu poziomu, �mierci itp.

class Level
{
public:
	Labirynth labirynth;

public:
	Level();
	~Level() {};
};

Level::Level()
{ 
	labirynth = *(new Labirynth()); 
}