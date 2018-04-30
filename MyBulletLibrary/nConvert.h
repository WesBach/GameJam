#ifndef _nConvert_HG_
#define _nConvert_HG_
#include <bullet\btBulletDynamicsCommon.h>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\quaternion.hpp>

namespace nPhysics {

	//Conversion namespace for easy switch between glm and bullet types
	namespace nConvert {
		inline btVector3 vec3ToBullet(const glm::vec3& v)
		{
			return btVector3(v.x, v.y, v.z);
		}
		inline glm::vec3 vec3ToGLM(const btVector3& v)
		{
			return glm::vec3(v[0], v[1], v[2]);
		}

		inline void vec3ToGLM(const btVector3& vIn, glm::vec3& vOut)
		{
			vOut.x = vIn[0];
			vOut.y = vIn[1];
			vOut.z = vIn[2];
		}

		inline void vec3ToGLM(const glm::vec3& vIn, btVector3& vOut)
		{
			vOut[0] = vIn.x;
			vOut[1] = vIn.y;
			vOut[2] = vIn.z;
		}

		//quaternion conversion from glm to bullet
		inline btQuaternion ToBullet(const glm::quat& q)
		{
			return btQuaternion(q.x, q.y, q.z,q.w);
		}

		//quaternion conversion from bullet to glm
		inline glm::quat ToGLM(const btQuaternion& v)
		{
			return glm::quat(v[0], v[1], v[2],v[3]);
		}

		//quaternion conversion from glm to bullet
		inline void ToGLM(const btQuaternion& v, glm::quat& out)
		{
			out = glm::quat(v[0], v[1], v[2], v[3]);
		}

		//mat4 conversion from glm to bullet
		inline void ToGLM(const btTransform& transform, glm::mat4& out)
		{
			transform.getOpenGLMatrix(&out[0][0]);
		}

		//mat4 convertsion from glm to bullet
		inline btTransform ToBullet(const glm::mat4& mat)
		{
			btTransform transform;
			transform.setFromOpenGLMatrix(&mat[0][0]);
			return transform;
		}
	}
}

#endif // !_nConvert_HG_
