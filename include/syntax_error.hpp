#ifndef JLOX_CPP_INCLUDE_SYNTAX_ERROR_HPP_
#define JLOX_CPP_INCLUDE_SYNTAX_ERROR_HPP_

#include <string>

namespace jlox_cpp {

struct LineColumn {
  std::size_t line_;
  std::size_t column_;
};

struct SyntaxError {
  explicit SyntaxError(const LineColumn line_column,
                       const std::string_view message = "")
      : line_column_{line_column}, message_{message} {}

  [[nodiscard]] auto line() const { return line_column_.line_; }
  [[nodiscard]] auto column() const { return line_column_.column_; }
  [[nodiscard]] auto message() const & { return message_; }

  LineColumn line_column_;
  std::string message_;
};

} // namespace jlox_cpp

#endif // JLOX_CPP_INCLUDE_SYNTAX_ERROR_HPP_
