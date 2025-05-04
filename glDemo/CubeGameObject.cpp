
// ET: "#pragma once" prevents double inclusion of files
#include ""

// ET: This header file inherits from the GameObject headerfile
#include "GameObject.h"

// ET: We need the definition of the cube
#include "core/cube.h"


class CubeGameObject : public GameObject {
public:
    // ET: Constructor (learned what that is in cube.cpp)
    CubeGameObject::CubeGameObject() {
        // ET: We create a Mesh-object (here, the cube)
        mesh = new Cube();

        // ET: Assigns mesh to GameObject
        SetMesh(mesh);

        // ET: Sets position of cube in the room (here: middle)
        SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        // ET: Sets scale of cube (here: standard size)
        SetScale(glm::vec3(1.0f));

        // ET: Sets rotation (here: resets it to 0)
        SetRotation(glm::vec3(0.0f));
    }
    // ET: Deconstructor (learned what that is in cube.cpp)
    ~CubeGameObject();
};
