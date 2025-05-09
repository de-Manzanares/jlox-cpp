#include "scanner.hpp"

int main() {

  namespace jl = jlox_cpp;

  constexpr std::size_t kilo_byte = 1024UL;
  constexpr std::size_t mega_byte = 1024UL * kilo_byte;

  int ret{};
  static std::array<char, mega_byte / 2UL> buffer;

  try {
    for (int i = 0; i < 100000; ++i) {
      auto src_len = jl::load_source(&buffer, "identifiers.lox");
      jl::scan(buffer.data(), src_len);

      src_len = jl::load_source(&buffer, "keywords.lox");
      jl::scan(buffer.data(), src_len);

      src_len = jl::load_source(&buffer, "numbers.lox");
      jl::scan(buffer.data(), src_len);

      src_len = jl::load_source(&buffer, "punctuators.lox");
      jl::scan(buffer.data(), src_len);

      src_len = jl::load_source(&buffer, "strings.lox");
      jl::scan(buffer.data(), src_len);

      src_len = jl::load_source(&buffer, "whitespace.lox");
      jl::scan(buffer.data(), src_len);
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    ret = EXIT_FAILURE;
  }
  return ret;
}
