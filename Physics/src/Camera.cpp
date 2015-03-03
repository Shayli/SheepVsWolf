#include "Camera.hpp"
#include "InputState.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

Camera::Camera(const glm::vec3& pos) :
	m_view(), m_proj(),
	m_pos(pos),
	m_projDirty(true), m_viewDirty(true),
	m_fov(45),
	m_near(.1f), m_far(10000.f),
	m_width(1), m_height(1),
	m_forward(absoluteForward()), m_left(absoluteLeft()), m_up(absoluteUp()),
	m_rotations(0,0,0), m_target(0,0,0),
	m_moveSpeed(1), m_rotateSpeed(1)
{
	lookAt(pos+m_forward);
}

Camera::~Camera() {
}
	
void Camera::setAspect(float width, float height)
{
	m_width = width;
	m_height = height;
	m_projDirty = true;
}

void Camera::setFrustum(float near, float far){
	m_near = near;
	m_far = far;
	m_projDirty = true;
}

void Camera::setFov(float fov){
	m_fov = fov;
	m_projDirty = true;
}


glm::vec2 Camera::getAspect() const {
	return glm::vec2(m_width, m_height);
}

glm::vec2 Camera::getFrustum() const {
	return glm::vec2(m_near, m_far);
}

void Camera::update(float elapsed) {
	onUpdate(elapsed);

	if(m_projDirty)
		updateProjectionMatrix();
	if(m_viewDirty)
		updateViewMatrix();
}

void Camera::draw() {
	return;
}

void Camera::lookAt(const glm::vec3& pos) {
	glm::vec3 dir = glm::normalize(pos - m_pos);
	m_target = m_pos+dir;
	m_forward = dir;
	m_left = -glm::cross(m_forward, m_up);
	m_viewDirty = true;
	/*m_rotations.x = glm::orientedAngle(m_forward, absoluteForward(), absoluteForward());
	m_rotations.y = glm::orientedAngle(m_forward, absoluteUp(), absoluteUp());
	m_rotations.z = glm::orientedAngle(m_forward, absoluteLeft(), absoluteLeft());*/
}

glm::mat4 Camera::getProjMatrix() const {
	return m_proj;
}

glm::mat4 Camera::getViewMatrix() const {
	return m_view;
}

void Camera::updateProjectionMatrix()
{
	m_proj = glm::perspective(m_fov, m_width/m_height, m_near, m_far);
	//m_proj = glm::ortho(-1, 1, -1, 1);
	m_projDirty = false;
}

void Camera::updateViewMatrix() {
	m_view = glm::lookAt(m_pos, m_target, m_up);
	//m_up = -glm::cross(m_left, m_forward);
	//m_view = glm::translate(glm::mat4(), m_pos);
	m_viewDirty = false;
}

glm::vec3 Camera::forward() {
	return m_forward;
}

glm::vec3 Camera::backward() {
	return -m_forward;
}

glm::vec3 Camera::left() {
	return m_left;
}

glm::vec3 Camera::right() {
	return -m_left;
}

glm::vec3 Camera::up() {
	return m_up;
}

glm::vec3 Camera::down() {
	return -m_up;
}

glm::vec3 Camera::absoluteForward() {
	return glm::vec3(-1,0,0);
}

glm::vec3 Camera::absoluteLeft() {
	return glm::vec3(0,0,1);
}

glm::vec3 Camera::absoluteUp() {
	return glm::vec3(0,1,0);
}

void Camera::onUpdate(float elapsed){
	glm::vec3 mv(0,0,0);

	if(InputState::get().isKeyPressed(SDLK_UP))
		mv += m_forward*elapsed*m_moveSpeed;
	else if(InputState::get().isKeyPressed(SDLK_DOWN))
		mv -= m_forward*elapsed*m_moveSpeed;

	if(InputState::get().isKeyPressed(SDLK_LEFT))
		mv += m_left*elapsed*m_moveSpeed;
	else if(InputState::get().isKeyPressed(SDLK_RIGHT))
		mv -= m_left*elapsed*m_moveSpeed;

	auto mz = InputState::get().getMouseDelta();

	rotate(mz.x*elapsed, mz.y*elapsed);

	move(mv);
}

void Camera::move(const glm::vec3& m) {
	m_pos += m;
	m_target += m;
	m_viewDirty = true;
}

void Camera::rotate(float horizontal, float vertical) {
	m_rotations.y -= horizontal*0.005f*m_rotateSpeed;
	m_rotations.x += vertical*0.005f*m_rotateSpeed;

	if(m_rotations.x < -89.f)
		m_rotations.x = -89.f;
	else if(m_rotations.x > 89.f)
		m_rotations.x = 89.f;

	glm::mat4 rot;
	rot = glm::rotate(rot, m_rotations.y, absoluteUp());
	rot = glm::rotate(rot, m_rotations.x, absoluteLeft());
	auto p = glm::vec3(rot*glm::vec4(absoluteForward(), 1.f));

	lookAt(m_pos+p);
}
