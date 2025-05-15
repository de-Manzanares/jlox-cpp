#ifndef JLOX_CPP_INCLUDE_SCANNER_HPP_
#define JLOX_CPP_INCLUDE_SCANNER_HPP_

#include "keywords.hpp"
#include "syntax_error.hpp"
#include "token.hpp"
#include "util.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <variant>
#include <vector>

namespace jlox_cpp {

struct SourceStats {
  std::size_t bytes;
  std::size_t lines;
};

template <std::size_t BUFF_SIZE>
std::size_t load_source(std::array<char, BUFF_SIZE> *buffer,
                        const char *file_name) {
  if (!std::filesystem::exists(file_name)) {
    throw std::runtime_error("File does not exist: " +
                             std::filesystem::absolute(file_name).string());
  }

  std::ifstream file{file_name, std::ios::binary | std::ios::ate};

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " +
                             std::filesystem::absolute(file_name).string());
  }
  SourceStats stats{};
  const auto file_size = file.tellg();
  stats.bytes = static_cast<std::size_t>(file_size);
  (void)file.seekg(0);
  (void)file.read(buffer->data(), file_size);
  buffer->at(stats.bytes) = '\0';
  return stats.bytes;
}

/**
 *
 * @param src characters to scan
 * @param src_len number of characters
 * @return tokens
 * @note not an on-demand lexer... uses more memory this way...
 */
inline std::vector<Token> scan(const char *src, const std::size_t src_len) {
  using tt = TokenType;
  using Literal = std::variant<std::monostate, bool, double, std::string>;

  // todo reserve based on analysis of lox files
  constexpr std::size_t est_bytes_per_token = 10UL;
  constexpr std::size_t est_bytes_per_line = 10UL;
  constexpr std::size_t many_many_errors = 10UL;

  std::vector<Token> tokens;
  tokens.reserve(src_len / est_bytes_per_token);

  std::vector<std::string_view> lines;
  lines.reserve(src_len / est_bytes_per_line);

  std::vector<SyntaxError> syntax_errors;
  syntax_errors.reserve(many_many_errors); // lol

  std::size_t line_n{};
  std::size_t column_n{1UL};
  const auto *line_begin = src;

  // loop counter is modified within the loop
  for (const auto *it = src; it != &src[src_len]; ++it, ++column_n) {

    auto add_token = [line_n, &it, &column_n,
                      &tokens](const TokenType tkn, const std::size_t len = 1UL,
                               const Literal &literal = std::monostate()) {
      (void)tokens.emplace_back(tkn, std::string_view{it, len},
                                LineColumn{line_n, column_n}, literal);
      if (len != 1UL) {
        it += len - 1UL;
        column_n += len - 1UL;
      }
    };

    auto choose_token = [&add_token, &it](const TokenType tkn_1,
                                          const TokenType tkn_2,
                                          const char ch) {
      if (*(it + 1) == ch) {
        add_token(tkn_2, 2UL);
      } else {
        add_token(tkn_1);
      }
    };

    auto test_comment = [&add_token, &it, &column_n] {
      if (*(it + 1) == '/') {
        while (*(it + 1) != '\n' && *(it + 1) != '\0') {
          ++it;
          ++column_n;
        }
      } else {
        add_token(tt::slash);
      }
    };

    auto test_string = [&add_token, line_n, &column_n, it, &syntax_errors] {
      const auto *i_it = it;
      std::size_t len = 1UL;
      while (*(i_it + 1) != '\n') { // todo this is broken?
        do {                        // NOLINT(*-avoid-do-while)
          ++i_it;
          ++len;
        } while (*i_it != '"');
      }
      if (*i_it == '"') {
        if (*i_it == '\n') {
          len--;
        }
        add_token(tt::string, len, std::string{it + 1, it + len - 1});
      } else {
        (void)syntax_errors.emplace_back(LineColumn{line_n, column_n},
                                         "missing closing \"");
      }
    };

    auto test_number = [&add_token, line_n, &column_n, it, &syntax_errors] {
      const auto *i_it = it;
      std::size_t len = 1UL;
      while (isdigit(*i_it) != 0 || *i_it == '.') {
        ++i_it;
        ++len;
      }
      // if (*i_it == '\n') {
      len--;
      // }
      // todo terminate cleanly?
      if (*(i_it - 1) != '.') {
        add_token(tt::number, len, std::stod(std::string{it, it + len}));
      } else if (syntax_errors.empty() ||
                 (!syntax_errors.empty() &&
                  syntax_errors.back().line() != line_n)) {
        syntax_errors.emplace_back(LineColumn{line_n, column_n + len - 1},
                                   "trailing decimal not allowed");
      }

      // todo syntax error
    };

    auto is_identifier_char = [](const auto ch) {
      return ch == '_' || std::isalnum(ch) != 0;
    };

    auto test_identifier = [&add_token, &is_identifier_char, line_n, &column_n,
                            it] {
      const auto *i_it = it;
      std::size_t len = 1UL;
      do { // NOLINT(*-avoid-do-while)
        ++i_it;
        ++len;
      } while (is_identifier_char(*i_it));
      if (*i_it == '\n' || *i_it == '\t' || *i_it == ' ') {
        len--;
      }
      std::string_view lexeme{it, len};

      if (const auto wk = std::find_if(
              keywords.begin(), keywords.end(),
              [lexeme](const auto &x) { return x.text == lexeme; });
          wk == keywords.end()) {
        add_token(tt::identifier, len);
      } else {
        add_token(wk->type, len);
      }
      // todo syntax errors for identifiers
    };

    switch (*it) {
    case ' ':
    case '\t':
    case '\r': break;
    case '\n': {
      (void)lines.emplace_back(line_begin, column_n);
      line_begin = it + 1;
      ++line_n;
      column_n = 0UL;
    } break;

    case '(': add_token(tt::left_paren); break;
    case ')': add_token(tt::right_paren); break;
    case '{': add_token(tt::left_brace); break;
    case '}': add_token(tt::right_brace); break;
    case ',': add_token(tt::comma); break;
    case '.': add_token(tt::dot); break;
    case '-': add_token(tt::minus); break;
    case '+': add_token(tt::plus); break;
    case ';': add_token(tt::semicolon); break;
    case '*': add_token(tt::star); break;
    case '/': test_comment(); break;

    case '!': choose_token(tt::bang, tt::bang_equal, '='); break;
    case '=': choose_token(tt::equal, tt::equal_equal, '='); break;
    case '>': choose_token(tt::greater, tt::greater_equal, '='); break;
    case '<': choose_token(tt::less, tt::less_equal, '='); break;

    case '"': test_string(); break;

    default: {
      if (std::isdigit(*it) != 0) {
        test_number();
      } else if (is_identifier_char(*it)) {
        test_identifier();
      } else if (syntax_errors.empty() ||
                 (!syntax_errors.empty() &&
                  syntax_errors.back().line() != line_n)) {
        (void)syntax_errors.emplace_back(LineColumn{line_n, column_n},
                                         "wut is dis");
      }
    }
    }
  }

  tokens.emplace_back(tt::eof, "eof", LineColumn{lines.size(), 0});

#define SCANNER_PRINT_TOKENS
#ifdef SCANNER_PRINT_TOKENS_
  util::println("Tokens: ", tokens.size(), '\n');
  for (const auto &token : tokens) {
    util::print(token.type_, ' ', token.lexeme_, ' ');
    std::visit([](const auto x) { util::println(x); }, token.literal_);
  }
  util::print('\n');
#endif

  for (const auto &err : syntax_errors) {
    const auto msg = err.message_;
    util::println("(line:column) => ", err.line() + 1UL, ':', err.column());
    util::print(lines[err.line()]);
    std::cout << std::setw(static_cast<int>(err.column()) + 1)
              << std::setfill(' ') << "^ ";
    util::println(msg, '\n');
  }
  return tokens;
}

} // namespace jlox_cpp

#endif // JLOX_CPP_INCLUDE_SCANNER_HPP_
