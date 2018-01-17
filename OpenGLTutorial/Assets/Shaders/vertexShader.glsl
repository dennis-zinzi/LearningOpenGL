#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

out vec4 myColor;
out vec2 myTex;

uniform mat4 transform;

void main(){
	gl_Position = transform * vec4(pos, 1.0f);
	myColor = color;
	//Minus as OpenGl coords inverted from most pics
	myTex = vec2(texCoord.x, 1.0 - texCoord.y);
}
