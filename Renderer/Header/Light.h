#pragma once

#include "Maths/FVector4.h"
#include "Maths/FVector3.h"
#include "Maths/Utils.h"
#include "cmath"

namespace Renderer
{
	struct Light
	{
		__declspec(align(16))Maths::FVector4 mDiffuse;
		__declspec(align(16))Maths::FVector4 mAmbient;
		__declspec(align(16))Maths::FVector4 mSpecular;

		Light(const Maths::FVector4& mDiffuse = Maths::FVector4(1, 1, 1, 0.8f), const Maths::FVector4& mAmbient = Maths::FVector4(1, 1, 1, 0.05f), const Maths::FVector4& mSpecular = Maths::FVector4(1, 1, 1, 1));
		bool operator==(Light& light);
	};


	struct DirectionalLight : public Light
	{
		__declspec(align(16)) Maths::FVector3 mDirection;

		DirectionalLight(const Maths::FVector3& pDirection = Maths::FVector3(0, -1, 0), const Maths::FVector4& pDiffuse = Maths::FVector4(1, 1, 1, 0.8), const Maths::FVector4& pAmbient = Maths::FVector4(1, 1, 1, 0.05f), const Maths::FVector4& pSpecular = Maths::FVector4(1, 1, 1, 1));
		bool operator==(DirectionalLight& pLight);
	};

	struct PointLight : public Light
	{
		__declspec(align(4))float mConstant;
		__declspec(align(4))float mLinear;
		__declspec(align(4))float mQuadratic;
		__declspec(align(16)) Maths::FVector3 mPosition;
		

		PointLight(const Maths::FVector3& pPosition = Maths::FVector3(0, 0, 0), float pConstant = 1.0f, float pLinear = 1.0f, float pQuadratic = 0.032f, const Maths::FVector4& pDiffuse = Maths::FVector4(1, 1, 1, 1), const Maths::FVector4& pAmbient = Maths::FVector4(1, 1, 1, 0.05f), const Maths::FVector4& pSpecular = Maths::FVector4(1, 1, 1, 0));
		bool operator==(PointLight& pLight);
	};

	struct SpotLight : public Light
	{
		__declspec(align(4))float mConstant;
		__declspec(align(4))float mLinear;
		__declspec(align(4))float mQuadratic;
		__declspec(align(4))float mCutOff;
		__declspec(align(4))float mOuterCutOff;
		__declspec(align(16))Maths::FVector3 mDirection;
		__declspec(align(16))Maths::FVector3 mPosition;

		SpotLight(const Maths::FVector3& pPosition = Maths::FVector3(5, 0, 0), const Maths::FVector3& pDirection = Maths::FVector3(0, 0, 1), float pCutOff = std::cos(Maths::degreesToRadians(12.5f)), float pOuterCutOff = std::cos(Maths::degreesToRadians(17.5f)), float pConstant = 1.0f, float pLinear = 0.09f, float pQuadratic = 0.032f, const Maths::FVector4& pDiffuse = Maths::FVector4(1, 1, 1, 1), const Maths::FVector4& pAmbient = Maths::FVector4(1, 1, 1, 0), const Maths::FVector4& pSpecular = Maths::FVector4(1, 1, 1, 1));
		bool operator==(SpotLight& pLight);
	};
}