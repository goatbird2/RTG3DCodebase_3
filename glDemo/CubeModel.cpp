#include "CubeModel.h"

CubeModel::CubeModel() {

    //ET: Allows us to know that it worked!
    std::cout << "CubeModel created :)" << std::endl;

    m_type = "CUBEMODEL"; //ET: used for manifest identification of type
    m_cube = new Cube();  //ET: Uses cube.cpp to build VAO/VBO internally (Constructor) right away (real time?)
}

CubeModel::~CubeModel() {
    delete m_cube; //ET: Clean-up of OpenGL buffers
}

void CubeModel::Load(std::ifstream& file) {
    //ET: Nothing needed here, as cube is hard-coded
}

void CubeModel::Render() {
    std::cout << "Rendering CubeModel..." << std::endl; //ET: lets us know its working!
    m_cube->render(); //ET: calls the Cube's OpenGl draw
}