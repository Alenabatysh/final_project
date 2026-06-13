cmake_minimum_required(VERSION 3.10)
project(sound_processor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(sound_processor
    src/main.cpp
    src/waveform.cpp
    src/pipeline.cpp
    src/wav/wav_reader.cpp
    src/wav/wav_writer.cpp
    src/cli/args_parser.cpp
    src/cli/filter_registry.cpp
    src/converter/filter_pipeline_builder.cpp
    src/app/application.cpp
    src/filters/ampl_filter.cpp
    src/filters/silence_filter.cpp
    src/filters/timestretch_filter.cpp
    src/filters/sin_gen_filter.cpp
    src/filters/normalize_filter.cpp
    src/filters/reverse_filter.cpp
    src/filters/lowpass_filter.cpp
    src/filters/fade_filter.cpp
)

target_include_directories(sound_processor PRIVATE include)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(sound_processor_tests
    tests/test_main.cpp
    src/waveform.cpp
    src/filters/ampl_filter.cpp
    src/filters/silence_filter.cpp
    src/filters/timestretch_filter.cpp
    src/filters/sin_gen_filter.cpp
    src/filters/normalize_filter.cpp
    src/filters/reverse_filter.cpp
    src/filters/lowpass_filter.cpp
    src/filters/fade_filter.cpp
    src/filters/am_gen_filter.cpp   
)

target_link_libraries(sound_processor_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(sound_processor_tests PRIVATE include)

add_test(NAME sound_processor_tests COMMAND sound_processor_tests)