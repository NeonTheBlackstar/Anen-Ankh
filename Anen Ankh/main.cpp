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
GLuint tex1, tex2,tex3,tex4;
std::vector<unsigned char> image1, image2,image3, image4;
unsigned error, 
	width1, height1,
	width2, height2,
	width3, height3,
	width4, height4;
vector<float> objV1, objN1, objT1;
vector<float> objV2, objN2, objT2;
vector<float> objV3, objN3, objT3;
Texture *texstairs = new Texture();
Texture *texbat = new Texture();

void initOpenGLProgram(GLFWwindow* window) {
	//Je¿eli bêdziemy chcieli bawic siê perspektyw¹, to trza to prze³o¿yæ to draw Scene

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie budora g³êbokoœci

	glEnable(GL_LIGHTING); //W³¹cz tryb cieniowania
	glEnable(GL_LIGHT0); //W³¹cz zerowe Ÿród³o œwiat³a
	
	glEnable(GL_TEXTURE_2D); //W³¹cz teksturowanie
	glEnable(GL_NORMALIZE); //W³¹cz automatyczn¹ normalizacjê wektorów normalnych
	
	/* Wczytywanie tesktur */
	unsigned error = lodepng::decode(image1, width1, height1, "Textures/floor.png");
	glGenTextures(1, &tex1);
	error = lodepng::decode(image2, width2, height2, "Textures/bricks.png");
	glGenTextures(1, &tex2);
	error = lodepng::decode(image3, width3, height3, "Textures/metal.png");
	glGenTextures(1, &tex3);
	error = lodepng::decode(image4, width4, height4, "Textures/bandage.png");
	glGenTextures(1, &tex4);

	processObj("models/cube.obj", objV1, objN1, objT1);
	processObj("models/schody_tr.obj", objV2, objN2, objT2);
	processObj("models/batman_tr.obj", objV3, objN3, objT3);

	texstairs = new Texture("Textures\\metal.png", "Textures\\metal_spec.png", objV2.size(), objV2.data(), objN2.data(), objT2.data(), objT2.data(), game.SetPerspective());
	//texbat = new Texture("Textures\\metal.png", "Textures\\metal_spec.png", objV3.size(), objV3.data(), objN3.data(), objT3.data(), objT3.data(), game.SetPerspective());
}

void drawScene(GLFWwindow* window) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyœæ bufor kolorów (czyli przygotuj "p³ótno" do rysowania)
	glMatrixMode(GL_PROJECTION); //W³¹cz tryb modyfikacji macierzy rzutowania
	glLoadMatrixf(value_ptr(game.SetPerspective())); //Za³aduj macierz rzutowania
	mat4 Mo = mat4(1), Mbat, Msch; //Mo - origin matrix
	
#pragma region schody

	Msch = mat4(1);
	Msch = translate(Msch, vec3(10, 48, -5));
	Msch = scale(Msch, vec3(11, 8, 5.5));
	Msch = rotate(Msch, 3.14f, vec3(1, 0, 0));
	Msch = rotate(Msch, 3.14f, vec3(0, 1, 0));
	texstairs->ShowTexture(game.GetPlayerPositionMatrix(), Msch, vec3(1, 1, 1));
	Msch = game.player.positionMatrix*Msch;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(Msch));

	glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); //Podczas rysowania u¿ywaj tablicy wsp. teksturowania
	glEnableClientState(GL_NORMAL_ARRAY); //Podczas rysowania u¿ywaj tablicy wektorów normalnych
	glVertexPointer(3, GL_FLOAT, 0, objV2.data()); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
	glTexCoordPointer(2, GL_FLOAT, 0, objT2.data()); //Ustaw tablicê myCubeTexCoords jako tablicê wsp. teksturowania
	glNormalPointer(GL_FLOAT, 0, objN2.data()); //Ustaw tablicê myCubeVertices jako tablicê wektorów normalnych - tutaj akurat wsp. wierzcho³ka=suma wektorów normalnych œcian s¹siaduj¹cych
	glDrawArrays(GL_TRIANGLES, 0, objV2.size() / 3); //Rysuj model

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	texstairs->EndTexture();

#pragma endregion

#pragma region batman

	glBindTexture(GL_TEXTURE_2D, tex4);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width4, height4, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image4.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	Mbat = mat4(1);
	Mbat = translate(Mbat, vec3(-85, 16, 0));
	Mbat = scale(Mbat, vec3(6, 6, 6));
	Mbat = rotate(Mbat, 3.14f, vec3(1, 0, 0));
	Mbat = rotate(Mbat, 3.14f / 2, vec3(0, 1, 0));	
	//texbat->ShowTexture(game.GetPlayerPositionMatrix(), Mbat, vec3(1, 1, 1));
	Mbat = game.player.positionMatrix*Mbat;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(Mbat));

	glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); //Podczas rysowania u¿ywaj tablicy wsp. teksturowania
	glEnableClientState(GL_NORMAL_ARRAY); //Podczas rysowania u¿ywaj tablicy wektorów normalnych

	glVertexPointer(3, GL_FLOAT, 0, objV3.data()); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
	glTexCoordPointer(2, GL_FLOAT, 0, objT3.data()); //Ustaw tablicê myCubeTexCoords jako tablicê wsp. teksturowania
	glNormalPointer(GL_FLOAT, 0, objN3.data()); //Ustaw tablicê myCubeVertices jako tablicê wektorów normalnych - tutaj akurat wsp. wierzcho³ka=suma wektorów normalnych œcian s¹siaduj¹cych
	glDrawArrays(GL_TRIANGLES, 0, objV3.size()); //Rysuj model

													 /*mat4 Mz = Mo; // za³adowanie macierzy g³ównej
													 Mz = scale(Mo, vec3(0.025f, 0.05f, 0.025f)); // przeskaluj macierz modelu
													 glLoadMatrixf(value_ptr(Mz));*/

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	//texbat->EndTexture();

#pragma endregion

	game.ShowLevelOne(objV1, objN1, objT1);
	glfwSwapBuffers(window); //Przerzuæ tylny bufor na przedni
}

#pragma region EXTERNAL INPUT
void keyCallback(GLFWwindow*window, int key, int scancode, int action, int mods)
{
	//if (action == GLFW_PRESS) {

	if (game.player.noClip == false)
	{
		if (key == GLFW_KEY_D)
		{
			game.player.GoRight();
			game.player.SetCollider();
		}

		if (key == GLFW_KEY_A)
		{
			game.player.GoLeft();
			game.player.SetCollider();
		}

		if (key == GLFW_KEY_W)
		{
				game.player.GoFoward();
				game.player.SetCollider();
		}

		if (key == GLFW_KEY_S)
		{
			game.player.GoBack();
			game.player.SetCollider();
		}
	}
	else if (game.player.noClip == true)
	{
		if (key == GLFW_KEY_D)
		{
			game.player.GoRight(true);
		}

		if (key == GLFW_KEY_A)
		{
			game.player.GoLeft(true);
		}

		if (key == GLFW_KEY_W)
		{
			game.player.GoFoward(true);
		}

		if (key == GLFW_KEY_S)
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

	game.SetLevelOne(objV1, objN1, objT1);

	//Texture * texStairs = (new Texture("Textures\\metal.png", "Textures\\metal_spec.png", objV2.size(), objV2.data(), objN2.data(), objT2.data(), objT2.data(), game.SetPerspective()));
	//Texture * texBat = (new Texture("Textures\\metal.png", "Textures\\metal_spec.png", objV3.size(), objV3.data(), objN3.data(), objT3.data(), objT3.data(), game.SetPerspective()));

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

