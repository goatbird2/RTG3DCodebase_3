#include "Model.h"
#include "stringHelp.h"


Model::Model()
{
	m_type = "MODEL";
}

Model::~Model()
{
}

void Model::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
}
