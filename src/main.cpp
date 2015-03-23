#include <Graphics/glm.hpp>
#include <Graphics/WindowManager.hpp>
#include <Graphics/Renderer/Deferred.hpp>
#include <Graphics/InputState.hpp>
#include <Particle.hpp>

#include <ParticleSystem.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/MeshLoader.hpp>
#include <Graphics/Heightmap.hpp>
#include <Graphics/Material.hpp>


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

    Shader heightmapShader;
    heightmapShader.loadFromFile("../Shaders/multitexture.vert", "../Shaders/multitexture.frag");
    if(!heightmapShader.compile()){
        return -1;
    }

    Shader skyShader;
    skyShader.loadFromFile("../Shaders/sky.vert", "../Shaders/sky.frag");
    if(!heightmapShader.compile()){
        return -1;
    }

    Entity entity;

    Model3DBufferPtr quadBuffer(Model3DBuffer::axis(glm::vec3(0,0,0), glm::vec3(1,1,1)));
    Model3DPtr axis(new Model3D);
    axis->addModelBuffer(quadBuffer);
    axis->setPosition(glm::vec3(0,0,0));
    axis->setShader(&shader);
   // renderer.registerModel(axis);
    
    // Model3DBufferPtr mapBuffer(new Model3DBuffer);
    // VertexBuffer vBuffer;
    // vBuffer.addVertex(glm::vec3(-Map::width/2, 0, Map::height/2), glm::vec3(0,1,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));
    // vBuffer.addVertex(glm::vec3(Map::width/2, 0, Map::height/2), glm::vec3(0,1,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));
    // vBuffer.addVertex(glm::vec3(Map::width/2, 0, -Map::height/2), glm::vec3(0,1,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));
    // vBuffer.addVertex(glm::vec3(-Map::width/2, 0, -Map::height/2), glm::vec3(0,1,0), glm::vec2(0,0), glm::vec4(0,0.5,0,1));

    // vBuffer.addTriangle(0,1,2);
    // vBuffer.addTriangle(0,2,3);

    // Model3DPtr map(new Model3D);
    // mapBuffer->loadFromMemory(vBuffer);
    // map->addModelBuffer(mapBuffer);
    // map->setPosition(glm::vec3(0,0,0));
    // map->setShader(&shader);
    Model3DPtr sky(std::make_shared<Model3D>());
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
        sky->addModelBuffer(modBuffer);
    }
    sky->setShader(&skyShader);
    //renderer.registerModel(sky);

    MaterialPtr snow(std::make_shared<Material>());
    snow->loadFromFile("../Resources/snow.bmp");
    MaterialPtr grass(std::make_shared<Material>());
    grass->loadFromFile("../Resources/grass.bmp");
    MaterialPtr dirt(std::make_shared<Material>());
    dirt->loadFromFile("../Resources/dirt.bmp");
    Heightmap heightmap;
    //heightmap.create(32);
    //heightmap.forceRebuild();
    heightmap.loadFromFile("../Resources/field.bmp");
    auto& mesh = heightmap.getMesh();
    mesh->setScale({1,1,1});
    mesh->setShader(&heightmapShader);
    mesh->setMaterial(snow, TextureChannel_0);
    mesh->setMaterial(dirt, TextureChannel_1);
    mesh->setMaterial(grass, TextureChannel_2);
    heightmap.setCamera(camera.get());
    renderer.registerModel(heightmap.getMesh());

    Model3DPtr house(new Model3D);
    house->setShader(&shader);
    house->setScale({0.3,0.3,0.3});
    house->setPosition({25,0,-25});
    house->setRotation({0,-30,0});
    MeshLoader::load(house.get(), "../Resources/house.obj");
    renderer.registerModel(house);

    MeshLoader::MeshParams params;
    params.color = glm::vec4(0,1,0,1);
    params.scale = glm::vec3(0.3,0.3,0.3);

    std::vector<Model3DPtr> trees;
    trees.push_back(std::make_shared<Model3D>());
    MeshLoader::load(trees[0].get(), "../Resources/FIR.3DS", params);
    trees[0]->setShader(&shader);
    renderer.registerModel(trees[0]);
    for(int i = 1; i<13; ++i)
    {
        trees.push_back(std::make_shared<Model3D>());
        trees[0]->copyTo(*trees[i].get());
        renderer.registerModel(trees[i]);
    }

    trees[0]->setPosition({-10,0,10});
    trees[1]->setPosition({-20,0,15});
    trees[2]->setPosition({-12,0,20});
    trees[3]->setPosition({-18,0,3});
    trees[4]->setPosition({-25,0,-7});
    trees[5]->setPosition({10,0,-19});
    trees[6]->setPosition({8,0,25});
    trees[7]->setPosition({22,0,-3});
    trees[8]->setPosition({15,0,22});
    trees[9]->setPosition({3,0, -14});
    trees[10]->setPosition({5,0,3});
    trees[11]->setPosition({9,0,8});
    trees[12]->setPosition({3,0, 14});
    
    

    for(int i = 0; i < 3; ++i){
        std::shared_ptr<Sheep> s(new Sheep);
        s->getModel()->setShader(&shader);
        MeshLoader::load(s->getModel().get(), "../Resources/SHEEP2.3DS");
        
        s->setPosition(glm::vec3(glm::linearRand(-10.f, 10.f),0,glm::linearRand(-10.f, 10.f)));
        s->setRotation(glm::vec3(180,0,0));
        s->setScale(glm::vec3(0.5,1,0.5));

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

   /* LightPtr point(std::make_shared<SpotLight>(glm::vec3(0,5,1),60.f,70.f));
    point->position = glm::vec3(0,-5,-1);
    point->color = glm::vec3(1,1,1);
    point->intensity = 30.f;
    renderer.registerLight(point);*/
    

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
        heightmap.update(dt);
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