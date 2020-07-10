#pragma once


class World
{
private:
	static double x_angle;
	static double y_angle;
	static double z_angle;

public:
	static glm::mat4 getWorldMat();

	static void key_callback(int key, int action);
};