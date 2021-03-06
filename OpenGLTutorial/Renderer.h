#pragma once

//SDL includes
#include <SDL.h>
#include <SDL_image.h>

//OpenGL includes
#include <GL/glew.h>


#include "common.h"
#include "Shader.h"
#include "Model.h"

#include <iostream>
#include <vector>
//#include <string>

//SOIL include
#include <SOIL2.h>
#pragma comment(lib, "SOIL.lib")

//glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::translate;
using glm::rotate;
using glm::scale;
using glm::value_ptr;
using glm::radians;


using std::cout;
using std::endl;
using std::vector;
//using std::string;

enum ShaderType{
	VERTEX = GL_VERTEX_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER
};


struct GLTriangleData{
	GLuint VBO;
	GLuint VAO;
};

struct GLQuadData{
	GLuint VBO;
	GLuint VAO;
	//GLuint EBO;

	GLuint texture;
	GLuint specularTexture;
};



class Renderer{
	public:
		Renderer();
		~Renderer();

		void RenderScene();

		void compileShader(const GLchar *&shaderSrc, const ShaderType shaderType, GLuint &shader);
		void createShaderProgram(GLuint &shaderProgram, GLuint &vertexShader, GLuint &fragShader);

		void PrepTriangle(GLTriangleData &tri, const GLfloat vertices[]);
		void DrawTriangle(GLTriangleData &triData, GLuint &shaderProgram);
		void DrawTriangle(GLTriangleData &triData, Shader &shader);

		void UnloadTri(GLTriangleData &tri);


		void PrepQuad(GLQuadData &quad, const GLfloat vertices[], const GLuint indices[]);
		void PrepQuadTexture(GLQuadData &quad, const string &texImgPath);
		void DrawQuad(GLQuadData &quadData, Shader &shader);
		void UnloadQuad(GLQuadData &quadData);

		void DrawCube(GLQuadData &quadData, const GLfloat vertices[], const mat4 &projection, Shader &shader);
		void DrawCubes(GLQuadData &quadData, const vector<vec3> &cubes, const mat4 &view, const mat4 &projection, Shader &shader);

		void DrawLighingCubes(GLQuadData &lightQuad, const vector<vec3> &cubes, vec3 pointLightPositions[], const vec3 &lightPos,
			const vec3 &camPos, const vec3 &camFront,
			const mat4 &view, const mat4 &projection, 
			const Shader &lightShader);
		void DrawLamp(GLQuadData &lampQuad, const vec3 &lampPos, const vec3 pointLightPositions[], const mat4 &view, const mat4 &projection, const Shader &lampShader);

		void PrepLightmapTexture(GLQuadData &quad, const string &texImgPath, const string &texSpecularImgPath);
		void DrawLightCubes(const vector<vec3> &cubes, const GLint &modelLoc, const GLint &mvpLoc, const mat4 &mvp);

		void DrawModel(const Model &model, const Shader &shader, const mat4 &view, const mat4 &projection);

		GLuint LoadTexture(const string &tex);
		GLuint LoadCubeTexture(const vector<string> &faces);

		void DrawSkybox(const GLuint &VAO, const GLuint &tex, const Shader &shader, const mat4 &view, const mat4 &projection);


	private:
		SDL_Window *screen;
		SDL_Renderer *renderer;
		SDL_GLContext context;
};