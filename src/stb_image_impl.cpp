#define STB_IMAGE_IMPLEMENTATION
#ifdef __MINGW32__
#include "thirdparty/stb_image.h"
#elif defined(__SWITCH__)
#include "thirdparty/stb_image.h"
#else
#include <stb_image.h>
#endif