
// ET: This is a header file (.h). Header files are like the powerpoint title slides of coding

// ET: "#pragma once" prevents double inclusion of files
#pragma once

// ET: This header file inherits transformations etc. from the GameObject headerfile
#include "GameObject.h"

/// ET: Thought about using Enums for customizable behavioural modes of cubes
/// ET: Enums (short for enumeration) = datatype usable to define clear and named modes or categories
/// ET: Won't use it afterall, as it works on "one or the other" principle, and I want option to make cubes transform in more than 1 way
//
//enum class CubeBehavior {
//None,    // 0
//Rotate,  // 1
//Move,    // 2
//Pulse    // 3
//};


// ET: Forward declaration: We are telling the complier that a class "cube" exists 
// ET: We hereby avoid including cube.h directly -> saves compilation time and reduces compile dependancies
class Cube;

// ET: Inheritance-> CubeGameObject is a GameObject (enables acess to SetPosition, SetRotation, etc.)
class CubeGameObject : public GameObject {

// ET: Accessmodifyers allow us to define, who get's access to what: "public" - everyone can, "private" - only class itself, "protected" - like private but inheriting classes can
public:
    // Constructor to create the cube
    CubeGameObject();

    // Deconstructor (called automatically when cube is deleted)
    ~CubeGameObject();

    // ET: Method is called to override logic per frame for transformations!
    void Update(float deltaTIME) override;

    // Public setter methods: Functions to customize cube behaviours (not setting the variables directly)
    // ET: bools = type of data with one 2 values: true (yes), false (no). We can use this to check for transformation!

    // rotation
    void SetRotateOverTime(bool enabled);

    // movement
    void SetMoveOverTime(bool enabled);

    // pulse
    void SetPulseOverTime(bool enabled);

// ET: we make this private, but give public access to a function which can set variables internally
private:

    // Pointer onto cube mesh
    // ET: Pointers: not objects themselves but like "letter with adress" to object, aka: mesh isn't a cube itself, but a pointer to a cube
    // ET: Why? c++ is savvy! We dont want to kopy big objects like meshes, so, we just "point" at them
    Cube* mesh;

    // ET: Bools set to not have any transformation initially set (default):
    
    // rotation
    bool rotateOverTime = false;

    // movement
    bool moveOverTime = false;

    // pulse
    bool pulseOverTime = false;

};