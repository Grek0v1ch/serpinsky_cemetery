#pragma once

#include <string>
#include <map>
#include <memory>

namespace Renderer {
    class ShaderProgram;
}

class ResourceManager {
private:
    using ShaderProgramsMap = std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>>;

    ShaderProgramsMap _shaders;
    std::string _resourcePath;

    ResourceManager() = default;

    [[nodiscard]] std::string getFileString(const std::string& filePath) const;
public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    static ResourceManager& instance();
    void setExecutablePath(const std::string& executablePath) noexcept;
    std::shared_ptr<Renderer::ShaderProgram> loadShaderProgram(const std::string& shaderName,
                                                               const std::string& vertexPath,
                                                               const std::string& fragmentPath);
    std::shared_ptr<Renderer::ShaderProgram>
    getShaderProgram(const std::string& shaderName) const noexcept;
};
