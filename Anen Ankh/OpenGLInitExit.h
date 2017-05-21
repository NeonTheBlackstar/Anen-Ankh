#pragma once

#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

//Procedura obs�ugi b��d�w - poniewa� jest �ci�le opisane w OpenGLu jak jej wywo�anie ma wygl�da�
//nie mo�na jej da� wewn�trz klasy, bo OpenGLStuff::error_calback to ju� nie samo error_callback.
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Klasa z wszystkimi �mieciami z maina
//Znajd� jak�� lepsz� i nie�mieszn� nazw� - b�d�my powa�ni. I zdecyduj czy tego nie rozbi� na OpenGLStuff i Window bo w sumie nie wiem co b�dzie lepsze.
class OpenGLInitExit
{
private:
	GLFWwindow* window; //Wska�nik na obiekt reprezentuj�cy okno

public:
	OpenGLInitExit() {};
	~OpenGLInitExit();

	GLFWwindow* initLibsAndWindow();

private:
	void initGlfw();
	void initWindow();
	void initGlew();
	void exitWindowAndGlwf();
};

OpenGLInitExit::~OpenGLInitExit()
{
	exitWindowAndGlwf();
}

GLFWwindow* OpenGLInitExit::initLibsAndWindow()
{
	glfwSetErrorCallback(error_callback);//Zarejestruj procedur� obs�ugi b��d�w
	initGlfw();
	initWindow();
	glfwSwapInterval(1); //Czekaj na 1 powr�t plamki przed pokazaniem ukrytego bufora
	initGlew();
	return window;
};

void OpenGLInitExit::initGlfw()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Nie mo�na zainicjowa� GLFW.\n");
		exit(EXIT_FAILURE);
	}
}

void OpenGLInitExit::initWindow()
{
	window = glfwCreateWindow(/*GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)*/500,500, "Anen Ankh", /*glfwGetPrimaryMonitor()*/NULL, NULL);  //Utw�rz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Je�eli okna nie uda�o si� utworzy�, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje si� aktywny i polecenia OpenGL b�d� dotyczy� w�a�nie jego.
};

void OpenGLInitExit::initGlew()
{
	GLenum err;
	if ((err = glewInit()) != GLEW_OK)
	{
		fprintf(stderr, "Nie mo�na zainicjowa� GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
}

void OpenGLInitExit::exitWindowAndGlwf()
{
	glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zaj�te przez GLFW
	exit(EXIT_SUCCESS);
}