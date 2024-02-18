
#include <algorithm>
#include <array>
#include <concepts>
#include <coroutine>
#include <iostream>
#include <optional>
#include <ranges>

#include "benchmark/benchmark.h"

template <std::movable T>
class Generator {
 public:
  struct promise_type {
    Generator<T> get_return_object() { return Generator{Handle::from_promise(*this)}; }
    static std::suspend_always initial_suspend() noexcept { return {}; }
    static std::suspend_always final_suspend() noexcept { return {}; }
    std::suspend_always yield_value(T value) noexcept {
      current_value = std::move(value);
      return {};
    }
    // Disallow co_await in generator coroutines.
    void await_transform() = delete;
    [[noreturn]] static void unhandled_exception() { throw; }
    void return_void() noexcept {}

    std::optional<T> current_value;
  };

  using Handle = std::coroutine_handle<promise_type>;

  explicit Generator(const Handle coroutine) : m_coroutine{coroutine} {}

  Generator() = default;
  ~Generator() {
    if (m_coroutine) {
      m_coroutine.destroy();
    }
  }

  Generator(const Generator&) = delete;
  Generator& operator=(const Generator&) = delete;

  Generator(Generator&& other) noexcept : m_coroutine{other.m_coroutine} { other.m_coroutine = {}; }
  Generator& operator=(Generator&& other) noexcept {
    if (this != &other) {
      if (m_coroutine) {
        m_coroutine.destroy();
      }
      m_coroutine = other.m_coroutine;
      other.m_coroutine = {};
    }
    return *this;
  }

  // Range-based for loop support.
  class Iter {
   public:
    void operator++() { m_coroutine.resume(); }
    const T& operator*() const { return *m_coroutine.promise().current_value; }
    bool operator==(std::default_sentinel_t) const { return !m_coroutine || m_coroutine.done(); }

    explicit Iter(const Handle coroutine) : m_coroutine{coroutine} {}

   private:
    Handle m_coroutine;
  };

  Iter begin() {
    if (m_coroutine) {
      m_coroutine.resume();
    }
    return Iter{m_coroutine};
  }

  std::default_sentinel_t end() { return {}; }

 private:
  Handle m_coroutine;
};

static constexpr size_t kSize = 16 * 16 * 16;

Generator<size_t> GenerateArray(size_t begin) {
  std::array<size_t, kSize> result;
  std::ranges::copy(std::views::iota(begin, begin + kSize), result.begin());

  for (const auto& i : result) {
    co_yield i;
  }
}

void TestGenerator1(benchmark::State& state) {
  for (auto _ : state) {
    std::array<size_t, kSize> result{static_cast<size_t>(rand())};
    benchmark::DoNotOptimize(result);
  }
}
BENCHMARK(TestGenerator1);

void TestGenerator2(benchmark::State& state) {
  for (auto _ : state) {
    auto x = GenerateArray(static_cast<size_t>(rand()));
    benchmark::DoNotOptimize(x);
  }
}
BENCHMARK(TestGenerator2);

void TestGenerator3(benchmark::State& state) {
  for (auto _ : state) {
    auto x = std::vector<int>(kSize, static_cast<size_t>(rand()));
    benchmark::DoNotOptimize(x);
  }
}
BENCHMARK(TestGenerator3);

BENCHMARK_MAIN();

template <class A, class B>
void fun(A a);

template <std::integral A>
void fun(A a) {
  std::cout << "int\n";
}

template <typename T>
concept X = requires(T t) {
  t.xxx(42);
  typename T::xxx_t;
  T::xyz::member;
};

void fun(X auto a) { std::cout << "double\n"; }

void func() { fun(1); }