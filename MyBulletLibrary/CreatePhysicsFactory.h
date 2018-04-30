#pragma once
#include <iPhysicsFactory.h>

//external function used to access the dll

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

namespace nPhysics {
	EXTERN_DLL_EXPORT iPhysicsFactory* CreateFactory();
}