#pragma once

#include <Graphics/Light.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Model3D.hpp>

class Renderer {
public:
	virtual void renderFrame() = 0;

	void registerLight(const LightPtr& l) { m_lights[l->type].push_back(l); }
	void unregisterLight(const LightPtr& l) {
		auto vec = m_lights[l->type];
		for(auto it = vec.begin(); it != vec.end(); ++it)
		{
			if(l.get() == (*it).get())
			{
				vec.erase(it);
				return;
			}
		}
	}
	void registerModel(const Model3DPtr& m) { m_meshs.push_back(m); }
	void unregisterModel(const Model3DPtr& m) {
		for(auto it = m_meshs.begin(); it != m_meshs.end(); ++it)
		{
			if(m.get() == (*it).get())
			{
				m_meshs.erase(it);
				return;
			}
		}
	}

	void setCamera(const CameraPtr& c) { m_camera = c; onCameraChanged(); }

protected:
	virtual void onCameraChanged() {}
	
	std::vector<LightPtr> m_lights[LightType_Max];
	std::vector<Model3DPtr> m_meshs;
	CameraPtr m_camera;
};