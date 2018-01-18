#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;


out vec2 myTex;

uniform mat4 mvp;

void main(){
	gl_Position = mvp * vec4(pos, 1.0f);

	//Minus as OpenGl coords inverted from most pics
	myTex = vec2(texCoord.x, 1.0 - texCoord.y);
}
