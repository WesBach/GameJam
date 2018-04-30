#ifndef _iCollisionListener_HG_
#define _iCollisionListener_HG_
#include "iCollisionBody.h"

namespace nPhysics {
	class iCollisionListener {
	public:
		virtual ~iCollisionListener() {}
		virtual void Collision(iCollisionBody* bodyA, iCollisionBody* bodyB) = 0;
	protected:
		iCollisionListener() {}
	private:
		iCollisionListener(const iCollisionListener& other) {}
		iCollisionListener& operator=(const iCollisionListener& other) { return *this; }
	};
}

#endif // !_iCollisionListener_HG_
