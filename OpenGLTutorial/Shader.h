#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "common.h"
#include "FileIO.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::stringstream;


class Shader{
	public:
		Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
		~Shader();

		inline void UseProgram(){
			if(shaderProgram == -1){
				cout << "Error with program, unable to use" << endl;
				return;
			}

			glUseProgram(shaderProgram);
		}

		inline GLuint GetShaderProgram(){
			return shaderProgram;
		}

	private:
		GLuint vertexShader,
			fragmentShader,
			shaderProgram;
		
		void CompileShader(const GLchar *shaderSrc, GLenum shaderType, GLuint &shader);
		void LinkProgram();
};