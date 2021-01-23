#ifndef BIDIRECTIONAL_LIST_H_
#define BIDIRECTIONAL_LIST_H_

#include <cassert>
#include <initializer_list>
#include <vector>

namespace containers {

template<class T>
class BiDirectionalList {
  friend class Tester;
 public:
  struct Node {
    friend class BiDirectionalList<T>;
   public:
    T value;

   private:
    friend class Tester;

    explicit Node(const T& value) : value(value) {}

    explicit Node(T&& value) : value(std::move(value)) {}

    Node(const T& value, Node* prev_ptr, Node* next_ptr)
        : value(value), prev_ptr_(prev_ptr), next_ptr_(next_ptr) {}

    Node(T&& value, Node* prev_ptr, Node* next_ptr)
        : value(std::move(value)), prev_ptr_(prev_ptr), next_ptr_(next_ptr) {}

    ~Node() = default;

    Node* prev_ptr_ = nullptr;
    Node* next_ptr_ = nullptr;
  };

  BiDirectionalList() = default;

  BiDirectionalList(const std::initializer_list<T>& list) {
    for (const T& val : list) {
      PushBack(val);
    }
  }

  BiDirectionalList(const BiDirectionalList<T>& list) {
    for (Node* ptr = list.begin_; ptr != nullptr; ptr = ptr->next_ptr_)
      PushBack(ptr->value);
  }

  BiDirectionalList(BiDirectionalList<T>&& list) noexcept
      : begin_(list.begin_), end_(list.end_), size_(list.size_) {
    list.ResetAfterMove();
  }

  ~BiDirectionalList() {
    while (size_ > 0) {
      PopBack();
    }
  }

  int Size() const {
    return size_;
  }

  bool IsEmpty() const {
    return size_ == 0;
  }

  Node* Front() {
    assert(size_ > 0);
    return begin_;
  }

  const Node* Front() const {
    assert(size_ > 0);
    return begin_;
  }

  Node* Back() {
    assert(size_ > 0);
    return end_;
  }

  const Node* Back() const {
    assert(size_ > 0);
    return end_;
  }

  std::vector<T> ToVector() const {
    std::vector<T> vec;
    vec.reserve(size_);
    for (Node* ptr = begin_; ptr != nullptr; ptr = ptr->next_ptr_) {
      vec.push_back(ptr->value);
    }
    return vec;
  }

  void InsertBefore(Node* element, T&& value) {
    assert(element != nullptr);
    ++size_;
    Node* new_node = new Node(std::move(value), element->prev_ptr_, element);
    Rebind(new_node);
  }

  void InsertBefore(Node* element, const T& value) {
    assert(element != nullptr);
    ++size_;
    Node* new_node = new Node(value, element->prev_ptr_, element);
    Rebind(new_node);
  }

  void InsertAfter(Node* element, T&& value) {
    assert(element != nullptr);
    ++size_;
    Node* new_node = new Node(std::move(value), element, element->next_ptr_);
    Rebind(new_node);
  }

  void InsertAfter(Node* element, const T& value) {
    assert(element != nullptr);
    ++size_;
    Node* new_node = new Node(std::move(value), element, element->next_ptr_);
    Rebind(new_node);
  }

  void Erase(Node* element) {
    assert(element != nullptr);
    --size_;
    if (element->prev_ptr_ != nullptr) {
      element->prev_ptr_->next_ptr_ = element->next_ptr_;
    }
    if (element->next_ptr_ != nullptr) {
      element->next_ptr_->prev_ptr_ = element->prev_ptr_;
    }
    if (element == begin_) {
      begin_ = element->next_ptr_;
    }
    if (element == end_) {
      end_ = element->prev_ptr_;
    }
    delete element;
  }

  void PushFront(T&& value) {
    ++size_;
    Node* new_node = new Node(std::move(value), nullptr, begin_);
    Rebind(new_node);
  }

  void PushFront(const T& value) {
    ++size_;
    Node* new_node = new Node(value, nullptr, begin_);
    Rebind(new_node);
  }

  void PushBack(T&& value) {
    ++size_;
    Node* new_node = new Node(std::move(value), end_, nullptr);
    Rebind(new_node);
  }

  void PushBack(const T& value) {
    ++size_;
    Node* new_node = new Node(value, end_, nullptr);
    Rebind(new_node);
  }

  void PopFront() {
    assert(size_ > 0);
    Erase(begin_);
  }

  void PopBack() {
    assert(size_ > 0);
    Erase(end_);
  }

  void Clear() {
    while (size_ > 0) {
      PopBack();
    }
  }

  int Find(const T& des_value) const {
    const Node* ptr = begin_;
    for (int ind = 0; ptr != nullptr; ++ind, ptr = ptr->next_ptr_) {
      if (ptr->value == des_value) {
        return ind;
      }
    }
    return -1;
  }

  std::vector<int> FindAll(const T& des_value) const {
    std::vector<int> occurrences;
    const Node* ptr = begin_;
    for (int ind = 0; ptr != nullptr; ++ind, ptr = ptr->next_ptr_) {
      if (ptr->value == des_value) {
        occurrences.push_back(ind);
      }
    }
    return occurrences;
  }

  BiDirectionalList<T>& operator=(const BiDirectionalList<T>& list) {
    if (this != &list) {
      Clear();
      for (const Node* ptr = list.begin_; ptr != nullptr;
           ptr = ptr->next_ptr_) {
        PushBack(ptr->value);
      }
    }
    return *this;
  }

  BiDirectionalList<T>& operator=(BiDirectionalList<T>&& list) noexcept {
    if (this != &list) {
      Clear();
      size_ = list.size_;
      begin_ = list.begin_;
      end_ = list.end_;
      list.ResetAfterMove();
    }
    return *this;
  }

  Node* operator[](int ind) {
    assert(ind < size_ && ind >= 0);
    Node* ptr = begin_;
    while (ind > 0) {
      ptr = ptr->next_ptr_;
      --ind;
    }
    return ptr;
  }

  const Node* operator[](int ind) const {
    assert(ind < size_ && ind >= 0);
    const Node* ptr = begin_;
    while (ind > 0) {
      ptr = ptr->next_ptr_;
      --ind;
    }
    return ptr;
  }

  bool operator==(const BiDirectionalList<T>& rhs) const {
    if (size_ != rhs.size_) {
      return false;
    }
    const Node* rhs_ptr = rhs.begin_;
    for (const Node* lhs_ptr = begin_; lhs_ptr != nullptr;
         lhs_ptr = lhs_ptr->next_ptr_, rhs_ptr = rhs_ptr->next_ptr_) {
      if (lhs_ptr->value != rhs_ptr->value) {
        return false;
      }
    }
    return true;
  }

  bool operator!=(const BiDirectionalList<T>& rhs) const {
    return !(*this == rhs);
  }

 private:
  void Rebind(Node* node) {
    if (node->prev_ptr_ != nullptr) {
      node->prev_ptr_->next_ptr_ = node;
    } else {
      begin_ = node;
    }
    if (node->next_ptr_ != nullptr) {
      node->next_ptr_->prev_ptr_ = node;
    } else {
      end_ = node;
    }
  }

  void ResetAfterMove() {
    size_ = 0;
    begin_ = nullptr;
    end_ = nullptr;
  }

  Node* begin_ = nullptr;
  Node* end_ = nullptr;
  int size_ = 0;
};

}  // namespace containers

#endif  // BIDIRECTIONAL_LIST_H_
