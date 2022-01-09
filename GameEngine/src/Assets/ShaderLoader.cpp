#include "ShaderLoader.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>
#include <algorithm>

namespace GameEngine {

	std::unordered_map<std::string, std::string> getSourcesFromFile(const char* filepath);

	std::shared_ptr<Shader> ShaderLoader::load(AssetData<Shader>* assetData) const {
        const std::string& filepath = assetData->filepath;

		std::unordered_map<std::string, std::string> sources = getSourcesFromFile(filepath.c_str());

		auto vertexShaderSource = sources.find("vertex");
		auto fragmentShaderSource = sources.find("fragment");

		if(vertexShaderSource == sources.end() || fragmentShaderSource == sources.end()) {
			std::cout << "ERROR: No vertex and/or fragment shader" << std::endl;
			return std::shared_ptr<Shader>();
		}

		std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexShaderSource->second.c_str(), fragmentShaderSource->second.c_str());

		if(!shader->compiledSuccessfully()) return std::shared_ptr<Shader>();

		return shader;
	}

	std::unordered_map<std::string, std::string> getSourcesFromFile(const char* filepath) {
		std::unordered_map<std::string, std::string> shaderSections;
		
		std::ifstream filestream(filepath, std::ios::in);
		if(!filestream.is_open()) {
			std::cout << "Could not open file " << filepath << std::endl;
			return shaderSections;
		}
		std::stringstream buffer;
		buffer << filestream.rdbuf();
		filestream.close();
		
		std::string shaderSource = buffer.str();

		const char* sectionToken = "#section";
		size_t sectionTokenLength = std::strlen(sectionToken);
		size_t pos = shaderSource.find(sectionToken, 0);
		while(pos != std::string::npos) {
			size_t eol = shaderSource.find_first_of("\r\n", pos);
			size_t sectionTypeBegin = pos + sectionTokenLength + 1;
			std::string sectionStr = shaderSource.substr(sectionTypeBegin, eol - sectionTypeBegin);
			
			size_t nextLinePos = shaderSource.find_first_of("\r\n", eol);
			pos = shaderSource.find(sectionToken, nextLinePos);

			shaderSections[sectionStr] = (pos == std::string::npos) ? shaderSource.substr(nextLinePos) : shaderSource.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSections;
	}



}
