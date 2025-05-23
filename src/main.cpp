#include "scanner.hpp"

namespace jlox_cpp {

void run_prompt() {}

template <std::size_t BUFF_SIZE>
void run_file(std::array<char, BUFF_SIZE> *buffer, char *file_name) {
  const auto source_len = load_source(buffer, file_name);
  scan(buffer->data(), source_len);
}

} // namespace jlox_cpp

int main(const int argc, char **argv) {
  namespace jl = jlox_cpp;

  constexpr std::size_t kilo_byte = 1024UL;
  constexpr std::size_t mega_byte = 1024UL * kilo_byte;

  int ret{};
  std::array<char, mega_byte / 2UL> buffer{};

  try {
    switch (argc) {
    case 1:
      jl::run_prompt();
      util::println("Usage: jlox-cpp [script]");
      break;
    case 2: jl::run_file(&buffer, argv[1]); break;
    default: {
      util::println("Usage: jlox-cpp [script]");
      break;
    }
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    ret = EXIT_FAILURE;
  }
  return ret;
}
