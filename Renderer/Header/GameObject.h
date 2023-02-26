#pragma once
#include "Camera.h"
#include "Animator.h"
#include "UniformBuffer.h"
#include "Animation.h"

namespace Renderer
{
	struct UniformBufferObject {
		__declspec(align(16)) Maths::FMatrix4 mModel;
		__declspec(align(16)) Maths::FMatrix3 mInverseModel;
		__declspec(align(16)) Maths::FMatrix4 mVP;
		__declspec(align(16)) Maths::FVector3 mView;
		__declspec(align(16)) Maths::FMatrix4 mFinalBonesMatrices[MAX_BONE];
		__declspec(align(16)) bool mHasAnimation;
	};

	class GameObject
	{
	public:
		static float factor;

		Camera& mCamera;
		VKRenderer& mRenderer;
		UniformBuffer mUniBuffer;
		UniformBufferObject mData{};

		Maths::FMatrix4 mLocal = Maths::FMatrix4::Identity;
		Maths::FMatrix4 mGlobal = Maths::FMatrix4::Identity;
		Maths::FVector3* mV = nullptr;
		Maths::FMatrix4* mVP = nullptr;


		Model** mModel = nullptr;
		Shader** mShader = nullptr;
		Texture** mTexture = nullptr;

		Maths::FVector3 mRight = Maths::FVector3::Right;
		Maths::FVector3 mForward = Maths::FVector3::Forward;
		Maths::FVector3 mUp = Maths::FVector3::Up;

		Maths::FVector3 mPosition = Maths::FVector3::Zero;
		Maths::FVector3 mRotation = Maths::FVector3::Zero;
		Maths::FVector3 mScale = Maths::FVector3::One;

		GameObject* mParent = nullptr;
		std::list<GameObject*> mChilds;

		Animator* mAnimator = nullptr;
		std::vector<Animation**> mAnimations;

		GameObject(VKRenderer& pRenderer, Camera& pCamera, Model** pModel, const std::vector<Animation**>& pAnimations, Shader** pShader, Texture** pTexture, const Maths::FVector3& pPosition, const Maths::FVector3& pRotation, const Maths::FVector3& pScale);
		~GameObject();

		void addChild(GameObject& pChild);
		void removeChild(GameObject& pChild);

		void updateGlobal();
		void updateLocal();
		void update(float pDeltaTime);
		void draw();

		Maths::FVector3 GameObject::getGlobalScale(const GameObject& pObj, Maths::FVector3& pScale);

		void transition(float duration);
	};
}