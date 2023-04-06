#include "ResourceManager.h"

#include <fstream>
#include <sstream>
#include <vector>

#include "../Exception/Exception.h"
#include "../Renderer/ShaderProgram.h"

using Renderer::ShaderProgram;

ResourceManager& ResourceManager::instance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::setExecutablePath(const std::string& executablePath) noexcept {
    size_t found = executablePath.find_last_of("/\\");
    _resourcePath = executablePath.substr(0, found);
}

std::shared_ptr<ShaderProgram>
ResourceManager::loadShaderProgram(const std::string& shaderName,
                                   const std::string& vertexPath,
                                   const std::string& fragmentPath) {
    std::string vertexString = getFileString(vertexPath);
    if (vertexString.empty()) {
        throw Exception::Exception("No vertex shader!");
    }
    std::string fragmentString = getFileString(fragmentPath);
    if (fragmentString.empty()) {
        throw Exception::Exception("No fragment shader!");
    }
    try {
        auto temp = _shaders.emplace(shaderName,
                                     std::make_shared<ShaderProgram>(vertexString, fragmentString));
        return temp.first->second;
    } catch (Exception::Exception& ex) {
        std::string msg = "\nCan't load shader program:\nVertex: ";
        msg += vertexPath + "\nFragment: ";
        msg += fragmentPath + "\n";
        ex.addMsg(msg);
        throw ex;
    }
}

std::shared_ptr<ShaderProgram>
ResourceManager::getShaderProgram(const std::string& shaderName) const noexcept {
    auto tmp = _shaders.find(shaderName);
    if (tmp != _shaders.end()) {
        return tmp->second;
    }
    return nullptr;
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const {
    std::ifstream fin(_resourcePath + '/' + relativeFilePath);
    if (! fin.is_open()) {
        throw Exception::Exception("Failed to open file: " + relativeFilePath);
    }
    std::stringstream buffer;
    buffer << fin.rdbuf();
    return buffer.str();
}
