#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include "filter_descriptor.h"
#include <optional>
#include <string>
#include <vector>

class ArgsParser {
public:
  enum class Result { ok, no_args, bad_args };

  Result parse(int argc, char *argv[]);

  void set_input_file(const std::string &path) { input_file_ = path; }

  void set_output_file(const std::string &path) { output_file_ = path; }
  
  void set_filters(const std::vector<FilterDescriptor> &filters) {
    filters_ = filters;
  }

  const std::optional<std::string> &config_file() const { return config_file_; }

  const std::optional<std::string> &input_file() const { return input_file_; }

  const std::optional<std::string> &output_file() const { return output_file_; }

  const std::vector<FilterDescriptor> &filters() const { return filters_; }

  bool help_requested() const { return help_requested_; }

  const std::string &error_message() const { return error_message_; }

private:
  bool consume_file_parameter(int argc, char *argv[], int &index,
                              std::optional<std::string> &target,
                              const std::string &flag);
  bool consume_filter(int argc, char *argv[], int &index);
  void set_error(const std::string &msg);

  std::optional<std::string> input_file_;
  std::optional<std::string> output_file_;
  std::vector<FilterDescriptor> filters_;
  bool help_requested_ = false;
  std::string error_message_;
  std::optional<std::string> config_file_;
};

#endif