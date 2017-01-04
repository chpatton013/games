#pragma once

#include "util/delegate.h"
#include "util/identifier.h"
#include <memory>
#include <vector>
#include "boost/optional.hpp"

namespace task {

namespace {
static util::Identifier gIdentifier;
} // namespace

using Priority = int;
static constexpr const Priority kPriorityHigh = 10;
static constexpr const Priority kPriorityNormal = 0;
static constexpr const Priority kPriorityLow = -10;

class Task {
public:
  using Id = util::Id;
  using SharedPtr = std::shared_ptr<const Task>;
  using SharedPtrOption = boost::optional<SharedPtr>;
  using WeakPtr = std::weak_ptr<const Task>;
  using WeakPtrOption = boost::optional<WeakPtr>;
  using List = std::vector<SharedPtr>;

  template <typename ... Args>
  static SharedPtr Create(Args&&... args) {
    return Task::SharedPtr(new Task(args...));
  }

  ~Task() = default;
  Task() = delete;
  Task(const Task&) = delete;
  Task(Task&&) = delete;
  Task& operator=(const Task&) = delete;
  Task& operator=(Task&&) = delete;

  int parentDepth() const {
    if (this->parent) {
      return 1 + this->parent.get()->parentDepth();
    }
    return 0;
  }

  int dependencyDepth() const {
    if (this->dependency) {
      return 1 + this->dependency.get().lock()->dependencyDepth();
    }
    return 0;
  }

  bool operator==(const Task& that) { return this->id == that.id; }

  const Id id;
  const util::Delegate delegate;
  const Priority priority;
  const SharedPtrOption parent;
  const WeakPtrOption dependency;

private:
  Task(const util::Delegate& delegate,
       const Priority& priority = kPriorityNormal,
       const SharedPtrOption& parent = boost::none,
       const WeakPtrOption& dependency = boost::none)
  : id(gIdentifier.reserve()),
    delegate(delegate),
    priority(priority),
    parent(parent),
    dependency(dependency) {}

  // Convenience constructor to save callers from manually creating weak
  // pointers.
  Task(const util::Delegate& delegate,
       const Priority& priority,
       const SharedPtrOption& parent,
       const SharedPtr& dependency)
  : Task(delegate, priority, parent, WeakPtr(dependency)) {}
};

} // namespace task
