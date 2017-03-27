#pragma once

#include "Labirynth.h"

//Klasa poziomu, dowolnego. Zak³adamy ¿e ró¿ni¹ siê tylko parametrami 
//z którymi s¹ wywo³ywane w Game poszczególne funkcje.
//Odpowiada za ca³e zarz¹dzanie poziomem i 
//oddawanie do game np. informacji o zakoñczeniu poziomu, œmierci itp.

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