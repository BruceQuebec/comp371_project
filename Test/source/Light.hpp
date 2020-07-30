#pragma once


#include <glm/glm.hpp>


class Light
{
private:
    glm::vec3 pos;
    glm::vec3 color;

public:
    Light(glm::vec3 pos);
    Light(glm::vec3 pos, glm::vec3 color);

    glm::vec3 getLightPos();

    glm::vec3 getLightColor();
};