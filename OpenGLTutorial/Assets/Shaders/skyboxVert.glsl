#version 330 core

layout(location = 0) in vec3 pos;

out vec3 texCoords;

uniform mat4 vp;

void main(){
	gl_Position = vec4(vp * vec4(pos, 1.0f)).xyww;
	texCoords = pos;
}
