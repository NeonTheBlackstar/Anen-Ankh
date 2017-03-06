#define _USE_MATH_DEFINE
#include <iostream>
#include "../glut.h"
#include <stdlib.h>
#include <cmath>

//Test

using namespace std;
GLdouble eyez = 0;
GLdouble eyex = 0;
GLdouble eyey = 0;

float angle = 0;
GLdouble centerY = 0, centerX = 0, centerZ = 0;
GLdouble worldX = 0, worldY = 0, worldZ = 0;
float xAngle = 0, yAngle = 0;

//Naciœniêcie przycisku myszki
int button_state = GLUT_UP;

//Po³o¿enie kursora myszki
int button_x, button_y;

void Display()
{
	//Przygotowanie bufora koloru (ustalenie koloru kosmosu + wyczeszczenie)
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Zaczêcie pracy od macierzy modelowania i przypisanie jej do macierzy jednostkowej.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//cout << angle << " " << eyex + sin(angle) << " " << eyez - cos(angle) << endl;

	//Umiejscowienie kamery - wspó¿êdne po³o¿enia kamery / punkt na który patrzy / wektor "w górê"
	//gluLookAt(eyex, eyey, eyez, eyex + sin(angle)*100, centery, eyez - cos(angle)*100, 0, 1, 0);
	//gluLookAt(eyex, eyey, eyez, eyex + sin(angle) * 100, centery, eyez - cos(angle) * 100, 0, 1, 0);

	glRotatef(-yAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(-xAngle, 0.0f, 1.0f, 0.0f);
	//glTranslatef(eyex + sin(xAngle), eyey - cos(yAngle), eyez);

	//Ustalenie koloru czajnika i stworzenie czajnika
	glColor3f(0.6, 0.2, 0.4);
	glTranslated(0, 0, -0.5);
	glutWireTeapot(0.5);

	//Wykonanie poleceñ
	glFlush();

	//Prze³adowanie bufora kolorów
	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	//Ustalenie wielkoœci wyœwietlania na ca³e okno
	glViewport(0, 0, width, height);

	// Przestawienie macierzy na macierz rzutowania
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Dopasowanie perspektywy. Wybieramy perspektywiczne, bo jest bardziej zajebiste.
	GLdouble aspect = (GLdouble)width / (GLdouble)height;
	gluPerspective(50, aspect, 0.1, 100.0);

	Display();
}

void specialKeys(int key, int x, int y) // CHUJ NIE DZIA£A :(
{
	if (key == GLUT_KEY_DOWN) {
		cout << "UP!" << endl;
		eyez -= 0.1;
	}
	else if (key == GLUT_KEY_UP) {
		eyez += 0.1;
	}
	//gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);

	if (key == GLUT_KEY_RIGHT) {
		eyex += 0.1;
	}
	else if (key == GLUT_KEY_LEFT)
	{
		eyex -= 0.1;
	}

	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			button_state = GLUT_DOWN;

			button_x = x;
			button_y = y;
		}
	}
}

void MouseMotion(int x, int y)
{
	if (button_state == GLUT_DOWN)
	{
		xAngle -= (x - button_x);
		button_x = x;
		yAngle -= y - button_y;
		button_y = y;

		/*
		//przeniesienie o x
		angle -= (x - button_x);
		if (angle < 0)
		angle = 360 + angle;
		if (angle > 360)
		angle = angle - 360;
		button_x = x;
		angle = (angle * 3.1425) / 180;
		//przeniesienie o y
		centery -= y - button_y;
		button_y = y;
		//gluLookAt(eyex, eyey, eyez, eyex + sin(angle) * 100, centery, eyez - cos(angle) * 100, 0, 1, 0);
		*/
	}
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// Pocz¹tkowe ustawienia sceny
void init()
{
	// Umiejscowienie kamery
	gluLookAt(0, 0, 0, 0, 0, -100, 0, 1, 0);
}

int main(int argc, char * argv[])
{
	// inicjalizacja gluta
	glutInit(&argc, argv);

	// inicjalizacja bufora ramki - RGB i podwójny bufor
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	//Stworzenie okna
	glutInitWindowSize(500, 500);
	glutCreateWindow("");
	//glutFullScreen();

	//Funkcje wywo³uj¹ce funkcje
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	//Obs³uga klawiatury
	glutSpecialFunc(specialKeys);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);

	init();

	//Stworzenie glutopêtli
	glutMainLoop();

	return 0;
}