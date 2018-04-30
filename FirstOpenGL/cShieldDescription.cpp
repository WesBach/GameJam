#include "cShieldDescription.h"
#include "cGameObject.h"
#include <iPhysicsFactory.h>

extern nPhysics::iPhysicsWorld* theWorld;
extern nPhysics::iPhysicsFactory* theFactory;

cShieldDescription::cShieldDescription(int numbodies,glm::vec3 mainBodyPosition) {

	//TODO:: get generic sphere and make a few bodies
	cGameObject* tempObject;
	float positionDisplacement = 3.f;
	tempObject = new cGameObject();
	tempObject->position = mainBodyPosition;

	for (int i = 0; i < numbodies; i++) {
		tempObject = new cGameObject();
		tempObject->position = mainBodyPosition;
		tempObject->position.y -= 1.f;
		glm::vec3 direction(0.0f);
		//give the outer objects a 2 point displacement
		if (i == 0) {
			tempObject->position.x += positionDisplacement;
		}
		else if (i == 1) {
			tempObject->position.x -= positionDisplacement;
		}
		else if (i % 1 == 0) {
			tempObject->position.z += positionDisplacement;
		}

		tempObject->meshName = "Sphere_xyz_n_uv.ply";
		tempObject->bIsWireFrame = true;
		tempObject->bIsSkyBoxObject = false;
		tempObject->diffuseColour = glm::vec4(1.f);
		tempObject->scale = 0.25f;
		tempObject->radius = 0.125f;
		tempObject->typeOfObject = eTypeOfObject::SPHERE;
		this->pShieldBodies.push_back(tempObject);
	}
}
