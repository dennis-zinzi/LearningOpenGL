#include "Shader.h"

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath){
	//Read Shader files as list of lines
	string vertPath = SHADER_PATH + string(vertexPath),
		fragPath = SHADER_PATH + string(fragmentPath);

	vector<string> vertexCodeVec = FileIO::ReadShaderFile(vertPath);
	vector<string> fragmentCodeVec = FileIO::ReadShaderFile(fragPath);

	//Compile shaders
	CompileShader(FileIO::vectorToString(vertexCodeVec, '\n').c_str(), GL_VERTEX_SHADER, vertexShader);
	CompileShader(FileIO::vectorToString(fragmentCodeVec, '\n').c_str(), GL_FRAGMENT_SHADER, fragmentShader);

	//Link program
	LinkProgram();
}

Shader::~Shader(){
	glDeleteProgram(shaderProgram);
}

void Shader::CompileShader(const GLchar *shaderSrc, GLenum shaderType, GLuint &shader){
	if(shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER){
		cout << "ERROR::SHADER::INVALID_TYPE" << endl;
		shader = -1;
		return;
	}

	//Create OpenGL Shader from source
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSrc, nullptr);
	//Compile Shader
	glCompileShader(shader);

	GLint success;
	GLchar infoLog[512];

	//Check for successful compilation
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success){
		//Get error log
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		cout << "ERROR::SHADER::" << shaderType << "::COMPILE_FAIL: " << infoLog << endl;

		shader = -1;
	}
}


void Shader::LinkProgram(){
	//Create OpenGL Shader Program
	shaderProgram = glCreateProgram();

	///Attach shaders to program
	//Attach vertex shader (if valid)
	if(vertexShader == -1){
		cout << "Error with vertex shader" << endl;
		shaderProgram = -1;
		return;
	}
	else{
		glAttachShader(shaderProgram, vertexShader);
	}

	//Attach fragment shader (if valid)
	if(fragmentShader == -1){
		cout << "Error with fragment shader" << endl;
		shaderProgram = -1;
		return;
	}
	else{
		glAttachShader(shaderProgram, fragmentShader);
	}

	//Link program
	glLinkProgram(shaderProgram);

	GLint success;
	GLchar infoLog[512];

	//Check for successful linking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success){
		//Get error log
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINK_FAIL: \n" << infoLog << endl;

		shaderProgram = -1;
	}

	//Delete/unbind shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
