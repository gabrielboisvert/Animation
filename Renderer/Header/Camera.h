#pragma once
#include "Maths/FMatrix4.h"

namespace Renderer
{
	enum class CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	class Camera
	{
		public:
			struct MouseHandler
			{
				bool mFirstMouse = true;

				float mLastX = 0;
				float mLastY = 0;

				float mMouseX = 0;
				float mMouseY = 0;
			};

			Maths::FVector3 mPosition;
			float mYaw = -90;
			float mPitch = 0;

			Maths::FVector3 mForward = Maths::FVector3::Forward;
			Maths::FVector3 mRight = Maths::FVector3::Right;
			Maths::FVector3 mUp = Maths::FVector3::Up;
			
			Maths::FMatrix4 mView;
			Maths::FMatrix4 mProjection;
			Maths::FMatrix4 mVp;
			
			float mSpeed = 20;
			float mSpeedMultiplier = 3;
			float mSensitivity = 0.1f;
			bool mIsRun = false;

			MouseHandler mMouse;

			Camera(const float pWidth, const float pHeight, const Maths::FMatrix4& pProjection, const Maths::FVector3& pPosition);

			Maths::FVector3 calculateFront();
			void changeViewPortSize(const Maths::FMatrix4& pProjection);
			void updatePos();
			void updateCameraVectors();
			void processKeyboard(CameraMovement pDirection, float pDeltaTime);
			void setMousePosition(float pMouseX, float pMouseY);
	};
}