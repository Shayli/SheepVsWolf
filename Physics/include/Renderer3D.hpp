#pragma once

#include "glm.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include <GL/glew.h>
#include <vector>

class Renderer3D {
private:
    std::vector<Model3Dptr> modelList;
    Camera* m_camera;

    // Ressources OpenGL
    glm::mat4 m_ProjMatrix;
    glm::mat4 m_ViewMatrix;

    GLint m_uMVPMatrix, m_uMVMatrix;
    GLint m_uParticleColor;

public:
    Renderer3D();

    ~Renderer3D();

    Renderer3D(const Renderer3D&) = delete;

    Renderer3D& operator =(const Renderer3D&) = delete;

    void setupViewMatrix(const glm::vec3& eye, const glm::vec3& target);
    void setupProjMatrix(float fov, float aspectRatio, float near, float far);


    void addModel(const Model3Dptr& model);
    void removeModel(const Model3Dptr& model);

    void clear();

    void draw();

    void setProjMatrix(const glm::mat4& P) {
        m_ProjMatrix = P;
    }

    void setViewMatrix(const glm::mat4& V) {
        m_ViewMatrix = V;
    }
    void setCamera(Camera* cam){
        m_camera = cam;
    }

};