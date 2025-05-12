#include "AIModel.h"
#include "stringHelp.h"
#include "AIMesh.h"

AIModel::AIModel()
{
	m_type = "AI";
	m_AImesh = nullptr;
}

AIModel::~AIModel()
{
	delete m_AImesh;
}

//ET: loads mesh of a "new" Model after decleration in manifest and objects creation in scene
void AIModel::Load(ifstream& _file)
{
	//ET: creates AIMesh based on fileName in the manifest
	Model::Load(_file);
	string fileName;
	StringHelp::String(_file, "FILE", fileName);

	// ASSIMP loads obj model
	m_AImesh = new AIMesh(fileName);
}

void AIModel::Render()
{
	m_AImesh->render();
}
