#pragma once

/**
 * Alias a function by creating a perfect-forwarding inline caller to that
 * function. Pass a cv-qualifier using variadic arguments.
 */
#define FUNCTION_ALIAS(before, after, ...)                                     \
  template <typename... Args>                                                  \
  inline auto after(Args... args)                                              \
      ->decltype((before(std::forward<Args>(args)...))) __VA_ARGS__ {          \
    return before(std::forward<Args>(args)...);                                \
  }
