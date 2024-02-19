#include <cstddef>
#include <utility>
module;
#include <concepts>
#include <functional>
#include <list>
#include <random>

export module dsn:skip_list;

namespace dsn {

namespace {
inline thread_local std::mt19937 generator{std::random_device{}()};
inline thread_local std::uniform_int_distribution<size_t> rand{};
static const constexpr size_t kMaxHeight = 12;

bool RandomOneIn(size_t n) { return rand(generator) % n > 0; }
size_t RandomHeight() {
  static const constexpr size_t kBranching = 4;

  int resp = 1;
  for (; resp < kMaxHeight && RandomOneIn(kBranching); ++resp) {
  }
  return resp;  // [1,kMaxHeight]
}
}  // namespace

export template <class Key, class Value, class Compare = std::less<>>
class SkipList {
 private:
  struct Node {
    const Key key;
    Value vaule;
    std::array<Node*, kMaxHeight> level_next;
  };

  Node* head_;

 public:
  SkipList() : head_(nullptr) {}

  SkipList(const SkipList&) = default;
  SkipList& operator=(const SkipList&) = default;

  template <class... Args>
  void Insert(Args&&... args);

  bool Contains(auto&& key);
  void Find(auto&& key);
  void UpperBound(auto&& key);
  void LowerBound(auto&& key);

 private:
  static Node* MakeNewNode(Key key, Value value) {
    return new Node{std::move(key), std::move(value)};
  }
};

}  // namespace dsn