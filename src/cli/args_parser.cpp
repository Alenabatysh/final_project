#include "cli/args_parser.h"
#include "cli/filter_descriptor.h"
#include <optional>
#include <string>
#include <vector>

ArgsParser::Result ArgsParser::parse(int argc, char *argv[]) {
  if (argc <= 1) {
    return Result::no_args;
  }

  for (int argument_index = 1; argument_index < argc; ++argument_index) {
    std::string token = argv[argument_index];
    if (token == "-h" || token == "--help") {
      help_requested_ = true;
    } else if (token == "-i") {
      if (!consume_file_parameter(argc, argv, argument_index, input_file_,
                                  "-i")) {
        return Result::bad_args;
      }
    } else if (token == "-o") {
      if (!consume_file_parameter(argc, argv, argument_index, output_file_,
                                  "-o")) {
        return Result::bad_args;
      }
    } else if (token == "-f") {
      if (!consume_filter(argc, argv, argument_index)) {
        return Result::bad_args;
      }
    } else {
      set_error("Unexpected argument: " + token);
      return Result::bad_args;
    }
  }
  return Result::ok;
}

bool ArgsParser::consume_file_parameter(int argc, char *argv[],
                                        int &argument_index,
                                        std::optional<std::string> &target,
                                        const std::string &flag) {
  if (target.has_value()) {
    set_error("Duplicate " + flag + " flag");
    return false;
  }
  if (argument_index + 1 >= argc) {
    set_error("Missing file name after " + flag);
    return false;
  }
  target = argv[++argument_index];
  return true;
}

bool ArgsParser::consume_filter(int argc, char *argv[], int &argument_index) {
  if (argument_index + 1 >= argc) {
    set_error("Missing filter name after -f");
    return false;
  }
  FilterDescriptor descriptor;
  descriptor.name = argv[++argument_index];
  while (argument_index + 1 < argc && argv[argument_index + 1][0] != '-') {
    descriptor.parameters.push_back(argv[++argument_index]);
  }
  filters_.push_back(std::move(descriptor));
  return true;
}

void ArgsParser::set_error(const std::string &message) {
  error_message_ = message;
}