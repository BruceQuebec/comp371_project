#include "ComplexModel.hpp"

ComplexModel::ComplexModel(string path, glm::vec3 position, glm::vec3 scale) {
	this->loadModel(path, position, scale);
}

void ComplexModel::loadModel(string path, glm::vec3 position, glm::vec3 scale) {
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene, position, scale);
}

vector<Mesh> ComplexModel::getMeshes() {
	return meshes;
}

void ComplexModel::draw(Shader &shader, Camera& camera) {
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw(shader, camera);
}

void ComplexModel::processNode(aiNode *node, const aiScene *scene, glm::vec3 position, glm::vec3 scale)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene, position, scale));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, position, scale);
	}
}

Mesh ComplexModel::processMesh(aiMesh *mesh, const aiScene *scene, glm::vec3 position, glm::vec3 scale)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	Material mat;
	aiColor3D color;
	

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;

		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		// Read mtl file vertex data
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		mat.Ka = glm::vec4(color.r, color.g, color.b, 1.0);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mat.Kd = glm::vec4(color.r, color.g, color.b, 1.0);
		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mat.Ks = glm::vec4(color.r, color.g, color.b, 1.0);
		material->Get(AI_MATKEY_SHININESS, mat.shininess);

		// 1. diffuse maps
		vector<Texture> diffuseMaps = loadMaterialTextures(material,
		aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// 2. specular maps
		vector<Texture> specularMaps = loadMaterialTextures(material,
		aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	
	return Mesh(vertices, indices, textures, mat, position, scale);
}

vector<Texture> ComplexModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.textureID = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

unsigned int ComplexModel::TextureFromFile(const char *path, const string &directory)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void ComplexModel::key_callback(int key, int action, int mods)
{
	// If W, S, A or D is pressed, shift the model
	if (key == GLFW_KEY_W && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		update_meshes('W', 0.1);
	if (key == GLFW_KEY_S && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		update_meshes('S', 0.1);
	if (key == GLFW_KEY_A && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		update_meshes('A', 0.1);
	if (key == GLFW_KEY_D && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		update_meshes('D', 0.1);

	// If U or J is pressed, scale the model
	if (key == GLFW_KEY_U && action == GLFW_REPEAT)
		update_meshes('U', 0.01);
	if (key == GLFW_KEY_J && action == GLFW_REPEAT)
		update_meshes('J', 0.01);

	// If w, s, a, or d is pressed, rotate the model
	if (key == GLFW_KEY_W && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		update_meshes('w', 0.01);
	if (key == GLFW_KEY_S && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		update_meshes('s', 0.01);
	if (key == GLFW_KEY_A && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		update_meshes('a', 0.01);
	if (key == GLFW_KEY_D && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		update_meshes('d', 0.01);
}

void ComplexModel::update_meshes(char key, float value) {
	vector<Mesh>::iterator it = this->getMeshes().begin();
	while (it != this->getMeshes().end()) {
		switch (key) {
			case 'W':
				it->setZpos(it->getZpos() - value);
			case 'S':
				it->setZpos(it->getZpos() + value);
			case 'A':
				it->setXpos(it->getXpos() - value);
			case 'D':
				it->setXpos(it->getXpos() + value);
			case 'U': {
				glm::vec3 scale = glm::vec3(it->getScale().x + value, it->getScale().y + value, it->getScale().z + value);
				it->setScale(scale);
			}
			case 'J': {
				glm::vec3 scale = glm::vec3(it->getScale().x - value, it->getScale().y - value, it->getScale().z - value);
				it->setScale(scale);
			}
			case 'w':
				it->setX_angle(it->getX_angle() + value);
			case 's':
				it->setX_angle(it->getX_angle() - value);
			case 'a':
				it->setY_angle(it->getY_angle() + value);
			case 'd':
				it->setY_angle(it->getY_angle() - value);
			break;
		}
		it++;
	}
}