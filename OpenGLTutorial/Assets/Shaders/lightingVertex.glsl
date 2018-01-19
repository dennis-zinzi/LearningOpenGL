#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out vec3 myNormal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 mvp;


void main(){
	gl_Position = mvp * vec4(pos, 1.0f);
	fragPos = vec3(model * vec4(pos, 1.0f));
	myNormal = mat3(transpose(inverse(model))) * normal;
}
