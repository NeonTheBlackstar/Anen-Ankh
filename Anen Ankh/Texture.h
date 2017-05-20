#pragma once

#include "Programmed models/lodepng.cpp"

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
	string plik;

private:

};

Texture::Texture(string plik)
{
	this->plik = plik;
}

void Texture::ShowTexture()
{
	error = lodepng::decode(image, width, height, "brick.png");
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
}

Texture::~Texture()
{
	glDeleteTextures(1, &tex);
}