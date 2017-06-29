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
	//Uchwyty na VAO i bufory wierzcho³ków
	GLuint vao;
	GLuint bufVertices; //Uchwyt na bufor VBO przechowuj¹cy tablicê wspó³rzêdnych wierzcho³ków
	GLuint bufColors;  //Uchwyt na bufor VBO przechowuj¹cy tablicê kolorów
	GLuint bufNormals; //Uchwyt na bufor VBO przechowuj¹cy tablicê wektorów normalnych
	GLuint bufTexCoords; //Uchwyt na bufor VBO przechowuj¹cy tablicê wspó³rzêdnych teksturowania


	GLuint readTexture(const char* filename) {
		GLuint tex;
		glActiveTexture(GL_TEXTURE0);

		//Wczytanie do pamiêci komputera
		std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
		unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
								  //Wczytaj obrazek
		unsigned error = lodepng::decode(image, width, height, filename);

		if (error != 0) {
			printf("Error while reading texture %s. Error code: %d. \n", filename, error);
		}

		//Import do pamiêci karty graficznej
		glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
		glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
										   //Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return tex;
	}

	GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
		GLuint handle;

		glGenBuffers(1, &handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê danych
		glBindBuffer(GL_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO
		glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Wgraj tablicê do VBO

		return handle;
	}

	//Przypisuje bufor VBO do atrybutu
	void assignVBOtoAttribute(ShaderProgram *shaderProgram, const char* attributeName, GLuint bufVBO, int vertexSize) {
		GLuint location = shaderProgram->getAttribLocation(attributeName); //Pobierz numery slotów dla atrybutu
		glBindBuffer(GL_ARRAY_BUFFER, bufVBO);  //Uaktywnij uchwyt VBO
		glEnableVertexAttribArray(location); //W³¹cz u¿ywanie atrybutu o numerze slotu zapisanym w zmiennej location
		glVertexAttribPointer(location, vertexSize, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location maj¹ byæ brane z aktywnego VBO
	}

};

Texture::Texture()
{

}

Texture::Texture(string plik, string plik_spec, int vertexCount, float* vertices, float* colors, float* normals, float* texCoords,mat4 Mp)
{
	mP = Mp;
	shaderProgram = new ShaderProgram("vshader(1).txt", NULL, "fshader(1).txt"); //Wczytaj program cieniuj¹cy

	bufVertices = makeBuffer(vertices, vertexCount/4, sizeof(float) * 4); //VBO ze wspó³rzêdnymi wierzcho³ków
	//bufColors = makeBuffer(colors, vertexCount, sizeof(float) * 4);//VBO z kolorami wierzcho³ków
	bufNormals = makeBuffer(normals, vertexCount/4, sizeof(float) * 4);//VBO z wektorami normalnymi wierzcho³ków
	bufTexCoords = makeBuffer(texCoords, vertexCount/4, sizeof(float) * 2);//VBO ze wspolrzednymi teksturowania

	glGenVertexArrays(1, &vao); //Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej

	glBindVertexArray(vao); //Uaktywnij nowo utworzony VAO

	assignVBOtoAttribute(shaderProgram, "vertex", bufVertices, 4); //"vertex" odnosi siê do deklaracji "in vec4 vertex;" w vertex shaderze
	//assignVBOtoAttribute(shaderProgram, "color", bufColors, 4); //"color" odnosi siê do deklaracji "in vec4 color;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "normal", bufNormals, 4); //"normal" odnosi siê do deklaracji "in vec4 normal;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram, "texCoords", bufTexCoords, 2); //"texCoords" odnosi siê do deklaracji "in vec2 texCoords;" w vertex shaderze

	glBindVertexArray(0); //Dezaktywuj VAO

	tex0 = readTexture(plik.c_str());
	tex1 = readTexture(plik_spec.c_str());
}

void Texture::ShowTexture(mat4 mV, mat4 mM, vec3 playerPos)
{
	//Przypisanie tekstur do jednostek teksturuj¹cych
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);

	shaderProgram->use();
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"), 1, false, glm::value_ptr(mP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"), 1, false, glm::value_ptr(mV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(mM));
	glUniform4f(shaderProgram->getUniformLocation("lp"), 0, 30, 0, 1); //Kolejne argumenty to kolejne wartoœci wektora
																	   //glUniform4fv(shaderProgram->getUniformLocation("lp"),1,value_ptr(vec4(0,0,-6,1))); //drugi argument - przesylamy tylko jeden wektor, trzeci argument - wektor do przes³ania
																	   //Przypisanie jednostek teksturuj¹cych do zmiennych
	glUniform1i(shaderProgram->getUniformLocation("textureMap0"), 0);
	glUniform1i(shaderProgram->getUniformLocation("textureMap1"), 1);

	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powi¹zañ slotów atrybutów z tablicami z danymi
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