#pragma once

/**
 * TEMPLATE_MAP: Compile-time value mapping using template class specialization.
 *
 * The `TEMPLATE_MAP` macro generates an unimplemented template class with the
 * given name and implements specializations for the provided key-value pairs.
 * Lookups of any provided key is calculable at compile-time. Lookups of a key
 * that was not provided yields a compile error.
 *
 * Template class specializations are not permitted within a class scope, but
 * partial specializations are. To support inner classes, `TEMPLATE_MAP`
 * generates classes with a defaulted dummy template parameter `_`.
 *
 * Classes generated with `TEMPLATE_MAP` are not instantiable. Their purpose is
 * to yield values that are known at compile-time. To this end, they should only
 * be used as static function containers. Non-instantiability is accomplished
 * via a deleted constructor and private destructor.
 *
 * Arguments:
 *   Name:  The desired name for the generated map-class.
 *   Key:   The type of the key field. Values of this type are used when
 *          performing a lookup. This type must be constexpr.
 *   Value: The type of the value field. This type must be constexpr.
 *   KV...: A variadic list of key-value pairs. These pairs must be enclosed in
 *          parentheses. eg: `(key, value)`
 *
 * Example: Create a template class `Squares` that maps the given key-value
 *          pairs with key-type `int` and value-type `double`.
 * ```
 * // Create the map.
 * TEMPLATE_MAP(Squares, int, doubles, (1, 1.0), (2, 4.0), (3, 9.0), (4, 16.0))
 *
 * // Stream its contents to stdout.
 * std::cout << "Squares: " << Squares<1>::key() << "->" << Squares<1>::value()
 *                  << ", " << Squares<2>::key() << "->" << Squares<2>::value()
 *                  << ", " << Squares<3>::key() << "->" << Squares<3>::value()
 *                  << ", " << Squares<4>::key() << "->" << Squares<4>::value();
 * ```
 *
 * The `TEMPLATE_MAP` macro usage above would generate something similar to the
 * following:
 * ```
 * template <int key, typename _ = void>
 * class Squares;
 *
 * template <typename _>
 * class Squares<1, _> {
 * public:
 *   static constexpr int key() noexcept { return 1; }
 *   static constexpr double value() noexcept { return 1.0; }
 *   // ...
 * };
 *
 * template <typename _>
 * class Squares<2, _> {
 * public:
 *   static constexpr int key() noexcept { return 2; }
 *   static constexpr double value() noexcept { return 4.0; }
 *   // ...
 * };
 *
 * template <typename _>
 * class Squares<3, _> {
 * public:
 *   static constexpr int key() noexcept { return 3; }
 *   static constexpr double value() noexcept { return 9.0; }
 *   // ...
 * };
 *
 * template <typename _>
 * class Squares<4, _> {
 * public:
 *   static constexpr int key() noexcept { return 4; }
 *   static constexpr double value() noexcept { return 16.0; }
 *   // ...
 * };
 * ```
 */

#include <boost/preprocessor.hpp>

#define _TEMPLATE_MAP_SPEC_IMPL(Name, Key, Value, kv_pair)                     \
  template <typename _>                                                        \
  class Name<BOOST_PP_TUPLE_ELEM(2, 0, kv_pair), _> {                          \
  public:                                                                      \
    static constexpr Key key() noexcept {                                      \
      return BOOST_PP_TUPLE_ELEM(2, 0, kv_pair);                               \
    }                                                                          \
    static constexpr Value value() noexcept {                                  \
      return BOOST_PP_TUPLE_ELEM(2, 1, kv_pair);                               \
    }                                                                          \
                                                                               \
  private:                                                                     \
    Name() = delete;                                                           \
    ~Name() = default;                                                         \
  };

#define _TEMPLATE_MAP_SPEC(z, n, data)                                         \
  _TEMPLATE_MAP_SPEC_IMPL(                                                     \
      BOOST_PP_TUPLE_ELEM(3, 0, data),                                         \
      BOOST_PP_TUPLE_ELEM(3, 1, data),                                         \
      BOOST_PP_TUPLE_ELEM(3, 2, data),                                         \
      BOOST_PP_ARRAY_ELEM(n, BOOST_PP_TUPLE_ELEM(3, 3, data)))

#define _TEMPLATE_MAP_IMPL(Name, Key, Value, kv_pairs)                         \
  template <Key key, typename _ = void>                                        \
  class Name;                                                                  \
                                                                               \
  BOOST_PP_REPEAT(BOOST_PP_ARRAY_SIZE(kv_pairs),                               \
                  _TEMPLATE_MAP_SPEC,                                          \
                  (Name, Key, Value, kv_pairs))

#define TEMPLATE_MAP(Name, Key, Value, ...)                                    \
  _TEMPLATE_MAP_IMPL(Name, Key, Value, BOOST_PP_VARIADIC_TO_ARRAY(__VA_ARGS__))
