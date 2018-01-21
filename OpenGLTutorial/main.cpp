#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"


bool CheckInputs(Camera *camera, const float time){
	SDL_Event event;

	//Runs whilst there is an event being queued
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN: {
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:
					case SDLK_x:
						return false;
					default:
						break;
				}
				break;
			}
			case SDL_MOUSEMOTION: {
				float xOffset = event.motion.xrel,
					yOffset = event.motion.yrel;

				camera->UpdateCameraLook(xOffset, -yOffset);

				break;
			}
			case SDL_MOUSEWHEEL: {
				camera->UpdateCameraZoom(event.wheel.y * 0.1f);
				
				break;
			}
		}
	}

	const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

	// Move player
	if((keyboard_state[SDL_SCANCODE_W] && !keyboard_state[SDL_SCANCODE_S])
		|| (keyboard_state[SDL_SCANCODE_UP] && !keyboard_state[SDL_SCANCODE_DOWN])) {
		//Apply force upwards
		camera->UpdateCameraPos(FORWARD, time * 0.001f);
	}
	else if((keyboard_state[SDL_SCANCODE_S] && !keyboard_state[SDL_SCANCODE_W])
		|| (keyboard_state[SDL_SCANCODE_DOWN] && !keyboard_state[SDL_SCANCODE_UP])) {
		//Apply force downwards
		camera->UpdateCameraPos(BACKWARD, time* 0.001f);
	}

	if((keyboard_state[SDL_SCANCODE_D] && !keyboard_state[SDL_SCANCODE_A])
		|| (keyboard_state[SDL_SCANCODE_RIGHT] && !keyboard_state[SDL_SCANCODE_LEFT])) {
		//Apply force rightwards
		camera->UpdateCameraPos(RIGHTWARD, time* 0.001f);
	}
	else if((keyboard_state[SDL_SCANCODE_A] && !keyboard_state[SDL_SCANCODE_D])
		|| (keyboard_state[SDL_SCANCODE_LEFT] && !keyboard_state[SDL_SCANCODE_RIGHT])) {
		//Apply force leftwards
		camera->UpdateCameraPos(LEFTWARD, time* 0.001f);
	}	
		


	return true;
}

int main(int argc, char **argv){
	Renderer *renderer = new Renderer;

	//Shader simple("vertexShader.glsl", "fragmentShader.glsl");


	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection

	// use with Perspective Projection
	GLfloat vertices[] = {
		//back
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,

		//front
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,

		//left
		-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,

		//right
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,

		//bottom
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,

		//top
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f
	};

	vector<vec3> cubePositions = {
		vec3(0.0f, 0.0f, 0.0f),
		vec3(2.0f, 5.0f, -15.0f),
		vec3(-1.5f, -2.0f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f, -0.4f, -3.5f),
		vec3(-1.7f, 3.0f, -7.5f),
		vec3(1.3f, -2.0f, -2.5f),
		vec3(1.5f, 2.0f, -2.5f),
		vec3(1.5f, 0.2f, -1.5f),
		vec3(-1.3f, 1.0f, -1.5f),
	};

	//Set texture
	//renderer->PrepQuadTexture(quad, "batman.jpg");

	Camera *camera = new Camera(vec3(0.0f, 0.0f, 3.0f));

	mat4 projection;


	Shader lightingShader("lightingVertex.glsl", "lightingFragment.glsl");
	Shader lampShader("lampVertex.glsl", "lampFrag.glsl");
	vec3 lightPos(1.2f, 1.0f, 2.0f);

	GLQuadData lightQuad;
	//lightQuad.VBO = quad.VBO;
	//renderer->PrepQuad(quad, vertices, indices);
	//Generate VAO and VBO
	glGenVertexArrays(1, &(lightQuad.VAO));
	glGenBuffers(1, &(lightQuad.VBO));

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(lightQuad.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightQuad.VBO);

	//Put data in buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	//Tell OpenGL how vertex data should be interpreted
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	//Tell OpenGL how normal data is parsed
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO,
	// but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray
	// anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
	// attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	bool running = true;

	float msec = 0.0f,
		newTime = 0.0f;

	while(running){
		newTime = (float)SDL_GetTicks();
		running = CheckInputs(camera, newTime - msec);
		msec = newTime;

		//lightPos.x -= 0.005f;
		//lightPos.z -= 0.005f;
		
		projection = glm::perspective(camera->GetZoom(), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f);

		//renderer->DrawCubes(quad, cubePositions, camera->GetVewMatrix(), projection, simple);
		renderer->DrawLighingCubes(lightQuad, lightPos, camera->GetPos(), camera->GetViewMatrix(), projection, lightingShader);
		renderer->DrawLamp(lightQuad, lightPos, camera->GetViewMatrix(), projection, lampShader);

		renderer->RenderScene();
	}


	//renderer->UnloadTri(tri);
	renderer->UnloadQuad(lightQuad);
	delete renderer;

	return 0;
}
