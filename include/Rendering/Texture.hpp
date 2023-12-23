#include <glad/gl.h>
#include <string>

enum TextureType {
    DIFFUSE,
    SPECULAR,
};

struct Texture {
    GLuint id;
    TextureType txType;
    std::string path;
};
