#ifndef _utils_H
#define _utils_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"

float fmax(float f1,float f2, float f3);
float fmin(float f1,float f2, float f3);

std::vector<double> to_vector(glm::vec3 vec);
glm::vec3 to_vec3(std::vector<double> vec);

std::ostream & operator << (std::ostream & stream, const glm::vec3 & obj);

#endif
