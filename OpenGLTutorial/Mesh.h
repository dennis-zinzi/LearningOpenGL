#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"


using glm::mat4;
using glm::vec3;
using glm::vec2;


using std::cout;
using std::endl;
using std::vector;
using std::string;



struct Vertex{
	vec3 pos;
	vec3 normal;
	vec2 texCoords;
};


struct Texture{
	GLuint ID;
	string type;
	aiString path;
};




class Mesh{
	public:
		Mesh(const vector<Vertex> &vertices, const vector<GLuint> &indices, const vector<Texture> &textures);

		void Draw(const Shader &shader);

	private:
		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures;

		GLuint VAO, VBO, EBO;

		void SetupMesh();
};