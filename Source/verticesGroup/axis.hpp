//
//  grid.hpp
//  COMP371_Labs
//
//  Created by ZhangDong on 2020-07-03.
//  Copyright © 2020 Concordia. All rights reserved.
//

#ifndef grid_h
#define grid_h
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

std::vector<glm::vec3> axis_vertices = {
    // x 轴
    glm::vec3 (0.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3 (1.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    
    // y 轴
    glm::vec3 (0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3 (0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    
    // z 轴
    glm::vec3 (0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3 (0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
};

#endif /* grid_h */
