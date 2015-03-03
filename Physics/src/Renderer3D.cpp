#include "Renderer3D.hpp"
#include "GLtools.hpp"


Renderer3D::Renderer3D(){
    // Récuperation des uniforms
    /*m_uParticleColor = glGetUniformLocation(m_SphereProgramID, "uParticleColor");
    m_uMVPMatrix = glGetUniformLocation(m_SphereProgramID, "uMVPMatrix");
    m_uMVMatrix = glGetUniformLocation(m_SphereProgramID, "uMVMatrix");*/

}

Renderer3D::~Renderer3D() {
}

void Renderer3D::setupViewMatrix(const glm::vec3& eye, const glm::vec3& target) {
    m_ViewMatrix = glm::lookAt(eye, target, glm::vec3(0,1,0));
}

void Renderer3D::setupProjMatrix(float fov, float aspectRatio, float near, float far) {
    m_ProjMatrix = glm::perspective(fov, aspectRatio, near, far);
}

void Renderer3D::addModel(const Model3Dptr& model){
    modelList.push_back(model);
}

void Renderer3D::removeModel(const Model3Dptr& model){
    for(auto it = modelList.begin(); it != modelList.end(); ++it){
        if((*it).get() == model.get()){
            modelList.erase(it);
            break;
        }
    }
}

void Renderer3D::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer3D::draw() {
    //glUseProgram(m_SphereProgramID);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    for(auto& model : modelList){
        Shader* shader = model->getShader();
        if(shader) {
            shader->bind();
            shader->send(UniformType_Mat4, "modelMatrix", glm::value_ptr(model->getModelMatrix()));
            if(m_camera){
                shader->send(UniformType_Mat4, "viewMatrix", glm::value_ptr(m_camera->getViewMatrix()));
                shader->send(UniformType_Mat4, "projMatrix", glm::value_ptr(m_camera->getProjMatrix()));
            } 
        }
        model->draw();
        if(shader)
            shader->unbind();
    }

    //glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    /* Dessine chacune des particules
    for(uint32_t i = 0; i < count; ++i) {
        auto modelMatrix = glm::scale(glm::translate(glm::mat4(1.f), positionArray[i]), glm::vec3(massScale * massArray[i]));
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(m_ProjMatrix * m_ViewMatrix * modelMatrix));
        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(m_ViewMatrix * modelMatrix));

        glUniform3fv(m_uParticleColor, 1, glm::value_ptr(colorArray[i]));
        glDrawArrays(GL_TRIANGLES, 0, m_nSphereVertexCount);
    }*/

}


