#include "OpenGLInitExit.h"
#include "Game.h"

using namespace glm;
using namespace std;

//Globalna �eby m�c u�ywa� w draw scene i jednocze�nie nie tworzy�o si� od nowa co klatk�.
//Poza tym - wszystkie rzeczy r�bmy za pomoc� game - bedzie �atwiej ogarn��. 
//Np id� w prz�d to b�dzie od razu id� w prz�d i 
//sprawd� czy nadal trzymasz si� kolizji czy spadasz.
Game game = *(new Game());
GLuint tex;

void initOpenGLProgram(GLFWwindow* window) {
	//Je�eli b�dziemy chcieli bawic si� perspektyw�, to trza to prze�o�y� to draw Scene

	glClearColor(0, 0, 0, 1);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT2);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);

	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, "bricks.png");

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void drawScene(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczy�� bufor kolor�w (czyli przygotuj "p��tno" do rysowania)
	glMatrixMode(GL_PROJECTION); //W��cz tryb modyfikacji macierzy rzutowania
	glLoadMatrixf(value_ptr(game.SetPerspective())); //Za�aduj macierz rzutowania
	game.ShowLevelOne();
	glfwSwapBuffers(window); //Przerzu� tylny bufor na przedni
}

#pragma region EXTERNAL INPUT
void keyCallback(GLFWwindow*window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_D && game.canGoLeft)
		{
			game.player.GoLeft();
			game.player.SetCollider();
		}

		if (key == GLFW_KEY_A && game.canGoRight)
		{
			game.player.GoRight();
			game.player.SetCollider();
		}

		if (key == GLFW_KEY_W && game.canGoFoward)
		{
			game.player.GoFoward();
			game.player.SetCollider();
		}

		if (key == GLFW_KEY_S && game.canGoBack)
		{
			game.player.GoBack();
			game.player.SetCollider();
		}
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
	
		initOpenGLProgram(window); //Operacje inicjuj�ce
	
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

	