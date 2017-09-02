// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

static const size_t SIZE = JSON_ARRAY_SIZE(3) + 2 * JSON_OBJECT_SIZE(1) + 8;

template <typename TArray>
TArray buildArray() {
  TArray array;
  array.add(42);
  array.createNestedObject()["abcdefg"] = "abcdefg";
  array.createNestedObject()["ABCDEFG"] = std::string("ABCDEFG");
  return array;
}

template <typename TArray>
void validateArray(TArray& array) {
  CHECK(array.size() == 3);
  REQUIRE(array[0] == 42);
  REQUIRE(array[1].template is<JsonObject>());
  REQUIRE(array[1]["abcdefg"] == std::string("abcdefg"));
  REQUIRE(array[2].template is<JsonObject>());
  REQUIRE(array[2]["ABCDEFG"] == std::string("ABCDEFG"));

  REQUIRE(SIZE == array.memoryUsage());
}

TEST_CASE("DynamicJsonArray::operator=()") {
  DynamicJsonArray array;
  array.add(666);
  array.add(666);

  SECTION("operator=(const DynamicJsonArray&)") {
    array = buildArray<DynamicJsonArray>();
    validateArray(array);
  }

  SECTION("operator=(const StaticJsonArray<N>&)") {
    array = buildArray<StaticJsonArray<SIZE> >();
    validateArray(array);
  }
}

TEST_CASE("StaticJsonArray::operator=()") {
  StaticJsonArray<SIZE> array;
  array.add(666);
  array.add(666);

  SECTION("operator=(const DynamicJsonArray&)") {
    array = buildArray<DynamicJsonArray>();
    validateArray(array);
  }

  SECTION("operator=(const StaticJsonArray<N>&)") {
    array = buildArray<StaticJsonArray<SIZE> >();
    validateArray(array);
  }

  SECTION("operator=(const StaticJsonArray<N+1>&)") {
    array = buildArray<StaticJsonArray<SIZE + 1> >();
    validateArray(array);
  }
}
