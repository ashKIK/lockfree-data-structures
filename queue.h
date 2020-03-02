#ifndef LOCKFREE_QUEUE
#define LOCKFREE_QUEUE

namespace interface {

class Queue {
public:
  virtual ~Queue() = default;

  virtual bool Enqueue(int data) = 0;

  virtual bool Dequeue(int &data) = 0;
};

}

#endif
