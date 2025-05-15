#ifndef JLOX_CPP_INCLUDE_PARSER_HPP_
#define JLOX_CPP_INCLUDE_PARSER_HPP_

#include "token.hpp"
#include "util.hpp"

#include <iostream>
#include <vector>

namespace jlox_cpp {

// todo error handling

class Parser {
public:
  // explicit Parser(std::vector<Token> &tokens) : _tokens{std::move(tokens)} {}
  explicit Parser(std::vector<Token> tokens) : _tokens{std::move(tokens)} {}
  void parse();

private:
  std::vector<Token> _tokens;
  std::vector<Token>::const_iterator lookahead = _tokens.begin();

  void expression();
  void equality();
  void comparison();
  void term();
  void factor();
  void unary();
  void primary();
  bool match(TokenType tt);
};

/*
 *  expression     → equality ;
 *  equality       → comparison ( ( "!=" | "==" ) comparison )* ;
 *  comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
 *  term           → factor ( ( "-" | "+" ) factor )* ;
 *  factor         → unary ( ( "/" | "*" ) unary )* ;
 *  unary          → ( "!" | "-" ) unary
 *                    | primary ;
 *  primary        → NUMBER | STRING | "true" | "false" | "nil"
 *                    | "(" expression ")" ;
 */
inline void Parser::parse() {
  util::println("Parsing ", _tokens.size(), " tokens");
  expression();
}

inline void Parser::expression() { equality(); }

inline void Parser::equality() {
  comparison();
  switch (lookahead->type_) {
  case TokenType::bang_equal:
  case TokenType::equal_equal:
    match(lookahead->type_);
    comparison();
    break;
  default: {
  }
  }
}

inline void Parser::comparison() {
  term();
  switch (lookahead->type_) {
  case TokenType::greater:
  case TokenType::greater_equal:
  case TokenType::less:
  case TokenType::less_equal:
    match(lookahead->type_);
    term();
    break;
  default: {
  }
  }
}

inline void Parser::term() {
  factor();
  switch (lookahead->type_) {
  case TokenType::plus:
  case TokenType::minus:
    match(lookahead->type_);
    factor();
    break;
  default: {
  }
  }
}

inline void Parser::factor() {
  unary();
  switch (lookahead->type_) {
  case TokenType::slash:
  case TokenType::star:
    match(lookahead->type_);
    unary();
    break;
  default: {
  }
  }
}

inline void Parser::unary() {
  if (match(TokenType::bang) || match(TokenType::minus)) {
    unary();
  } else {
    primary();
  }
}

inline void Parser::primary() {
  if (match(TokenType::number)) {
  } else if (match(TokenType::string)) {
  } else if (match(TokenType::kw_true)) {
  } else if (match(TokenType::kw_false)) {
  } else if (match(TokenType::nil)) {
  } else if (match(TokenType::left_paren)) {
    expression();
    match(TokenType::right_paren);
  }
}

inline bool Parser::match(TokenType tt) {
  if (lookahead->type_ == tt) {
    ++lookahead;
    return true;
  }
  return false;
}

} // namespace jlox_cpp

#endif // JLOX_CPP_INCLUDE_PARSER_HPP_
