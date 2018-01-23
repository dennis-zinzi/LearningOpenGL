#pragma once



#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SOIL2.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "common.h"



using std::cout;
using std::endl;
using std::vector;
using std::string;

using glm::mat4;
using glm::vec3;
using glm::vec2;

using Assimp::Importer;



class Model{
	public:
		Model(GLchar *path);

		GLint TexFromFile(const char *path, const string &dir);

		void Draw(const Shader &shader) const;


	private:
		vector<Mesh> meshes;
		vector<Texture> loadedTextures;
		string directory;

		void LoadModel(const string &path);
		void ProcessNode(aiNode *node, const aiScene *scene);
		Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
		vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, string namePattern);
		
};