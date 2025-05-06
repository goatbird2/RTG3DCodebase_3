
// ET: This is a header file (.h). Header files are like the powerpoint title slides of coding

// ET: "#pragma once" prevents double inclusion of files
#pragma once

// ET: This header file inherits transformations etc. from the GameObject headerfile
#include "GameObject.h"

/// ETR: Thought about using Enums for customizable behavioural modes of cubes
/// ETR: Enums (short for enumeration) = datatype usable to define clear and named modes or categories
/// ETR: Won't use it afterall, as it works on "one or the other" principle, and I want option to make cubes transform in more than 1 way
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


    // Transformation setters
    // ET: Public setter methods: Functions to customize cube behaviours (not setting the variables directly)
    // ET: bools = type of data with one 2 values: true (yes), false (no). We can use this to check for transformation!

    // rotation
    void SetRotateOverTime(bool enabled);

    // movement
    void SetMoveOverTime(bool enabled);

    // pulse
    void SetPulseOverTime(bool enabled);

    // Colour controls

    // colour
    void SetColor(const glm::vec4& color);

    // transparency
    void SetAlpha(float alpha);

    // for shaders
    glm::vec4 GetColor() const;

// ET: We make this private, but give public access to a function which can set variables internally
private:

    // Pointer onto cube mesh4
    // ET: Pointers: not objects themselves but like "letter with adress" to object, aka: mesh isn't a cube itself, but a pointer to a cube
    // ETR: Why? c++ is savvy! We dont want to copy big objects like meshes, so, we just "point" at them
    Cube* mesh;

    // ET: Bools set to not have any transformation initially set (default):
    
    // rotation
    bool rotateOverTime = false; // no rotation

    // movement
    bool moveOverTime = false; // no movement

    // pulse
    bool pulseOverTime = false; //no pulse transformation

    // colours & alpaha
    glm::vec4 color = glm::vec4(1.0f); //opaque white

};