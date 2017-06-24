#include "OpenGLInitExit.h"
#include "Game.h"
#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"

using namespace glm;
using namespace std;

//Globalna ¿eby móc u¿ywaæ w draw scene i jednoczeœnie nie tworzy³o siê od nowa co klatkê.
//Poza tym - wszystkie rzeczy róbmy za pomoc¹ game - bedzie ³atwiej ogarn¹æ. 
//Np idŸ w przód to bêdzie od razu idŸ w przód i 
//sprawdŸ czy nadal trzymasz siê kolizji czy spadasz.
Game game = *(new Game());
GLuint tex;

void initOpenGLProgram(GLFWwindow* window) {
	//Je¿eli bêdziemy chcieli bawic siê perspektyw¹, to trza to prze³o¿yæ to draw Scene

	glClearColor(0, 0, 0, 1);

	//glEnable(GL_LIGHTING); //W³¹cz tryb cieniowania
	glEnable(GL_LIGHT0); //W³¹cz zerowe Ÿród³o œwiat³a
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie budora g³êbokoœci
	glEnable(GL_TEXTURE_2D); //W³¹cz teksturowanie
	glEnable(GL_NORMALIZE); //W³¹cz automatyczn¹ normalizacjê wektorów normalnych

	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, "Textures/floor.png");

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void drawScene(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyœæ bufor kolorów (czyli przygotuj "p³ótno" do rysowania)
	glMatrixMode(GL_PROJECTION); //W³¹cz tryb modyfikacji macierzy rzutowania
	glLoadMatrixf(value_ptr(game.SetPerspective())); //Za³aduj macierz rzutowania
	game.ShowLevelOne();
	glfwSwapBuffers(window); //Przerzuæ tylny bufor na przedni
}

#pragma region EXTERNAL INPUT
void keyCallback(GLFWwindow*window, int key, int scancode, int action, int mods)
{
	//if (action == GLFW_PRESS) {

	if (game.player.noClip == false)
	{
		if (key == GLFW_KEY_D && game.canGoLeft)
		{
			game.player.GoRight();
			game.player.SetCollider();
		}

		if (key == GLFW_KEY_A && game.canGoRight)
		{
			game.player.GoLeft();
			game.player.SetCollider();
		}

		if (key == GLFW_KEY_W)
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
	else if (game.player.noClip == true)
	{
		if (key == GLFW_KEY_D && game.canGoLeft)
		{
			game.player.GoRight(true);
		}

		if (key == GLFW_KEY_A && game.canGoRight)
		{
			game.player.GoLeft(true);
		}

		if (key == GLFW_KEY_W && game.canGoFoward == true)
		{
			game.player.GoFoward(true);
		}

		if (key == GLFW_KEY_S && game.canGoBack)
		{
			game.player.GoBack(true);
		}

		if (key == GLFW_KEY_R)
		{
			game.player.GoUp();
		}
		if (key == GLFW_KEY_F)
		{
			game.player.GoDown();
		}
	}

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_N)
		{
			game.player.noClip = game.player.noClip == true ? false : true;
		}
	}
	//}
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

	initOpenGLProgram(window); //Operacje inicjuj¹ce

#pragma region EXTERNAL INPUT
	double xCursorPos = 0.0f;
	double yCursorPos = 0.0f;
	double new_xCursorPos, new_yCursorPos;
	float xMotionDelta, yMotionDelta;
#pragma endregion

	game.SetLevelOne();

	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
		drawScene(window); //Wykonaj procedurê rysuj¹c¹

#pragma region HANDLE MOUSE MOTION
		if (mouseRightClick == true) {
			glfwGetCursorPos(window, &new_xCursorPos, &new_yCursorPos);
			xMotionDelta = -float(new_xCursorPos - xCursorPos);
			yMotionDelta = -float(new_yCursorPos - yCursorPos);

			vec3 viewDirection = game.player.viewVector;
			float speed = 0.01f;

			vec3 crossVector = cross(viewDirection, game.player.UP_vector);
			mat4 rotator = rotate(mat4(1.0f), xMotionDelta * speed, game.player.UP_vector) *
				rotate(mat4(1.0f), yMotionDelta * speed, crossVector);

			viewDirection = mat3(rotator) * viewDirection;

			game.player.updateView(viewDirection);

			xCursorPos = new_xCursorPos;
			yCursorPos = new_yCursorPos;

			/*printf("|| (%f, %f, %f) | (%f, %f, %f) | (%f, %f, %f) ||\n",
			game.player.positionVector.x, game.player.positionVector.y, game.player.positionVector.z,
			game.player.viewVector.x, game.player.viewVector.y, game.player.viewVector.z,
			game.player.UP_vector.x, game.player.UP_vector.y, game.player.UP_vector.z);*/
		}
#pragma endregion

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

