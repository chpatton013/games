#pragma once

#include "task/task.h"
#include <algorithm>
#include <memory>
#include <mutex>
#include <vector>
#include "boost/optional.hpp"

namespace task {

namespace {
struct TaskIdLess {
  bool operator<(const Task& lhs, const Task& rhs) {
    return lhs.id < rhs.id;
  }

  bool operator<(const Task::SharedPtr& lhs, const Task::SharedPtr& rhs) {
    return this->operator<(*lhs, *rhs);
  }

  bool operator<(const Wrapper::SharedPtr& lhs,
                 const Wrapper::SharedPtr& rhs) {
    return this->operator<(lhs->task, rhs->task);
  }
};

struct TaskPriorityLess {
  bool operator<(const Task& lhs, const Task& rhs) {
    return lhs.priority < rhs.priority;
  }

  bool operator<(const Task::SharedPtr& lhs, const Task::SharedPtr& rhs) {
    return this->operator<(*lhs, *rhs);
  }

  bool operator<(const Wrapper::SharedPtr& lhs,
                 const Wrapper::SharedPtr& rhs) {
    return this->operator<(lhs->task, rhs->task);
  }
};
} // namespace

class ScheduleLock;

class Schedule {
public:
  class Lock {
  public:
    explicit Lock(Schedule& schedule)
      : schedule(schedule), mutex(schedule.mutex_) {
      this->mutex.lock();
    }

    ~Lock() {
      this->mutex.unlock();
    }

    void add(const Task::SharedPtr& task) {
      schedule.add(task);
    }

  private:
    Schedule& schedule;
    mutable std::mutex& mutex;
  };

  class Wrapper {
  public:
    using SharedPtr = std::shared_ptr<Wrapper>;
    using SharedPtrOption = boost::optional<SharedPtr>;
    using WeakPtr = std::weak_ptr<Wrapper>;
    using WeakPtrOption = boost::optional<WeakPtr>;
    using List = std::vector<SharedPtr>;

    template <typename ... Args>
    static SharedPtr Create(Schedule& schedule, Args&&... args) {
      auto wrapper = Wrapper::SharedPtr(new Wrapper(schedule, args...));
      schedule.parents_.emplace(wrapper->task->id, wrapper);
      schedule.dependencies_.emplace(wrapper->task->id,
                                     Wrapper::WeakPtr(wrapper));
      return wrapper;
    }

    ~Wrapper() {
      auto parent_itr = schedule.parents_.find(this->task->id);
      if (parent_itr != schedule.parents_.end()) {
        schedule.parents_.erase(parent_itr);
      }

      auto dependency_itr = schedule.dependencies_.find(this->task->id);
      if (dependency_itr != schedule.dependencies_.end()) {
        schedule.dependencies_.erase(dependency_itr);
      }
    }

    Wrapper() = delete;
    Wrapper(const Wrapper&) = delete;
    Wrapper(Wrapper&&) = delete;
    Wrapper& operator=(const Wrapper&) = delete;
    Wrapper& operator=(Wrapper&&) = delete;

    bool ready() const {
      return !this->dependency || !this->dependency.get().lock();
    }

    Schedule& schedule;
    const Task::SharedPtr task;
    const SharedPtrOption parent;
    const WeakPtrOption dependency;

  private:
    Wrapper(Schedule& schedule,
            const Task::SharedPtr& task,
            const SharedPtrOption& parent = boost::none,
            const WeakPtrOption& dependency = boost::none)
    : schedule(schedule), task(task), parent(parent), dependency(dependency) {
    }

    // Convenience constructor to save callers from manually creating weak
    // pointers.
    Wrapper(Schedule& schedule,
            const Task::SharedPtr& task,
            const SharedPtrOption& parent,
            const SharedPtr& dependency)
    : Wrapper(schedule, task, parent, WeakPtr(dependency)) {}
  };

  ~Schedule() = default;
  Schedule() = default;
  Schedule(const Schedule&) = delete;
  Schedule(Schedule&&) = default;
  Schedule& operator=(const Schedule&) = delete;
  Schedule& operator=(Schedule&&) = default;

  bool empty() const {
    return this->ready_.empty() && this->not_ready_.empty();
  }

  Wrapper::SharedPtrOption get() {
    std::lock_guard<std::mutex> lock(this->mutex_);
  }

  Lock lock() { return Lock(*this); }

private:
  void add(const Task::SharedPtr& task) {
    auto parent = this->findParent(task);
    auto dependency = this->findDependency(task);
    auto wrapper = Wrapper::Create(*this, task, parent, dependency);

    if (wrapper->ready()) {
      std::push_heap(this->ready_.begin(), this->ready_.end(), TaskIdLess());
    } else {
      std::push_heap(this->not_ready_.begin(),
                     this->not_ready_.end(),
                     TaskIdLess());
    }
  }

  void remove(const Task::Id& id) {
  }

  Wrapper::SharedPtrOption findParent(const Task::SharedPtr& task) {
    auto itr = this->parents_.find(task->id);
    if (itr == this->parents_.end()) {
      return boost::none;
    } else {
      return *itr;
    }
  }

  Wrapper::SharedPtrOption findDependency(const Task::SharedPtr& task) {
    auto itr = this->dependencies_.find(task->id);
    if (itr == this->dependencies_.end()) {
      return boost::none;
    } else {
      return *itr;
    }
  }

  Wrapper::List ready_;
  std::map<Task::Id, Wrapper::SharedPtr> instances_;
  std::map<Task::Id, Wrapper::SharedPtr> by_parent_;
  std::map<Task::Id, Wrapper::SharedPtr> by_dependency_;
  mutable std::mutex mutex_;
};

} // namespace task
