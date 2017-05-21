#pragma once

#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

//Procedura obs³ugi b³êdów - poniewa¿ jest œciœle opisane w OpenGLu jak jej wywo³anie ma wygl¹daæ
//nie mo¿na jej daæ wewn¹trz klasy, bo OpenGLStuff::error_calback to ju¿ nie samo error_callback.
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Klasa z wszystkimi œmieciami z maina
//ZnajdŸ jak¹œ lepsz¹ i nieœmieszn¹ nazwê - b¹dŸmy powa¿ni. I zdecyduj czy tego nie rozbiæ na OpenGLStuff i Window bo w sumie nie wiem co bêdzie lepsze.
class OpenGLInitExit
{
private:
	GLFWwindow* window; //WskaŸnik na obiekt reprezentuj¹cy okno

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
	glfwSetErrorCallback(error_callback);//Zarejestruj procedurê obs³ugi b³êdów
	initGlfw();
	initWindow();
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora
	initGlew();
	return window;
};

void OpenGLInitExit::initGlfw()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		exit(EXIT_FAILURE);
	}
}

void OpenGLInitExit::initWindow()
{
	window = glfwCreateWindow(/*GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)*/500,500, "Anen Ankh", /*glfwGetPrimaryMonitor()*/NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
};

void OpenGLInitExit::initGlew()
{
	GLenum err;
	if ((err = glewInit()) != GLEW_OK)
	{
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
}

void OpenGLInitExit::exitWindowAndGlwf()
{
	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);
}