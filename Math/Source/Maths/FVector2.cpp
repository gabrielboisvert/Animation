#include <utility>
#include <stdexcept>

#include "Maths/FVector2.h"

using namespace Maths;

const FVector2 FVector2::One(1.0f, 1.0f);
const FVector2 FVector2::Zero(0.0f, 0.0f);

FVector2::FVector2(float pX, float pY) :
	x(pX), y(pY)
{
}

FVector2::FVector2(const FVector2& pToCopy) :
	x(pToCopy.x), y(pToCopy.y)
{
}

FVector2 FVector2::operator-() const
{
	return operator*(-1);
}

FVector2 FVector2::operator=(const FVector2& pOther)
{
	x = pOther.x;
	y = pOther.y;

	return *this;
}

FVector2 FVector2::operator+(const FVector2& pOther) const
{
	return add(*this, pOther);
}

FVector2& FVector2::operator+=(const FVector2& pOther)
{
	*this = add(*this, pOther);
	return *this;
}

FVector2 FVector2::operator-(const FVector2& pOther) const
{
	return substract(*this, pOther);
}

FVector2& FVector2::operator-=(const FVector2& pOther)
{
	*this = substract(*this, pOther);
	return *this;
}

FVector2 FVector2::operator*(float pScalar) const
{
	return multiply(*this, pScalar);
}

FVector2& FVector2::operator*=(float pScalar)
{
	*this = multiply(*this, pScalar);
	return *this;
}

FVector2 FVector2::operator/(float pScalar) const
{
	return divide(*this, pScalar);
}

FVector2& FVector2::operator/=(float pScalar)
{
	*this = divide(*this, pScalar);
	return *this;
}

bool FVector2::operator==(const FVector2 & pOther)
{
	return x == pOther.x && y == pOther.y;
}

bool FVector2::operator!=(const FVector2 & pOther)
{
	return !operator==(pOther);
}

FVector2 FVector2::add(const FVector2 & pLeft, const FVector2 & pRight)
{
	return FVector2(pLeft.x + pRight.x, pLeft.y + pRight.y);
}

FVector2 FVector2::substract(const FVector2 & pLeft, const FVector2 & pRight)
{
	return FVector2(pLeft.x - pRight.x, pLeft.y - pRight.y);
}

FVector2 FVector2::multiply(const FVector2& pTarget, float pScalar)
{
	return FVector2(pTarget.x * pScalar, pTarget.y * pScalar);
}

FVector2 FVector2::divide(const FVector2& pLeft, float pScalar)
{
	FVector2 result(pLeft);

	if (pScalar == 0)
		throw std::logic_error("Division by 0");

	result.x /= pScalar;
	result.y /= pScalar;

	return result;
}

float FVector2::length(const FVector2& pTarget)
{
	return sqrtf(pTarget.x * pTarget.x + pTarget.y * pTarget.y);
}

float Maths::FVector2::length()
{
	return length(*this);
}

float FVector2::dot(const FVector2& pLeft, const FVector2& pRight)
{
	return pLeft.x * pRight.x + pLeft.y * pRight.y;
}

float Maths::FVector2::dot(const FVector2& pRight)
{
	return dot(*this, pRight);
}

FVector2 FVector2::normalize(const FVector2& pTarget)
{
	float aLength = length(pTarget);

	if (aLength > 0.0f)
	{
		float targetLength = 1.0f / aLength;

		return FVector2(pTarget.x * targetLength, pTarget.y * targetLength);
	}

	return FVector2::Zero;
}

FVector2 Maths::FVector2::normalize()
{
	return normalize(*this);
}

FVector2 FVector2::lerp(const FVector2 & p_start, const FVector2 & p_end, float p_alpha)
{
	return (p_start + (p_end - p_start) * p_alpha);
}

float FVector2::angleBetween(const FVector2 & p_from, const FVector2 & p_to)
{
	float lengthProduct = length(p_from) * length(p_to);

	if (lengthProduct > 0.0f)
	{
		float fractionResult = dot(p_from, p_to) / lengthProduct;

		if (fractionResult >= -1.0f && fractionResult <= 1.0f)
			return acosf(fractionResult);
	}

	return 0.0f;
}
