#pragma once

#include "Programmed models/lodepng.cpp"
#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "shaderprogram.cpp"

using namespace std;

class Texture
{
public:
	Texture::Texture();
	Texture::Texture(string plik, string plik_spec, int vertexCount, float* vertices, float* colors, float* normals, float* texCoords, mat4 Mp);
	Texture::~Texture();
	void EndTexture();
	void ShowTexture(mat4 mV, mat4 mM, vec3 playerPos);
	GLuint tex0;
	GLuint tex1;
	vector<unsigned char> image;
	unsigned width, height;
	unsigned error;
	string filepath;
	int test = 1337;
	ShaderProgram *shaderProgram;
	mat4 mP;
	//Uchwyty na VAO i bufory wierzcho�k�w
	GLuint vao;
	GLuint bufVertices; //Uchwyt na bufor VBO przechowuj�cy tablic� wsp�rz�dnych wierzcho�k�w
	GLuint bufColors;  //Uchwyt na bufor VBO przechowuj�cy tablic� kolor�w
	GLuint bufNormals; //Uchwyt na bufor VBO przechowuj�cy tablic� wektor�w normalnych
	GLuint bufTexCoords; //Uchwyt na bufor VBO przechowuj�cy tablic� wsp�rz�dnych teksturowania


	GLuint readTexture(const char* filename) {
		GLuint tex;
		glActiveTexture(GL_TEXTURE0);

		//Wczytanie do pami�ci komputera
		std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
		unsigned width, height;   //Zmienne do kt�rych wczytamy wymiary obrazka
								  //Wczytaj obrazek
		unsigned error = lodepng::decode(image, width, height, filename);

		if (error != 0) {
			printf("Error while reading texture %s. Error code: %d. \n", filename, error);
		}

		//Import do pami�ci karty graficznej
		glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
		glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
										   //Wczytaj obrazek do pami�ci KG skojarzonej z uchwytem
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return tex;
	}

	GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
		GLuint handle;

		glGenBuffers(1, &handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), kt�ry b�dzie zawiera� tablic� danych
		glBindBuffer(GL_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO
		glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Wgraj tablic� do VBO

		return handle;
	}

	//Przypisuje bufor VBO do atrybutu
	void assignVBOtoAttribute(ShaderProgram *shaderProgram, const char* attributeName, GLuint bufVBO, int vertexSize) {
		GLuint location = shaderProgram->getAttribLocation(attributeName); //Pobierz numery slot�w dla atrybutu
		glBindBuffer(GL_ARRAY_BUFFER, bufVBO);  //Uaktywnij uchwyt VBO
		glEnableVertexAttribArray(location); //W��cz u�ywanie atrybutu o numerze slotu zapisanym w zmiennej location
		glVertexAttribPointer(location, vertexSize, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location maj� by� brane z aktywnego VBO
	}

};

Texture::Texture()
{

}

Texture::Texture(string plik, string plik_spec, int vertexCount, float* vertices, float* colors, float* normals, float* texCoords,mat4 Mp)
{
	mP = Mp;
	shaderProgram = new ShaderProgram("vshader(1).txt", NULL, "fshader(1).txt"); //Wczytaj program cieniuj�cy

	bufVertices = makeBuffer(vertices, vertexCount/4, sizeof(float) * 4); //VBO ze wsp�rz�dnymi wierzcho�k�w
	//bufColors = makeBuffer(colors, vertexCount, sizeof(float) * 4);//VBO z kolorami wierzcho�k�w
	bufNormals = makeBuffer(normals, vertexCount/4, sizeof(float) * 4);//VBO z wektorami normalnymi wierzcho�k�w
	bufTexCoords = makeBuffer(texCoords, vertexCount/4, sizeof(float) * 2);//VBO ze wspolrzednymi teksturowania

	glGenVertexArrays(1, &vao); //Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej

	glBindVertexArray(vao); //Uaktywnij nowo utworzony VAO

	assignVBOtoAttribute(shaderProgram, "vertex", bufVertices, 4); //"vertex" odnosi si� do deklaracji "in vec4 vertex;" w vertex shaderze
	//assignVBOtoAttribute(shaderProgram, "color", bufColors, 4); //"color" odnosi si� do deklaracji "in vec4 color;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "normal", bufNormals, 4); //"normal" odnosi si� do deklaracji "in vec4 normal;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "texCoords", bufTexCoords, 2); //"texCoords" odnosi si� do deklaracji "in vec2 texCoords;" w vertex shaderze

	glBindVertexArray(0); //Dezaktywuj VAO

	tex0 = readTexture(plik.c_str());
	tex1 = readTexture(plik_spec.c_str());
}

void Texture::ShowTexture(mat4 mV, mat4 mM, vec3 playerPos)
{
	//Przypisanie tekstur do jednostek teksturuj�cych
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);

	shaderProgram->use();
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"), 1, false, glm::value_ptr(mP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"), 1, false, glm::value_ptr(mV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(mM));
	glUniform4f(shaderProgram->getUniformLocation("lp"), 0, 30, 0, 1); //Kolejne argumenty to kolejne warto�ci wektora
																	   //glUniform4fv(shaderProgram->getUniformLocation("lp"),1,value_ptr(vec4(0,0,-6,1))); //drugi argument - przesylamy tylko jeden wektor, trzeci argument - wektor do przes�ania
																	   //Przypisanie jednostek teksturuj�cych do zmiennych
	glUniform1i(shaderProgram->getUniformLocation("textureMap0"), 0);
	glUniform1i(shaderProgram->getUniformLocation("textureMap1"), 1);

	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powi�za� slot�w atrybut�w z tablicami z danymi
	glBindVertexArray(vao);
	//glBindTexture(GL_TEXTURE_2D, tex0);
	//glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

void Texture::EndTexture()
{
	glBindVertexArray(0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &tex0);
	glDeleteTextures(1, &tex1);
}