module;
#include <concepts>
#include <type_traits>
#include <utility>
export module Dsn:Array;

namespace Dsn {
export template <class T, size_t N>
  requires std::is_object_v<T> && (N > 0)
class Array {
 public:
  T data[N];

 public:
  T* begin() noexcept { return data; }
  T* end() noexcept { return data + N; }
  const T* begin() const noexcept { return data; }
  const T* end() const noexcept { return data + N; }

  T& operator[](size_t index) noexcept { return data[index]; }
  const T& operator[](size_t index) const noexcept { return data[index]; }
};

export template <class D = void, class... Args>
constexpr auto MakeArray(Args&&... args) {
  if constexpr (std::same_as<D, void>) {
    using CommType = std::common_type_t<Args...>;
    return Array<CommType, sizeof...(Args)>{static_cast<CommType>(std::forward<Args>(args))...};
  } else {
    return Array<D, sizeof...(Args)>{static_cast<D>(std::forward<Args>(args))...};
  }
}

namespace detail {
template <class T, size_t N, size_t... I>
constexpr auto ToArrayImpl(T (&a)[N], std::index_sequence<I...> /*unused*/) {
  return Array<std::remove_cv_t<T>, N>{{a[I]...}};
}
template <class T, size_t N, size_t... I>
constexpr auto ToArrayImpl(T (&&a)[N], std::index_sequence<I...> /*unused*/) {
  return Array<std::remove_cv_t<T>, N>{{std::move(a[I])...}};
}
}  // namespace detail

export template <class T, size_t N>
constexpr Array<std::remove_cv_t<T>, N> ToArray(T (&a)[N]) {
  return detail::ToArrayImpl(a, std::make_integer_sequence<size_t, N>{});
}
export template <class T, size_t N>
constexpr Array<std::remove_cv_t<T>, N> ToArray(T (&&a)[N]) {
  return detail::ToArrayImpl(std::move(a), std::make_integer_sequence<size_t, N>{});
}
}  // namespace Dsn