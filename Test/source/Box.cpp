#include "Box.hpp"


#include <iostream>


using namespace std;


vector<Box *> Box::boxes;

Box::Box() {};
Box::Box(float x_bound, float x_bound_, float y_bound, float y_bound_, float z_bound, float z_bound_)
{
	this->x_bound = x_bound;
	this->x_bound_ = x_bound_;
	this->y_bound = y_bound;
	this->y_bound_ = y_bound_;
	this->z_bound = z_bound;
	this->z_bound_ = z_bound_;
}

int Box::detectCollision(Box & box)
{
	bool x_collision = false, y_collision = false, z_collision = false;
	if (!(x_bound > box.x_bound_ || x_bound_ < box.x_bound))
		x_collision = true;
	if (!(y_bound > box.y_bound_ || y_bound_ < box.y_bound))
		y_collision = true;
	if (!(z_bound > box.z_bound_ || z_bound_ < box.z_bound))
		z_collision = true;

	if (y_collision && z_collision && x_bound - 0.21 < box.x_bound_ && x_bound_ > box.x_bound)
		return 1;
	if (y_collision && z_collision && x_bound_ + 0.21 > box.x_bound && x_bound < box.x_bound_)
		return 2;
	if (x_collision && z_collision && y_bound - 0.21 < box.y_bound_ && y_bound_ > box.y_bound)
		return 3;
	if (x_collision && z_collision && y_bound_ + 0.21 > box.y_bound && y_bound < box.y_bound_)
		return 4;
	if (x_collision && y_collision && z_bound - 0.21 < box.z_bound_ && z_bound_ > box.z_bound)
		return 5;
	if (x_collision && y_collision && z_bound_ + 0.21 > box.z_bound && z_bound < box.z_bound_)
		return 6;

	return 0;
}