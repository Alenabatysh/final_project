#include "app/application.h"
#include "filters/ampl_filter.h"
#include "filters/fade_filter.h"
#include "filters/lowpass_filter.h"
#include "filters/normalize_filter.h"
#include "filters/reverse_filter.h"
#include "filters/silence_filter.h"
#include "filters/sin_gen_filter.h"
#include "filters/timestretch_filter.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

Application::Application(std::ostream &output, std::ostream &error)
    : out_(output), err_(error), builder_(registry_) {}

void Application::configure() {
  register_filters();
  configured_ = true;
}

void Application::register_filters() {
  registry_.register_filter(
      "ampl",
      [](const FilterDescriptor &descriptor) -> std::unique_ptr<IFilter> {
        if (descriptor.parameters.empty()) {
          throw std::runtime_error("ampl requires factor");
        }
        double factor = std::stod(descriptor.parameters[0]);
        return std::make_unique<AmplFilter>(factor);
      });

  registry_.register_filter(
      "normalize",
      [](const FilterDescriptor &descriptor) -> std::unique_ptr<IFilter> {
        double peak = 1.0;
        if (!descriptor.parameters.empty()) {
          peak = std::stod(descriptor.parameters[0]);
        }
        return std::make_unique<NormalizeFilter>(peak);
      });

  registry_.register_filter(
      "silence",
      [](const FilterDescriptor &descriptor) -> std::unique_ptr<IFilter> {
        if (descriptor.parameters.size() < 3) {
          throw std::runtime_error("silence requires unit start end");
        }
        std::string unit = descriptor.parameters[0];
        double start = std::stod(descriptor.parameters[1]);
        double end = std::stod(descriptor.parameters[2]);
        if (unit == "ms") {
          start /= 1000.0;
          end /= 1000.0;
        } else if (unit != "sec") {
          throw std::runtime_error("silence unit must be sec or ms");
        }
        return std::make_unique<SilenceFilter>(start, end);
      });

  registry_.register_filter(
      "timestretch",
      [](const FilterDescriptor &descriptor) -> std::unique_ptr<IFilter> {
        if (descriptor.parameters.empty()) {
          throw std::runtime_error("timestretch requires factor");
        }
        double factor = std::stod(descriptor.parameters[0]);
        return std::make_unique<TimestretchFilter>(factor);
      });

  registry_.register_filter(
      "lowpass",
      [](const FilterDescriptor &descriptor) -> std::unique_ptr<IFilter> {
        if (descriptor.parameters.empty()) {
          throw std::runtime_error("lowpass requires window_size");
        }
        std::size_t window =
            static_cast<std::size_t>(std::stoul(descriptor.parameters[0]));
        return std::make_unique<LowpassFilter>(window);
      });

  registry_.register_filter(
      "reverse", [](const FilterDescriptor &) -> std::unique_ptr<IFilter> {
        return std::make_unique<ReverseFilter>();
      });

  registry_.register_filter(
      "fade_in",
      [](const FilterDescriptor &descriptor) -> std::unique_ptr<IFilter> {
        if (descriptor.parameters.empty()) {
          throw std::runtime_error("fade_in requires duration_ms");
        }
        double duration_ms = std::stod(descriptor.parameters[0]);
        return std::make_unique<FadeFilter>(FadeFilter::Mode::in, duration_ms);
      });

  registry_.register_filter(
      "fade_out",
      [](const FilterDescriptor &descriptor) -> std::unique_ptr<IFilter> {
        if (descriptor.parameters.empty()) {
          throw std::runtime_error("fade_out requires duration_ms");
        }
        double duration_ms = std::stod(descriptor.parameters[0]);
        return std::make_unique<FadeFilter>(FadeFilter::Mode::out, duration_ms);
      });

  registry_.register_filter(
      "gen_sin",
      [](const FilterDescriptor &descriptor) -> std::unique_ptr<IFilter> {
        if (descriptor.parameters.size() < 2) {
          throw std::runtime_error(
              "gen_sin requires frequency_hz and duration_ms");
        }
        double frequency_hz = std::stod(descriptor.parameters[0]);
        double duration_ms = std::stod(descriptor.parameters[1]);
        return std::make_unique<SinGeneratorFilter>(frequency_hz, duration_ms);
      });
}

int Application::run(int argc, char *argv[]) {
  if (!configured_) {
    throw std::logic_error("Application not configured");
  }

  ArgsParser::Result parse_result = parser_.parse(argc, argv);
  if (parse_result == ArgsParser::Result::no_args || parser_.help_requested()) {
    print_help();
    return 0;
  }
  if (parse_result != ArgsParser::Result::ok) {
    err_ << "Error: " << parser_.error_message() << std::endl;
    print_help();
    return 1;
  }

  try {
    Waveform waveform;
    if (parser_.input_file().has_value()) {
      out_ << "Reading input file: " << *parser_.input_file() << std::endl;
      waveform = reader_.read(parser_.input_file()->c_str());
    } else {
      out_ << "No input file, starting with empty waveform" << std::endl;
    }

    Pipeline pipeline = builder_.build_pipeline(parser_.filters());

    if (!pipeline.empty()) {
      out_ << "Applying " << pipeline.size() << " filter(s)..." << std::endl;
      pipeline.apply(waveform);
    }

    if (parser_.output_file().has_value()) {
      const std::string &output_filename = *parser_.output_file();
      std::ifstream test_file(output_filename);
      if (test_file.is_open()) {
        test_file.close();
        out_ << "File '" << output_filename
             << "' already exists. Overwrite? (y/n): ";
        std::string user_answer;
        std::getline(std::cin, user_answer);
        if (user_answer != "y" && user_answer != "Y") {
          out_ << "Write aborted." << std::endl;
          return 0;
        }
      }
      out_ << "Writing output file: " << output_filename << std::endl;
      writer_.write(output_filename.c_str(), waveform);
    } else {
      out_ << "No output file specified, result not saved" << std::endl;
    }
    return 0;
  } catch (const std::exception &error) {
    err_ << "Processing error: " << error.what() << std::endl;
    return 2;
  }
}

void Application::print_help() const {
  out_ << "Sound Processor – command-line WAV filter tool\n"
       << "Usage:\n"
       << "  sound_processor [-i input.wav] [-o output.wav] [-f filter "
          "[params...]]...\n"
       << "Filters:\n"
       << "  ampl factor                     – multiply amplitude by factor\n"
       << "  normalize [peak]                – normalize to peak (0..1)\n"
       << "  silence sec|ms start end        – insert silence\n"
       << "  timestretch factor              – stretch/compress by factor\n"
       << "  lowpass window_size             – moving average (odd size)\n"
       << "  reverse                         – reverse audio\n"
       << "  fade_in duration_ms             – linear fade-in\n"
       << "  fade_out duration_ms            – linear fade-out\n"
       << "  gen_sin frequency_hz duration_ms – generate sine wave\n";
}