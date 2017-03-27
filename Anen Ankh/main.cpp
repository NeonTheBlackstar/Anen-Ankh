//Trza to potem pousuwa� �eby niedublowa� inklud�w
#include "OpenGLInitExit.h"
#include "Game.h"

using namespace glm;
using namespace std;

//Globalna �eby m�c u�ywa� w draw scene i jednocze�nie nie tworzy�o si� od nowa co klatk�.
//Poza tym - wszystkie rzeczy r�bmy za pomoc� game - bedzie �atwiej ogarn��. 
//Np id� w prz�d to b�dzie od razu id� w prz�d i 
//sprawd� czy nadal trzymasz si� kolizji czy spadasz.
Game game = *(new Game());

//Procedura inicjuj�ca
void initOpenGLProgram() {

	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� raz, na pocz�tku programu************
	glClearColor(0, 0, 0, 1);
	//Je�eli b�dziemy chcieli bawic si� perspektyw�, to trza to prze�o�y� to draw Scene
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(game.SetPerspective()));
}

//Procedura rysuj�ca zawarto�� sceny
void drawScene(GLFWwindow* window) {

	//************Tutaj umieszczaj kod rysuj�cy obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Powiedzenie GLowi �e co� si� dzieje - stary OpenGL ma 2 macierze tylko, P i MV
	//Zostawiam to tutaj dlatego, �e praktycznie wszystkie operacje w openGLu to operacje na tych macierzach - po kija je wywala� poza
	mat4 M = mat4(1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(game.GetPlayerPositionMatrix()*M));

	game.ShowLevelOne();

	glfwSwapBuffers(window);
}

#pragma region EXTERNAL INPUT
void keyCallback(GLFWwindow*window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A)
			printf("A\n");
		if (key == GLFW_KEY_D)
			printf("D\n");
		if (key == GLFW_KEY_W && (mods&GLFW_MOD_ALT) != 0)
			printf("ALT + W\n");
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_W)
			printf("puszczone W\n");
	}
}

bool mouseRightClick = false;
void mouseCallback(GLFWwindow*window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			mouseRightClick = true;
	}
	if (action == GLFW_RELEASE)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			mouseRightClick = false;
	}
}
#pragma endregion

int main(void)
{
	OpenGLInitExit* openGLInitExit = new OpenGLInitExit();
	GLFWwindow* window = (*openGLInitExit).initLibsAndWindow();

	initOpenGLProgram(); //Operacje inicjuj�ce

	#pragma region EXTERNAL INPUT
	double xCursorPos;
	double yCursorPos;
	#pragma endregion
	game.SetLevelOne();

	while (!glfwWindowShouldClose(window)) //Tak d�ugo jak okno nie powinno zosta� zamkni�te
	{
		drawScene(window); //Wykonaj procedur� rysuj�c�

	#pragma region EXTERNAL INPUT
		//Musz� by� tutaj poniewa� jak si� wsadzi do klasy, to niema zainicjowanego glfw
		//Funkcje tak samo, bo wywo�aniez klasy nie dzia�a
		glfwSetKeyCallback(window, keyCallback);
		glfwSetMouseButtonCallback(window, mouseCallback);
		glfwGetCursorPos(window, &xCursorPos, &yCursorPos);
	#pragma endregion

		glfwPollEvents(); //Wykonaj procedury callback w zalezno�ci od zdarze� jakie zasz�y.
	}

	delete &game;
	delete openGLInitExit;
}
