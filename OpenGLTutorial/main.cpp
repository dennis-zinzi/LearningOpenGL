#include "Renderer.h"
#include "Shader.h"


bool CheckInputs(){
	SDL_Event event;

	//Runs whilst there is an event being queued
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
			{
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:
					case SDLK_x:
						return false;
					default:
						break;
				}
				break;
			}
		}
	}

	return true;
}

int main(int argc, char **argv){
	Renderer *renderer = new Renderer;

	Shader simple("vertexShader.glsl", "fragmentShader.glsl");

	//Triangle verts
	//float vertices[] = {
	//	-0.5f, -0.5f, 0.0f, /*left*/ 1.0f, 0.0f, 0.0f, 1.0f, /*color*/ 
	//	0.5f, -0.5f, 0.0f, /*right*/ 0.0f, 1.0f, 0.0f, 1.0f, /*color*/
	//	0.0f, 0.5f, 0.0f,  /*top*/ 0.0f, 0.0f, 1.0f, 1.0f	/*color*/
	//};

	//Rect vert
	//float vertices[] = {
	//	//Pos				//Color						//Tex coords
	//	0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, //top - right
	//	0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f, //bottom - right
	//	-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 0.0f, //bottom - left
	//	-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f	//top - left
	//};

	//GLuint indices[] = {
	//	0, 1, 3, //1st tri
	//	1, 2, 3 //2nd tri
	//};


	//GLQuadData quad;

	////Generate VAO and VBO
	//glGenVertexArrays(1, &(quad.VAO));
	//glGenBuffers(1, &(quad.VBO));
	//glGenBuffers(1, &(quad.EBO));

	//// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	//glBindVertexArray(quad.VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, quad.VBO);

	////Put data in buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	////Tell OpenGL how vertex data should be interpreted
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	////Tell OpenGL how color data should be interpreted
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	////Tell OpenGL how texture data should be interpreted
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);

	//// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// note that this is allowed, the call to glVertexAttribPointer registered EBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	//// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);

	///*GLuint texture;*/

	//int width, height;
	//glGenTextures(1, &(quad.texture));
	//glBindTexture(GL_TEXTURE_2D, quad.texture);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//string imgPath = IMAGE_PATH + string("batman.jpg");

	//unsigned char *img = SOIL_load_image(imgPath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//glGenerateMipmap(GL_TEXTURE_2D);

	//SOIL_free_image_data(img);
	//glBindTexture(GL_TEXTURE_2D, 0);


	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		//Pos					//Color						//Tex coords
		0.25f, 0.25f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, //top - right
		0.25f, -0.25f, 0.0f,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f, //bottom - right
		-0.25f, -0.25f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 0.0f, //bottom - left
		-0.25f, 0.25f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f	//top - left
	};
	GLuint indices[] = {
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};


	GLQuadData quad;
	//renderer->PrepQuad(quad, vertices, indices);
	//Generate VAO and VBO
	glGenVertexArrays(1, &(quad.VAO));
	glGenBuffers(1, &(quad.VBO));
	glGenBuffers(1, &(quad.EBO));
	
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(quad.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, quad.VBO);

	//Put data in buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Tell OpenGL how vertex data should be interpreted
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Tell OpenGL how color data should be interpreted
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Tell OpenGL how texture data should be interpreted
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO,
	// but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray
	// anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
	// attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// note that this is allowed, the call to glVertexAttribPointer registered EBO as the 
	// vertex attribute's bound element buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//Set texture
	renderer->PrepQuadTexture(quad, "luigi.png");


	bool running = true;

	while(running){
		running = CheckInputs();
		
		//draw triangle
		//renderer->DrawTriangle(tri, simple);
		//Draw quad
		renderer->DrawQuad(quad, simple);

		renderer->RenderScene();
	}


	//renderer->UnloadTri(tri);
	renderer->UnloadQuad(quad);
	delete renderer;

	return 0;
}
