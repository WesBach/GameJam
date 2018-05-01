#include "cMathHelper.h"

//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
//static
bool doQuaternionsMatch(const glm::quat &q1, const glm::quat &q2)
{
	float dotProd = glm::dot(q1, q2);
	if (abs(dotProd - 1.0f) < 0.001)
	{
		return true;
	}
	return false;
}

//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
//static
float angleBetweenQuaternionsRadians(const glm::quat &q1, const glm::quat &q2)
{
	float dotProd = glm::dot(q1, q2);
	//You can also get the angle between q1 and q2 by taking the acos() of this dot product.
	return acos(dotProd);
}

// From
// http://mmmovania.blogspot.ca/2014/03/making-opengl-object-look-at-another.html
//static
glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
{
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f)
	{
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length(rotationAxis) < 0.01)
		{
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
		}
		rotationAxis = glm::normalize(rotationAxis);
		return glm::angleAxis(180.0f, rotationAxis);
	}

	rotationAxis = glm::cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

// From: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
//static
glm::quat RotateTowards(glm::quat qStart, glm::quat qEnd, float maxAngleRadians)
{
	if (maxAngleRadians < 0.001f)
	{
		// No rotation allowed. Prevent dividing by 0 later.
		return qStart;
	}

	float cosTheta = glm::dot(qStart, qEnd);

	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if (cosTheta > 0.9999f)
	{
		return qEnd;
	}

	// Avoid taking the long path around the sphere
	if (cosTheta < 0)
	{
		qStart = qStart*-1.0f;
		cosTheta *= -1.0f;
	}

	float angle = acos(cosTheta);

	// If there is only a 2 degree; difference, and we are allowed 5 degrees
	// then we arrived.
	if (angle < maxAngleRadians)
	{
		return qEnd;
	}

	float fT = maxAngleRadians / angle;
	angle = maxAngleRadians;

	glm::quat res = (sin((1.0f - fT) * angle) * qStart + sin(fT * angle) * qEnd) / sin(angle);
	res = glm::normalize(res);
	return res;
}

////static
//glm::mat4 CMathHelper::ObjectLookAt_GetMatrixMethod_Quaternion(const glm::vec3& object, const glm::vec3& target)
//{
//    glm::vec3 delta =  (target-object);
//    glm::vec3 desiredUp(0,1,0.00001);
//    glm::quat rot1 = RotationBetweenVectors(glm::vec3(0,0,1), delta);
//    glm::vec3 right = glm::cross(delta, desiredUp);
//    desiredUp = glm::cross(right, delta);
//
//    glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
//    glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);
//    glm::quat targetOrientation = rot2 * rot1;
//	glm::mat4 M=glm::toMat4(targetOrientation);
//    M[3][0] = object.x;
//    M[3][1] = object.y;
//    M[3][2] = object.z;
//    return M;
//}

//static
glm::mat4 ObjectLookAt_GetMatrixMethod_Quaternion(const glm::vec3& objectEye, const glm::vec3& targetAt, glm::quat &resultingEyeOrientation)
{
	glm::vec3 delta = (targetAt - objectEye);
	glm::vec3 desiredUp(0, 1, 0.00001);
	glm::quat rot1 = RotationBetweenVectors(glm::vec3(0, 0, 1), delta);
	glm::vec3 right = glm::cross(delta, desiredUp);
	desiredUp = glm::cross(right, delta);

	glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
	glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);

	//glm::quat targetOrientation = rot2 * rot1;
	resultingEyeOrientation = rot2 * rot1;
	glm::mat4 M = glm::toMat4(resultingEyeOrientation);
	// This looks like it's directly setting the position offsets, rather than applying a matrix
	M[3][0] = objectEye.x;
	M[3][1] = objectEye.y;
	M[3][2] = objectEye.z;
	return M;
}

//static
glm::mat4 ObjectLookAt_GetMatrixMethod_MatrixOnly(const glm::vec3& objectEye, const glm::vec3& targetAt)
{
	//second method
	glm::vec3 up;
	glm::vec3 direction(glm::normalize(targetAt - objectEye));
	if (abs(direction.x) < 0.00001 && abs(direction.z) < 0.00001)
	{
		if (direction.y > 0)
		{
			up = glm::vec3(0.0, 0.0, -1.0);
		}
		else
		{
			up = glm::vec3(0.0, 0.0, 1.0);
		}
	}
	else
	{
		up = glm::vec3(0.0, 1.0, 0.0);
	}
	up = glm::normalize(up);

	glm::vec3 right = glm::normalize(glm::cross(up, direction));
	up = glm::normalize(glm::cross(direction, right));

	return glm::mat4(right.x, right.y, right.z, 0.0f,
		up.x, up.y, up.z, 0.0f,
		direction.x, direction.y, direction.z, 0.0f,
		objectEye.x, objectEye.y, objectEye.z, 1.0f);
}

//static
float calcDistanceBetweenTwoPoints(glm::vec3 a, glm::vec3 b)
{
	//return sqrt( pow(a.x-b.x, 2) + pow(a.y-b.y, 2) + pow( a.z-b.z, 2) );
	return glm::distance(a, b);
}

//static
float degToRad(float degrees)
{
	return glm::radians(degrees);
}

//static
float radToDeg(float radians)
{
	return glm::degrees(radians);
}

float GetMag(glm::vec3 input)
{
	return sqrt(
		pow(input.x, 2.f) +
		pow(input.y, 2.f) +
		pow(input.z, 2.f)
	);
}

glm::vec3 GetUnitVector(glm::vec3 input, float mag)
{
	return glm::vec3(input.x / mag, input.y / mag, input.z / mag);
}

glm::vec3 VectorMultiplyMatrices(const Matrix* m1, const glm::vec3* v1)
{
	glm::vec3 out;
	unsigned int row, column, row_offset;

	out.x = (m1->m[0] * v1->x) +
		(m1->m[4] * v1->y) +
		(m1->m[8] * v1->z) +
		(m1->m[12] * 1.0f);

	out.y = (m1->m[1] * v1->x) +
		(m1->m[5] * v1->y) +
		(m1->m[9] * v1->z) +
		(m1->m[13] * 1.0f);

	out.z = (m1->m[2] * v1->x) +
		(m1->m[6] * v1->y) +
		(m1->m[10] * v1->z) +
		(m1->m[14] * 1.0f);

	return out;
}

glm::vec3 VectorMultiplyMatrices(const glm::mat4& m1, const glm::vec3* v1)
{
	glm::vec3 out;
	unsigned int row, column, row_offset;

	//TODO:: test to see outcome ... could be wrong
	out.x = (m1[0].x * v1->x) +
		(m1[0].w * v1->y) +
		(m1[1].w * v1->z) +
		(m1[2].w * 1.0f);

	out.y = (m1[0].y * v1->x) +
		(m1[1].x * v1->y) +
		(m1[2].x * v1->z) +
		(m1[3].x * 1.0f);

	out.z = (m1[0].y * v1->x) +
		(m1[1].y * v1->y) +
		(m1[2].y * v1->z) +
		(m1[3].y * 1.0f);

	return out;
}

void RotateAboutY(Matrix* m, float angle) {
	Matrix rotation = IDENTITY_MATRIX;
	float sine = (float)sin(angle);
	float cosine = (float)cos(angle);

	rotation.m[0] = cosine;
	rotation.m[8] = sine;
	rotation.m[2] = -sine;
	rotation.m[10] = cosine;

	memcpy(m->m, MultiplyMatrices(m, &rotation).m, sizeof(m->m));
}
//Xx, Xy, Xz, 0
//Yx, Yy, Yz, 0
//Zx, Zy, Zz, 0
//Tx, Ty, Tz, 1

void RotateAboutY(glm::mat4* m, float angle) {
	glm::mat4 rotation = IDENTITYMATRIX;
	float sine = (float)sin(angle);
	float cosine = (float)cos(angle);

	//TODO:: fix this
	//iswrong
	rotation[0].x = cosine;
	rotation[1].w = sine;
	rotation[0].y = -sine;
	rotation[2].y = cosine;

	*m = glm::matrixCompMult(*m, rotation);
	//memcpy(m, glm::matrixCompMult(*m, rotation).length, sizeof(m));
}

Matrix MultiplyMatrices(const Matrix* m1, const Matrix* m2)
{
	Matrix out = IDENTITY_MATRIX;
	unsigned int row, column, row_offset;

	for (row = 0, row_offset = row * 4; row < 4; ++row, row_offset = row * 4)
		for (column = 0; column < 4; ++column)
			out.m[row_offset + column] =
			(m1->m[row_offset + 0] * m2->m[column + 0]) +
			(m1->m[row_offset + 1] * m2->m[column + 4]) +
			(m1->m[row_offset + 2] * m2->m[column + 8]) +
			(m1->m[row_offset + 3] * m2->m[column + 12]);

	return out;
}

void TranslateMatrix(Matrix* m, float x, float y, float z)
{
	Matrix translation = IDENTITY_MATRIX;

	translation.m[12] = x;
	translation.m[13] = y;
	translation.m[14] = z;

	memcpy(m->m, MultiplyMatrices(m, &translation).m, sizeof(m->m));
}