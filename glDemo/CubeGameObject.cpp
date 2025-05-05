
// ET: This is a code file (.cpp). Code files are like the powerpoint Content slides of coding and "fills in" what the header "promises"

// ET: Brings in the header with all declerations
#include "CubeGameObject.h"

/// ETR: Originally, this was included to make sure this header file inherits from the GameObject headerfile
/// ETR: It was later realized, that this wasn't neccesary, as "GameObject.h" is already included in "CubeGameObject.h" which is included above
/// ETR: a"CubeGameObject.h" #includes b"GameObject.h" -> content from b is insterted into a
/// ETR: In the .cpp we only have to include, what the .h doesn't already take care of
///#include "GameObject.h"

// ET: Lets us use the actual cube mesh
#include "core/cube.h"


 // Constructor 
 // ET: a constructor is a function called automatically when a new object is created
    CubeGameObject::CubeGameObject() {
        // ET: We create a new cube mesh and assign it to our pointer (-> def in SubeGameObject.h)
        mesh = new Cube();

        // ET: Assigns mesh to GameObject-Rendering-System (this is a function from GameObject that sets the mesh as the one to render)
        SetMesh(mesh);

        /// ETR: Originally, the following was included for transforming, till it was realized: 
        /// ETR: it is a bad approach to include the transformations in here, as we want multiple cubes in different variations
        //
        /// ETR: Later it was considered, that "default" values should be set that would then be overriden if so wished when creating a cube in main.
        /// ETR: This, however, was not a good solution for customizably transformable cubes, as the constructor would OVERRIDE the non-default values in main
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
    // Deconstructor (learned what that is in cube.cpp!)
    CubeGameObject::~CubeGameObject() {
        delete mesh; 
    }

    // Update per frame
    // ET: deltaTime = time passed since last frame
    void CubeGameObject::Update(float deltaTime) {

        // ROTATION
        if (rotateOverTime) {
            // aktuelle Rotation holen
            glm::vec3 currentRotation = GetRotation();
            // z.B. jede Sekunde um 90° auf der Y-Achse drehen
            currentRotation.y += 90.0f * deltaTime;
            SetRotation(currentRotation);
        }

        // MOVEMENT
        if (moveOverTime) {
            glm::vec3 currentPosition = GetPosition();
            // simple sin-wave movement auf Y-Achse (Hüpfen)
            float time = static_cast<float>(glfwGetTime());
            currentPosition.y = sin(time) * 2.0f; // hüpft von -2 bis +2
            SetPosition(currentPosition);
        }

        // PULSE (Scaling)
        if (pulseOverTime) {
            float time = static_cast<float>(glfwGetTime());
            float scale = abs(sin(time)) + 0.5f; // Skaliert zw. 0.5 und 1.5
            SetScale(glm::vec3(scale));
        }
    }