// ET: This is a header file (.h). Header files are like the powerpoint title slides of coding

// ET: "#pragma once" prevents double inclusion of files
#pragma once

// ET: This header file inherits transformations etc. from the GameObject headerfile
#include "ExampleGO.h"
#include "Model.h" //TO-DO: Ensure Model base class exists
#include "Shader.h" //TO-DO: Ensure Shader class exists and ShaderManager/Get function

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
class Scene; // ET: Forward declaration for scene reference

// ET: Inheritance-> CubeGameObject is a GameObject (enables acess to SetPosition, SetRotation, etc.)
class CubeGameObject : public ExampleGO {

    // ET: Accessmodifyers allow us to define, who get's access to what: "public" - everyone can, "private" - only class itself, "protected" - like private but inheriting classes can
public:
    // Constructor to create the cube
    CubeGameObject();

    // Deconstructor (called automatically when cube is deleted)
    ~CubeGameObject();

    // Overrides to GameObject:
    // ET: The override is there, because there is already one inside GameObject- "override" tells c++, that we want to override exactly one function of a basic class (this is actually really cool)
    // ET: "Overriding" doesn't mean "overwriting" we *add* to the base class, we are not replacing it entirely!

    // Loads name/position/rotation/scale/rot_increment from manifest
    virtual void Load(std::ifstream& file) override;

    // Per frame rotation increment update
    virtual void Tick(float dt) override;

    // Sets shader uniforms 
    virtual void PreRender() override;

    // rendering using model pointer
    virtual void Render() override;

    // Optional init hook
    virtual void Init(Scene* scene) override;

private:

    Model* m_model = nullptr; // ET: points to shared Mesh (VAO/VBO)
    Shader* m_shader = nullptr; // ET: points to GLSL shader program

    Scene* m_scene = nullptr; // ET: saved reference to current scene (used to fetch shader)

    //bool rotateOverTime = false;
    //bool moveOverTime = false;
    //bool pulseOverTime = false;
    //glm::vec4 color = glm::vec4(1.0f); //opaque white
};