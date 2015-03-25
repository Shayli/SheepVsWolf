#pragma once

#include <Graphics/Model3D.hpp>

namespace MeshLoader {
	struct MeshParams {
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::vec3 translation;
		glm::vec4 color;
		MeshParams() : scale(1,1,1), rotation(0,0,0), translation(0,0,0), color(1,1,1,1) {}
	};

	bool load(Model3D* ptr, const std::string& filename, const MeshParams& params = MeshParams());
}