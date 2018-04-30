#ifndef _iCollisionBody_HG_
#define _iCollisionBody_HG_
#include "enums.h"

namespace nPhysics {
	class iCollisionBody {
	public:
		virtual ~iCollisionBody() {}
		virtual eCollisionBodyType GetBodyType() { return mBodyType; };
		virtual void SetBodyType(eCollisionBodyType bodyType) { this->mBodyType = bodyType; };
	protected:
		iCollisionBody() {}
		iCollisionBody(const iCollisionBody& other) {}
		iCollisionBody& operator=(const iCollisionBody& other) { return *this; }
	private:
		eCollisionBodyType mBodyType;
	};
}

#endif // !_iCollisionBody_HG_

