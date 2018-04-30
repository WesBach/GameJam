#ifndef  _iVectorCreator_HG_
#define _iVectorCreator_HG_
#include "iMath.h"
namespace MyMath {

	class iVectorCreator {
	public:
		virtual iVector2* createVec2(float num) = 0;
		virtual iVector2* createVec2(float num1, float num2) = 0;

		virtual iVector3* createVec3(float num) = 0;
		virtual iVector3* createVec3(float num1, float num2, float num3 ) = 0;

		virtual iVector4* createVec4(float num) = 0;
		virtual iVector4* createVec4(float num1, float num2, float num3, float num4) = 0;
	};
}

#endif // ! _iVectorCreator_HG_

