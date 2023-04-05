#pragma once

#include <string>
#include <unordered_map>

#include "../Renderer/ShaderProgram.h"

class ResourceManager {
private:
    using ShaderProgramMap = std::unordered_map<std::string,
                                                std::shared_ptr<Renderer::ShaderProgram>>;
    std::string _resourcePath;
    ShaderProgramMap _shaders;

public:

};
