#ifndef _Math_HG_
#define _Math_HG_

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

namespace MyMath {

	class Vector2 {
	public:
		//Vector2(const float& num);
		//Vector2(const float& num1, const float& num2);

		Vector2(float num);
		Vector2(float num1, float num2);

		float& operator[](int x);

		float x;
		float y;
	};

	class Vector3 {
	public:
		Vector3(float num);
		Vector3(float num1, float num2, float num3);

		float& operator[](int x);

		float x;
		float y;
		float z;
	};

	class Vector4 {
	public:
		Vector4(float num);
		Vector4(float num1, float num2, float num3, float num4);

		float& operator[](int x);

		float x;
		float y;
		float z;
		float w;
	};
}

#endif // !_Math_HG_

