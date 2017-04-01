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

/// Dirty globals for getting cursor on window 2D position - need to be replaced ///
double X_WINDOW = 0.0f; // nie lepiej centrum ekranu tu podac, koordynaty?
double Y_WINDOW = 0.0f;
bool holdMouse = false;

//Procedura inicjuj¹ca
void initOpenGLProgram() {

	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************
	glClearColor(0, 0, 0, 1);
	//Je¿eli bêdziemy chcieli bawic siê perspektyw¹, to trza to prze³o¿yæ to draw Scene

	glEnable(GL_LIGHTING); //W³¹cz tryb cieniowania
	glEnable(GL_LIGHT0); //W³¹cz domyœlne zerowe Ÿród³o œwiat³a
	glEnable(GL_DEPTH_TEST);

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

	mat4 V = lookAt(
		game.GetPlayerPositionVector(),
		game.GetPlayerViewVector() + game.GetPlayerPositionVector(),
		vec3(0.0f,1.0f,0.0f)
		);

	glLoadMatrixf(value_ptr(V*M));

	game.ShowLevelOne();

	mat4 Mtemp = translate(M, vec3(0.0f, 0.0f, -30.0f));

	//printf("%f | %f | %f\n", game.GetPlayerViewVector().x, game.GetPlayerViewVector().y, game.GetPlayerViewVector().z);
	//printf("%f | %f | %f\n", game.GetPlayerPositionVector().x, game.GetPlayerPositionVector().y, game.GetPlayerPositionVector().z);

	Mtemp = scale(Mtemp, vec3(0.5, 0.5, 0.5));
	glLoadMatrixf(value_ptr(Mtemp*V));

	glutSolidCube(1);

	glfwSwapBuffers(window);
}

#pragma region EXTERNAL INPUT
void keyCallback(GLFWwindow*window, int key, int scancode, int action, int mods)
{
	/// Camera Movement ///
	if (key == GLFW_KEY_W)
	{
		game.GetPlayer()->GoFoward();
	}

	if (key == GLFW_KEY_S)
	{
		game.GetPlayer()->GoBackward();
	}

	if (key == GLFW_KEY_A)
	{
		game.GetPlayer()->StrafeLeft();
	}

	if (key == GLFW_KEY_D)
	{
		game.GetPlayer()->StrafeRight();
	}
	/// END ///

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
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			glfwGetCursorPos(window, &X_WINDOW, &Y_WINDOW);
			holdMouse = true;
		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			mouseRightClick = true;
	}
	if (action == GLFW_RELEASE)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			mouseRightClick = false;

		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			holdMouse = false;
		}
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
		/// Camera Rotator ///
		if (holdMouse == true)
		{
			double newX, newY;
			glfwGetCursorPos(window, &newX, &newY);

			float xDelta = -float(newX - X_WINDOW);
			float yDelta = -float(newY - Y_WINDOW);

			vec3 crossVector = cross(game.GetPlayerViewVector(), game.GetPlayer()->UP_VECTOR);
			mat4 rotator = rotate(mat4(1.0f), xDelta * game.GetPlayer()->ROTATIONAL_SPEED, game.GetPlayer()->UP_VECTOR) *
				rotate(mat4(1.0f), yDelta * game.GetPlayer()->ROTATIONAL_SPEED, crossVector);

			game.GetPlayer()->viewDirection = mat3(rotator) * game.GetPlayer()->viewDirection;

			X_WINDOW = newX;
			Y_WINDOW = newY;

			printf("VIEW: %f | %f | %f\n", game.GetPlayerViewVector().x, game.GetPlayerViewVector().y, game.GetPlayerViewVector().z);
			printf("POS: %f | %f | %f\n", game.GetPlayerPositionVector().x, game.GetPlayerPositionVector().y, game.GetPlayerPositionVector().z);
		}
		/// END ///

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