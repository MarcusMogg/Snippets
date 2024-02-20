#include <algorithm>
#include <iostream>
#include <random>
#include <ranges>
import dsn;

#include "catch2/catch_test_macros.hpp"

TEST_CASE("", "[skip_list]") {
  SECTION("construct empty list") {
    dsn::SkipList<int, int> s1{};
    dsn::SkipList<int, int> s;
    CHECK(s.begin() == s.end());
    CHECK(s.cbegin() == s.cend());
  }

  SECTION("range check") {
    CHECK(std::ranges::range<dsn::SkipList<int, int>>);
    CHECK(std::ranges::range<const dsn::SkipList<int, int>>);

    // false, beceause you can't modify *iterator
    CHECK(!std::ranges::output_range<dsn::SkipList<int, int>, std::pair<const int, int>>);

    CHECK(std::ranges::forward_range<dsn::SkipList<int, int>>);
    CHECK(std::ranges::forward_range<const dsn::SkipList<int, int>>);
  }

  SECTION("insert and find") {
    dsn::SkipList<int, int> s;

    CHECK(!s.Contains(1));
    CHECK(!s.TryGet(1).has_value());
    s.TryEmplace(1, 1);
    CHECK(s.Contains(1));
    CHECK(s.TryGet(1).value().get().second == 1);
    CHECK(s.begin() != s.end());

    CHECK(!s.Contains(2));
    CHECK(static_cast<bool>(s.TryGet(2)) == false);
    s.TryEmplace(2, 2);
    CHECK(s.Contains(2));
    CHECK(s.TryGet(2).value().get().second == 2);
    CHECK(s.begin() != s.end());
  }

  SECTION("Ordered traversal") {
    dsn::SkipList<int, int> s;
    std::vector<std::pair<int, int>> target;

    auto equal = [&]() {
      std::ranges::sort(target);
      auto res = std::ranges::mismatch(s, target);
      return (res.in1 == s.end() && res.in2 == target.end());
    };

    CHECK(equal());

    using namespace std::ranges::views;
    thread_local std::mt19937 generator{std::random_device{}()};
    thread_local std::uniform_int_distribution<int> rand{};
    for (auto [i, j] : iota(1, 100) | transform([&](auto&& i) {
                         return std::pair{rand(generator), i};
                       })) {
      if (!s.Contains(i)) {
        target.emplace_back(i, j);
      }
      s.TryEmplace(i, j);

      CHECK(equal());
    }
  }
}
