#pragma once



class Camera
{
private:
	double x_pos;
	double y_pos;
	double z_pos;

	double theta;
	double rho;

	double fov;

	double aspect;

public:
	Camera();

	glm::mat4 getCameraMat();
};