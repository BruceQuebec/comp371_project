#include <glm/glm.hpp>
#include "Mesh.hpp"
#include "Model.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material mats, glm::vec3 position, glm::vec3 scale) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->position = position;
	this->mats = mats;
	this->scale = scale;
	this->x_angle = 0;
	this->y_angle = 0;
	this->z_angle = 0;

	this->setupMesh();
}

void Mesh::setupMesh() {
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

void Mesh::setXpos(float xpos) {
	this->position.x = xpos;
}

void Mesh::setYpos(float ypos) {
	this->position.y = ypos;
}

void Mesh::setZpos(float zpos) {
	this->position.z = zpos;
}

void Mesh::setScale(glm::vec3 scale) {
	this->scale = scale;
}

void Mesh::setX_angle(double X_angle) {
	this->x_angle = X_angle;
}

void Mesh::setY_angle(double Y_angle) {
	this->y_angle = Y_angle;
}

void Mesh::setZ_angle(double Z_angle) {
	this->z_angle = Z_angle;
}

float Mesh::getXpos() { return position.x; }
float Mesh::getYpos() { return position.y; }
float Mesh::getZpos() { return position.z; }
glm::vec3 Mesh::getScale() { return scale; }
double Mesh::getX_angle() { return x_angle; }
double Mesh::getY_angle() { return y_angle; }
double Mesh::getZ_angle() { return z_angle; }
glm::mat4 Mesh::getModelMat()
{
	glm::mat4 translate_mat = translate(glm::mat4(1), this->position);

	glm::mat4 scale_mat = glm::scale(glm::mat4(1), this->scale);
	glm::mat4 rotate_x_mat = rotate(glm::mat4(1), float(x_angle), glm::vec3(1, 0, 0));
	glm::mat4 rotate_y_mat = rotate(glm::mat4(1), float(y_angle), glm::vec3(0, 1, 0));
	glm::mat4 rotate_z_mat = rotate(glm::mat4(1), float(z_angle), glm::vec3(0, 0, 1));
	glm::mat4 rotate_mat = rotate_x_mat * rotate_y_mat * rotate_z_mat;

	glm::mat4 model_mat = translate_mat * scale_mat * rotate_mat;

	return model_mat;
}

void Mesh::draw(Shader& shader, Camera &camera) {
	//synchronize material uniforms
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	if (textures.size() > 0) {
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream

			// now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i].textureID);
		}
		shader.setBool("useTexture", true);
	}
	
	if (sizeof(this->mats) > 0) {
		shader.setVec4("material.ambient", this->mats.Ka);
		shader.setVec4("material.diffuse", this->mats.Kd);
		shader.setVec4("material.specular", this->mats.Ks);
	}
	shader.setFloat("material.shininess", this->mats.shininess*4.f);
	//std::cout << this->mats.Kd.x << "-" << this->mats.Kd.y << "-" << this->mats.Kd.z << "-" << this->mats.Kd.w << std::endl;
	
	//synchronize light uniforms
	shader.setVec3("light.position", Model::models["lightCube"]->getPosition());
	shader.setVec3("view_position", camera.getCameraPosition());

	// point light properties
	glm::vec3 lightColor(1.0, 1.0, 1.0);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.9f); // low influence
	shader.setVec3("light.ambient", ambientColor);
	shader.setVec3("light.diffuse", diffuseColor);
	shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	shader.setFloat("light.constant", 1.0f);
	shader.setFloat("light.linear", 0.027f);
	shader.setFloat("light.quadratic", 0.0028f);

	//synchronize matrices
	// view/projection transformations
	glm::mat4 view_projection = camera.getCameraMat();
	shader.setMat4("view_projection", view_projection);
	glm::mat4 model = getModelMat();
	shader.setMat4("model", model);
	
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}