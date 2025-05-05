
// Et: This is a code file (.cpp). Code files are like the powerpoint Content slides of coding

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


 // ET: Constructor (a function called automatically when a new object is created))
    CubeGameObject::CubeGameObject() {
        // ET: We create a Mesh-object out of cube.cpp
        mesh = new Cube();

        // ET: Assigns mesh to GameObject-Rendering-System
        SetMesh(mesh);

        // ET: Originally, the following was included for transforming, till it was realized: 
        // ET: it is a bad approach to include the transformations in here, as we want multiple cubes in different variations
        
        // ET: Later it was considered, that "default" values should be set that would then be overriden if so wished when creating a cube in main.
        // ET: This, however, was not a good solution for customizably transformable cubes, as the constructor would OVERRIDE the non-default values in main
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
