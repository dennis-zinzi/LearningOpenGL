#pragma once

#include <vector>

#define GLEW_STATIC

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::vector;

using glm::mat4;
using glm::vec3;
using glm::translate;
using glm::rotate;
using glm::scale;
using glm::lookAt;
using glm::radians;
using glm::normalize;
using glm::cross;

enum Camera_Movement{
	FORWARD,
	LEFTWARD,
	RIGHTWARD,
	BACKWARD
};


class Camera{
	public:
		Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 worldUp = vec3(0.0f, 1.0f, 0.0f),
			float yaw = -90.0f, float pitch = 0.0f, 
			float speed = 6.0f, float sensitivity = 0.25f, float zoom = 45.0f,
			vec3 front = vec3(0.0f, 0.0f, -1.0f), vec3 right = vec3(0.0f, 0.0f, 0.0f));

		Camera(float x = 0.0f, float y = 0.0f, float z = 0.0f,
			float fwdX = 0.0f, float fwdY = 1.0f, float fwdZ = 0.0f,
			float yaw = 0.0f, float pitch = 0.0f,
			float speed = 6.0f, float sensitivity = 0.25f, float zoom = 45.0f,
			vec3 front = vec3(0.0f, 0.0f, -1.0f), vec3 right = vec3(0.0f, 0.0f, 0.0f));


		inline mat4 GetVewMatrix(){
			return lookAt(position, position + front, forward);
		}


		void UpdateCameraPos(Camera_Movement dir, GLfloat time);

		void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
		void ProcessMouseScroll(float yOffset);


		inline float GetZoom(){
			return zoom;
		}

		void UpdateCamera();


	private:
		float yaw,
			pitch;

		vec3 worldUp;
		vec3 position;
		vec3 forward;

		float speed;
		float sensitivity;
		float zoom; //fov

		vec3 front;
		vec3 right;
};
