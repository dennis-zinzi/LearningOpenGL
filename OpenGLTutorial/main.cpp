#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
				float xOffset = (float)event.motion.xrel,
					yOffset = (float)event.motion.yrel;

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

	Camera *camera = new Camera(vec3(0.0f, 0.0f, 3.0f));

	mat4 projection;

	bool running = true;

	float msec = 0.0f,
		newTime = 0.0f;

	Shader modelShade("modelLoadVert.glsl", "modelLoadFrag.glsl");
	Model model("nanosuit.obj");
	
	Shader skyboxShader("skyboxVert.glsl", "skyboxFrag.glsl");
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	GLQuadData skyQuad;
	glGenVertexArrays(1, &(skyQuad.VAO));
	glGenBuffers(1, &(skyQuad.VBO));

	glBindVertexArray(skyQuad.VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, skyQuad.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	vector<string> skyFaces = {
		"skybox/right.tga",
		"skybox/left.tga",
		"skybox/top.tga",
		"skybox/bottom1.png",
		"skybox/back.tga",
		"skybox/front.tga",
	};

	skyQuad.texture = renderer->LoadCubeTexture(skyFaces);
	

	//Enables wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while(running){
		newTime = (float)SDL_GetTicks();
		running = CheckInputs(camera, newTime - msec);
		msec = newTime;
		
		projection = glm::perspective(camera->GetZoom(), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f);

		//Draw Objects
		renderer->DrawSkybox(skyQuad.VAO, skyQuad.texture, skyboxShader, camera->GetViewMatrix(), projection);
		renderer->DrawModel(model, modelShade, camera->GetViewMatrix(), projection);


		renderer->RenderScene();
	}

	renderer->UnloadQuad(skyQuad);

	delete renderer;

	return 0;
}
