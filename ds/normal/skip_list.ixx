module;
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <random>
#include <tuple>
#include <type_traits>
#include <utility>
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
 public:
  template <bool ConstRef>
  struct Iterator;

  using value_type = std::pair<const Key, Value>;
  using size_type = size_t;
  using difference_t = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;

 private:
  struct Node {
    std::pair<const Key, Value> data;
    std::array<Node*, kMaxHeight> height_next;

    const Key& key() const { return data.first; }
    Value& value() { return data.second; }

    Node* Next(size_t height) { return height_next[height]; }
    void SetNext(size_t height, Node* node) { height_next[height] = node; }
  };

  Node* head_;
  size_t max_height_;
  [[no_unique_address]] Compare key_comp_;

 public:
  explicit SkipList(Compare key_comp = {})
      : head_(MakeNewNode({})), max_height_(1), key_comp_(std::move(key_comp)) {}

  SkipList(const SkipList&) = delete;
  SkipList& operator=(const SkipList&) = delete;

  ~SkipList() {
    while (head_ != nullptr) {
      auto* cur = head_;
      head_ = head_->height_next[0];
      delete cur;
    }
  }

  template <class K, class... Args>
  void TryEmplace(K&& key, Args&&... args) {
    auto [node, node_height_prev] = FoundWithPrev([&key, this](const Key& node_key) {
      // node_key < key
      return key_comp_(node_key, key);
    });

    if (node != nullptr && Equal(node->key(), key)) {
      return;
    }

    const auto height = RandomHeight();
    if (height > GetMaxHeight()) {
      max_height_ = height;
    }

    node = MakeNewNode(std::pair{std::forward<K>(key), Value{std::forward<Args>(args)...}});

    for (int i = 0; i < height; i++) {
      node->SetNext(i, node_height_prev[i]->Next(i));
      node_height_prev[i]->SetNext(i, node);
    }
  }

  bool Contains(auto&& key) const {
    const auto it = LowerBound(key);
    return it != cend() && !key_comp_(key, it->first);
  }

  std::optional<std::reference_wrapper<value_type>> TryGet(auto&& key) {
    auto it = LowerBound(key);
    if (it != end() && !key_comp_(key, it->first)) {
      return *it;
    }
    return {};
  }

  std::optional<std::reference_wrapper<const value_type>> TryGet(auto&& key) const {
    auto it = LowerBound(key);
    if (it != cend() && !key_comp_(key, it->first)) {
      return *it;
    }
    return {};
  }
  const_iterator UpperBound(auto&& key) const {
    return {this, std::get<0>(FoundWithPrev([&key, this](const Key& node_key) {
              // node_key <= key
              return !key_comp_(key, node_key);
            }))};
  }
  const_iterator LowerBound(auto&& key) const {
    return {this, std::get<0>(FoundWithPrev([&key, this](const Key& node_key) {
              // node_key < key
              return key_comp_(node_key, key);
            }))};
  }

  iterator UpperBound(auto&& key) {
    return {this, std::get<0>(FoundWithPrev([&key, this](const Key& node_key) {
              // node_key <= key
              return !key_comp_(key, node_key);
            }))};
  }
  iterator LowerBound(auto&& key) {
    return {this, std::get<0>(FoundWithPrev([&key, this](const Key& node_key) {
              // node_key < key
              return key_comp_(node_key, key);
            }))};
  }

  [[nodiscard]] size_t GetMaxHeight() const { return max_height_; }

  [[nodiscard]] constexpr iterator begin() noexcept { return iterator{this, head_->Next(0)}; }
  [[nodiscard]] constexpr iterator end() noexcept { return iterator{this, nullptr}; }
  [[nodiscard]] constexpr const_iterator begin() const noexcept {
    return const_iterator{this, head_->Next(0)};
  }
  [[nodiscard]] constexpr const_iterator end() const noexcept {
    return const_iterator{this, nullptr};
  }

  [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
    return const_iterator{this, head_->Next(0)};
  }
  [[nodiscard]] constexpr const_iterator cend() const noexcept {
    return const_iterator{this, nullptr};
  }

  template <bool ConstRef>
  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::
        conditional_t<ConstRef, const typename SkipList::value_type, typename SkipList::value_type>;
    using difference_type = SkipList::difference_t;
    using reference = std::
        conditional_t<ConstRef, typename SkipList::const_reference, typename SkipList::reference>;
    using pointer =
        std::conditional_t<ConstRef, typename SkipList::const_pointer, typename SkipList::pointer>;

    Iterator(const SkipList* list, Node* node) noexcept : list_(list), node_(node) {}
    Iterator() noexcept : list_(nullptr), node_(nullptr) {}

    Iterator(const Iterator&) noexcept = default;
    Iterator& operator=(const Iterator&) noexcept = default;
    Iterator(Iterator&&) noexcept = default;
    Iterator& operator=(Iterator&&) noexcept = default;

    reference operator*() const { return node_->data; }
    pointer operator->() const { return std::addressof(node_->data); }

    Iterator& operator++() {
      if (node_) {
        node_ = node_->Next(0);
      }
      return *this;
    }

    Iterator operator++(int) {
      Iterator resp(*this);
      ++(*this);
      return resp;
    }

    template <bool RightConst>
    bool operator==(const Iterator<RightConst>& r) const {
      return list_ == r.list_ && node_ == r.node_;
    }

   private:
    const SkipList* list_;
    Node* node_;

    friend Iterator<!ConstRef>;
  };

 private:
  static Node* MakeNewNode(value_type data) {
    Node* resp = new Node{std::move(data)};
    resp->height_next.fill(nullptr);
    return resp;
  }

  // find  result >= key
  std::tuple<Node*, std::array<Node*, kMaxHeight>> FoundWithPrev(auto&& before_key) const {
    std::array<Node*, kMaxHeight> node_height_prev;
    node_height_prev.fill(head_);

    Node* node{head_};
    for (size_t height = GetMaxHeight() - 1;;) {
      auto* next = node->Next(height);

      if (next != nullptr && before_key(next->key())) {  // next < key
        node = next;
      } else {  // next >= key or next == nullptr
        node_height_prev[height] = node;
        if (height == 0) {
          return {next, node_height_prev};
        }
        height--;
      }
    }
  }

  bool Equal(const Key& my_key, auto&& another_key) const {
    return !key_comp_(my_key, another_key) && !key_comp_(another_key, my_key);
  }
};

}  // namespace dsn