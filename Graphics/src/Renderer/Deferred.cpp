#include <Graphics/Renderer/Deferred.hpp>

static const uint32_t SHADOW_RES = 1024;

Deferred::Deferred() : width(1), height(1)
{
	Model3DBufferPtr modBuffer(std::make_shared<Model3DBuffer>());
	VertexBuffer buffer;
	buffer.addVertex(glm::vec3(-1, -1, 0), glm::vec3(0,0,0), glm::vec2(0,0));
	buffer.addVertex(glm::vec3(-1, 1, 0), glm::vec3(0,0,0), glm::vec2(0,1));
	buffer.addVertex(glm::vec3(1, 1, 0), glm::vec3(0,0,0), glm::vec2(1,1));
	buffer.addVertex(glm::vec3(1, -1, 0), glm::vec3(0,0,0), glm::vec2(1,0));
	buffer.addTriangle(0,1,2);
	buffer.addTriangle(0,3,2);
	modBuffer->loadFromMemory(buffer);
	screenQuad.addModelBuffer(modBuffer);

	blit.loadFromFile("../Shaders/blit.vert", "../Shaders/blit.frag");
	blit.compile();
    compose.loadFromFile("../Shaders/blit.vert", "../Shaders/compose.frag");
    compose.compile();
    directional.loadFromFile("../Shaders/directional.vert", "../Shaders/directional.frag");
    directional.compile();
    point.loadFromFile("../Shaders/point.vert", "../Shaders/point.frag");
    point.compile();
    spot.loadFromFile("../Shaders/spot.vert", "../Shaders/spot.frag");
    spot.compile();
    sobel.loadFromFile("../Shaders/sobel.vert", "../Shaders/sobel.frag");
    sobel.compile();
    shadow.loadFromFile("../Shaders/shadow.vert", "../Shaders/shadow.frag");
    shadow.compile();
} 

void Deferred::renderFrame()
{
	geometryPass();
	//shadowPass();
	lightPass();
    postFXPass();

	{
        // Passe finale
        blit.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, postFxBuffer.getChannel(1));
        blit.send(UniformType_Integer, "TextureChannel0", 0);
        glViewport( 0, 0, width, height);
        screenQuad.draw();

        // Debug
		blit.bind();
		glActiveTexture(GL_TEXTURE0);
        float subSize = 9.f;
        int subCount = 0;
        glBindTexture(GL_TEXTURE_2D, frameBuffer.getChannel(FBOChannel_Position));
        glViewport( 0, 0, width/subSize, height/subSize);
        screenQuad.draw();

        glViewport( 1*width/subSize, 0, width/subSize, height/subSize);
        glBindTexture(GL_TEXTURE_2D, frameBuffer.getChannel(FBOChannel_Normal));
        screenQuad.draw();

        glViewport( 2*width/subSize, 0, width/subSize, height/subSize );
        glBindTexture(GL_TEXTURE_2D, frameBuffer.getChannel(FBOChannel_Depth));
        screenQuad.draw();

        glViewport( 3*width/subSize, 0, width/subSize, height/subSize );
        glBindTexture(GL_TEXTURE_2D, lightBuffer.getChannel());
        screenQuad.draw();

        glBindTexture(GL_TEXTURE_2D, shadowBuffer.getChannel());
        glViewport(4*width/subSize, 0, width/subSize, width/subSize);
        screenQuad.draw();

        glBindTexture(GL_TEXTURE_2D, postFxBuffer.getChannel(0));
        glViewport(5*width/subSize, 0, width/subSize, height/subSize);
        screenQuad.draw();

        glBindTexture(GL_TEXTURE_2D, postFxBuffer.getChannel(1));
        glViewport(6*width/subSize, 0, width/subSize, height/subSize);
        screenQuad.draw();
        glBindTexture(GL_TEXTURE_2D, postFxBuffer.getChannel(2));
        glViewport(7*width/subSize, 0, width/subSize, height/subSize);
        screenQuad.draw();

        glBindTexture(GL_TEXTURE_2D, postFxBuffer.getChannel(3));
        glViewport(8*width/subSize, 0, width/subSize, height/subSize);
        screenQuad.draw();
        blit.unbind();
    }
}

void Deferred::onCameraChanged()
{
	if(m_camera)
	{
		auto aspect = m_camera->getAspect();
        width = aspect.x; height = aspect.y;
        std::cout << "aspect: " << width << "*" << height << std::endl;
		frameBuffer.initialize(aspect.x, aspect.y);
		lightBuffer.initialize(aspect.x, aspect.y);
		postFxBuffer.initialize(aspect.x, aspect.y);
        shadowBuffer.initialize(SHADOW_RES,SHADOW_RES); // inits 1024*1024 shadow texture
	}
}

void Deferred::geometryPass()
{
	frameBuffer.bind();
    // Default states
    glViewport(0,0,width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Get camera matrices
    glm::mat4 projection = m_camera->getProjMatrix();
    glm::mat4 worldToView = m_camera->getViewMatrix();
    //glm::mat4 worldToView = glm::lookAt(glm::vec3(10,10,10), glm::vec3(0,0,0), glm::vec3(0,1,0));
    //glm::mat4 projection = glm::perspective(90.f, 800/600.f, 0.01f, 1000.f);
    
    Shader* current = nullptr;
    for(auto& m :m_meshs)
    {
    	glm::mat4 objectToWorld = m->getModelMatrix();
        glm::mat4 mv = worldToView * objectToWorld;
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(mv));
    	glm::mat4 mvp = projection * mv;

    	if(current != m->getShader())
    	{
    		current = m->getShader();
    		if(!current)
    			continue;
            current->bind();
			current->send(UniformType_Mat4, "projMatrix", glm::value_ptr(projection));
    		current->send(UniformType_Mat4, "viewMatrix", glm::value_ptr(worldToView));
    	}
    	current->send(UniformType_Mat4, "modelMatrix", glm::value_ptr(objectToWorld));
        current->send(UniformType_Mat4, "normalMatrix", glm::value_ptr(normalMatrix));
    	current->send(UniformType_Mat4, "mvpMatrix", glm::value_ptr(mvp));

    	m->draw();
    	//std::cout << mvp << std::endl;
    }
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    frameBuffer.unbind();
}

void Deferred::shadowPass()
{
    if(m_lights[LightType_Spot].empty())
        return;

    auto& light = m_lights[LightType_Spot][0];
    glm::mat4 projection = glm::perspective(glm::radians(70*2.f), 1.f, 1.f, 1000.f);
    glm::mat4 worldToLight = glm::lookAt(light->position, glm::vec3(0,0,0), glm::vec3(0,1,0));

    shadowBuffer.bind();
    glViewport(0,0,SHADOW_RES, SHADOW_RES);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    shadow.bind();
    shadow.send(UniformType_Mat4, "projMatrix", glm::value_ptr(projection));
    shadow.send(UniformType_Mat4, "viewMatrix", glm::value_ptr(worldToLight));
    for(auto& m :m_meshs)
    {
        glm::mat4 objectToWorld = m->getModelMatrix();
        glm::mat4 mv = worldToLight * objectToWorld;
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(mv));
        glm::mat4 mvp = projection * mv;

        shadow.send(UniformType_Mat4, "modelMatrix", glm::value_ptr(objectToWorld));
        shadow.send(UniformType_Mat4, "normalMatrix", glm::value_ptr(normalMatrix));
        shadow.send(UniformType_Mat4, "mvpMatrix", glm::value_ptr(mvp));

        m->draw();
        //std::cout << mvp << std::endl;
    }
    shadow.unbind();
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    shadowBuffer.unbind();
}

void Deferred::lightPass()
{
	lightBuffer.bind();
    glViewport(0,0,width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frameBuffer.getChannel(FBOChannel_Normal));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, frameBuffer.getChannel(FBOChannel_Position));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, frameBuffer.getChannel(FBOChannel_Depth));
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowBuffer.getChannel());

    glm::mat4 projection = m_camera->getProjMatrix();
    glm::mat4 worldToView = m_camera->getViewMatrix();
    glm::mat4 screenToWorld = glm::inverse(projection*worldToView);
    glm::vec3 camPos = m_camera->getPosition();

    directional.bind();
    directional.send(UniformType_Integer, "TextureChannel0", 0);
    directional.send(UniformType_Integer, "TextureChannel1", 1);
    directional.send(UniformType_Integer, "TextureChannel2", 2);
    directional.send(UniformType_Integer, "TextureChannel3", 3);
    for(auto& m: m_lights[LightType_Directional])
    {
        glm::mat4 worldToLight = glm::lookAt(m->position, glm::vec3(0,0,0), glm::vec3(0,1,0));
        glm::mat4 worldToLightScreen = projection * worldToLight;
        directional.send(UniformType_Mat4, "WorldToLightScreen", glm::value_ptr(worldToLightScreen));
        directional.send(UniformType_Mat4, "ScreenToWorld", glm::value_ptr(screenToWorld));
        directional.send(UniformType_Vec3, "Light.Direction", glm::value_ptr(m->position));
        directional.send(UniformType_Vec3, "Light.Color", glm::value_ptr(m->color));
        directional.send(UniformType_Float, "Light.Intensity", m->intensity);
        screenQuad.draw();
    }
    directional.unbind();
    point.bind();
    point.send(UniformType_Integer, "TextureChannel0", 0);
    point.send(UniformType_Integer, "TextureChannel1", 1);
    for(auto& m: m_lights[LightType_Point])
    {
        point.send(UniformType_Mat4, "ScreenToWorld", glm::value_ptr(screenToWorld));
        
        point.send(UniformType_Vec3, "Light.Position", glm::value_ptr(m->position));
        point.send(UniformType_Vec3, "Light.Color", glm::value_ptr(m->color));
        point.send(UniformType_Float, "Light.Intensity", m->intensity);
        screenQuad.draw();
    }
    point.unbind();
    spot.bind();
    spot.send(UniformType_Integer, "TextureChannel0", 0);
    spot.send(UniformType_Integer, "TextureChannel1", 1);
    spot.send(UniformType_Integer, "TextureChannel2", 2);
    spot.send(UniformType_Vec3, "Eye", glm::value_ptr(camPos));
    for(auto& m: m_lights[LightType_Spot])
    {
        SpotLight* s = static_cast<SpotLight*>(m.get());
        spot.send(UniformType_Mat4, "ScreenToWorld", glm::value_ptr(screenToWorld));
        
        spot.send(UniformType_Vec3, "Position", glm::value_ptr(s->position));
        spot.send(UniformType_Vec3, "Color", glm::value_ptr(s->color));
        spot.send(UniformType_Vec3, "Direction", glm::value_ptr(s->direction));
        spot.send(UniformType_Float, "Intensity", s->intensity);
        spot.send(UniformType_Float, "Angle", s->angle);
        spot.send(UniformType_Float, "Light.Falloff", s->falloff);
        screenQuad.draw();
    }
    spot.unbind();
    glDisable(GL_BLEND);

    lightBuffer.unbind();
}

void Deferred::postFXPass()
{
    // Disable depth test
    glDisable(GL_DEPTH_TEST);

    postFxBuffer.bind();
    postFxBuffer.attachChannel(0);
    postFxBuffer.clear();
    // Passe d'éclairage
    compose.bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frameBuffer.getChannel(FBOChannel_Albedo));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, lightBuffer.getChannel());
    compose.send(UniformType_Integer, "TextureChannel0", 0);
    compose.send(UniformType_Integer, "TextureChannel1", 1);
    glViewport( 0, 0, width, height);
    screenQuad.draw();


    postFxBuffer.attachChannel(1);
    postFxBuffer.clear();
    

    sobel.bind();
    sobel.send(UniformType_Integer, "TextureChannel0", 0);
    sobel.send(UniformType_Integer, "TextureChannel1", 1);
    sobel.send(UniformType_Float, "Factor", .6f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, postFxBuffer.getChannel(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, frameBuffer.getChannel(FBOChannel_Normal));
    screenQuad.draw();
    sobel.unbind();
    
    postFxBuffer.attachChannel(2);
    postFxBuffer.clear();
    postFxBuffer.attachChannel(3);
    postFxBuffer.clear();
    postFxBuffer.unbind();
}