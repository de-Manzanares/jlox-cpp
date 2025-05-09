#ifndef JLOX_CPP_INCLUDE_TOKEN_HPP_
#define JLOX_CPP_INCLUDE_TOKEN_HPP_

#include "syntax_error.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <variant>

#ifdef SCANNER_PRINT_TOKENS_

// for printing token literals
inline std::ostream &operator<<(std::ostream &os, std::monostate tkn) {
  os << std::string_view{"null"};
  return os;
}

#endif

namespace jlox_cpp {

enum class TokenType : std::uint8_t {
  // Single-character tokens.
  left_paren,
  right_paren,
  left_brace,
  right_brace,
  comma,
  dot,
  minus,
  plus,
  semicolon,
  slash,
  star,

  // One or two character tokens.
  bang,
  bang_equal,
  equal,
  equal_equal,
  greater,
  greater_equal,
  less,
  less_equal,

  // Literals.
  identifier,
  string,
  number,

  // Keywords.
  kw_and,
  kw_class,
  kw_else,
  kw_false,
  kw_fun,
  kw_for,
  kw_if,
  nil,
  kw_or,
  print,
  kw_return,
  super,
  kw_this,
  kw_true,
  var,
  kw_while,

  eof
};

using Literal = std::variant<std::monostate, bool, double, std::string>;

class Token {
public:
  Token(const TokenType type, const std::string_view lexeme,
        const LineColumn &line_column, Literal literal = std::monostate{})
      : type_{type}, lexeme_{lexeme}, line_column_{line_column},
        literal_{std::move(literal)} {}

  // private:
  TokenType type_;
  std::string_view lexeme_;
  LineColumn line_column_;
  Literal literal_;
};

struct TokenWord {
  const char *text;
  TokenType type;
};

using tt = TokenType;

constexpr static std::array<TokenWord, 39UL> token_words = {{
    // clang-format off
  {"left_paren",    tt::left_paren},
  {"right_paren",   tt::right_paren},
  {"left_brace",    tt::left_brace},
  {"right_brace",   tt::right_brace},
  {"comma",         tt::comma},
  {"dot",           tt::dot},
  {"minus",         tt::minus},
  {"plus",          tt::plus},
  {"semicolon",     tt::semicolon},
  {"slash",         tt::slash},
  {"star",          tt::star},

  {"bang",          tt::bang},
  {"bang_equal",    tt::bang_equal},
  {"equal",         tt::equal},
  {"equal_equal",   tt::equal_equal},
  {"greater",       tt::greater},
  {"greater_equal", tt::greater_equal},
  {"less",          tt::less},
  {"less_equal",    tt::less_equal},

  {"identifier",    tt::identifier},
  {"string",        tt::string},
  {"number",        tt::number},

  {"kw_and",        tt::kw_and},
  {"kw_class",      tt::kw_class},
  {"kw_else",       tt::kw_else},
  {"kw_false",      tt::kw_false},
  {"kw_fun",        tt::kw_fun},
  {"kw_for",        tt::kw_for},
  {"kw_if",         tt::kw_if},
  {"nil",           tt::nil},
  {"kw_or",         tt::kw_or},
  {"print",         tt::print},
  {"kw_return",     tt::kw_return},
  {"super",         tt::super},
  {"kw_this",       tt::kw_this},
  {"kw_true",       tt::kw_true},
  {"var",           tt::var},
  {"kw_while",      tt::kw_while},

  {"eof",           tt::eof},
    // clang-format on
}};

inline std::ostream &operator<<(std::ostream &os, const TokenType &tkn) {
  const auto wt = std::find_if(token_words.begin(), token_words.end(),
                               [tkn](const auto x) { return x.type == tkn; });
  if (wt != token_words.end()) {
    os << std::string{wt->text};
  } else {
    os << std::string{
        "ostream error: token type not found in <std::array<TokenWord, "
        "39UL> token_words"};
  }
  return os;
}

} // namespace jlox_cpp

#endif // JLOX_CPP_INCLUDE_TOKEN_HPP_
