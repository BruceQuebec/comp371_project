#pragma once


#include <glm/glm.hpp>


class Material
{
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    double shininess;

public:
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, double shininess);

    glm::vec3 getAmbient();

    glm::vec3 getDiffuse();

    glm::vec3 getSpecular();

    double getShininess();
};