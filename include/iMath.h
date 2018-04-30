#ifndef _iMath_HG_
#define _iMath_HG_

namespace MyMath {

	class iVector2 {
	public:
		virtual float& getX() = 0;
		virtual float& getY() = 0;
	};

	class iVector3 {
	public:
		virtual float& getX() = 0;
		virtual float& getY() = 0;
		virtual float& getZ() = 0;
	};

	class iVector4 {
	public:
		virtual float& getX() = 0;
		virtual float& getY() = 0;
		virtual float& getZ() = 0;
		virtual float& getW() = 0;
	};
}

#endif
