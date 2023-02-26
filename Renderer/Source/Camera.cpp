#include "Camera.h"
#include <cmath>
#include <Maths/Utils.h>


Renderer::Camera::Camera(const float pWidth, const float pHeight, const Maths::FMatrix4& pProjection, const Maths::FVector3& pPosition) 
    : mProjection(pProjection),
      mPosition(pPosition)
{
    mMouse.mLastX = pWidth * 0.5f;
    mMouse.mLastY = pHeight * 0.5f;
}

Maths::FVector3 Renderer::Camera::calculateFront()
{
    Maths::FVector3 front;
    front.x = cos(mYaw * (PI / HALF_CIRCLE)) * cos(mPitch * (PI / HALF_CIRCLE));
    front.y = sin(mPitch * (PI / HALF_CIRCLE));
    front.z = sin(mYaw * (PI / HALF_CIRCLE)) * cos(mPitch * (PI / HALF_CIRCLE));
    front = front.normalize();
    return front;
}

void Renderer::Camera::setMousePosition(float pMouseX, float pMouseY)
{
    if (mMouse.mFirstMouse)
    {
        mMouse.mLastX = pMouseX;
        mMouse.mLastY = pMouseY;
        mMouse.mFirstMouse = false;
    }

    float xoffset = pMouseX - mMouse.mLastX;
    float yoffset = mMouse.mLastY - pMouseY;
    mMouse.mLastX = pMouseX;
    mMouse.mLastY = pMouseY;

    xoffset *= mSensitivity;
    yoffset *= mSensitivity;

    mYaw -= xoffset;
    mPitch += yoffset;

    if (mPitch > 90.0f)
        mPitch = 90.0f;
    if (mPitch < -90.0f)
        mPitch = -90.0f;
    
    updateCameraVectors();
}

void Renderer::Camera::changeViewPortSize(const Maths::FMatrix4& pProjection)
{
    mProjection = pProjection;
}

void Renderer::Camera::updateCameraVectors()
{
    Maths::FVector3 front;
    front.x = cos(mYaw * (PI / HALF_CIRCLE)) * cos(mPitch * (PI / HALF_CIRCLE));
    front.y = sin(mPitch * (PI / HALF_CIRCLE));
    front.z = sin(mYaw * (PI / HALF_CIRCLE)) * cos(mPitch * (PI / HALF_CIRCLE));
    front.normalize();
    mForward = front.normalize();

    mRight = mForward.cross(Maths::FVector3::Up).normalize();
    mUp = mRight.cross(mForward).normalize();
}

void Renderer::Camera::processKeyboard(CameraMovement pDirection, float pDeltaTime)
{
    float velocity = mSpeed * (mIsRun? mSpeedMultiplier : 1) * pDeltaTime;
    
    Maths::FVector3 front = calculateFront();

    if (pDirection == CameraMovement::FORWARD)
        mPosition += front * velocity;
    if (pDirection == CameraMovement::BACKWARD)
        mPosition -= front * velocity;
    if (pDirection == CameraMovement::LEFT)
        mPosition += mRight * velocity;
    if (pDirection == CameraMovement::RIGHT)
        mPosition -= mRight * velocity;
}

void Renderer::Camera::updatePos()
{
	mView = Maths::FMatrix4::lookAt(mPosition, mPosition + mForward, mUp);
	mVp = mProjection * mView;
}