#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(){
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	programId = glCreateProgram();
}

Shader::~Shader(){
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(programId);
}

bool Shader::loadFromFile(const std::string& vertex, const std::string& fragment){
	std::ifstream vertFile(vertex);
	if(!vertFile.is_open())
		return false;
	
	std::string vertexSource;
    char c;
    while(!vertFile.eof()) {
        c = vertFile.get();
        vertexSource += c;
    }
    vertexSource.resize(vertexSource.size()-1);

	const char* vertSourceBuffer = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &vertSourceBuffer, 0);


	std::cout << vertexSource << std::endl;
	
	std::ifstream fragFile(fragment);
	if(!fragFile.is_open())
		return false;

	std::string fragmentSource;
    while(!fragFile.eof()) {
        c = fragFile.get();
        fragmentSource += c;
    }
    fragmentSource.resize(fragmentSource.size()-1);

	const char* fragSourceBuffer = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fragSourceBuffer, 0);
	std::cout << "fragment: " << fragmentSource << std::endl;
	return true;
}
bool Shader::compile(){
	// Compilation du vertex shader
    glCompileShader(vertexShader);

    /// Vérification que la compilation a bien fonctionnée (très important !)
    GLint compileStatus;

    // Récupération du status de compilation
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus == GL_FALSE) {
        // Si echec, récupération de la taille du log de compilation
        GLint logLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

        // Allocation d'une chaine de caractère suffisement grande pour contenir le log
        std::unique_ptr<char[]> log(new char[logLength]);

        glGetShaderInfoLog(vertexShader, logLength, 0, log.get());
        std::cerr << "Vertex Shader error:" << log.get() << std::endl;

        return false;
    }

    // Compilation du shader
    glCompileShader(fragmentShader);

    /// Vérification que la compilation a bien fonctionnée (très important !)

    // Récupération du status de compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus == GL_FALSE) {
        // Si echec, récupération de la taille du log de compilation
        GLint logLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

        // Allocation d'une chaine de caractère suffisement grande pour contenir le log
        std::unique_ptr<char[]> log(new char[logLength]);

        glGetShaderInfoLog(fragmentShader, logLength, 0, log.get());
        std::cerr << "Fragment Shader error:" << log.get() << std::endl;
        return false;
    }

    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);

    // Edition de lien
    glLinkProgram(programId);

    /// Vérification que l'édition de liens a bien fonctionnée (très important aussi !)
    GLint linkStatus;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
    if(linkStatus == GL_FALSE) {
        // Si echec, récupération de la taille du log de link
        GLint logLength;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

        // Allocation d'une chaine de caractère suffisement grande pour contenir le log
        std::unique_ptr<char[]> log(new char[logLength]);

        glGetProgramInfoLog(programId, logLength, 0, log.get());
        std::cerr << "Program link error:" << log.get() << std::endl;

        return false;
    }

    return true;
}
void Shader::bind(){
	glUseProgram(programId);
}
void Shader::unbind(){
	glUseProgram(0);
}