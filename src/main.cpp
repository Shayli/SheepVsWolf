#include <Graphics/glm.hpp>
#include <Graphics/WindowManager.hpp>
#include <Graphics/Renderer/Deferred.hpp>
#include <Graphics/InputState.hpp>
#include <Particle.hpp>

#include <ParticleSystem.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/MeshLoader.hpp>

#include "HookForce.hpp"
#include "BrakeForce.hpp"
#include "ChaseForce.hpp"
#include "Sheep.hpp"
#include "Wolf.hpp"
#include "Behavior/WolfIdleBehavior.hpp"
#include "Behavior/SheepIdleBehavior.hpp"
#include "Entity.hpp"
#include "Map.hpp"

#include <vector>


static const Uint32 WINDOW_WIDTH = 800;
static const Uint32 WINDOW_HEIGHT = 600;



int main() {
    WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Newton was a Geek");
    wm.setFramerate(30);

    srand(0);

    Deferred renderer;
    CameraPtr camera(std::make_shared<Camera>(glm::vec3(10,5,0)));
    camera->setAspect(WINDOW_WIDTH,WINDOW_HEIGHT);
    camera->lookAt(glm::vec3(0,0,0));

    ParticleSystem sys;

    Shader shader;
    shader.loadFromFile("../Shaders/basic.vert", "../Shaders/basic.frag");
    if(!shader.compile()){
        return -1;
    }

    Entity entity;

    Model3DBufferPtr quadBuffer(Model3DBuffer::axis(glm::vec3(0,0,0), glm::vec3(1,1,1)));
    Model3DPtr axis(new Model3D);
    axis->addModelBuffer(quadBuffer);
    axis->setPosition(glm::vec3(0,0,0));
    axis->setShader(&shader);
    renderer.registerModel(axis);
    
    Model3DBufferPtr mapBuffer(new Model3DBuffer);
    VertexBuffer vBuffer;
    vBuffer.addVertex(glm::vec3(-Map::width/2, 0, Map::height/2), glm::vec3(0,1,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));
    vBuffer.addVertex(glm::vec3(Map::width/2, 0, Map::height/2), glm::vec3(0,1,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));
    vBuffer.addVertex(glm::vec3(Map::width/2, 0, -Map::height/2), glm::vec3(0,1,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));
    vBuffer.addVertex(glm::vec3(-Map::width/2, 0, -Map::height/2), glm::vec3(0,1,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));

    vBuffer.addTriangle(0,1,2);
    vBuffer.addTriangle(0,2,3);

    Model3DPtr map(new Model3D);
    mapBuffer->loadFromMemory(vBuffer);
    map->addModelBuffer(mapBuffer);
    map->setPosition(glm::vec3(0,0,0));
    map->setShader(&shader);
    renderer.registerModel(map);

    for(int i = 0; i < 3; ++i){
        std::shared_ptr<Sheep> s(new Sheep);
        s->getModel()->setShader(&shader);
        MeshLoader::load(s->getModel().get(), "../Resources/SHEEP2.3DS");
        
        s->setPosition(glm::vec3(glm::linearRand(-10.f, 10.f),0,glm::linearRand(-10.f, 10.f)));
        s->setRotation(glm::vec3(180,0,0));
        s->setScale(glm::vec3(0.5, 1,0.5));

        renderer.registerModel(s->getModel());
        sys.addParticle(s);
        entity.sheeps.push_back(s);
    }

    
    for(int i = 0; i < 1; ++i){
        std::shared_ptr<Wolf> w(new Wolf);
        w->getModel()->setShader(&shader);
        MeshLoader::load(w->getModel().get(), "../Resources/WOLF.3DS");
        
        w->setPosition(glm::vec3(0,0,0));//glm::vec3(glm::linearRand(-10.f, 10.f),0,glm::linearRand(-10.f, 10.f)));
        w->setRotation(glm::vec3(-90,0,0));
        
        renderer.registerModel(w->getModel());
        sys.addParticle(w);
        entity.wolves.push_back(w);
    }

    /*for(auto s : sheeps){
        ForcePtr f(new ChaseForce(wolves[0], s, 100));
        sys.addForce(f);
    }*/

    WolfIdleBehavior behav(entity);
    entity.wolves[0]->setBehavior(&behav);

    SheepIdleBehavior behav2(entity);
    for(auto& s : entity.sheeps)
        s->setBehavior(&behav2);

    LightPtr directional(std::make_shared<DirectionalLight>());
    directional->position = glm::vec3(-100,200,0);
    directional->color = glm::vec3(1,1,1);
    directional->intensity = 1.f;
    renderer.registerLight(directional);

    /*LightPtr point(std::make_shared<PointLight>());
    point->position = glm::vec3(0,10,0);
    point->color = glm::vec3(1,1,1);
    point->intensity = 10.f;
    renderer.registerLight(point);
    */

    LightPtr point(std::make_shared<SpotLight>(glm::vec3(0,5,1),60.f,70.f));
    point->position = glm::vec3(0,-5,-1);
    point->color = glm::vec3(1,1,1);
    point->intensity = 30.f;
    renderer.registerLight(point);
    

    renderer.setCamera(camera);

    InputState::get().setWindow(&wm);
    InputState::get().update();

    // Temps s'écoulant entre chaque frame
    float dt = 0.0000001f;

	bool done = false;
    while(!done) {
        wm.startMainLoop();

        // Rendu
        renderer.renderFrame();


        // Gestion des evenements
        SDL_Event e;
        while(wm.pollEvent(e)) {
            switch(e.type) {
                case SDL_KEYDOWN:
                    InputState::get().updateKey(e.key.keysym.sym, true);
                    break;
                case SDL_KEYUP:
                    InputState::get().updateKey(e.key.keysym.sym, false);
                    break;
                default:
                    break;
                case SDL_QUIT:
                    done = true;
                    break;
            }
		}

        if(InputState::get().isKeyPressed(SDLK_ESCAPE))
            done = true;

        camera->update(dt);
        /*sys.update(dt);
        for(auto& s : entity.sheeps)
            s->update(dt);
        for(auto& w : entity.wolves)
            w->update(dt);
*/
        for(auto it = entity.sheeps.begin(); it < entity.sheeps.end(); ++it){
            if((*it)->getLife() < 0){
                sys.removeParticle(*it);
                renderer.unregisterModel((*it)->getModel());
                it = entity.sheeps.erase(it);
            }

        }

        InputState::get().update();

        // Mise à jour de la fenêtre
         dt= wm.update();
	}

	return EXIT_SUCCESS;
}