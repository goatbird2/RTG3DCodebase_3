
// ET: This is a code file (.cpp). Code files are like the powerpoint Content slides of coding and "fills in" what the header "promises"

// ET: Brings in the header with all declerations
#include "CubeGameObject.h"
#include "Helper.h"//TO-DO: Ensure Helper::SetUniformLocation exists
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <GL/glew.h>


/// ETR: Originally, this was included to make sure this header file inherits from the GameObject headerfile
/// ETR: It was later realized, that this wasn't neccesary, as "GameObject.h" is already included in "CubeGameObject.h" which is included above
/// ETR: a"CubeGameObject.h" #includes b"GameObject.h" -> content from b is insterted into a
/// ETR: In the .cpp we only have to include, what the .h doesn't already take care of
///#include "GameObject.h"

/// ETR: Remnants of old immediate mode approach (=ETRIA in this file going forward)):
///#include "core/cube.h"


 // constructor 
 // ET: a constructor is a function called automatically when a new object is created
CubeGameObject::CubeGameObject() {

    // ET: Sets the Type of string of this object as "CUBE"
    // ET: Is helpful to tell apart different objects!
    m_type = "CUBE";


        /// ETRIA:
        /// immedmesh = new Cube();
        /// ET: Assigns mesh to GameObject-Rendering-System (this is a function from GameObject that sets the mesh as the one to render)
        ///SetMesh(mesh);
       
        /// ETR: Originally, the following was included for transforming, till it was realized: 
        /// ETR: it is a bad approach to include the transformations in here, as we want multiple cubes in different variations
        /// ETR: Later it was considered, that "default" values should be set that would then be overriden if so wished when creating a cube in main.
        /// ETR: This, however, was not a good solution for customizably transformable cubes, as the constructor would OVERRIDE the non-default values in main <- INSTRUCTORS RUN IMMEDEATELY WHEN OBJECT IS CREATED
        /// ET: Sets position of cube in the room (here: middle)
        ///SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        /// ET: Sets scale of cube (here: standard size)
        ///SetScale(glm::vec3(1.0f));
        /// ET: Sets rotation (here: resets it to 0)
        ///SetRotation(glm::vec3(0.0f));}

	/*m_numFaces = 6 * 2;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// setup vbo for position attribute
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), positionArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	// setup vbo for colour attribute
	glGenBuffers(1, &m_colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), colourArray, GL_STATIC_DRAW); 
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(4);

	// setup vbo for cube) index buffer
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indexArray, GL_STATIC_DRAW);

	glBindVertexArray(0);*/

}
     
    
    // Deconstructor (learned what that is in cube.cpp! YAY!)
    CubeGameObject::~CubeGameObject() {

        // ETRIA: delete mesh; 

        delete m_model;//TO-DO: Confirm Model cleanup
        delete m_shader;//TO-DO: Confirm Shader cleanup
    }

    // Loading function
    // ET: Runs, when game is laoding an object from a file
    void CubeGameObject::Load(std::ifstream& file) {
        
        // ET: Calls the base class Load() method, which already already loads name, position, rotation, scale, rot_inc!
        /*ExampleGO::Load(file);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteBuffers(1, &m_vertexBuffer);
        glDeleteBuffers(1, &m_colourBuffer);
        glDeleteBuffers(1, &m_indexBuffer);*/

        // Acquire shader (data-driven via manifest file entry 'SHADER')
        //TO-DO: Read shader name from manifest into a member variable
        //std::string shaderName = /*TO-DO: get from manifest data */ "basic";

        // ET: Retrieves a shader from ShaderManager via the name
        // ET: Looks up a shared shade robject and returns it, so we don't have to reload them multiple times!
        //m_shader = ShaderManager::Get(shaderName); //TO-DO: Implement ShaderManager::Get

        // Instantiate and load the cube mesh
        // ET: creates new 3D mesh object of type "CubeModel" and loads itself from the same file
        //m_model = new CubeModel();    //TO-DO: CubeModel must impledment Load(file)
        //m_model->Load(file);          //TO-DO: Ensure Load handles no extra parameters
    }

    void CubeGameObject::Tick(float dt) {
        // Apply rotation increment from manifest
        m_rot += m_rot_incr;

        // Build world matrix: Translate -> RotateX -> RotateY -> RotateZ -> Scale
        glm::mat4 mat(1.0f); // ET: starts with identity
        mat = glm::translate(mat, m_pos); // ET: translation (moving)
        mat = glm::rotate(mat, glm::radians(m_rot.x), glm::vec3(1, 0, 0)); // ET: rotation x
        mat = glm::rotate(mat, glm::radians(m_rot.y), glm::vec3(0, 1, 0)); // ET: rotation y
        mat = glm::rotate(mat, glm::radians(m_rot.z), glm::vec3(0, 0, 1)); // ET: rotiation z
        mat = glm::scale(mat, m_scale); // ET: scale (resizing)

        m_worldMatrix = mat; // ET: saves it for rendering!

        // ETQ: But that looks like hard coding, why is it not?
        // ETQ: It's just the math to apply already loaded transform values from the manifest files. Remember matrizen in maths class? yea., its that special oen you can use with other matrices! its called: MODEL MATRIX and is essentially the backbone of 3d moddeling
    }

    void CubeGameObject::PreRender() {
        //TO-DO: Use ShaderProgram method instead of direct use
        //m_shader->Use();
        GLint loc;
        Helper::SetUniformLocation(m_shader->GetProg(), "modelMatrix", &loc); //TO-DO: Check API
        glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)&m_worldMatrix);
    }

    void CubeGameObject::Render() {
        // Delegate the actual draw call to the Mesh (Model)
        if (m_model) {
            m_model->Render(); //TO-DO: Ensure Model::Render calls VAO draw
        }
    }

    void CubeGameObject::Init(Scene* scene) {
        // no extra setup yet
        //TO-DO: If lights or scene-specific data needed, initialize here
        ExampleGO::Init(scene);
    }

    // Integration: in your Scene loader or main, do:
    //   auto obj = new CubeGameObject();
    //   obj->Load(fileStream);  // TO-DO: Open appropriate manifest stream
    //   scene->AddObject(obj);  // TO-DO: Implement AddObject in Scene




    /// ETRIA:
    /// Update per frame
    /// ET: aka: this function runs once per frame to enable smooth movement
    /// ET: deltaTime = time passed since last frame
    ///void CubeGameObject::Update(float deltaTime) {
        /// rotation
        ///if (rotateOverTime) {

            /// gets current rotation angle
            ///glm::vec4 rot = GetRotation();

            /// Adds rotaion over time to y axis 
            /// ET: (-> spins!) (90° p/s)
            ///currentRotation.y += 90.0f * deltaTime;

            /// Applies new rotation
            ///SetRotation(currentRotation);}

        /// movement
        ///if (moveOverTime) {

            /// gets current position (x,y,z)
            ///glm::vec3 currentPosition = GetPosition();

            /// jumpia via simple sin() movement on axis
            ///float time = static_cast<float>(glfwGetTime());

            /// jumps from -2 to +2
            ///currentPosition.y = sin(time) * 2.0f; 
            ///SetPosition(currentPosition);}

        /// scaling pulse
        ///if (pulseOverTime) {
            ///float time = static_cast<float>(glfwGetTime());

            /// Scales between 0.5 and 1.5
            ///float scale = abs(sin(time)) + 0.5f; 
            ///SetScale(glm::vec3(scale));}
    ///}


 
