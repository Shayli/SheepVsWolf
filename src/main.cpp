#include <glm.hpp>
#include <WindowManager.hpp>
#include <Renderer3D.hpp>
#include <InputState.hpp>
#include <Particle.hpp>

#include <ParticleSystem.hpp>
#include <Shader.hpp>
#include <MeshLoader.hpp>

#include "HookForce.hpp"
#include "BrakeForce.hpp"
#include "ChaseForce.hpp"
#include "MapForce.hpp"
#include "Sheep.hpp"
#include "Wolf.hpp"
#include "Behavior/WolfIdleBehavior.hpp"
#include "Behavior/SheepIdleBehavior.hpp"
#include "Entity.hpp"
#include "Map.hpp"

#include <vector>


static const Uint32 WINDOW_WIDTH = 1024;
static const Uint32 WINDOW_HEIGHT = 1024;



int main() {
    WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Newton was a Geek");
    wm.setFramerate(30);

    srand(0);

    Renderer3D renderer;
    Camera camera(glm::vec3(100,50,0));
    camera.setAspect(WINDOW_WIDTH,WINDOW_HEIGHT);
    camera.lookAt(glm::vec3(0,0,0));

    ParticleSystem sys;

    Shader shader;
    shader.loadFromFile("../Shaders/basic.vert", "../Shaders/basic.frag");
    if(!shader.compile()){
        return -1;
    }

    Entity entity;

    Model3DBufferPtr quadBuffer(Model3DBuffer::axis(glm::vec3(0,0,0), glm::vec3(1,1,1)));
    Model3Dptr axis(new Model3D);
    axis->addModelBuffer(quadBuffer);
    axis->setPosition(glm::vec3(0,0,0));
    axis->setShader(&shader);
    renderer.addModel(axis);
    
    Model3DBufferPtr mapBuffer(new Model3DBuffer);
    VertexBuffer vBuffer;
    vBuffer.addVertex(glm::vec3(-Map::width/2, 0, Map::height/2), glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));
    vBuffer.addVertex(glm::vec3(Map::width/2, 0, Map::height/2), glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));
    vBuffer.addVertex(glm::vec3(Map::width/2, 0, -Map::height/2), glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));
    vBuffer.addVertex(glm::vec3(-Map::width/2, 0, -Map::height/2), glm::vec3(0,0,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));

    vBuffer.addTriangle(0,1,2);
    vBuffer.addTriangle(3,2,0);

    Model3Dptr map(new Model3D);
    mapBuffer->loadFromMemory(vBuffer);
    map->addModelBuffer(mapBuffer);
    map->setPosition(glm::vec3(0,0,0));
    map->setShader(&shader);
    renderer.addModel(map);

    for(int i = 0; i < 3; ++i){
        std::shared_ptr<Sheep> s(new Sheep);
        s->getModel()->setShader(&shader);
        MeshLoader::load(s->getModel().get(), "../Resources/SHEEP2.3DS");
        
        s->setPosition(glm::vec3(glm::linearRand(-10.f, 10.f),0,glm::linearRand(-10.f, 10.f)));
        s->setRotation(glm::vec3(180,0,0));
        s->setScale(glm::vec3(0.5, 1,0.5));

        renderer.addModel(s->getModel());
        sys.addParticle(s);
        entity.sheeps.push_back(s);
    }

    
    for(int i = 0; i < 1; ++i){
        std::shared_ptr<Wolf> w(new Wolf);
        w->getModel()->setShader(&shader);
        MeshLoader::load(w->getModel().get(), "../Resources/WOLF.3DS");
        
        w->setPosition(glm::vec3(glm::linearRand(-10.f, 10.f),0,glm::linearRand(-10.f, 10.f)));
        w->setRotation(glm::vec3(-90,0,0));
        
        renderer.addModel(w->getModel());
        sys.addParticle(w);
        entity.wolves.push_back(w);
    }

    for(auto s : entity.sheeps){
        ForcePtr f(new MapForce(s));
        sys.addForce(f);
    }

    for(auto w : entity.wolves){
        ForcePtr f(new MapForce(w));
        sys.addForce(f);
    }

    WolfIdleBehavior behav(entity);
    entity.wolves[0]->setBehavior(&behav);

    SheepIdleBehavior behav2(entity);
    for(auto& s : entity.sheeps)
        s->setBehavior(&behav2);


    renderer.setCamera(&camera);

    InputState::get().setWindow(&wm);
    InputState::get().update();

    // Temps s'écoulant entre chaque frame
    float dt = 0.0000001f;

	bool done = false;
    while(!done) {
        wm.startMainLoop();

        // Rendu
        renderer.clear();
        renderer.draw();


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

        camera.update(dt);
        sys.update(dt);
        for(auto& s : entity.sheeps)
            s->update(dt);
        for(auto& w : entity.wolves)
            w->update(dt);

        for(auto it = entity.sheeps.begin(); it < entity.sheeps.end(); ++it){
            if((*it)->getLife() < 0){
                sys.removeParticle(*it);
                renderer.removeModel((*it)->getModel());
                it = entity.sheeps.erase(it);
            }

        }

        InputState::get().update();

        // Mise à jour de la fenêtre
         dt= wm.update();
	}

	return EXIT_SUCCESS;
}