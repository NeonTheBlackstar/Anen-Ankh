#include "OpenGLInitExit.h"
#include "Game.h"
#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"

using namespace glm;
using namespace std;

//Globalna �eby m�c u�ywa� w draw scene i jednocze�nie nie tworzy�o si� od nowa co klatk�.
//Poza tym - wszystkie rzeczy r�bmy za pomoc� game - bedzie �atwiej ogarn��. 
//Np id� w prz�d to b�dzie od razu id� w prz�d i 
//sprawd� czy nadal trzymasz si� kolizji czy spadasz.
Game game = *(new Game());
GLuint tex1, tex2;
std::vector<unsigned char> image1, image2;
unsigned error, 
	width1, height1, 
	width2, height2;

void initOpenGLProgram(GLFWwindow* window) {
	//Je�eli b�dziemy chcieli bawic si� perspektyw�, to trza to prze�o�y� to draw Scene

	glClearColor(0, 0, 0, 1);

	//glEnable(GL_LIGHTING); //W��cz tryb cieniowania
	glEnable(GL_LIGHT0); //W��cz zerowe �r�d�o �wiat�a
	glEnable(GL_DEPTH_TEST); //W��cz u�ywanie budora g��boko�ci
	glEnable(GL_TEXTURE_2D); //W��cz teksturowanie
	glEnable(GL_NORMALIZE); //W��cz automatyczn� normalizacj� wektor�w normalnych
	
	/* Wczytywanie tesktur */
	unsigned error = lodepng::decode(image1, width1, height1, "Textures/floor.png");
	glGenTextures(1, &tex1);
	error = lodepng::decode(image2, width2, height2, "Textures/bricks.png");
	glGenTextures(1, &tex2);
}

void drawScene(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczy�� bufor kolor�w (czyli przygotuj "p��tno" do rysowania)
	glMatrixMode(GL_PROJECTION); //W��cz tryb modyfikacji macierzy rzutowania
	glLoadMatrixf(value_ptr(game.SetPerspective())); //Za�aduj macierz rzutowania

	/*glBindTexture(GL_TEXTURE_2D, tex1);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image1.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/

	game.ShowLevelOne();

	#pragma region showModel
	vector<float> objV, objN, objT;

	/////////////

	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image2.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	processObj("models/cube.obj", objV, objN, objT);

	mat4 M = mat4(1);

	M = game.player.positionMatrix*M;

	M = translate(M, vec3(0, -50, 0));
	M = scale(M, vec3(10, 10, 10));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(M));

	glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u�ywaj tablicy wierzcho�k�w
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); //Podczas rysowania u�ywaj tablicy wsp. teksturowania
	glEnableClientState(GL_NORMAL_ARRAY); //Podczas rysowania u�ywaj tablicy wektor�w normalnych

	glVertexPointer(3, GL_FLOAT, 0, objV.data()); //Ustaw tablic� myCubeVertices jako tablic� wierzcho�k�w
	glTexCoordPointer(2, GL_FLOAT, 0, objT.data()); //Ustaw tablic� myCubeTexCoords jako tablic� wsp. teksturowania
	glNormalPointer(GL_FLOAT, 0, objN.data()); //Ustaw tablic� myCubeVertices jako tablic� wektor�w normalnych - tutaj akurat wsp. wierzcho�ka=suma wektor�w normalnych �cian s�siaduj�cych
	glDrawArrays(GL_TRIANGLES, 0, objV.size() / 3); //Rysuj model

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	#pragma endregion


	glfwSwapBuffers(window); //Przerzu� tylny bufor na przedni
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

	initOpenGLProgram(window); //Operacje inicjuj�ce

#pragma region EXTERNAL INPUT
	double xCursorPos = 0.0f;
	double yCursorPos = 0.0f;
	double new_xCursorPos, new_yCursorPos;
	float xMotionDelta, yMotionDelta;
#pragma endregion

	game.SetLevelOne();

	while (!glfwWindowShouldClose(window)) //Tak d�ugo jak okno nie powinno zosta� zamkni�te
	{
		drawScene(window); //Wykonaj procedur� rysuj�c�

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

