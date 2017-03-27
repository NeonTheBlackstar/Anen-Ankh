//Trza to potem pousuwaæ ¿eby niedublowaæ inkludów
#include "OpenGLInitExit.h"
#include "Game.h"

using namespace glm;
using namespace std;

//Globalna ¿eby móc u¿ywaæ w draw scene i jednoczeœnie nie tworzy³o siê od nowa co klatkê.
//Poza tym - wszystkie rzeczy róbmy za pomoc¹ game - bedzie ³atwiej ogarn¹æ. 
//Np idŸ w przód to bêdzie od razu idŸ w przód i 
//sprawdŸ czy nadal trzymasz siê kolizji czy spadasz.
Game game = *(new Game());

//Procedura inicjuj¹ca
void initOpenGLProgram() {

	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************
	glClearColor(0, 0, 0, 1);
	//Je¿eli bêdziemy chcieli bawic siê perspektyw¹, to trza to prze³o¿yæ to draw Scene
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(game.SetPerspective()));
}

//Procedura rysuj¹ca zawartoœæ sceny
void drawScene(GLFWwindow* window) {

	//************Tutaj umieszczaj kod rysuj¹cy obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Powiedzenie GLowi ¿e coœ siê dzieje - stary OpenGL ma 2 macierze tylko, P i MV
	//Zostawiam to tutaj dlatego, ¿e praktycznie wszystkie operacje w openGLu to operacje na tych macierzach - po kija je wywalaæ poza
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

	initOpenGLProgram(); //Operacje inicjuj¹ce

	#pragma region EXTERNAL INPUT
	double xCursorPos;
	double yCursorPos;
	#pragma endregion
	game.SetLevelOne();

	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
		drawScene(window); //Wykonaj procedurê rysuj¹c¹

	#pragma region EXTERNAL INPUT
		//Musz¹ byæ tutaj poniewa¿ jak siê wsadzi do klasy, to niema zainicjowanego glfw
		//Funkcje tak samo, bo wywo³aniez klasy nie dzia³a
		glfwSetKeyCallback(window, keyCallback);
		glfwSetMouseButtonCallback(window, mouseCallback);
		glfwGetCursorPos(window, &xCursorPos, &yCursorPos);
	#pragma endregion

		glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
	}

	delete &game;
	delete openGLInitExit;
}
