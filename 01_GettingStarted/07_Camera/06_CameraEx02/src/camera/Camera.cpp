#include "Camera.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
{
	mPosition = position;
	mWorldUp = up;
	mYaw = yaw;
	mPitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::vec3 zAxis = glm::normalize(-mFront);
	glm::vec3 xAxis = glm::normalize(glm::cross(mWorldUp, zAxis));
	glm::vec3 yAxis = glm::cross(zAxis, xAxis);

	glm::mat4 rotation = glm::mat4(glm::vec4(xAxis, 0), glm::vec4(yAxis, 0), glm::vec4(zAxis, 0), glm::vec4(0, 0, 0, 1));
	glm::mat4 translation = glm::mat4(1);
	translation[3][0] = -mPosition.x;
	translation[3][1] = -mPosition.y;
	translation[3][2] = -mPosition.z;

	return glm::transpose(rotation) * translation;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = mMovementSpeed * deltaTime;
	if (direction == FORWARD)
		mPosition += mFront * velocity;
	if (direction == BACKWARD)
		mPosition -= mFront * velocity;
	if (direction == LEFT)
		mPosition -= mRight * velocity;
	if (direction == RIGHT)
		mPosition += mRight * velocity;
}

void Camera::ProcessMouseMovement(float xpos, float ypos, GLboolean constrainPitch)
{
	xpos *= mMouseSensitivity;
	ypos *= mMouseSensitivity;

	mYaw += xpos;
	mPitch += ypos;

	if (constrainPitch)
	{
		if (mPitch > 89.0f)
			mPitch = 89.0f;
		if (mPitch < -89.0f)
			mPitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	mZoom -= (float)yoffset;
	if (mZoom < 1.0f)
		mZoom = 1.0f;
	if (mZoom > 45.0f)
		mZoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));
}