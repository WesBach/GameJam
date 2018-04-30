//#ifndef  _GTest_HG_
//#define _GTest_HG_
//#include <gtest\gtest.h>
//#include <iMath.h>
//#include <iVectorCreator.h>
//
//typedef MyMath::iVectorCreator* (*f_CreateVectorCreator)();
//extern MyMath::iVectorCreator* pVectorCreator;
//
////ASSERTS
//
//
//
//TEST(TCVec3, vec3) {
//	MyMath::iVector3* tempVec = pVectorCreator->createVec3(0.0f);
//	ASSERT_TRUE(tempVec->getX() == 0.0f);
//}
//
//TEST(TCVec2, vec2) {
//	MyMath::iVector2* tempVec = pVectorCreator->createVec2(0.0f);
//	ASSERT_FALSE(tempVec->getX() == 1.0f);
//}
//
//TEST(TCVec2Two, vec2Two) {
//	MyMath::iVector2* tempVec = pVectorCreator->createVec2(0.0f);
//	ASSERT_GE(tempVec->getX(), 0.0f);
//}
//
//TEST(TCVec4, vec4) {
//	MyMath::iVector4* tempVec = pVectorCreator->createVec4(0.0f);
//	ASSERT_LT(tempVec->getX(), 1.0f);
//}
//
//TEST(TCVec41, vec4) {
//	MyMath::iVector4* tempVec = pVectorCreator->createVec4(0.0f, 1.0f, 2.0f, 3.0f);
//	ASSERT_NE(tempVec->getW(), 0.0f);
//}
//
////EXPECT
//TEST(TCVec3Expect, vec3Ex) {
//	MyMath::iVector3* tempVec = pVectorCreator->createVec3(0.0f, 1.0f, 2.0f);
//	EXPECT_TRUE(tempVec->getX() == 0.0f);
//	EXPECT_TRUE(tempVec->getY() == 1.0f);
//	EXPECT_TRUE(tempVec->getZ() == 2.0f);
//};
//
//TEST(TCVec2Expect, vec2Ex) {
//	MyMath::iVector2* tempVec = pVectorCreator->createVec2(0.0f, 1.0f);
//	EXPECT_FALSE(tempVec->getX() != 1.0f);
//	EXPECT_FALSE(tempVec->getY() != 0.0f);
//};
//
//TEST(TCVec2TwoExpect, vec2Ex) {
//	MyMath::iVector2* tempVec = pVectorCreator->createVec2(0.0f, 1.0f);;
//	EXPECT_GE(tempVec->getX(), 0.0f);
//}
//
//TEST(TCVec4Expect, vec4Ex) {
//	MyMath::iVector4* tempVec = pVectorCreator->createVec4(0.0f);
//	tempVec->getX() = 1.0f;
//	EXPECT_LT(tempVec->getX(), 2.0f);
//	EXPECT_TRUE(tempVec->getZ() == tempVec->getY());
//}
//
//TEST(TCVec4TwoExpect, vec4Ex) {
//	MyMath::iVector4* tempVec = pVectorCreator->createVec4(0.0f, 1.0f, 2.0f, 0.0f);
//	EXPECT_NE(tempVec->getX(), tempVec->getY());
//	EXPECT_TRUE(tempVec->getX() == tempVec->getW());
//}
//
//#endif // ! _GTest_HG_