#include "Light.hpp"


using namespace glm;


Light::Light(vec3 pos)
{
    this->pos = pos;
    color = vec3(1, 1, 1);
}

Light::Light(vec3 pos, vec3 color)
{
    this->pos = pos;
    this->color = color;
}

vec3 Light::getLightPos()
{
    return pos;
}

vec3 Light::getLightColor()
{
    return color;
}