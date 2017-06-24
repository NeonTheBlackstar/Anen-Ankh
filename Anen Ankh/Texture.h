#pragma once

#include "Programmed models/lodepng.cpp"
#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"

using namespace std;

class Texture
{
public:
	Texture(string plik);
	~Texture();
	void ShowTexture();
	GLuint tex;
	vector<unsigned char> image;
	unsigned width, height;
	unsigned error;
	string filepath;
	int test = 1337;

private:

};

Texture::Texture(string plik)
{
	filepath = plik;
	error = lodepng::decode(image, width, height, filepath);
	glGenTextures(1, &tex);
}

void Texture::ShowTexture()
{
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
	glDeleteTextures(1, &tex);
}