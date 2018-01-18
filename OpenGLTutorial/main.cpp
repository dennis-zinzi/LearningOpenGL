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


	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection
	
	//GLfloat vertices[] = {
	//-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f,
	//0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 0.0f,
	//0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
	//0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
	//-0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
	//-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f,

	//-0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
	//0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
	//0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,
	//0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,
	//-0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 1.0f,
	//-0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,

	//-0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
	//-0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
	//-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
	//-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
	//-0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
	//-0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,

	//0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
	//0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
	//0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
	//0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
	//0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
	//0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,

	//-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
	//0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
	//0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
	//0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
	//-0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
	//-0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,

	//-0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
	//0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
	//0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
	//0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
	//-0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
	//-0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f
	//};
	

	// use with Perspective Projection
	GLfloat vertices[] = {
		//back
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		//front
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		//left
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		//right
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		//bottom
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		//top
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};


	GLQuadData quad;
	//renderer->PrepQuad(quad, vertices, indices);
	//Generate VAO and VBO
	glGenVertexArrays(1, &(quad.VAO));
	glGenBuffers(1, &(quad.VBO));
	
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(quad.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, quad.VBO);

	//Put data in buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	//Tell OpenGL how vertex data should be interpreted
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	//Tell OpenGL how texture data should be interpreted
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO,
	// but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray
	// anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
	// attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//Set texture
	renderer->PrepQuadTexture(quad, "batman.jpg");


	mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f);
	//projection = glm::ortho(0.0f, (GLfloat)WINDOW_WIDTH, 0.0f, (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f);

	bool running = true;

	while(running){
		running = CheckInputs();
		
		//draw triangle
		//renderer->DrawTriangle(tri, simple);
		//Draw quad
		//renderer->DrawQuad(quad, simple);
		renderer->DrawCube(quad, vertices, projection, simple);

		renderer->RenderScene();
	}


	//renderer->UnloadTri(tri);
	renderer->UnloadQuad(quad);
	delete renderer;

	return 0;
}
