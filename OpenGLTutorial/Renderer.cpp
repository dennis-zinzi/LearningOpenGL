#include "Renderer.h"

Renderer::Renderer(){
	SDL_Init(SDL_INIT_EVERYTHING);

	//Set OpenGL attribs
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	screen = SDL_CreateWindow("OpenGL Learning", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(screen, -1, 0);
	context = SDL_GL_CreateContext(screen);
	IMG_Init(IMG_INIT_JPG);
	IMG_Init(IMG_INIT_PNG);


	//Enables GLEW + new OpenGL functions & pointers
	glewExperimental = GL_TRUE;


	if(GLEW_OK != glewInit()){
		cout << "Failed to initialize GLEW" << endl;
		//return EXIT_FAILURE;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//OpenGL
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 500.0f);
	//glMatrixMode(GL_MODELVIEW);

	//Enable Alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Enable depth test (Causes z-fighting)
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);

	////Enable texture mapping
	//glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Don't show mouse + helps with mouse motion
	SDL_SetRelativeMouseMode(SDL_TRUE);
}


Renderer::~Renderer(){
	SDL_GL_DeleteContext(context);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);

	SDL_Quit();
}



void Renderer::RenderScene(){
	SDL_GL_SwapWindow(screen);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Move mouse to center of screen
	SDL_WarpMouseInWindow(screen, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}



void Renderer::compileShader(const GLchar *&shaderSrc, const ShaderType shaderType, GLuint &shader){
	//Create OpenGL Shader from source
	shader = glCreateShader((shaderType == VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER));
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
		cout << "ERROR::SHADER::" << (shaderType == VERTEX ? "VERTEX" : "FRAGMENT") << "::COMPILE_FAIL: " << infoLog << endl;
		
		shader = -1;
	}
}



void Renderer::createShaderProgram(GLuint &shaderProgram, GLuint &vertexShader, GLuint &fragShader){
	//Create OpenGL Shader Program
	shaderProgram = glCreateProgram();

	///Attach shaders to program
	//Attach vertex shader (if valid)
	if(vertexShader == -1){
		cout << "Error with shader" << endl;
	}
	else{
		glAttachShader(shaderProgram, vertexShader);
	}

	//Attach fragment shader (if valid)
	if(fragShader == -1){
		cout << "Error with shader" << endl;
	}
	else{
		glAttachShader(shaderProgram, fragShader);
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
	glDeleteShader(fragShader);

}


void Renderer::PrepTriangle(GLTriangleData &tri, const GLfloat vertices[]){
	//Generate VAO and VBO
	glGenVertexArrays(1, &(tri.VAO));
	glGenBuffers(1, &(tri.VBO));

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(tri.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, tri.VBO);

	//Put data in buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Tell OpenGL how vertex data should be interpreted
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void Renderer::DrawTriangle(GLTriangleData &triData, GLuint &shaderProgram){
	glUseProgram(shaderProgram);
	glBindVertexArray(triData.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Renderer::DrawTriangle(GLTriangleData & triData, Shader &shader){
	shader.UseProgram();
	glBindVertexArray(triData.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Renderer::UnloadTri(GLTriangleData &tri){
	// De-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &(tri.VAO));
	glDeleteBuffers(1, &(tri.VBO));
}




void Renderer::PrepQuad(GLQuadData &quad, const GLfloat vertices[], const GLuint indices[]){
	//Generate VAO and VBO
	glGenVertexArrays(1, &(quad.VAO));
	glGenBuffers(1, &(quad.VBO));

	//// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(quad.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, quad.VBO);

	////Put data in buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	////Tell OpenGL how vertex data should be interpreted
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	////Tell OpenGL how color data should be interpreted
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	////Tell OpenGL how texture data should be interpreted
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);


	//// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	//// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);

	//// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// note that this is allowed, the call to glVertexAttribPointer registered EBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::PrepQuadTexture(GLQuadData &quad, const string &texImgPath){
	GLint width, height;
	glGenTextures(1, &(quad.texture));
	glBindTexture(GL_TEXTURE_2D, quad.texture);

	//Tell OpenGL how to treat texCoords
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Apply Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	string imgPath = IMAGE_PATH + texImgPath;

	unsigned char *img = SOIL_load_image(imgPath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	
	//Generate image mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unload texture
	SOIL_free_image_data(img);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Renderer::DrawQuad(GLQuadData &quadData, Shader &shader){
	shader.UseProgram();

	mat4 transform;
	transform = translate(transform, vec3(0.5f, 0.5f, 0.0f));
	//transform = rotate(transform, (GLfloat)SDL_GetTicks() * 2.0f, vec3(0.0f, 0.0f, 1.0f));

	GLint transformLoc = glGetUniformLocation(shader.GetShaderProgram(), "mvp");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, quadData.texture);
	glUniform1i(glGetUniformLocation(shader.GetShaderProgram(), "tex"), 0);

	glBindVertexArray(quadData.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void Renderer::UnloadQuad(GLQuadData &quadData){
	glDeleteVertexArrays(1, &(quadData.VAO));
	glDeleteBuffers(1, &(quadData.VBO));
}



void Renderer::DrawCube(GLQuadData &quadData, const GLfloat vertices[], const mat4 &projection, Shader &shader){
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, quadData.texture);
	glUniform1i(glGetUniformLocation(shader.GetShaderProgram(), "tex"), 0);


	shader.UseProgram();


	mat4 model, view;
	model = rotate(model, (GLfloat)(SDL_GetTicks())*0.001f, vec3(0.5f, 1.0f, 0.0f));
	view = translate(view, vec3(0.0f, 0.0f, -3.0f));
	//model = rotate(model, 0.5f, vec3(1.0f, 0.0f, 0.0f));
	//view = translate(view, vec3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, -700.0f));

	mat4 mvp = projection * view * model;
	GLint mvpLoc = glGetUniformLocation(shader.GetShaderProgram(), "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, value_ptr(mvp));

	glBindVertexArray(quadData.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}



void Renderer::DrawCubes(GLQuadData &quadData, const vector<vec3> &cubes, const mat4 &view, const mat4 &projection, Shader &shader){
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, quadData.texture);
	//glUniform1i(glGetUniformLocation(shader.GetShaderProgram(), "tex"), 0);


	shader.UseProgram();


	mat4 mvp = projection * view;// *model;
	GLint mvpLoc = glGetUniformLocation(shader.GetShaderProgram(), "mvp");


	glBindVertexArray(quadData.VAO);


	mat4 model;


	for(size_t i = 0; i < cubes.size(); i++){
		model = translate(model, cubes[i]);
		GLfloat angle = 20.0f * i;
		model = rotate(model, angle, vec3(1.0f, 0.3f, 0.5f));

		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, value_ptr((mvp * model)));
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindVertexArray(0);
}

void Renderer::DrawLighingCubes(GLQuadData &lightQuad, const vector<vec3> &cubes, vec3 pointLightPositions[], const vec3 &lightPos, const vec3 &camPos, const vec3 &camFront, const mat4 &view, const mat4 &projection, const Shader &lightShader){
	lightShader.UseProgram();

	const GLuint shaderProg = lightShader.GetShaderProgram();

	//GLint lightPosLoc = glGetUniformLocation(shaderProg, "light.pos");
	GLint viewPosLoc = glGetUniformLocation(shaderProg, "viewPos");
	glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);
	//glUniform3f(lightPosLoc, camPos.x, camPos.y, camPos.z);
	//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

	
	//Directional Lighting
	glUniform3f(glGetUniformLocation(shaderProg, "dirLight.dir"),
		-0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(shaderProg, "dirLight.ambient"),
		0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProg, "dirLight.diffuse"),
		0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(shaderProg, "dirLight.specular"),
		0.5f, 0.5f, 0.5f);


	///Point Lights
	//Point Light 0
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[0].pos"),
		pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[0].ambient"),
		0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[0].diffuse"),
		0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[0].specular"),
		1.0f, 1.0f, 1.0f);

	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[0].constant"),
		1.0f);
	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[0].linear"),
		0.09f);
	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[0].quadratic"),
		0.032f);

	//Point Light 1
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[1].pos"),
		pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[1].ambient"),
		0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[1].diffuse"),
		0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[1].specular"),
		1.0f, 1.0f, 1.0f);

	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[1].constant"),
		1.0f);
	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[1].linear"),
		0.09f);
	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[1].quadratic"),
		0.032f);

	//Point Light 2
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[2].pos"),
		pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[2].ambient"),
		0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[2].diffuse"),
		0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[2].specular"),
		1.0f, 1.0f, 1.0f);

	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[2].constant"),
		1.0f);
	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[2].linear"),
		0.09f);
	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[2].quadratic"),
		0.032f);


	//Point Light 3
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[3].pos"),
		pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[3].ambient"),
		0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[3].diffuse"),
		0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shaderProg, "pointLights[3].specular"),
		1.0f, 1.0f, 1.0f);

	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[3].constant"),
		1.0f);
	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[3].linear"),
		0.09f);
	glUniform1f(glGetUniformLocation(shaderProg, "pointLights[3].quadratic"),
		0.032f);
	

	//Spotlights
	glUniform3f(glGetUniformLocation(shaderProg, "spotLight.pos"),
		camPos.x, camPos.y, camPos.z);
	glUniform3f(glGetUniformLocation(shaderProg, "spotLight.dir"),
		camFront.x, camFront.y, camFront.z);
	glUniform1f(glGetUniformLocation(shaderProg, "spotLight.cutOff"),
		cos(radians(12.5f)));
	glUniform1f(glGetUniformLocation(shaderProg, "spotLight.outerCutOff"),
		cos(radians(17.5f)));

	glUniform3f(glGetUniformLocation(shaderProg, "spotLight.ambient"),
		0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shaderProg, "spotLight.diffuse"),
		1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProg, "spotLight.specular"),
		1.0f, 1.0f, 1.0f);

	glUniform1f(glGetUniformLocation(shaderProg, "spotLight.constant"),
		1.0f);
	glUniform1f(glGetUniformLocation(shaderProg, "spotLight.linear"),
		0.09f);
	glUniform1f(glGetUniformLocation(shaderProg, "spotLight.quadratic"),
		0.032f);

	
	

	//image at texture0
	glUniform1i(glGetUniformLocation(shaderProg, "material.diffuse"),
		0);
	//specular image at texture1
	glUniform1i(glGetUniformLocation(shaderProg, "material.specular"),
		1);
	glUniform1f(glGetUniformLocation(shaderProg, "material.shininess"),
		32.0f);

	
	//mat4 model;
	GLint modelLoc = glGetUniformLocation(shaderProg, "model");
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

	mat4 mvp = projection * view;
	GLint mvpLoc = glGetUniformLocation(shaderProg, "mvp");
	//glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lightQuad.texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lightQuad.specularTexture);


	glBindVertexArray(lightQuad.VAO);
	DrawLightCubes(cubes, modelLoc, mvpLoc, mvp);
	glBindVertexArray(0);

}

void Renderer::DrawLamp(GLQuadData &lampQuad, const vec3 &lampPos, const vec3 pointLightPositions[], const mat4 &view, const mat4 &projection, const Shader &lampShader){
	lampShader.UseProgram();

	mat4 model;
	model = translate(model, lampPos);
	model = scale(model, vec3(0.2f, 0.2f, 0.2f));

	mat4 mvp = projection * view * model;
	GLint mvpLoc = glGetUniformLocation(lampShader.GetShaderProgram(), "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, value_ptr(mvp));


	glBindVertexArray(lampQuad.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	for(size_t i = 0; i < 4; i++){
		model = mat4();
		model = translate(model, pointLightPositions[i]);
		model = scale(model, vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, value_ptr(projection * view * model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

void Renderer::PrepLightmapTexture(GLQuadData &quad, const string &texImgPath, const string &texSpecularImgPath){
	glGenTextures(1, &(quad.texture));
	glGenTextures(1, &(quad.specularTexture));

	GLint width, height;

	//Prep image
	string imgPath = IMAGE_PATH + texImgPath;

	unsigned char *img = SOIL_load_image(imgPath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glBindTexture(GL_TEXTURE_2D, quad.texture);

	//Tell OpenGL how to treat texCoords
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Apply Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//Generate image mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unload texture
	SOIL_free_image_data(img);
	glBindTexture(GL_TEXTURE_2D, 0);


	//Prep specular image
	string specularImgPath = IMAGE_PATH + texSpecularImgPath;

	img = SOIL_load_image(specularImgPath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glBindTexture(GL_TEXTURE_2D, quad.specularTexture);

	//Tell OpenGL how to treat texCoords
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Apply Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//Generate image mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unload texture
	SOIL_free_image_data(img);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::DrawLightCubes(const vector<vec3>& cubes, const GLint &modelLoc, const GLint & mvpLoc, const mat4 & mvp){
	//mat4 model;
	for(size_t i = 0; i < cubes.size(); i++){
		mat4 model;
		model = translate(model, cubes[i]);
		GLfloat angle = 20.0f * i;
		model = rotate(model, angle, vec3(1.0f, 0.3f, 0.5f));

		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, value_ptr((mvp * model)));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void Renderer::DrawModel(const Model &model, const Shader &shader, const mat4 &view, const mat4 &projection){
	shader.UseProgram();
	
	GLuint shaderProg = shader.GetShaderProgram();

	mat4 modelMat;
	modelMat = translate(modelMat, vec3(0.0f, -1.75f, -1.0f));
	modelMat = scale(modelMat, vec3(0.2f, 0.2f, 0.2f));

	mat4 mvp = projection * view * modelMat;
	GLint mvpLoc = glGetUniformLocation(shaderProg, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, value_ptr(mvp));

	model.Draw(shader);
}


GLuint Renderer::LoadTexture(const string &tex){
	GLuint texture;

	GLint width, height;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	string imgPath = IMAGE_PATH + tex;

	unsigned char *img = SOIL_load_image(imgPath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

	//Generate image mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Tell OpenGL how to treat texCoords
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Apply Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//Unload texture
	SOIL_free_image_data(img);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

GLuint Renderer::LoadCubeTexture(const vector<string> &faces){
	GLuint texture;

	GLint width, height;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	string imgPath;

	unsigned char *img;
	
	for(GLuint i = 0; i < faces.size(); i++){
		imgPath = IMAGE_PATH + faces[i];

		img = SOIL_load_image(imgPath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		SOIL_free_image_data(img);
	}
	
	//Tell OpenGL how to treat texCoords
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//Apply Linear Filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//Unload texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return texture;
}



void Renderer::DrawSkybox(const GLuint &VAO, const GLuint &tex, const Shader &shader, const mat4 &view, const mat4 &projection){
	shader.UseProgram();
	
	glUniformMatrix4fv(glGetUniformLocation(shader.GetShaderProgram(), "vp"),
		1, GL_FALSE, value_ptr(projection * mat4(mat3(view))));

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
