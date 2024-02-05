module;
#include <algorithm>
#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>
export module dsn:array;

namespace dsn {
export template <class T, size_t N>
  requires std::is_object_v<T> && (N > 0)
class Array {
 public:
  using value_type = T;
  using size_type = size_t;
  using difference_t = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = value_type*;
  using const_iterator = const value_type*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<reverse_iterator>;

  value_type data_[N];

  [[nodiscard]] constexpr reference operator[](size_t index) noexcept { return data_[index]; }
  [[nodiscard]] constexpr reference front() noexcept { return *begin(); }
  [[nodiscard]] constexpr reference back() noexcept { return *rbegin(); }

  [[nodiscard]] constexpr const_reference operator[](size_t index) const noexcept {
    return data_[index];
  }
  [[nodiscard]] constexpr const_reference front() const noexcept { return *begin(); }
  [[nodiscard]] constexpr const_reference back() const noexcept { return *rbegin(); }

  [[nodiscard]] constexpr pointer data() noexcept { return begin(); }
  [[nodiscard]] constexpr const_pointer data() const noexcept { return begin(); }

  [[nodiscard]] constexpr iterator begin() noexcept { return data_; }
  [[nodiscard]] constexpr iterator end() noexcept { return data_ + N; }
  [[nodiscard]] constexpr const_iterator begin() const noexcept { return data_; }
  [[nodiscard]] constexpr const_iterator end() const noexcept { return data_ + N; }

  [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return data_; }
  [[nodiscard]] constexpr const_iterator cend() const noexcept { return data_ + N; }

  // *reverse_iterator -> return *--iterator
  [[nodiscard]] constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  [[nodiscard]] constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

  [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  [[nodiscard]] constexpr size_type size() const noexcept { return N; }
  [[nodiscard]] constexpr size_type max_size() const noexcept { return N; }
  [[nodiscard]] constexpr bool empty() const noexcept { return false; }

  constexpr void swap(Array<T, N>& other) noexcept(std::is_nothrow_swappable_v<T>) {
    std::ranges::swap_ranges(begin(), end(), other.begin(), other.end());
  }
};

template <class T, std::size_t N>
constexpr bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs) {
  return std::ranges::equal(lhs, rhs);
}
template <class T, std::size_t N>
constexpr auto operator<=>(const std::array<T, N>& lhs, const std::array<T, N>& rhs) {
  return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
// ducing guide
export template <class T, class... U>
// requires std::conjunction_v<std::is_same<T, U>...>
  requires(std::same_as<T, U> && ...)
Array(T, U...) -> Array<T, 1 + sizeof...(U)>;

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
}  // namespace dsn

namespace std {
template <class T, std::size_t N>
constexpr void swap(dsn::Array<T, N>& lhs, dsn::Array<T, N>& rhs) noexcept(
    noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}
}  // namespace std