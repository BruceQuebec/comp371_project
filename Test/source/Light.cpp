#include "Light.hpp"


using namespace glm;


Light::Light(vec3 pos)
{
    pos_x = pos[0];
    pos_y = pos[1];
    pos_z = pos[2];

    color = vec3(1, 1, 1);

    theta = 45;
}

Light::Light(vec3 pos, vec3 color)
{
    pos_x = pos[0];
    pos_y = pos[1];
    pos_z = pos[2];

    this->color = color;

    theta = 45;
}

vec3 Light::getLightPos()
{
    theta += 0.001;

    pos_x = 100 * cos(radians(theta));
    pos_y = 100 * sin(radians(theta));
    pos_z = 10;

    return vec3(pos_x, pos_y, pos_z);
}

vec3 Light::getLightColor()
{
    return color;
}