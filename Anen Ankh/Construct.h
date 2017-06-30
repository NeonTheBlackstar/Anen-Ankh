#pragma once

#include "Collider.h"
#include "Texture.h"
#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include "Programmed models/Cube.h"
#include "Programmed models/Stairs.h"
#include <sstream>
#include <list>

using namespace std;

#pragma region PARSER
//PARSER OBJ
void split(string &in, vector<string> &out, char sep = ' ') {
	out.clear();
	std::istringstream iss(in);
	std::string sub;

	while (std::getline(iss, sub, sep)) {
		out.push_back(sub);
	}
}

void processVertex(vector<string> &in, vector<vec4> &out) {
	out.push_back(vec4(atof(in[1].c_str()),
		atof(in[2].c_str()),
		atof(in[3].c_str()),
		1.0f
	));
}

void processNormal(vector<string> &in, vector<vec4> &out) {
	//processVertex(in,out);
	out.push_back(vec4(atof(in[1].c_str()),
		atof(in[2].c_str()),
		atof(in[3].c_str()),
		0.0f
	));
}

void processTexCoord(vector<string> &in, vector<vec2> &out) {
	out.push_back(vec2(atof(in[1].c_str()),
		atof(in[2].c_str())
	));
}

void decompressVertex(string vertexStr,
	vector<vec4> &vertices, vector<vec4> &normals, vector<vec2> &texCoords,
	vector<float> &outV, vector<float> &outN, vector<float> &outT) {

	vector<string> tmp;
	split(vertexStr, tmp, '/');

	outV.push_back(vertices[atoi(tmp[0].c_str()) - 1].x);
	outV.push_back(vertices[atoi(tmp[0].c_str()) - 1].y);
	outV.push_back(vertices[atoi(tmp[0].c_str()) - 1].z);
	outV.push_back(vertices[atoi(tmp[0].c_str()) - 1].w);

	outT.push_back(texCoords[atoi(tmp[1].c_str()) - 1].x);
	outT.push_back(texCoords[atoi(tmp[1].c_str()) - 1].y);

	outN.push_back(normals[atoi(tmp[2].c_str()) - 1].x);
	outN.push_back(normals[atoi(tmp[2].c_str()) - 1].y);
	outN.push_back(normals[atoi(tmp[2].c_str()) - 1].z);
	outN.push_back(normals[atoi(tmp[2].c_str()) - 1].w);

}

vec3 getPos(string vertexStr, vector<vec4> &vertices) {

	vector<string> tmp;
	split(vertexStr, tmp, '/');
	return vertices[atoi(tmp[0].c_str()) - 1];

}

void processFace(vector<string> &in,
	vector<vec4> &vertices, vector<vec4> &normals, vector<vec2> &texCoords,
	vector<float> &outV, vector<float> &outN, vector<float> &outT) {

	decompressVertex(in[1], vertices, normals, texCoords, outV, outN, outT);
	decompressVertex(in[2], vertices, normals, texCoords, outV, outN, outT);
	decompressVertex(in[3], vertices, normals, texCoords, outV, outN, outT);
}

void processObj(string filename, vector<float> &outV, vector<float> &outN, vector<float> &outT) {
	vector<vec4> vertices;
	vector<vec4> normals;
	vector<vec2> texCoords;

	ifstream inFile;
	inFile.open(filename.c_str());

	string line;
	vector<string> tmp;

	while (getline(inFile, line)) {
		split(line, tmp);
		if (tmp.size()>0) {
			if (tmp[0] == "v") {
				processVertex(tmp, vertices);
				//printf("wierzcholek %f,%f,%f, %f\n",vertices.back().x,vertices.back().y,vertices.back().z,vertices.back().w);
			}
			else if (tmp[0] == "vn") {
				processNormal(tmp, normals);
				//printf("normalna %f,%f,%f, %f\n",normals.back().x,normals.back().y,normals.back().z,normals.back().w);
			}
			else if (tmp[0] == "vt") {
				processTexCoord(tmp, texCoords);
				//printf("wsp. tex. %f,%f\n",texCoords.back().x,texCoords.back().y);
			}
		}
	}

	inFile.close();

	inFile.open(filename.c_str());

	while (getline(inFile, line)) {
		split(line, tmp);
		if (tmp.size()>0) {
			if (tmp[0] == "f") {
				processFace(tmp, vertices, normals, texCoords, outV, outN, outT);
			}
		}
	}

	inFile.close();
}
#pragma endregion


class Construct
{
public:
	static enum figureType
	{
		cube, stairs
	};
	CubeCollider3D cubeCollider;
	LineCollider2D lineCollider;

private:
	mat4 modelMatrix;
	mat4 parentMatrix;
	string name;
	Texture * p_texture;
	int selectedFigureType;
	vec3 position;
	vec3 size;
	float rotationAngle;
	vec3 rotationAxis;
	vec3 playerPosOrigin;

public:
	Construct(string name, int selectedFigureType, mat4 parentMatrix, Texture * texture, vec3 position, vec3 size, vec3 rotationAxis, float rotationAngle);
	~Construct() {};
	void SetCubeCollider(vec3 positionOfCenter, vec3 difference, vec3 size);
	void SetLineCollider(float firstPositionX, float secondPositionX, float firstPositionY, float secondPositionY);
	void DrawSolid(mat4 playerPosition, vector<float> objV, vector<float> objN, vector<float> objT, Texture * tex = NULL);
};

Construct::Construct(string name, int selectedFigureType, mat4 parentMatrix, Texture * texture, vec3 position = vec3(0, 0, 0), vec3 size = vec3(1, 1, 1), vec3 rotationAxis = vec3(1, 1, 1), float rotationAngle = 0)
{
	this->selectedFigureType = selectedFigureType;
	this->parentMatrix = parentMatrix;
	this->p_texture = texture;
	this->modelMatrix = parentMatrix;
	this->position = position;//translate(modelMatrix, position);
	this->size = size;//scale(modelMatrix, size);
	this->rotationAngle = rotationAngle;//rotate(modelMatrix, rotationAngle, rotationAxis);
	this->rotationAxis = rotationAxis;
	this->playerPosOrigin = vec3(0.0f, 0.0f, 0.0f);
}

void Construct::SetCubeCollider(vec3 positionOfCenter, vec3 difference, vec3 size)
{
	cubeCollider = *(new CubeCollider3D(positionOfCenter, difference, size));
}

void Construct::SetLineCollider(float firstPositionX, float secondPositionX, float firstPositionY, float secondPositionY)
{
	lineCollider = *(new LineCollider2D(firstPositionX, secondPositionX, firstPositionY, secondPositionY));
}

void Construct::DrawSolid(mat4 playerPosition, vector<float> objV, vector<float> objN, vector<float> objT, Texture * tex)
{
	mat4 M = mat4(1);
	//printf("%s\n", p_texture->test);
	//printf("%s | %s\n", p_texture->filepath, tex->filepath);
	/*if (tex->filepath != p_texture->filepath) {
		p_texture->ShowTexture();
		tex = p_texture;
	}*/
	

	switch (selectedFigureType)
	{
	case cube:
		M = rotate(M, rotationAngle, rotationAxis);
		M = translate(M, position);
		M = scale(M, size);
		p_texture->ShowTexture(playerPosition, M, playerPosOrigin);

		M = playerPosition*modelMatrix;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(value_ptr(M));
		glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); //Podczas rysowania u¿ywaj tablicy wsp. teksturowania
		glEnableClientState(GL_NORMAL_ARRAY); //Podczas rysowania u¿ywaj tablicy wektorów normalnych

		glVertexPointer(3, GL_FLOAT, 0, objV.data()); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
		glTexCoordPointer(2, GL_FLOAT, 0, objT.data()); //Ustaw tablicê myCubeTexCoords jako tablicê wsp. teksturowania
		glNormalPointer(GL_FLOAT, 0, objN.data()); //Ustaw tablicê myCubeVertices jako tablicê wektorów normalnych - tutaj akurat wsp. wierzcho³ka=suma wektorów normalnych œcian s¹siaduj¹cych
		glDrawArrays(GL_TRIANGLES, 0, objV.size()); //Rysuj model

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		p_texture->EndTexture();
		break;

	case stairs:
		M = playerPosition*modelMatrix;
		M = rotate(M, rotationAngle, rotationAxis);
		M = translate(M, position);
		M = scale(M, size);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(value_ptr(M));

		glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); //Podczas rysowania u¿ywaj tablicy wsp. teksturowania
		//glEnableClientState(GL_NORMAL_ARRAY); //Podczas rysowania u¿ywaj tablicy wektorów normalnych

		glVertexPointer(3, GL_FLOAT, 0, stairsVertices); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
		glTexCoordPointer(2, GL_FLOAT, 0, stairsTexCoords); //Ustaw tablicê myCubeTexCoords jako tablicê wsp. teksturowania
		//glNormalPointer(GL_FLOAT, 0, stairsVertices); //Ustaw tablicê myCubeVertices jako tablicê wektorów normalnych - tutaj akurat wsp. wierzcho³ka=suma wektorów normalnych œcian s¹siaduj¹cych
		glDrawArrays(GL_QUADS, 0, stairsVertexCount); //Rysuj model
													  //Posprz¹taj po sobie
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//glEnableClientState(GL_NORMAL_ARRAY);

		break;
	}
}
