#pragma once

namespace util {

/**
 * Extend this trait to inherit simple relational operators based on your
 * object's const operator== and operator< overloads. Note that your object must
 * have these overloads implemented to use this trait.
 */
template <typename T>
class RelOps {
public:
  bool operator==(const T& that) { return this->cast() == that; }
  bool operator<(const T& that) { return this->cast() < that; }
  bool operator!=(const T& that) { return !(this->cast() == that); }
  bool operator>(const T& that) { return that < this->cast(); }
  bool operator<=(const T& that) { return !(that < this->cast()); }
  bool operator>=(const T& that) { return !(this->cast() < that); }

  bool operator!=(const T& that) const { return !(this->cast() == that); }
  bool operator>(const T& that) const { return that < this->cast(); }
  bool operator<=(const T& that) const { return !(that < this->cast()); }
  bool operator>=(const T& that) const { return !(this->cast() < that); }

private:
  const T& cast() const { return *static_cast<const T* const>(this); }
};

} // namespace util
