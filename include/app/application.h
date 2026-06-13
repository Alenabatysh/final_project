#ifndef APPLICATION_H
#define APPLICATION_H

#include "cli/args_parser.h"
#include "cli/filter_registry.h"
#include "converter/filter_pipeline_builder.h"
#include "wav/wav_reader.h"
#include "wav/wav_writer.h"
#include <iostream>

class Application {
public:
  Application(std::ostream &output = std::cout,
              std::ostream &error = std::cerr);
  void configure();
  int run(int argc, char *argv[]);

private:
  void register_filters();
  void print_help() const;

  std::ostream &out_;
  std::ostream &err_;
  ArgsParser parser_;
  FilterRegistry registry_;
  FilterPipelineBuilder builder_;
  WavReader reader_;
  WavWriter writer_;
  bool configured_ = false;
};

#endif