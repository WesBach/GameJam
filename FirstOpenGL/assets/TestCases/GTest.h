#ifndef  _GTest_HG_
#define _GTest_HG_
#include <string>
#include <iostream>
#include <VectorCreator.h>
#include <Math.h>
#include <windows.h>
#include <gtest\gtest.h>
//BLACKBOX TESTS FOR GLM

typedef MyMath::VectorCreator* (*f_CreateVectorCreator)();
MyMath::VectorCreator* pVectorCreator;

void CreateVectorCreator() {
	//if debug use the debug dll else use release dll
#if _DEBUG
	std::string libraryFile = "MyMathDLL.dll";
#endif

	std::string createFactoryName = "createVectorCreator";

	HINSTANCE hGetProckDll = LoadLibraryA(libraryFile.c_str());
	if (!hGetProckDll)
	{
		std::cout << "oh no!" << std::endl;
	}

	f_CreateVectorCreator CreateFactory = 0;

	CreateFactory = (f_CreateVectorCreator)GetProcAddress(hGetProckDll, createFactoryName.c_str());
	if (!CreateFactory)
	{
		std::cout << "where's the CreateFactory???" << std::endl;
		system("pause");

	}

	pVectorCreator = CreateFactory();
}

//ASSERTS

TEST(TCVec3, vec3) {
	MyMath::Vector3 tempVec(0.0f);
	ASSERT_TRUE(tempVec.x == 0.0f);
}

TEST(TCVec2, vec2) {
	MyMath::Vector2 tempVec(0.0f);
	ASSERT_FALSE(tempVec[0] == 1.0f);
}

TEST(TCVec2Two, vec2Two) {
	MyMath::Vector2 tempVec(1.0f);
	ASSERT_GE(tempVec[1], 0.0f);
}

TEST(TCVec4, vec4) {
	MyMath::Vector4 tempVec(0.0f);
	ASSERT_LT(tempVec[0], 1.0f);
}

TEST(TCVec41, vec4) {
	MyMath::Vector4 tempVec(0.0f, 1.0f, 2.0f, 3.0f);
	ASSERT_NE(tempVec[3], 0.0f);
}

//EXPECT
TEST(TCVec3Expect, vec3Ex) {
	MyMath::Vector3 tempVec(0.0f, 1.0f, 2.0f);
	EXPECT_TRUE(tempVec[0] == 0.0f);
	EXPECT_TRUE(tempVec[1] == 1.0f);
	EXPECT_TRUE(tempVec[2] == 2.0f);
};

TEST(TCVec2Expect, vec2Ex) {
	MyMath::Vector2 tempVec(0.0f, 1.0f);
	EXPECT_FALSE(tempVec[0] != 0.0f);
	EXPECT_FALSE(tempVec[1] != 1.0f);
};

TEST(TCVec2TwoExpect, vec2Ex) {
	MyMath::Vector2 tempVec(1.0f);
	EXPECT_GE(tempVec[1], 0.0f);
}

TEST(TCVec4Expect, vec4Ex) {
	MyMath::Vector4 tempVec(0.0f);
	tempVec.x = 1.0f;
	EXPECT_LT(tempVec[0], 2.0f);
	EXPECT_TRUE(tempVec.x == tempVec[0]);
}

TEST(TCVec4TwoExpect, vec4Ex) {
	MyMath::Vector4 tempVec(0.0f, 1.0f, 2.0f, 0.0f);
	EXPECT_NE(tempVec[0], tempVec[1]);
	EXPECT_TRUE(tempVec[0] == tempVec[3]);
}
#endif // ! _GTest_HG_
