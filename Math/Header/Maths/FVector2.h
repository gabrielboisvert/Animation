#pragma once


namespace Maths
{
	struct FVector2
	{
		static const FVector2 One;
		static const FVector2 Zero;

		float x;
		float y;

		
		FVector2(float pX = 0.0f, float pY = 0.0f);

		FVector2(const FVector2& pToCopy);

		FVector2(FVector2&& pToMove) noexcept = default;

		FVector2 operator-() const;

		FVector2 operator=(const FVector2& pOther);

		FVector2 operator+(const FVector2& pOther) const;

		FVector2& operator+=(const FVector2& pOther);

		FVector2 operator-(const FVector2& pOther) const;

		FVector2& operator-=(const FVector2& pOther);

		FVector2 operator*(float pScalar) const;

		FVector2& operator*=(float pScalar);

		FVector2 operator/(float pScalar) const;

		FVector2& operator/=(float pScalar);

		bool operator==(const FVector2& pOther);

		bool operator!=(const FVector2& pOther);

		static FVector2 add(const FVector2& pLeft, const FVector2& pRight);

		static FVector2 substract(const FVector2& pLeft, const FVector2& pRight);

		static FVector2 multiply(const FVector2& pTarget, float pScalar);

		static FVector2 divide(const FVector2& pLeft, float pScalar);

		static float length(const FVector2& pTarget);

		float length();

		static float dot(const FVector2& pLeft, const FVector2& pRight);

		float dot(const FVector2& pRight);

		static FVector2 normalize(const FVector2& pTarget);

		FVector2 normalize();

		static FVector2 lerp(const FVector2& pA, const FVector2& pB, float pT);

		static float angleBetween(const FVector2& pFrom, const FVector2& pTo);
	};
}