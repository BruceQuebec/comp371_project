#include "Material.hpp"


using namespace glm;


Material::Material(vec3 ambient, vec3 diffuse, vec3 specular, double shininess)
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
}

vec3 Material::getAmbient()
{
    return ambient;
}

vec3 Material::getDiffuse()
{
    return diffuse;
}

vec3 Material::getSpecular()
{
    return specular;
}

double Material::getShininess()
{
    return shininess;
}