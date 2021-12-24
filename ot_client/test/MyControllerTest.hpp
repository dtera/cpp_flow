#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "oatpp-test/UnitTest.hpp"

class MyControllerTest : public oatpp::test::UnitTest {
public:

  MyControllerTest() : UnitTest("TEST[MyControllerTest]"){}
  void onRun() override;

};