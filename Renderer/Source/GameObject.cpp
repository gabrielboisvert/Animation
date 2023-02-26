#include "GameObject.h"

using namespace Renderer;

float GameObject::factor = 0;

GameObject::GameObject(VKRenderer& pRenderer, Camera& pCamera, Model** pModel, const std::vector<Animation**>& pAnimations, Shader** pShader, Texture** pTexture, const Maths::FVector3& pPosition, const Maths::FVector3& pRotation, const Maths::FVector3& pScale) :
	mCamera(pCamera),
	mRenderer(pRenderer),
	mV(&pCamera.mPosition),
	mVP(&pCamera.mVp),
	mModel(pModel),
	mShader(pShader),
	mTexture(pTexture),
	mLocal(Maths::FMatrix4::createTransformMatrix(pPosition, pRotation, pScale)),
	mGlobal(mLocal),
	mPosition(pPosition),
	mRotation(pRotation),
	mScale(pScale),
	mUniBuffer(mRenderer),
	mAnimations(pAnimations)
{
	mUniBuffer.init(sizeof(UniformBufferObject), VK_SHADER_STAGE_VERTEX_BIT);
	
}

GameObject::~GameObject()
{
	for (std::list<GameObject*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
		delete* it;
	mChilds.clear();

	if (mAnimator != nullptr)
		delete mAnimator;
}

void GameObject::addChild(GameObject& pChild)
{
	if (pChild.mParent == this)
		return;

	if (pChild.mParent != nullptr)
		pChild.mParent->removeChild(pChild);

	pChild.mParent = this;
	mChilds.push_back(&pChild);


	Maths::FVector3 scale = getGlobalScale(*this, Maths::FVector3(1, 1, 1));
	pChild.mScale = Maths::FVector3(pChild.mScale.x / scale.x, pChild.mScale.y / scale.y, pChild.mScale.z / scale.z);
	pChild.mPosition = Maths::FVector3((pChild.mPosition.x - mPosition.x) / scale.x, (pChild.mPosition.y - mPosition.y) / scale.y, (pChild.mPosition.z - mPosition.z) / scale.z);
	pChild.updateLocal();
	updateGlobal();
}

void GameObject::removeChild(GameObject& pChild)
{
	if (pChild.mParent != this)
		return;

	pChild.mParent = nullptr;
	mChilds.remove(&pChild);


	Maths::FVector3 scale = getGlobalScale(*this, Maths::FVector3(1, 1, 1));
	pChild.mScale = Maths::FVector3(pChild.mScale.x * scale.x, pChild.mScale.y * scale.y, pChild.mScale.z * scale.z);
	pChild.mPosition = Maths::FVector3((pChild.mPosition.x * scale.x) + mPosition.x, (pChild.mPosition.y * scale.y) + mPosition.y, (pChild.mPosition.z * scale.z) + mPosition.z);
	pChild.updateLocal();
	updateGlobal();
}

void GameObject::updateGlobal()
{
	if (mParent != nullptr)
		mGlobal = mParent->mGlobal * mLocal;
	else
		mGlobal = mLocal;

	for (std::list<GameObject*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
		(*it)->updateGlobal();
}

void GameObject::updateLocal()
{
	mLocal = Maths::FMatrix4::createTransformMatrix(mPosition, mRotation, mScale);
	updateGlobal();
}

void GameObject::update(float pDeltaTime)
{
	if (pDeltaTime == 0)
		return;

	if (mModel != nullptr && *mModel != nullptr)
	{
		if (mAnimations.size() > 0)
		{
			for (int i = 0; i < mAnimations.size(); i++)
				if (mAnimations[i] != nullptr && *mAnimations[i] != nullptr)
				{
					if (!(*mAnimations[i])->mIsInit)
					{
						(*mAnimations[i])->init();

						if (i == 0)
							mAnimator = new Animator(*mAnimations[0]);
					}

				}
		}

		if (mAnimator != nullptr)
			if (mAnimations[0] != nullptr && *mAnimations[0] != nullptr && mAnimations[1] != nullptr && *mAnimations[1] != nullptr)
				mAnimator->updateCrossFade(*mAnimations[0], *mAnimations[1], pDeltaTime);
	}

	updateGlobal();
}

void GameObject::draw()
{
	for (std::list<GameObject*>::iterator it = mChilds.begin(); it != mChilds.end(); it++)
		(*it)->draw();

	if (mModel == nullptr || *mModel == nullptr)
		return;

	if (mShader == nullptr || *mShader == nullptr)
		return;

	(*mShader)->bind();

	if (mTexture != nullptr && *mTexture != nullptr)
		(*mShader)->setTexture((*mTexture)->mTextureSets[mRenderer.mCurrentFrame]);


	mData.mModel = mGlobal;
	//mData.mInverseModel = lm::mat3(mGlobal).inverse().transpose();
	mData.mVP = (*mVP) * mGlobal;
	mData.mView = (*mV);

	if (mAnimator != nullptr)
	{
		std::vector<Maths::FMatrix4>& transforms = mAnimator->mFinalBoneMatrices;
		for (int i = 0; i < transforms.size(); ++i)
			mData.mFinalBonesMatrices[i] = transforms[i];

		mData.mHasAnimation = true;
	}
	else
		mData.mHasAnimation = false;

	(*mShader)->setMVP(mUniBuffer.mDescriptorSets[mRenderer.mCurrentFrame], mUniBuffer.mUniformBuffersMapped[mRenderer.mCurrentFrame], &mData, sizeof(UniformBufferObject));
	
	(*mModel)->draw();
}

Maths::FVector3 GameObject::getGlobalScale(const GameObject& pObj, Maths::FVector3& pScale)
{
	if (pObj.mParent == nullptr)
		return pScale.scale(pObj.mScale);
	return getGlobalScale(*pObj.mParent, pScale.scale(pObj.mScale));
}

void GameObject::transition(float duration)
{
	if (mAnimations.size() > 1)
		if (mAnimations[0] != nullptr && *mAnimations[0] != nullptr && mAnimations[1] != nullptr && *mAnimations[1] != nullptr)
			mAnimator->crossFade(*mAnimations[0], *mAnimations[1], duration);
}