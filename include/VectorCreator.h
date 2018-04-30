#ifndef _VectorCreator_HG_
#define _VectorCreator_HG

#include "iVectorCreator.h"

namespace MyMath {

	class VectorCreator: public iVectorCreator {
	public:

		VectorCreator();

		iVector2* createVec2(float num);
		iVector2* createVec2(float num1, float num2);
				
		iVector3* createVec3(float num);
		iVector3* createVec3(float num1, float num2, float num3);
				
		iVector4* createVec4(float num);
		iVector4* createVec4(float num1, float num2, float num3, float num4);
	};

}
#endif // !_VectorCreator_HG_

