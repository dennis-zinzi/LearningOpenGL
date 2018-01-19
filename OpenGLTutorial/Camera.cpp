#include "Camera.h"

Camera::Camera(vec3 position, vec3 worldUp,	float yaw, float pitch, float speed, float sensitivity, float zoom, vec3 front, vec3 right){
	this->position = position;
	this->worldUp = worldUp;
	
	this->yaw = yaw;
	this->pitch = pitch;

	this->speed = speed;
	this->sensitivity = sensitivity;
	this->zoom = zoom;

	this->front = front;
	this->right = right;

	UpdateCamera();
}

Camera::Camera(float x, float y, float z, float fwdX, float fwdY, float fwdZ, float yaw, float pitch, float speed, float sensitivity, float zoom, vec3 front, vec3 right){
	position = vec3(x, y, z);
	worldUp = vec3(fwdX, fwdY, fwdZ);

	this->yaw = yaw;
	this->pitch = pitch;

	this->speed = speed;
	this->sensitivity = sensitivity;
	this->zoom = zoom;

	this->front = front;
	this->right = right;

	UpdateCamera();
}

void Camera::UpdateCameraPos(Camera_Movement dir, GLfloat time){
	GLfloat vel = speed * time;

	if(dir == FORWARD){
		position += front * vel;
	}
	else if(dir == BACKWARD){
		position -= front * vel;
	}
		
	if(dir == RIGHTWARD){
		position += right * vel;
	}
	else if(dir == LEFTWARD){
		position -= right * vel;
	}
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch){
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if(constrainPitch){
		if(pitch > 90.0f){
			pitch = 90.0f;
		}
		if(pitch < -90.0f){
			pitch = -90.0f;
		}
	}

	UpdateCamera();
}

void Camera::ProcessMouseScroll(float yOffset){
	if(zoom >= 1.0f && zoom <= 45.0f){
		zoom -= yOffset;
	}

	if(zoom < 1.0f){
		zoom = 1.0f;
	}

	if(zoom > 45.0f){
		zoom = 45.0f;
	}
}


void Camera::UpdateCamera(){
	float yawRad = radians(yaw),
		pitchRad = radians(pitch);

	vec3 f;
	f.x = cos(yawRad * cos(pitchRad));
	f.y = sin(pitchRad);
	f.z = sin(yawRad) * cos(pitchRad);

	front = normalize(f);
	right = normalize(cross(front, worldUp));
	forward = normalize(cross(right, front));
}
