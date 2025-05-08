
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
    //TO-DO: Implement reading of "SHADER" name into m_shaderName or similar
    virtual void Load(std::ifstream& file) override;

    // Per frame rotation increment update
    // ET: Tick(dt) calls things every frame (animation is just "ticking" forward)
    virtual void Tick(float dt) override;
    
    // Sets shader uniforms 
    // ET: Sends object's model matrix to the GPU for rendering
    virtual void PreRender() override;

    // rendering using model pointer
    //TO-DO: Ensure CubeModel class exists and is registered with Model system
    // ET: Actually draws the cube!
    virtual void Render() override;

    // Optional init hook
    // ET: Just means it runs once after creation
    virtual void Init(Scene* scene) override;

    // ETQ: But what about position? It's in GameObjects afterall, why not here?


    /// ETR: At first, we thought it was a good idea to do an "all in one booleans"-cube. This turned out to not be the smartest idea.
    /// ETR: Putting rotation, movement, colour, pulse, transparency and flags all in one class "feels" smart, but isn't the most sustainable, because:
    /// We should instead act on the "single responsibility principle" and seperate loading, rendering, shader-setup, drawing and put behaviours into their own subclasses for scalability, clarify, readability and "iterative-ness"
    /// 
    /// Transformation setters
    /// ET: Public setter methods: Functions to customize cube behaviours (not setting the variables directly)
    /// ET: bools = type of data with one 2 values: true (yes), false (no). We can use this to check for transformation!
    //
    /// rotation
    //void SetRotateOverTime(bool enabled);
    /// movement
    //void SetMoveOverTime(bool enabled);
    /// pulse
    //void SetPulseOverTime(bool enabled);
    /// Colour controls
    /// colour
    //void SetColor(const glm::vec4& color);
    /// transparency
    //void SetAlpha(float alpha);
    /// for shaders
    //glm::vec4 GetColor() const;

// ET: We make this private, but give public access to a function which can set variables internally

private:

    // ET: Pointers: not objects themselves but like "letter with adress" to object, aka: mesh isn't a cube itself, but a pointer to a cube
    // ETQ: Why? c++ is savvy! We dont want to copy big objects like meshes, so, we just "point" at them
    /// ETR: Used to have a pointer to cube mesh, BUT, that would have worked with hard-coding glBegin(GL_Quads) aka. immediate mode, where changes are sent to GPU every frame (slow, outdated, inefficient, unflexible, unable to laod in models etc.)
    /// ETR: modern and improved method of pointers to a shared mesh (VAO+VBO setup) and GLSL shader program set-up for 

    //TO-DO: Create concrete CubeModel subclass of Model
    Model* m_model = nullptr;// ET: points to shared Mesh (VAO/VBO)
    //To-DO: Retrieve Shader from ShaderManager::Get(name)
    Shader* m_shader = nullptr;// ET: points to GLSL shader program

    /// ETR: At first, we thought it was a good idea to do an "all in one booleans"-cube. This turned out to not be the smartest idea.
    /// ETR: Putting rotation, movement, colour, pulse, transparency and flags all in one class "feels" smart, but isn't the most sustainable, because:
    /// We should instead act on the "single responsibility principle" and seperate loading, rendering, shader-setup, drawing and put behaviours into their own subclasses for scalability, clarify, readability and "iterative-ness"
    /// 
    /// ET: Bools set to not have any transformation initially set (default):
    /// rotation
    //bool rotateOverTime = false; // no rotation
    /// movement
    //bool moveOverTime = false; // no movement
    /// pulse
    //bool pulseOverTime = false; //no pulse transformation
    /// colours & alpaha
    //glm::vec4 color = glm::vec4(1.0f); //opaque white

};