
// ET: "#pragma once" prevents double inclusion of files
#pragma once

// ET: This header file inherits from the GameObject headerfile
#include "GameObject.h"

class CubeGameObject : public GameObject {
public:
    // ET: Constructor to create the cube
    CubeGameObject();
    // Et: Deconstructor to 
    ~CubeGameObject();
};
