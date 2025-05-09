#include <onnxruntime_cxx_api.h>
#include <iostream>

int main() 
{
  Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
  Ort::SessionOptions seesion_options;
  std::cout << "ONNX Runtime setup successfully!" << "\n";

  return 0;
}
