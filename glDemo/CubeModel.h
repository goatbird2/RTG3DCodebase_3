
#pragma once
//ET: inherits from Model, so that rest of engine trats it like any other model
#include "Model.h"
#include "cube.h" //ET: gives us Cube class


class CubeModel : public Model {
public:
    CubeModel();
    ~CubeModel();
    virtual void Load(std::ifstream& file) override; //ET: overrides load but leave sit empty, as har-coded approach doesn't need anything currently
    virtual void Render() override; //ET: draws the cube

private:
    Cube* m_cube; //ET: Pointer to VAO/VBO wrapped Cube mesh
    GLuint m_shaderProgram; //ET: Allows us to test, whether it worked
};
