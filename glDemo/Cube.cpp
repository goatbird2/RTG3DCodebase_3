#include "Cube.h"


using namespace std;
using namespace glm;


// Example data for cube model

// Packed vertex buffer for cube

// ET: OpenGl needs Points made of values to "draw" bodies
// ET: Each point has 4 values: x,y,z = position in 3D space; w = 1.0f
static float positionArray[] = {

	-1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,

	-1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 1.0f
};

// Packed colour buffer for principle axes model

// ET: OpenGl can take RGBA values and use them to colour in the cubes edges
// ET: R,G,B = colour (red, green, blue); A = alpha (transparency, 1.0 = completely visable)

static float colourArray[] = {

	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};


// Line list topology to render principle axes

// ET: We build "bodies" out of triangles, therefore: 
// ET: We need 3 values per trangle -> 12 triangles for 8 Points

static unsigned int indexArray[] = {

	// Top face
	2, 1, 0,  
	3, 2, 0,

	// Bottom face
	5, 6, 4, 
	6, 7, 4,

	// Right face
	3, 7, 2, 
	7, 6, 2,

	// Front face
	0, 4, 3,
	4, 7, 3,

	// Left face
	0, 1, 5,
	4, 0, 5,
	
	// Back face
	2, 6, 1,
	6, 5, 1
};


// Constructor
Cube::Cube() {

	m_numFaces = 6 * 2;

	// VAO = Vertex Array Object; saves how Data in VBO is interpreted
	// ETR: Purpose example: You don't have to say "these 3 numbers make one corner" every time

	// ET: "make me a new VAO"
	glGenVertexArrays(1, &m_vao);

	// ET: I am "opening the draw that is the new VAO. Everything following belongs in there."

	glBindVertexArray(m_vao);

	// VBOs = Vertex Buffer Objects; saves vertex data, so that Graficcard knows where Corners are
	// ET: VBO is a bit of storage on grafficcard, where points in a room /their colour can be loaded 1ce;
	// ET: OpenGL can then with every "render()" use them again without having to load them new


	// setup vbo for position attribute:
	
	// ET: Creates new memory area for positions on the graphics card
	glGenBuffers(1, &m_vertexBuffer);

	// ET: Indicates this storage area for vertex data will now be worked on
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

	// ET: "copy the 32 values from "positionArray" into the grafics memory"
	// ET: GL_STATIC_DRAW = "copied 1ce, then stays that way"
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), positionArray, GL_STATIC_DRAW);

	// ET: telling OpenGL that if it needs attribute 0 = position, it should take 4 float values (x,y,z,w) to build the point (they lie directly next to eachother)
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

	// ET: Enables that attribute 0 = position is used for rendering
	glEnableVertexAttribArray(0);


	// setup vbo for colour attribute:
	
	// ET: Creates new memory area for colours on the graphics card
	glGenBuffers(1, &m_colourBuffer);

	// ET: Indicates this storage area for vertex data will now be worked on
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);

	// ET: "copy the 32 values from "colourArray" into the grafics memory"
	// ET: 32 = 8 colours x 4 values (r,g,b,a)
	// ET: GL_STATIC_DRAW = "copied 1ce, then stays that way"
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), colourArray, GL_STATIC_DRAW); 

	// ET: telling OpenGL that if it needs attribute 4 = colour, it should take 4 float values (r,g,b,a) to colour (they lie directly next to eachother)
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

	// ET: Enables that attribute 4 = colour is used for rendering
	glEnableVertexAttribArray(4);


	// setup vbo for cube index buffer:

	// ET: Creates new memory area for index
	glGenBuffers(1, &m_indexBuffer);

	// ET: Indicates this storage area for vertex data will now be worked on
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// ET: "copy the 36 values from "indexArray" into the grafics memory"
	// ET: 32 = 8 colours x 4 values (r,g,b,a)
	// ET: GL_STATIC_DRAW = "copied 1ce, then stays that way"
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indexArray, GL_STATIC_DRAW);

	// ET: Closes VOA
	glBindVertexArray(0);
}

// Deconstructor
// ET: Deconstructors "clean up" our grafic memory when objects are deleted or scope is exceeded
// ET: We reserved space in grafic memory... but without a deconstructor, it isn't just freed by itself
// ET: Without Deconstructors, old VBOs would still exist and waste RAM, memory leak/slowing/GPU filled up with rubbish
// ET: Deconstructors are important in this project espechially, due to amount of objects we will want to put into the scene
Cube::~Cube() {

	// ET: To make sure we don't accidentally delete something because it's still bound to an Array buffer we want to delete, we say "Don't work with any Array Buffer"
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ET: To make sure we don't accidentally delete something because it's still bound to an Index buffer we want to delete, we say "Don't work with any Index Buffer"
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ET: "delete the position memory in GPU"
	glDeleteBuffers(1, &m_vertexBuffer);

	// ET: "delete the colour memory"
	glDeleteBuffers(1, &m_colourBuffer);

	// ET: "delete the index memory"
	glDeleteBuffers(1, &m_indexBuffer);
}

// Renderer
// ET: "Show me the VAO content!"
void Cube::render() {
	glBindVertexArray(m_vao);
	// ET: "glDrawElements(...)" = triangles should be drawn
	// ET: "m_numFaces * 3" = each cube has 6 surfaces, each surface has 2 triangles, * 3 = 36 points
	// ET: "GL_UNSIGNED_INT" = type of int
	// ET: "(const GLvoid*)0" = start at the beginning of the index-array
	glDrawElements(GL_TRIANGLES, m_numFaces * 3 , GL_UNSIGNED_INT, (const GLvoid*)0);
}
