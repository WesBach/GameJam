#ifndef _cPhysicsEntity_HG_
#define _cPhysicsEntity_HG_
#include <iCollisionBody.h>
#include <ObjectDescriptions.h>

namespace nPhysics {
	class cPhysicsEntity : public iCollisionBody {
	public:
		cPhysicsEntity(const sEntityDescription& desc);

		float& GetPlayerHealth();
		void GetPlayerRigidBody(iRigidBody* theBody);
	private:
		float mHealth;
		iRigidBody* mBody;
	};
}

#endif // !_cPlayer_HG_