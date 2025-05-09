#ifndef JLOX_CPP_INCLUDE_KEYWORDS_HPP_
#define JLOX_CPP_INCLUDE_KEYWORDS_HPP_

#include "token.hpp"

#include <array>

namespace jlox_cpp {
using tt = TokenType;

struct Keyword {
  const char *text;
  TokenType type;
};

static constexpr std::array<Keyword, 16UL> keywords = {{
    // clang-format off
  {"and",     tt::kw_and},
  {"class",   tt::kw_class},
  {"else",    tt::kw_else},
  {"false",   tt::kw_false},
  {"fun",     tt::kw_fun},
  {"for",     tt::kw_for},
  {"if",      tt::kw_if},
  {"nil",     tt::nil},
  {"or",      tt::kw_or},
  {"print",   tt::print},
  {"return",  tt::kw_return},
  {"super",   tt::super},
  {"this",    tt::kw_this},
  {"true",    tt::kw_true},
  {"var",     tt::var},
  {"while",   tt::kw_while},
    // clang-format on
}};

} // namespace jlox_cpp

#endif // JLOX_CPP_INCLUDE_KEYWORDS_HPP_
