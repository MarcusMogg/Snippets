import dsn;

#include "catch2/catch_test_macros.hpp"

TEST_CASE("array init", "[array]") {
  SECTION("uninit") { [[maybe_unused]] dsn::Array<int, 1> a; }
  SECTION("default init") {
    [[maybe_unused]] dsn::Array<int, 1> a{};
    CHECK(a[0] == 0);
    [[maybe_unused]] dsn::Array<int, 1> b{1};
    CHECK(b[0] == 1);
  }
  SECTION("make array") {
    {
      auto a = dsn::MakeArray(1, 2, 3);
      CHECK(std::same_as<decltype(a), dsn::Array<int, 3>>);
      CHECK(a[0] == 1);
      CHECK(a[1] == 2);
      CHECK(a[2] == 3);
    }
    {
      const auto a = dsn::MakeArray(1, 2, 3.);
      CHECK(std::same_as<decltype(a), const dsn::Array<double, 3>>);
      CHECK(a[0] == 1.);
      CHECK(a[1] == 2.);
      CHECK(a[2] == 3.);
    }
    {
      auto a = dsn::MakeArray<std::string>("1", "2", "3");
      CHECK(std::same_as<decltype(a), dsn::Array<std::string, 3>>);
      CHECK(a[0] == "1");
      CHECK(a[1] == "2");
      CHECK(a[2] == "3");
    }
  }

  SECTION("to array") {
    {
      auto a = dsn::ToArray({1, 2, 3});
      CHECK(std::same_as<decltype(a), dsn::Array<int, 3>>);
      CHECK(a[0] == 1);
      CHECK(a[1] == 2);
      CHECK(a[2] == 3);
    }
    {
      double b[]{1, 2, 3};
      const auto a = dsn::ToArray(b);

      CHECK(std::same_as<decltype(a), const dsn::Array<double, 3>>);
      CHECK(a[0] == 1.);
      CHECK(a[1] == 2.);
      CHECK(a[2] == 3.);
    }
    {
      using namespace std::string_literals;
      const auto a = dsn::ToArray({"1"s, "2"s, "3"s});
      CHECK(std::same_as<decltype(a), const dsn::Array<std::string, 3>>);
      CHECK(a[0] == "1");
      CHECK(a[1] == "2");
      CHECK(a[2] == "3");
    }
  }

  SECTION("ducing guide") {
    {
      auto a = dsn::Array{1, 2, 3};
      CHECK(std::same_as<decltype(a), dsn::Array<int, 3>>);
      CHECK(a[0] == 1);
      CHECK(a[1] == 2);
      CHECK(a[2] == 3);
    }
    {
      // complie fail
      // how to check it?
      // auto b = dsn::Array{1, 2u, 3};
      dsn::Array<int, 1> x;
      x.rend();
    }
  }
}

TEST_CASE("range", "[array]") {
  SECTION("range concept check") {
    CHECK(std::ranges::range<dsn::Array<int, 1>>);
    CHECK(std::ranges::range<const dsn::Array<int, 1>>);

    CHECK(std::ranges::output_range<dsn::Array<int, 1>, int>);

    CHECK(std::ranges::random_access_range<const dsn::Array<int, 1>>);
    CHECK(std::ranges::random_access_range<dsn::Array<int, 1>>);

    CHECK(std::ranges::contiguous_range<const dsn::Array<int, 1>>);
    CHECK(std::ranges::contiguous_range<dsn::Array<int, 1>>);
    CHECK(std::ranges::common_range<const dsn::Array<int, 1>>);
    CHECK(std::ranges::common_range<dsn::Array<int, 1>>);
  }

  SECTION("type check") { [[maybe_unused]] dsn::Array<int, 1> a{}; }
}
