#pragma once
#include "Model.h"

// Forward-declare Cube (to avoid circular include)
class Cube;

// A model that renders a procedural cube
class CubeModel : public Model {
public:
    CubeModel();
    virtual ~CubeModel();

    void Load(std::ifstream& file) override;
    void Render() override;

private:
    Cube* m_cube; // pointer to the cube mesh (holds VAO/VBO data)
};
