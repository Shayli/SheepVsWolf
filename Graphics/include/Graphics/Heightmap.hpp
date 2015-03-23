#pragma once

#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <string>
#include <memory>
#include <Graphics/Model3D.hpp>
#include <Graphics/Quadtree.hpp>

#include <atomic>

#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>

struct SdlImage {
	SDL_Surface* data;

	SdlImage(const std::string& file)
	{
		data = SDL_LoadBMP(file.c_str());
		if(data == NULL)
			std::cerr << "Unable to open image: " << SDL_GetError() << std::endl;
	}

	~SdlImage()
	{
		SDL_FreeSurface(data);
	}
};


#define NMS_USING_QUADTREE 1
#define NMS_USING_ADAPTATIVE_QUADTREE_LOD 1
#define NMS_UPDATE_TIME 1/16.f

class Camera;

class Heightmap
{
public:
	Heightmap();
	Heightmap(const Heightmap& other) = delete;
	virtual ~Heightmap();
	bool loadFromFile(const std::string& image);
	bool loadFromMemory(float* data, uint32_t size);
	bool create(uint32_t size);

	float getHeightAt(uint32_t x, uint32_t y);
	void setHeightAt(uint32_t x, uint32_t y, float height);

	void update(float elapsed);
	void setCamera(Camera* camera);
	Model3DPtr& getMesh();

	uint32_t getPolycount() const;
	void forceRebuild();

	uint32_t getSize() const;
	std::shared_ptr<QuadTree<float>>& getQuadtree();

protected:
	virtual void buildMap();
	void loadMap(const SdlImage& image);

	void resetMapLODRecursive(QuadTree<float>* node, int32_t val);
	void computeMapLODRecursive(QuadTree<float>* node);
	void buildMapRecursive(QuadTree<float>* node, uint32_t size);
	bool getTmpVertex(VertexBuffer& buffer, const glm::vec2& pos, Vertex& vertex, uint32_t& index);

	std::unique_ptr<float> m_data;
	std::shared_ptr<QuadTree<float>> m_quadtree;
	uint32_t m_size;
	Model3DPtr m_mesh;
	float m_lastUpdate;
	Camera* m_camera;
	std::vector<Triangle> m_tempFaces;
	std::atomic_bool m_updateFinish, m_updatePending, m_buildPending, m_buildFinished;
	uint32_t m_currentTris;
	VertexBuffer m_buffer;
	int gdepth;
};

#endif