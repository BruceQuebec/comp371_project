#pragma once


#include <vector>


class Box
{
public:
    static std::vector<Box *> boxes;

public:
    float x_bound;
    float x_bound_;
    float y_bound;
    float y_bound_;
    float z_bound;
    float z_bound_;

public:
    Box(float x_bound, float x_bound_, float y_bound, float y_bound_, float z_bound, float z_bound_);

    int detectCollision(Box & box);
};