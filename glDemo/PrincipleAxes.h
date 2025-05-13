#pragma once

#include "core.h"
#include "Model.h"



class CGPrincipleAxes: public Model {

private:

	GLuint				m_numFaces = 0;
	GLuint				m_vao = 0;

	GLuint				m_vertexBuffer;
	GLuint				m_colourBuffer;
	GLuint				m_indexBuffer;

public:

	CGPrincipleAxes();
	~CGPrincipleAxes();

    void Load(std::ifstream& file);

	void render(bool _showZAxis = true);

	
};
