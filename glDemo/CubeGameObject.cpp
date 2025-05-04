
// ET: "#pragma once" prevents double inclusion of files
#include "CubeGameObject.h"

// ET: Originally, this was included to make sure this header file inherits from the GameObject headerfile
// ET: It was later realized, that this wasn't neccesary, as "GameObject.h" is already included in "CubeGameObject.h" which is included above
// ET: a"CubeGameObject.h" #includes b"GameObject.h" -> content from b is insterted into a
// ET: In the .cpp we only have to include, what the .h doesn't already take care of
// 
///#include "GameObject.h"

// ET: We need the definition of the cube
#include "core/cube.h"


class CubeGameObject : public GameObject {
public:
    // ET: Constructor (learned what that is in cube.cpp)
    CubeGameObject::CubeGameObject() {
        // ET: We create a Mesh-object out of cube.cpp
        mesh = new Cube();

        // ET: Assigns mesh to GameObject-Rendering-System
        SetMesh(mesh);

        // ET: Originally, the following was included for transforming, till it was realized: 
        // ET: it is a bad approach to include the transformations in here, as we want multiple cubes in different variations
        //
        /// ET: Sets position of cube in the room (here: middle)
        ///SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        /// 
        /// ET: Sets scale of cube (here: standard size)
        ///SetScale(glm::vec3(1.0f));
        /// 
        /// ET: Sets rotation (here: resets it to 0)
        ///SetRotation(glm::vec3(0.0f));
    }
    // ET: Deconstructor (learned what that is in cube.cpp)
    ~CubeGameObject(); {

    }

    void CubeGameObject::Update() {
       
    }

};
