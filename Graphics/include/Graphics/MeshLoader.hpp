#pragma once

#include <Graphics/Model3D.hpp>

namespace MeshLoader {
	struct MeshParams {
		glm::vec3 scale;
		glm::vec4 color;
		MeshParams() : scale(1,1,1), color(1,1,1,1) {}
	};

	bool load(Model3D* ptr, const std::string& filename, const MeshParams& params = MeshParams());
}