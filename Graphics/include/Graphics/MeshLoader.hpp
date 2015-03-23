#pragma once

#include <Graphics/Model3D.hpp>

namespace MeshLoader {
	bool load(Model3D* ptr, const std::string& filename);
}