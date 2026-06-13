#include "app/application.h"
#include <iostream>

int main(int argc, char *argv[]) {
  try {
    Application app;
    app.configure();
    return app.run(argc, argv);
  } catch (const std::exception &error) {
    std::cerr << "Fatal error: " << error.what() << std::endl;
    return 255;
  }
}