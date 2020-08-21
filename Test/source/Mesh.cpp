#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include "Mesh.hpp"
#include "World.hpp"
#include "ComplexModel.hpp"


using namespace glm;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<mTexture> textures, mMaterial mats, glm::vec3 position, glm::vec3 scale) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->position = position;
	this->mats = mats;
	this->scale = scale;
	this->x_angle = 0;
	this->y_angle = 0;
	this->z_angle = 0;
	this->x_shear = 0;
	this->y_shear = 0;
	this->z_shear = 0;

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
	mat4 scale_mat = glm::scale(mat4(1), scale);

	mat4 rotate_x_mat = rotate(mat4(1), float(x_angle), vec3(1, 0, 0));
	mat4 rotate_y_mat = rotate(mat4(1), float(y_angle), vec3(0, 1, 0));
	mat4 rotate_z_mat = rotate(mat4(1), float(z_angle), vec3(0, 0, 1));
	mat4 rotate_mat = rotate_x_mat * rotate_y_mat * rotate_z_mat;

	mat4 translate_mat = translate(mat4(1), this->position);

	mat4 shear_mat = glm::shearY3D(mat4(1), (float)this->x_shear, (float)this->z_shear);

	mat4 model_mat = shear_mat * translate_mat * rotate_mat * scale_mat;

	mat4 world_mat = World::getWorldMat();
	model_mat = world_mat * model_mat;

	return model_mat;
}

void Mesh::draw(Shader_v2& shader, Camera &camera, Light& light, GLuint shadow_map, bool render_shadow) {
	shader.use();

	//synchronize material uniforms
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	unsigned int totalSamplerUnitNum = 0;
	std::cout << "textures.size(): " << textures.size() << std::endl;
	if (textures.size() > 0) {
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = textures[i].type;
			totalSamplerUnitNum++;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream

			// now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i].textureID);
		}
		shader.setBool("useTexture", true);
	}
	
	if (sizeof(this->mats) > 0) {
		shader.setVec4("material.ambient", this->mats.Ka);
		shader.setVec4("material.diffuse", this->mats.Kd);
		shader.setVec4("material.specular", this->mats.Ks);
		shader.setFloat("material.shininess", this->mats.shininess*4.f);
	}
	
	//synchronize light uniforms
	glm::vec3 diffuseColor = light.getLightColor() * vec3(1.0f); // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(1.0f); // low influence
	
	shader.setVec3("light.position", light.getLightPos());
	shader.setVec3("light.ambient", ambientColor);
	shader.setVec3("light.diffuse", diffuseColor);
	shader.setVec3("light.specular", light.getLightColor());
	shader.setFloat("light.constant", 1.0f);
	shader.setFloat("light.linear", 0.027f);
	shader.setFloat("light.quadratic", 0.0028f);

	//synchronize shadow
	if (shadow_map!=NULL) {
		glActiveTexture(GL_TEXTURE0 + totalSamplerUnitNum);
		shader.setInt("shadow_map", totalSamplerUnitNum);
		glBindTexture(GL_TEXTURE_2D, shadow_map);
		
	}

	//synchronize matrices
	// view/projection transformations

	vec3 eye = vec3(0, 30, 0);
	vec3 center = vec3(0, 0, 0);
	vec3 up = vec3(1, 0, 0);
	mat4 view_mat = lookAt(eye, center, up);
	mat4 projection_mat = perspective((float)90, 1.0f, 10.0f, 40.0f);
	
	mat4 light_camera_mat = projection_mat * view_mat;
	mat4 depth_mvp_mat = light_camera_mat * this->getModelMat();
	shader.setMat4("light_mvp_mat", depth_mvp_mat);
	shader.setBool("render_shadow", render_shadow);

	glm::mat4 view =  camera.getViewMat();
	glm::mat4 projection = camera.getProjectionMat();
	glm::mat4 model = getModelMat();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("view_position", camera.getCameraPos());
	shader.setMat4("model", model);
	

	
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}