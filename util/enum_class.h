#pragma once

/**
 * ENUM_CLASS: Enumeration type that contains a symbol-to-string mapping for
 * enum values.
 *
 * The `ENUM_CLASS` macro generates a class with the given name which holds an
 * enum with the given symbols and a static mapping of enum values to names.
 * The generated class can be instantated as to provide easy association of
 * values and names.
 *
 * Classes generated with `ENUM_CLASS` contain several static utility functions
 * to facilitate iteration over enum values, names, or instances.
 *
 * Arguments:
 *   ClassName: The desired name for the generated enum class.
 *   Values...: The variadic list of enum symbol names.
 *
 * Example: Create an enum class `Direction` to hold values `Left` and `Right`.
 * ```
 * // Create the class.
 * ENUM_CLASS(Direction, Left, Right)
 *
 * // Stream the names and values of the symbols.
 * std::cout << Direction::Name<Left> << ": " << Direction::Enum::Left << ", "
 *           << Direction::Name<Right> << ": " << Direction::Enum::Right;
 *
 * // Create instances.
 * const auto left = Direction::Create<Direction::Enum::Left>();
 * const auto right = Direction::Create<Direction::Enum::Right>();
 *
 * // Stream the name and value of the instances.
 * std::cout << left.name << ": " << left.value << ", "
 *           << right.name << ": " << right.value;
 *
 * // Iterate over the enum values.
 * for (const auto& direction : Direction::Values()) { ... }
 *
 * // Iterate over the enum names.
 * for (const auto& direction : Direction::Names()) { ... }
 *
 * // Iterate over the enum instances.
 * for (const auto& direction : Direction::Instances()) { ... }
 * ```
 *
 * The `ENUM_CLASS` macro usage above would generate something similar to the
 * following:
 * ```
 * class Direction {
 * public:
 *   enum class Enum { Left, Right };
 *
 *   TEMPLATE_MAP(Name,
 *                Enum,
 *                util::StaticString,
 *                (Left, "Left"),
 *                (Right, "Right"))
 *
 *   template <Enum value>
 *   static constexpr Direction Create() noexcept {
 *     return Direction(value, Name<value>::value());
 *   }
 *
 *   static constexpr std::array<Enum, 2> Values() noexcept {
 *     return {{Enum::Left, Enum::Right}};
 *   }
 *
 *   static constexpr std::array<util::StaticString, 2> Names() noexcept {
 *     return {{"Left", "Right"}};
 *   }
 *
 *   static constexpr std::array<Direction, 2> Instances() noexcept {
 *     return {{Create<Enum::Left>(), Create<Enum::Right>()}};
 *   }
 *
 *   const Enum value;
 *   const util::StaticString name;
 *
 * private:
 *   constexpr Direction(Enum value, util::StaticString name) noexcept
 *     : value(std::move(value)), name(std::move(name)) {}
 * };
 * ```
 */

#include "trait/rel_ops.h"
#include "util/static_string.h"
#include "util/template_map.h"
#include <array>
#include <boost/preprocessor.hpp>
#include <ostream>

// Apply the given macro to the Nth member in the given members array.
// Add a comma after all but the last member.
#define _ENUM_CLASS_REPEAT_IMPL(n, members, macro)                             \
  macro(BOOST_PP_ARRAY_ELEM(n, members)) BOOST_PP_COMMA_IF(                    \
      BOOST_PP_SUB(BOOST_PP_DEC(BOOST_PP_ARRAY_SIZE(members)), n))

// Unpack the data tuple into the provided members and macro. Delegate to the
// impl macro.
#define _ENUM_CLASS_REPEAT_UNPACK(z, n, data)                                  \
  _ENUM_CLASS_REPEAT_IMPL(                                                     \
      n, BOOST_PP_TUPLE_ELEM(0, data), BOOST_PP_TUPLE_ELEM(1, data))

// Repeat the provided macro for each member in the provided array.
#define _ENUM_CLASS_REPEAT(members, macro)                                     \
  BOOST_PP_REPEAT(BOOST_PP_ARRAY_SIZE(members),                                \
                  _ENUM_CLASS_REPEAT_UNPACK,                                   \
                  (members, macro))

// Transform the provided member.
#define _ENUM_CLASS_KV_SYMBOL(member) (Enum::member, BOOST_PP_STRINGIZE(member))
#define _ENUM_CLASS_VALUES_SYMBOL(member) Enum::member
#define _ENUM_CLASS_NAMES_SYMBOL(member) Name<Enum::member>::value()
#define _ENUM_CLASS_INSTANCES_SYMBOL(member) Create<Enum::member>()

// Enumerate provided members after tranforming with a specific macro.
#define _ENUM_CLASS_KV_ENUM(members)                                           \
  _ENUM_CLASS_REPEAT(members, _ENUM_CLASS_KV_SYMBOL)
#define _ENUM_CLASS_VALUES_ENUM(members)                                       \
  _ENUM_CLASS_REPEAT(members, _ENUM_CLASS_VALUES_SYMBOL)
#define _ENUM_CLASS_NAMES_ENUM(members)                                        \
  _ENUM_CLASS_REPEAT(members, _ENUM_CLASS_NAMES_SYMBOL)
#define _ENUM_CLASS_INSTANCES_ENUM(members)                                    \
  _ENUM_CLASS_REPEAT(members, _ENUM_CLASS_INSTANCES_SYMBOL)

// Generate a class with the provided name and several free functions. Enumerate
// the provided members in various ways to produce symbol, value, name, and
// instance lists.
#define _ENUM_CLASS_IMPL(ClassName, members)                                   \
  class ClassName;                                                             \
                                                                               \
  inline constexpr bool operator==(const ClassName&,                           \
                                   const ClassName&) noexcept;                 \
  inline constexpr bool operator<(const ClassName&,                            \
                                  const ClassName&) noexcept;                  \
  inline std::ostream& operator<<(std::ostream&, const ClassName&) noexcept;   \
                                                                               \
  class ClassName : public trait::RelOps<ClassName> {                          \
  public:                                                                      \
    enum class Enum { BOOST_PP_ARRAY_ENUM(members) };                          \
                                                                               \
    TEMPLATE_MAP(Name, Enum, util::StaticString, _ENUM_CLASS_KV_ENUM(members)) \
                                                                               \
    template <Enum value>                                                      \
    static constexpr ClassName Create() noexcept {                             \
      return ClassName(value, Name<value>::value());                           \
    }                                                                          \
                                                                               \
    static constexpr std::array<Enum, BOOST_PP_ARRAY_SIZE(members)>            \
    Values() noexcept {                                                        \
      return {{_ENUM_CLASS_VALUES_ENUM(members)}};                             \
    }                                                                          \
                                                                               \
    static constexpr std::array<util::StaticString,                            \
                                BOOST_PP_ARRAY_SIZE(members)>                  \
    Names() noexcept {                                                         \
      return {{_ENUM_CLASS_NAMES_ENUM(members)}};                              \
    }                                                                          \
                                                                               \
    static constexpr std::array<ClassName, BOOST_PP_ARRAY_SIZE(members)>       \
    Instances() noexcept {                                                     \
      return {{_ENUM_CLASS_INSTANCES_ENUM(members)}};                          \
    }                                                                          \
                                                                               \
    const Enum value;                                                          \
    const util::StaticString name;                                             \
                                                                               \
  private:                                                                     \
    constexpr ClassName(Enum value, util::StaticString name) noexcept          \
      : value(std::move(value)), name(std::move(name)) {}                      \
  };                                                                           \
                                                                               \
  constexpr bool operator==(const ClassName& lhs,                              \
                            const ClassName& rhs) noexcept {                   \
    return lhs.value == rhs.value;                                             \
  }                                                                            \
                                                                               \
  constexpr bool operator<(const ClassName& lhs,                               \
                           const ClassName& rhs) noexcept {                    \
    return lhs.value < rhs.value;                                              \
  }                                                                            \
                                                                               \
  std::ostream& operator<<(std::ostream& lhs, const ClassName& rhs) noexcept { \
    return lhs << rhs.name;                                                    \
  }

// Bundle the provided variadic arguments into an array, then delegate to
// another macro to generate a class with the provided name.
#define ENUM_CLASS(ClassName, ...)                                             \
  _ENUM_CLASS_IMPL(ClassName, BOOST_PP_VARIADIC_TO_ARRAY(__VA_ARGS__))
