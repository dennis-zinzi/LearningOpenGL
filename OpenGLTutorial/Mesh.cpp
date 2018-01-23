#include "Mesh.h"

Mesh::Mesh(const vector<Vertex> &vertices, const vector<GLuint> &indices, const vector<Texture> &textures){
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

void Mesh::Draw(const Shader &shader){
	GLuint diffuseNum = 1,
		specularNum = 1;

	const GLuint shaderProg = shader.GetShaderProgram();

	for(GLuint i = 0; i < textures.size(); i++){
		glActiveTexture(GL_TEXTURE0 + i);

		stringstream stream;
		string num;
		string name = textures[i].type;

		if(name == "texture_diffuse"){
			stream << diffuseNum++;
		}
		else if(name == "texture_specular"){
			stream << specularNum++;
		}

		num = stream.str();

		glUniform1i(glGetUniformLocation(shaderProg, (name + num).c_str()),
			i);

		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}

	glUniform1f(glGetUniformLocation(shaderProg, "material.shininess"),
		16.0f);

	//Draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//free textures
	for(GLuint i = 0; i < textures.size(); i++){
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::SetupMesh(){
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	//Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, normal)));

	//Vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, texCoords)));

	glBindVertexArray(0);
}
