#include <Graphics/Heightmap.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Model3D.hpp>
#include <Graphics/Model3DBuffer.hpp>
#include <iostream>
#include <thread>

namespace {
template <typename T>
bool FEquals(const T& a, const T& b)
{
	static T EPSILON = T(0.0001);
	return std::fabs(a - b) < EPSILON;
}
}


Heightmap::Heightmap() :
m_data(nullptr), m_size(0), m_lastUpdate(0), m_currentTris(0), m_camera(nullptr), gdepth(0),
m_mesh(std::make_shared<Model3D>())
{
	m_updatePending = false;
	m_updateFinish = false;
	m_buildFinished = false;
	m_buildPending = true;
}

Heightmap::~Heightmap()
{
	while (!m_updateFinish && m_updatePending)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

bool Heightmap::loadFromFile(const std::string& image)
{
	SdlImage map(image);
	if(!map.data)
	//sf::Image map;
	//if (!map.loadFromFile(image))
		return false;

	loadMap(map);
	return true;
}

bool Heightmap::loadFromMemory(float* data, uint32_t size)
{
	m_data.reset(new float[size*size]);
	std::memcpy(m_data.get(), data, size*size*sizeof(float));
	m_size = size;
	std::thread t([this]{
		this->buildMap();
		m_buildFinished = true;
	});
	t.detach();
	return true;
}

bool Heightmap::create(uint32_t s)
{
	// We have to set the size at the nearest higher power of 2 to optimize better
	uint32_t nearest = std::pow(2, std::ceil(std::log2(s)));
	uint32_t size = nearest;

	m_data.reset(new float[size*size]);
	if (m_data)
	{
		for (uint32_t x = 0; x < size*size; ++x)
		{
			m_data.get()[x] = 0;
		}
		m_size = size;
		return true;
	}
	return false;
	
}

void Heightmap::buildMap()
{
#if NMS_USING_QUADTREE 
	m_quadtree.reset(new QuadTree<float>());
	m_quadtree->feed(m_data.get(), m_size);
	gdepth = 0;
	for (uint32_t x = 0; x < m_size + 1; ++x)
		for (uint32_t y = 0; y < m_size + 1; ++y)
		{
		float h = 0;
		if (x < m_size && y < m_size)
			h = m_data.get()[x*m_size + y];
		else
			if (x < m_size)
				h = m_data.get()[x*m_size + y - 1];
			else if (y < m_size)
				h = m_data.get()[(x - 1)*m_size + y];
			else
				h = m_data.get()[(x - 1)*m_size + y - 1];

			m_buffer.addVertex(glm::vec3((-(int32_t) m_size / 2.f + x*1.f), h, (-(int32_t) m_size / 2.f + y*1.f)), glm::vec3(0, 1, 0), glm::vec2(x*.5f, y*.5f), glm::vec4(1, 1, 1, 1));
		}
	for(size_t x = 0; x<m_size; ++x)
		for(size_t y = 0; y<m_size; ++y) {
			glm::vec3 p1,p2,p3, v1,v2,tmp;
			int v[4];
			v[0] = x*(m_size+1)+y;
			v[1] = x*(m_size+1)+y+1;
			v[2] = (x+1)*(m_size+1)+y;
			v[3] = (x+1)*(m_size+1)+y+1;
			p1 = m_buffer.getVertex(v[0]).position;
			p2 = m_buffer.getVertex(v[3]).position;
			p3 = m_buffer.getVertex(v[2]).position;
			v1 = p2 - p1; v2 = p3 - p1;
			tmp = glm::cross(v1,v2);
			m_buffer.getVertex(v[0]).normal += tmp;
			m_buffer.getVertex(v[3]).normal += tmp;
			m_buffer.getVertex(v[2]).normal += tmp;
			p1 = m_buffer.getVertex(v[1]).position;
			p2 = m_buffer.getVertex(v[3]).position;
			p3 = m_buffer.getVertex(v[0]).position;
			v1 = p2 - p1; v2 = p3 - p1;
			tmp = glm::cross(v1,v2);
			m_buffer.getVertex(v[1]).normal += tmp;
			m_buffer.getVertex(v[3]).normal += tmp;
			m_buffer.getVertex(v[0]).normal += tmp;
		}
	for(size_t x = 0; x<m_size+1; ++x)
		for(size_t y = 0; y<m_size+1; ++y) {
			auto& norm = m_buffer.getVertex(x*(m_size+1)+y).normal;
			if(FEquals(glm::length(norm), 0.f))
				norm = glm::vec3(0,1,0);
		}
	
#if NMS_USING_ADAPTATIVE_QUADTREE_LOD

#else
	/*auto& node = m_quadtree.Childs[QuadTree_BottomRight]->Childs[QuadTree_BottomRight]->Childs[QuadTree_BottomLeft];
		
	buildMapRecursive(node.get(), m_size/8, false);
	buildMapRecursive(node->Neighboors[QuadTree_Left].get(), m_size / 8, false);*/
	buildMapRecursive(m_quadtree.get(), m_size);
	for (auto& t : m_tempFaces)
		m_buffer.addTriangle(t.v1, t.v2, t.v3);
	m_tempFaces.clear();
	m_currentTris = m_buffer.getTrianglesCount();

#endif
#else
	for (uint32_t x = 0; x < m_size; ++x)
		for (uint32_t y = 0; y < m_size; ++y)
		{
		uint8_t h = m_data.get()[x*m_size + y];
		m_buffer.addVertex(glm::vec3(-(int32_t) m_size / 2 + x*1.f, h, -(int32_t) m_size / 2 + y*1.f), glm::vec3(0, 0, 0), glm::vec2(x*1.f / m_size, y*1.f / m_size), glm::vec4(h/255.f, 10/255.f, 100/255.f, 1));
		}


	for (uint32_t x = 0; x < m_size - 1; ++x)
	{
		for (uint32_t y = 0; y < m_size - 1; ++y)
		{
			m_buffer.addTriangle(x*m_size + y, x*m_size + y + 1, (x + 1)*m_size + (y + 1));
			m_buffer.addTriangle(x*m_size + y, (x + 1)*m_size + y + 1, (x + 1)*m_size + y);
		}
	}

	for(size_t x = 0; x<m_size; ++x)
		for(size_t y = 0; y<m_size; ++y) {
			if(x < m_size-1 && y < m_size-1) {
				glm::vec3 p1,p2,p3, v1,v2,tmp;
				int v[4];
				v[0] = x*m_size+y;
				v[1] = x*m_size+y+1;
				v[2] = (x+1)*m_size+y;
				v[3] = (x+1)*m_size+y+1;
				p1 = m_buffer.getVertex(v[0]).position;
				p2 = m_buffer.getVertex(v[3]).position;
				p3 = m_buffer.getVertex(v[2]).position;
				v1 = p2 - p1; v2 = p3 - p1;
				tmp = glm::cross(v1,v2);
				m_buffer.getVertex(v[0]).normal += tmp;
				m_buffer.getVertex(v[3]).normal += tmp;
				m_buffer.getVertex(v[2]).normal += tmp;
				p1 = m_buffer.getVertex(v[1]).position;
				p2 = m_buffer.getVertex(v[3]).position;
				p3 = m_buffer.getVertex(v[0]).position;
				v1 = p2 - p1; v2 = p3 - p1;
				tmp = glm::cross(v1,v2);
				m_buffer.getVertex(v[1]).normal += tmp;
				m_buffer.getVertex(v[3]).normal += tmp;
				m_buffer.getVertex(v[0]).normal += tmp;
			} else if(y < m_size-1){
				p1 = m_buffer.getVertex(x*m_size+y).normal = m_buffer.getVertex((x-1)*m_size+y).normal;
			} else {
				p1 = m_buffer.getVertex(x*m_size+y).normal = m_buffer.getVertex(x*m_size+y-1).normal;
			}
		}

	m_currentTris = m_buffer.getTrianglesCount();
#endif
}

void Heightmap::loadMap(const SdlImage& image)
{
	uint32_t w = image.data->w;
	uint32_t size = std::max(image.data->w , image.data->h);

	// We have to set the size at the nearest higher power of 2 to optimize better
	uint32_t nearest = std::pow(2, std::ceil(std::log2(size)));
	size = nearest;

	m_data.reset(new float[size*size]);

	int channels = image.data->format->Amask != 0 ? 4 : 3;
	uint8_t* pixels = (uint8_t*)image.data->pixels;
	if (m_data)
	{
		for (uint32_t x = 0; x < size; ++x)
		{
			for (uint32_t y = 0; y < size; ++y)
			{
				if (x < image.data->w && y < image.data->h)
					m_data.get()[x*size + y] = pixels[y*w*channels+x*channels];
				else // Fill with 0 to fit the nearest power-of-2 size
					m_data.get()[x*size + y] = 0;
			}
		}

		m_size = size;
		m_buildPending = true;
		std::thread t([this]{
			this->buildMap();
			m_buildFinished = true;
		});
		t.detach();
	}
}

Model3DPtr& Heightmap::getMesh()
{
	return m_mesh;
}

void Heightmap::resetMapLODRecursive(QuadTree<float>* node, int32_t val)
{
	if (!node)
		return;
	node->LOD = val;
	if (node->Type == QuadTree_Leaf)
		return;
	for (auto& c : node->Childs)
		resetMapLODRecursive(c.get(), val);
}
void Heightmap::computeMapLODRecursive(QuadTree<float>* node)
{
	if (!node)
		return;
	static const float sqrt2 = std::sqrt(2);
	node->LOD = -1;
	glm::vec3 camPos(m_camera->getPosition().x);
	glm::vec3 nodePos(-(int32_t) m_size / 2 + node->Position.x, 0, -(int32_t) m_size / 2 + node->Position.y);
	nodePos += m_mesh->getPosition();
	glm::vec3 diff(nodePos - camPos);
	float dist = (diff.x*diff.x + diff.y*diff.y + diff.z*diff.z) / (node->Depth*node->Depth*2);
	glm::vec3 scale = m_mesh->getScale();
	bool forceDraw = dist > (node->Size*scale.x * node->Size*scale.z * scale.y)+10000;
	if (forceDraw) {
		node->LOD = gdepth;
		resetMapLODRecursive(node, gdepth);
	}
	else
	{
		if (node->Type == QuadTree_Leaf)
			return;
		++gdepth;
		for (auto& c : node->Childs)
			computeMapLODRecursive(c.get());
		--gdepth;
	}
}

void Heightmap::buildMapRecursive(QuadTree<float>* node, uint32_t size)
{
	if (!node)
		return;
#if NMS_USING_ADAPTATIVE_QUADTREE_LOD
	bool draw = (node->LOD == gdepth);
#else
	bool draw = false;
#endif
	if (draw || node->Type == QuadTree_Leaf)
	{
		float decal = 0;
		if (size == 1)
			decal = 0.5f;

		uint32_t p[9] {0};
		p[0] = ((uint32_t) (node->Position.x - size / 2.f + decal))*(m_size+1) + (uint32_t) (node->Position.y - size / 2.f + decal);
		p[1] = ((uint32_t) (node->Position.x - size / 2.f + decal))*(m_size+1) + (uint32_t) (node->Position.y + size / 2.f + decal);
		p[2] = ((uint32_t) (node->Position.x + size / 2.f + decal))*(m_size+1) + (uint32_t) (node->Position.y + size / 2.f + decal);
		p[3] = ((uint32_t) (node->Position.x + size / 2.f + decal))*(m_size+1) + (uint32_t) (node->Position.y - size / 2.f + decal);
		p[4] = ((uint32_t) (node->Position.x + decal))*(m_size+1) + (uint32_t) (node->Position.y + decal);

		p[5] = ((uint32_t) (node->Position.x - size / 2.f + decal))*(m_size + 1) + (uint32_t) (node->Position.y - size / 2.f + decal) + size / 2.f;
		p[6] = ((uint32_t) (node->Position.x - size / 2.f + decal + size/2.f))*(m_size + 1) + (uint32_t) (node->Position.y + size / 2.f + decal);

		p[7] = ((uint32_t) (node->Position.x + size / 2.f + decal))*(m_size + 1) + (uint32_t) (node->Position.y - size / 2.f + decal) + size / 2.f;
		p[8] = ((uint32_t) (node->Position.x - size / 2.f + decal + size / 2.f))*(m_size + 1) + (uint32_t) (node->Position.y - size / 2.f + decal);

		//
#if NMS_USING_ADAPTATIVE_QUADTREE_LOD
		if (node->Neighboors[QuadTree_Bottom] && (node->Neighboors[QuadTree_Bottom]->LOD > node->LOD || node->Neighboors[QuadTree_Bottom]->LOD == -1))
		{
			m_tempFaces.push_back(Triangle(p[4], p[6], p[2]));
			m_tempFaces.push_back(Triangle(p[4], p[1], p[6]));
		}
		else
			m_tempFaces.push_back(Triangle(p[4], p[1], p[2]));

		if (node->Neighboors[QuadTree_Right] && (node->Neighboors[QuadTree_Right]->LOD > node->LOD || node->Neighboors[QuadTree_Right]->LOD == -1))
		{
			m_tempFaces.push_back(Triangle(p[4], p[2], p[7]));
			m_tempFaces.push_back(Triangle(p[4], p[7], p[3]));
		}
		else
			m_tempFaces.push_back(Triangle(p[4], p[2], p[3]));
		
		if (node->Neighboors[QuadTree_Top] && (node->Neighboors[QuadTree_Top]->LOD > node->LOD || node->Neighboors[QuadTree_Top]->LOD == -1))
		{
			m_tempFaces.push_back(Triangle(p[4], p[3], p[8]));
			m_tempFaces.push_back(Triangle(p[4], p[8], p[0]));
		}
		else
			m_tempFaces.push_back(Triangle(p[4], p[3], p[0]));
		
		if (node->Neighboors[QuadTree_Left] && (node->Neighboors[QuadTree_Left]->LOD > node->LOD || node->Neighboors[QuadTree_Left]->LOD == -1))
		{
			m_tempFaces.push_back(Triangle(p[4], p[0], p[5]));
			m_tempFaces.push_back(Triangle(p[4], p[5], p[1]));
		}
		else
			m_tempFaces.push_back(Triangle(p[4], p[0], p[1]));
#else
		// Right face
		if (!node->Neighboors[QuadTree_Right] || node->Neighboors[QuadTree_Right]->Type == QuadTree_Leaf)
			m_tempFaces.push_back(Triangle(p[4], p[2], p[3]));
		else
		{
			m_tempFaces.push_back(Triangle(p[4], p[2], p[7]));
			m_tempFaces.push_back(Triangle(p[4], p[7], p[3]));
		}
		
		// Top Face
		if (!node->Neighboors[QuadTree_Top] || node->Neighboors[QuadTree_Top]->Type == QuadTree_Leaf)
			m_tempFaces.push_back(Triangle(p[4], p[3], p[0]));
		else
		{
			// Top Subdiv
			m_tempFaces.push_back(Triangle(p[4], p[3], p[8]));
			m_tempFaces.push_back(Triangle(p[4], p[8], p[0]));
		}
		
		// Left face
		if (!node->Neighboors[QuadTree_Left] || node->Neighboors[QuadTree_Left]->Type == QuadTree_Leaf)
			m_tempFaces.push_back(Triangle(p[4], p[0], p[1]));
		else
		{
			// Left subdiv
			m_tempFaces.push_back(Triangle(p[4], p[0], p[5]));
			m_tempFaces.push_back(Triangle(p[4], p[5], p[1]));
		}

		if (!node->Neighboors[QuadTree_Bottom] || node->Neighboors[QuadTree_Bottom]->Type == QuadTree_Leaf)
			m_tempFaces.push_back(Triangle(p[4], p[1], p[2]));
		else
		{
			m_tempFaces.push_back(Triangle(p[4], p[6], p[2]));
			m_tempFaces.push_back(Triangle(p[4], p[1], p[6]));
		}
#endif
		//m_tempFaces.push_back(Triangle(p[4], p[6], p[2]));
	}
	else
	{
		gdepth++;
		buildMapRecursive(node->Childs[0].get(), size / 2);
		buildMapRecursive(node->Childs[1].get(), size / 2);
		buildMapRecursive(node->Childs[2].get(), size / 2);
		buildMapRecursive(node->Childs[3].get(), size / 2);
		gdepth--;
	}
}

bool Heightmap::getTmpVertex(VertexBuffer& buffer, const glm::vec2& pos, Vertex& vertex, uint32_t& index)
{
	auto& tab = buffer.getVerticesArray();
	int i = 0;
	for (Vertex& v : tab)
	{
		if (FEquals(v.position.x, pos.x) && FEquals(v.position.z, pos.y))
		{
			vertex = v;
			index = i;
			return true;
		}
		++i;
	}
	return false;
}

void Heightmap::update(float elapsed)
{
	m_lastUpdate += elapsed;

	if (m_buildPending)
	{
		if (m_buildFinished)
		{
			Model3DBufferPtr tmpBuffer(std::make_shared<Model3DBuffer>());
			tmpBuffer->loadFromMemory(m_buffer);
			m_mesh->addModelBuffer(tmpBuffer);
			m_buffer.clear();
			//m_mesh->transform.setScale(m_mesh->transform.scale*glm::vec3(m_size, m_size, m_size));
			m_buildPending = false;
		}
		return;
	}
#if NMS_USING_QUADTREE && NMS_USING_ADAPTATIVE_QUADTREE_LOD
	if (m_lastUpdate > NMS_UPDATE_TIME)
	{
		if (m_updatePending && m_updateFinish)
		{
			auto& buffer = m_mesh->getModelBuffer(0)->getVertexBuffer();
			buffer.getTrianglesArray().clear();
			buffer.getTrianglesArray().swap(m_tempFaces);
			m_mesh->getModelBuffer(0)->refreshIndices();
			m_lastUpdate = 0;
			m_updatePending = false;
		}
		else if (!m_updatePending)
		{
			m_updatePending = true;
			m_tempFaces.clear();
			m_updateFinish = false;
			std::thread t([this]{
				resetMapLODRecursive(m_quadtree.get(), -1);
				gdepth = 0;
				computeMapLODRecursive(m_quadtree.get());
				gdepth = 0;
				buildMapRecursive(m_quadtree.get(), m_size);
				m_currentTris = m_tempFaces.size();
				m_updateFinish = true;
			});
			t.detach();
		}
	}
#endif
}

void Heightmap::setCamera(Camera* camera)
{
	m_camera = camera;
}

uint32_t Heightmap::getPolycount() const
{
	return m_currentTris;
}


uint32_t Heightmap::getSize() const
{
	return m_size;
}

void Heightmap::forceRebuild()
{
	m_buildPending = true;
	m_buildFinished = false;
	while (m_mesh->getModelBufferCount())
		m_mesh->removeModelBuffer(0);
	
	std::thread t([this]{
		this->buildMap();
		m_buildFinished = true;
	});
}

void Heightmap::setHeightAt(uint32_t x, uint32_t y, float height)
{
	if (m_data)
		m_data.get()[x*m_size + y] = height;
}

std::shared_ptr<QuadTree<float>>& Heightmap::getQuadtree()
{
	return m_quadtree;
}