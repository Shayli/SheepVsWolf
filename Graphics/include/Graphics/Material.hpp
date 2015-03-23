#pragma once

#include <GL/glew.h>
#include <memory>

class Material {
public:
	Material();
	~Material();

	bool loadFromFile(const std::string& str);
	GLuint getId() const;

protected:
	GLuint m_id;
};

typedef std::shared_ptr<Material> MaterialPtr;