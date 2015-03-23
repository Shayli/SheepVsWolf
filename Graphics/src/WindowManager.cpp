#include <Graphics/WindowManager.hpp>

#include <GL/glew.h>
#include <SDL/SDL_image.h>

#include <iostream>
#include <stdexcept>

WindowManager::WindowManager(uint32_t w, uint32_t h, const char *title) : m_width(w), m_height(h){
    setFramerate(30);

    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("Unable to initialize SDL");
    }

    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w,h,SDL_WINDOW_OPENGL);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GLContext glcontext = SDL_GL_CreateContext(m_window);
    IMG_Init(IMG_INIT_PNG);
    /*
    if(!SDL_SetVideoMode(w, h, 32, SDL_OPENGL)) {
        throw std::runtime_error("Unable to open a window");
    }
    SDL_WM_SetCaption(title, 0);
*/
    glewExperimental = true;
    GLenum error = glewInit();
    if(error != GLEW_OK) {
        throw std::runtime_error("Unable to init GLEW: " + std::string((const char*) glewGetErrorString(error)));
    }
}

WindowManager::~WindowManager() {
    SDL_Quit();
}

float WindowManager::update() {
    SDL_GL_SwapWindow(m_window);

    Uint32 currentTime = SDL_GetTicks();
    Uint32 d = currentTime - m_nStartTime;
    if(d < m_nFrameDuration) {
        SDL_Delay(m_nFrameDuration - d);
    }
    return 0.01f * (SDL_GetTicks() - m_nStartTime);
}