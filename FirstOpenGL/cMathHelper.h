#ifndef _cMathHelper_HG_
#define _cMathHelper_HG_

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/quaternion.hpp>	// For the target look at

struct Matrix
{
	float m[16];
};

const glm::mat4 IDENTITYMATRIX = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};

const Matrix IDENTITY_MATRIX = { {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	} };

// From
// http://mmmovania.blogspot.ca/2014/03/making-opengl-object-look-at-another.html
static glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
//	static glm::mat4 ObjectLookAt_GetMatrixMethod_Quaternion(const glm::vec3& objectEye, const glm::vec3& targetAt); 
static glm::mat4 ObjectLookAt_GetMatrixMethod_Quaternion(const glm::vec3& objectEye, const glm::vec3& targetAt, glm::quat &resultingEyeOrientation);
static glm::mat4 ObjectLookAt_GetMatrixMethod_MatrixOnly(const glm::vec3& objectEye, const glm::vec3& targetAt);
// From: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
static glm::quat RotateTowards(glm::quat qStart, glm::quat qEnd, float maxAngleRadians);
//static glm::mat4 ObjectLookAt_GetMatrixMethod3(const glm::vec3& object, const glm::vec3& target);
static bool doQuaternionsMatch(const glm::quat &q1, const glm::quat &q2);
static float angleBetweenQuaternionsRadians(const glm::quat &q1, const glm::quat &q2);

static float calcDistanceBetweenTwoPoints(glm::vec3 a, glm::vec3 b);

inline static float degToRad(float degrees);
inline static float radToDeg(float radians);

float GetMag(glm::vec3 input);
glm::vec3 GetUnitVector(glm::vec3 input, float mag);

float getRotationAmount(const glm::vec3& vecToTurnTo, const glm::vec3& myPos, const glm::vec3& forward);

float GetMag(glm::vec3 input);
glm::vec3 GetUnitVector(glm::vec3 input, float mag);

void RotateAboutY(Matrix* m, float angle);
Matrix MultiplyMatrices(const Matrix* m1, const Matrix* m2);
glm::vec3 VectorMultiplyMatrices(const Matrix* m1, const glm::vec3* v1);
void TranslateMatrix(Matrix* m, float x, float y, float z);
void RotateAboutY(glm::mat4* m, float angle);
glm::vec3 VectorMultiplyMatrices(const glm::mat4& m1, const glm::vec3* v1);

#endif