#include "Light.h"

using namespace Renderer;


Light::Light(const Maths::FVector4& pDiffuse, const Maths::FVector4& pAmbient, const Maths::FVector4& pSpecular) : mDiffuse(pDiffuse), mAmbient(pAmbient), mSpecular(pSpecular)
{
}

bool Renderer::Light::operator==(Light& pLight)
{
	return mDiffuse == pLight.mDiffuse && mAmbient == pLight.mAmbient && mSpecular == pLight.mSpecular;
}

DirectionalLight::DirectionalLight(const Maths::FVector3& pDirection, const Maths::FVector4& pDiffuse, const Maths::FVector4& pAmbient, const Maths::FVector4& pSpecular)
	: Light(pDiffuse, pAmbient, pSpecular), mDirection(pDirection)
{
}

bool Renderer::DirectionalLight::operator==(DirectionalLight& pLight)
{
	return mDiffuse == pLight.mDiffuse && mAmbient == pLight.mAmbient && mSpecular == pLight.mSpecular && mDirection == pLight.mDirection;
}

PointLight::PointLight(const Maths::FVector3& pPosition, float pConstant, float pLinear, float pQuadratic, const Maths::FVector4& pDiffuse, const Maths::FVector4& pAmbient, const Maths::FVector4& pSpecular) 
	: Light(pDiffuse, pAmbient, pSpecular), mPosition(pPosition), mConstant(pConstant), mLinear(pLinear), mQuadratic(pQuadratic)
{
}

bool Renderer::PointLight::operator==(PointLight& pLight)
{
	return mDiffuse == pLight.mDiffuse && mAmbient == pLight.mAmbient && mSpecular == pLight.mSpecular && mPosition == pLight.mPosition && mConstant == pLight.mConstant && mLinear == pLight.mLinear && mQuadratic == pLight.mQuadratic;
}

SpotLight::SpotLight(const Maths::FVector3& pPosition, const Maths::FVector3& pDirection, float pCutOff, float pOuterCutOff, float pConstant, float pLinear, float pQuadratic, const Maths::FVector4& pDiffuse, const Maths::FVector4& pAmbient, const Maths::FVector4& pSpecular)
	: Light(pDiffuse, pAmbient, pSpecular), mDirection(pDirection), mPosition(pPosition), mConstant(pConstant), mLinear(pLinear), mQuadratic(pQuadratic), mCutOff(pCutOff), mOuterCutOff(pOuterCutOff)
{

}

bool Renderer::SpotLight::operator==(SpotLight& pLight)
{
	return mDiffuse == pLight.mDiffuse && mAmbient == pLight.mAmbient && mSpecular == pLight.mSpecular && mDirection == pLight.mDirection && mPosition == pLight.mPosition && mConstant == pLight.mConstant && mLinear == pLight.mLinear && mQuadratic == pLight.mQuadratic && mCutOff == pLight.mCutOff && mOuterCutOff == pLight.mOuterCutOff;
}
