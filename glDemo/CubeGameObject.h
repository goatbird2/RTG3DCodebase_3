
// ET: "#pragma once" prevents double inclusion of files
#pragma once

// ET: This header file inherits transformations etc. from the GameObject headerfile
#include "GameObject.h"

// ET: Forward declaration: We are telling the complier that a class "cube" exists (doesnt load it yet tho)
// ET: We hereby avoid including cube.h directly -> saves compilation time
class Cube;

// ET: Inheritance-> CubeGameObject is a GameObject (enables acess to SetPosition, SetRotation, etc.)
class CubeGameObject : public GameObject {
public:
    // ET: Constructor to create the cube
    CubeGameObject();
    // ET: Deconstructor
    ~CubeGameObject();
    // ET: Overrides logic per frame to for example. rotate it
    void Update(float deltaTIME) override;
};
