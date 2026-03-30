// Definição de MACROS e do vulkan.hpp melhorando a questão de erros por
// parametros
#include <vulkan/vulkan.hpp>
#define VULKAN_HHP_NO_STRUCT_CONSTRUCTORS

// Definição de qual moculo do Cmake vai se usada
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
// O GLFW cria as janelas no linux, gerenciamento do teclado e mouse e oferecer
// o local onde vai ser desenhado os objetos
#include <GLFW/glfw3.h>

// Biblioteca que possui MACROS é as funçoes EXIT_FAILURE e EXIT_SUCCESS
#include <cstdlib>
#include <iostream>
#include <stdexcept>

class HelloTriangleApplication {
public:
  void run() {
    initVulkan();
    mainloop();
    cleanup();
  }

private:
  void initVulkan() {}

  void mainloop() {}

  void cleanup() {}

  void initWindow() {}
};

int main() {
  try {
    HelloTriangleApplication app;
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
