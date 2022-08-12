#ifndef CIRCULAR_LINKED_LIST_HPP
#define CIRCULAR_LINKED_LIST_HPP

#include <iostream>

#include "node.hpp"

enum class Order { kASC, kDESC };

template <typename T>
class CircularLinkedList {
public:
  CircularLinkedList() = default;
  CircularLinkedList(const CircularLinkedList<T>& source);
  CircularLinkedList<T>& operator=(const CircularLinkedList<T>& source);
  ~CircularLinkedList();

  void InsertInOrder(const T& data);
  void HelperASC(const T& data);
  void HelperDESC(const T& data);
  void Reverse();

  void PushBack(T data);
  void Print();
  void DestructorCopy();

  template <typename U>
  friend std::ostream& operator<<(std::ostream& os,
                                  const CircularLinkedList<U>& cll);

private:
  Node<T>* head_ = nullptr;
  Node<T>* tail_ = nullptr;
  Order node_order_ = Order::kASC;
};

template <typename T>
CircularLinkedList<T>::CircularLinkedList(const CircularLinkedList<T>& source) {
  node_order_ = source.node_order_;
  if (source.head_ == nullptr) {
    head_ = nullptr;
    tail_ = nullptr;
  } else if (source.head_ == source.tail_) {
    Node<T>* temp = new Node<T>(source.head_->data);
    head_ = temp;
    tail_ = temp;
    tail_->next = head_;
  } else {
    head_ = new Node<T>(source.head_->data);
    head_->next = new Node<T>(0);
    Node<T>* current = head_->next;
    Node<T>* node = source.head_->next;
    while (node != source.tail_) {
      current->data = node->data;
      Node<T>* temp = new Node<T>(0);
      current->next = temp;
      current = current->next;
      node = node->next;
    }
    current->data = source.tail_->data;
    tail_ = current;
    tail_->next = head_;
  }
}

template <typename T>
void CircularLinkedList<T>::DestructorCopy() {
  if (head_ == nullptr) {
    return;
  }
  if (head_ != tail_) {
    Node<T>* current = head_->next;
    while (current != head_) {
      Node<T>* temp = current->next;
      delete current;
      current = temp;
    }
  }
  delete head_;
  head_ = nullptr;
  tail_ = nullptr;
}

template <typename T>
CircularLinkedList<T>& CircularLinkedList<T>::operator=(
    const CircularLinkedList<T>& source) {
  if (this == &source) {
    return *this;
  }
  DestructorCopy();
  node_order_ = source.node_order_;
  if (source.head_ == nullptr) {
    node_order_ = Order::kASC;
    head_ = nullptr;
    tail_ = nullptr;
  } else if (source.head_ == source.tail_) {
    Node<T>* temp = new Node<T>(source.head_->data);
    head_ = temp;
    tail_ = temp;
    tail_->next = head_;
  } else {
    head_ = new Node<T>(source.head_->data);
    head_->next = new Node<T>(0);
    Node<T>* current = head_->next;
    Node<T>* node = source.head_->next;
    while (node != source.tail_) {
      current->data = node->data;
      Node<T>* temp = new Node<T>(0);
      current->next = temp;
      current = current->next;
      node = node->next;
    }
    current->data = source.tail_->data;
    tail_ = current;
    tail_->next = head_;
  } 
  return *this; 
}

template <typename T>
CircularLinkedList<T>::~CircularLinkedList() {
  if (head_ == nullptr) {
    return;
  }
  if (head_ != tail_) {
    Node<T>* current = head_->next;
    while (current != head_) {
      Node<T>* temp = current->next;
      delete current;
      current = temp;
    }
  }
  delete head_;
  head_ = nullptr;
  tail_ = nullptr;
}

template <typename T>
void CircularLinkedList<T>::HelperASC(const T& data) {
  Node<T>* node = new Node<T>(data);
  Node<T>* current = head_;
  if (data <= head_->data) {
    tail_->next = node;
    node->next = head_;
    head_ = node;
    return;
  }
  if (data >= tail_->data) {
    tail_->next = node;
    node->next = head_;
    tail_ = node;
    return;
  }
  while (current->next != head_) {
    if (current->data <= data && current->next->data >= data) {
      Node<T>* temp = current->next;
      current->next = node;
      node->next = temp;
      return;
    }
    current = current->next;
  }
}
template <typename T>
void CircularLinkedList<T>::HelperDESC(const T& data) {
  Node<T>* node = new Node<T>(data);
  Node<T>* current = head_;
  if (data >= head_->data) {
    tail_->next = node;
    node->next = head_;
    head_ = node;
    return;
  }
  if (data <= tail_->data) {
    tail_->next = node;
    node->next = head_;
    tail_ = node;
    return;
  }
  while (current->next != head_) {
    if (current->data >= data && current->next->data <= data) {
      Node<T>* temp = current->next;
      current->next = node;
      node->next = temp;
      return;
    }
    current = current->next;
  }
}
template <typename T>
void CircularLinkedList<T>::InsertInOrder(const T& data) {
  if (head_ == nullptr) {
    head_ = new Node<T>(data);
    head_->next = head_;
    tail_ = head_;
  } else {
    if (node_order_ == Order::kASC) {
      HelperASC(data);
    }
    if (node_order_ == Order::kDESC) {
      HelperDESC(data);
    }
  }
}

template <typename T>
void CircularLinkedList<T>::Reverse() {
  if (node_order_ == Order::kASC) {
    node_order_ = Order::kDESC;
  } else {
    node_order_ = Order::kASC;
  }
  if (head_ == nullptr || tail_ == nullptr) {
    return;
  }
  Node<T>* current = head_;
  Node<T>* previous = nullptr;
  do {
    Node<T>* temp = current->next;
    current->next = previous;
    previous = current;
    current = temp;
  } while (current != head_);
  tail_ = head_;
  head_->next = previous;
  head_ = previous;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const CircularLinkedList<T>& cll) {
  Node<T>* iter = cll.head_;
  // empty list condition
  if (iter == nullptr) {
    os << "Empty list";
    return os;
  }
  // non-empty list condition
  do {
    os << iter->data << '\t';
    iter = iter->next;
  } while (iter != cll.head_);

  return os;
}

template <typename T>
void CircularLinkedList<T>::PushBack(T data) {
  Node<T>* node = new Node<T>(data);
  if (head_ == nullptr) {
    head_ = node;
    node->next = head_;
    tail_ = node;
    return;
  }
  tail_->next = node;
  node->next = head_;
  tail_ = node;
}

template <typename T>
void CircularLinkedList<T>::Print() {
  if (node_order_ == Order::kASC) {
    std::cout << "ascending" << "\n";
  } else {
    std::cout << "descending" << "\n";
  }

  Node<T>* current = head_;
  if (current != nullptr) {
    while (true) {
      if (current->next == head_) {
        tail_ = current;
      }
      std::cout << current->data << " "
                << "address: " << current << "  curr.next: " << current->next
                << "\n";
      current = current->next;
      if (current == head_) {
        break;
      }
    }
    std::cout << "\n"
              << "head is: " << head_->data << "  address: " << head_ << "\n";
    std::cout << "tail is: " << tail_->data << "  address: " << tail_ << "\n";
    std::cout << "tail points to: " << tail_->next->data
              << "  address: " << tail_->next;
    std::cout << std::endl;
  } else {
    std::cout << "list is empty";
  }
}

#endif