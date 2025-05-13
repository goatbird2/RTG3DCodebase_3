#include "ModelFactory.h"
#include <assert.h>
#include "AIModel.h"
#include "Cube.h"
#include "CubeModel.h"
#include "PrincipleAxes.h"

Model* ModelFactory::makeNewModel(std::string _type)
{
	printf("TYPE: %s \n", _type.c_str());
	//There is no point in making one of the model base class 
	//as it doesn't do anything 
	if (_type == "AI")
	{
		return new AIModel();
	}
	//else if (_type == "CUBE")
	//{
		//return new Cube();
	//}

	if (_type == "AXES")
	{
		return new CGPrincipleAxes();
	}

	if (_type == "CUBE")
	{
		return new CubeModel(); //ET: Now returns the proper Model subclass
	}

	else
	{
		printf("UNKNOWN MODEL TYPE: %s \n", _type.c_str());
		assert(0);
		return nullptr;
	}
}
