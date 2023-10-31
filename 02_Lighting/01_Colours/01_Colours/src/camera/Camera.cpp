#include "Camera.h"

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
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
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