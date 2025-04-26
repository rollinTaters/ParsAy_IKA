#include <onnxruntime_cxx_api.h>
#include <iostream>

int main() 
{
  Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
  Ort::SessionOptions seesion_options;
  std::cout << "ONNX Runtime setup successfully!" << "\n";
	/* 
          .-.
         o   \     .-.
            .----.'   \
          .'o)  / `.   o
         /         |
         \_)       /-.
           '_.`    \  \
            `.      |  \
             |       \ |
         .--/`-.     / /
       .'.-/`-. `.  .\|
      /.' /`._ `-    '-.
 ____(|__/`-..`-   '-._ \
|`------.'-._ `      ||\ \
|| #   /-.   `   /   || \|
||   #/   `--'  /  /_::_|)__
`|____|-._.-`  /  ||`--------`
      \-.___.` | / || #      |
       \       | | ||   #  # |
       /`.___.'\ |.`|________|
       | /`.__.'|'.`
     __/ \    __/ \
    /__.-.)  /__.-.) 
	*/

  return 0;
}
