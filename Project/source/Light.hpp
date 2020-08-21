#pragma once


#include <glm/glm.hpp>


class Light
{
private:
    float pos_x;
    float pos_y;
    float pos_z;

    glm::vec3 color;

    float theta;

public:
    Light(glm::vec3 pos);
    Light(glm::vec3 pos, glm::vec3 color);

    glm::vec3 getLightPos();

    glm::vec3 getLightColor();
};