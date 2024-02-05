import Dsn;
import <ranges>;

#include "catch2/catch_test_macros.hpp"

TEST_CASE("array init", "[array]") {
  SECTION("uninit") { [[maybe_unused]] Dsn::Array<int, 1> a; }
  SECTION("default init") {
    [[maybe_unused]] Dsn::Array<int, 1> a{};
    CHECK(a[0] == 0);
    [[maybe_unused]] Dsn::Array<int, 1> b{1};
    CHECK(b[0] == 1);
  }
  SECTION("make array") {
    {
      auto a = Dsn::MakeArray(1, 2, 3);
      CHECK(std::same_as<decltype(a), Dsn::Array<int, 3>>);
      CHECK(a[0] == 1);
      CHECK(a[1] == 2);
      CHECK(a[2] == 3);
    }
    {
      const auto a = Dsn::MakeArray(1, 2, 3.);
      CHECK(std::same_as<decltype(a), const Dsn::Array<double, 3>>);
      CHECK(a[0] == 1.);
      CHECK(a[1] == 2.);
      CHECK(a[2] == 3.);
    }
    {
      auto a = Dsn::MakeArray<std::string>("1", "2", "3");
      CHECK(std::same_as<decltype(a), Dsn::Array<std::string, 3>>);
      CHECK(a[0] == "1");
      CHECK(a[1] == "2");
      CHECK(a[2] == "3");
    }
  }

  SECTION("to array") {
    {
      auto a = Dsn::ToArray({1, 2, 3});
      CHECK(std::same_as<decltype(a), Dsn::Array<int, 3>>);
      CHECK(a[0] == 1);
      CHECK(a[1] == 2);
      CHECK(a[2] == 3);
    }
    {
      double b[]{1, 2, 3};
      const auto a = Dsn::ToArray(b);

      CHECK(std::same_as<decltype(a), const Dsn::Array<double, 3>>);
      CHECK(a[0] == 1.);
      CHECK(a[1] == 2.);
      CHECK(a[2] == 3.);
    }
    {
      using namespace std::string_literals;
      const auto a = Dsn::ToArray({"1"s, "2"s, "3"s});
      CHECK(std::same_as<decltype(a), const Dsn::Array<std::string, 3>>);
      CHECK(a[0] == "1");
      CHECK(a[1] == "2");
      CHECK(a[2] == "3");
    }
  }
}

TEST_CASE("range", "[array1]") {
  SECTION("range concept check") {
    CHECK(std::ranges::range<Dsn::Array<int, 1>>);
    CHECK(std::ranges::range<const Dsn::Array<int, 1>>);
  }

  SECTION("type check") { [[maybe_unused]] Dsn::Array<int, 1> a{}; }
}
