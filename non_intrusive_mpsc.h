#include <atomic>

#include "queue.h"

namespace non_intrusive_mpsc {

class Queue : public interface::Queue {
public:
  Queue() : stub_(new Node(0)), head_(stub_), tail_(stub_) {}

  ~Queue() override {
    auto head = head_.load(std::memory_order_relaxed);
    while (head) {
      auto node = head;
      head = head->next.load(std::memory_order_relaxed);
      delete node;
    }
  }

  bool Enqueue(int data) override {
    auto node = new Node(data);
    auto prev = tail_.exchange(node, std::memory_order_acq_rel);
    prev->next.store(node, std::memory_order_release);
    return true;
  }

  bool Dequeue(int &data) override {
    auto head = head_.load(std::memory_order_relaxed);
    auto next = head->next.load(std::memory_order_acquire);
    if (next) {
      head_.store(next, std::memory_order_relaxed);
      data = next->data;
      delete head;
      return true;
    }
    return false;
  }

private:
  struct Node {
    int data;
    std::atomic<Node *> next;
    explicit Node(int v) : data(v), next(nullptr) {}
  };

  Node *stub_;
  std::atomic<Node *> head_;
  std::atomic<Node *> tail_;
};

} // namespace non_intrusive_mpsc
