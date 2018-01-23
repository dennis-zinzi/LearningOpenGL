#include "Model.h"

Model::Model(GLchar *path){
	string filePath = MODEL_PATH + string(path);
	LoadModel(filePath);
}

GLint Model::TexFromFile(const char * path, const string & dir){
	string fileName = string(path);
	fileName = dir + '/' + fileName;

	GLuint texID;
	glGenTextures(1, &texID);

	GLint width, height;

	//Prep image
	unsigned char *img = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	//Generate image mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Tell OpenGL how to treat texCoords
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Apply Linear Filtering MipMap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Unload texture
	SOIL_free_image_data(img);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

void Model::Draw(const Shader &shader) const{
	for(auto mesh : meshes){
		mesh.Draw(shader);
	}
}

void Model::LoadModel(const string &path){
	Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene){
	for(GLuint i = 0; i < node->mNumMeshes; i++){
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for(GLuint i = 0; i < node->mNumChildren; i++){
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene){
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	//Add Vertex data
	for(GLuint i = 0; i < mesh->mNumVertices; i++){
		Vertex v;
		vec3 vec;

		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		v.pos = vec;

		vec.x = mesh->mNormals[i].x;
		vec.y = mesh->mNormals[i].y;
		vec.z = mesh->mNormals[i].z;
		v.normal = vec;

		if(mesh->mTextureCoords[0]){
			vec2 tvec;
			tvec.x = mesh->mTextureCoords[0][i].x;
			tvec.y = mesh->mTextureCoords[0][i].y;
			v.texCoords = tvec;
		}
		else{
			v.texCoords = vec2(0.0f, 0.0f);
		}

		vertices.push_back(v);
	}

	//Add Index data
	for(GLuint i = 0; i < mesh->mNumFaces; i++){
		aiFace face = mesh->mFaces[i];

		for(GLuint j = 0; j < face.mNumIndices; j++){
			indices.push_back(face.mIndices[j]);
		}
	}

	//Check for material
	if(mesh->mMaterialIndex >= 0){
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}


	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial * material, aiTextureType type, string namePattern){
	vector<Texture> textures;

	for(GLuint i = 0; i < material->GetTextureCount(type); i++){
		aiString str;
		material->GetTexture(type, i, &str);

		GLboolean skip = false;
		
		for(auto &tex : loadedTextures){
			if(tex.path == str){
				textures.push_back(tex);
				skip = true;
				break;
			}
		}

		if(!skip){
			Texture tex;
			tex.ID = TexFromFile(str.C_Str(), directory);
			tex.type = namePattern;
			tex.path = str;
			textures.push_back(tex);

			loadedTextures.push_back(tex);
		}
	}


	return textures;
}
