#include "parser.hpp"
#include "scanner.hpp"

int main() {

  namespace jl = jlox_cpp;

  constexpr std::size_t kilo_byte = 1024UL;
  constexpr std::size_t mega_byte = 1024UL * kilo_byte;

  int ret{};
  static std::array<char, mega_byte / 2UL> buffer;

  try {
    for (int i = 0; i < 1; ++i) {
      const auto src_len = jl::load_source(&buffer, "expressions/parse.lox");
      jl::Parser p(jl::scan(buffer.data(), src_len));
      p.parse();
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    ret = EXIT_FAILURE;
  }
  return ret;
}
