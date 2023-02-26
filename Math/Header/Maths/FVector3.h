#pragma once


namespace Maths
{
	struct FVector3
	{
		static const FVector3 One;
		static const FVector3 Zero;
		static const FVector3 Forward;
		static const FVector3 Backward;
		static const FVector3 Right;
		static const FVector3 Left;
		static const FVector3 Up;
		static const FVector3 Bottom;

		float x;
		float y;
		float z;

		FVector3(float pX = 0.0f, float pY = 0.0f, float pZ = 0.0f);

		FVector3(const FVector3& pToCopy);

		FVector3(FVector3&& pToMove) noexcept = default;

		FVector3 operator-() const;

		FVector3 operator=(const FVector3& pOther);

		FVector3 operator+(const FVector3& pOther) const;

		FVector3& operator+=(const FVector3& pOther);

		FVector3 operator-(const FVector3& pOther) const;

		FVector3& operator-=(const FVector3& pOther);

		FVector3 operator*(const FVector3& pOther) const;

		FVector3 operator*=(const FVector3& pOther);

		FVector3 operator*(float pScalar) const;

		FVector3& operator*=(float pScalar);

		FVector3 operator/(float pScalar) const;

		FVector3& operator/=(float pScalar);

		bool operator==(const FVector3& pOther);

		bool operator!=(const FVector3& pOther);

		float& operator[](int pIdx);

		float operator[](int pIdx) const;

		static FVector3 add(const FVector3& pLeft, const FVector3& pRight);

		static FVector3 substract(const FVector3& pLeft, const FVector3& pRight);

		static FVector3 multiply(const FVector3& pTarget, float pScalar);

		static FVector3 scale(const FVector3& pLeft, const FVector3& pRight);

		FVector3 scale(const FVector3& pRight);

		static FVector3 divide(const FVector3& pLeft, float pScalar);

		static float length(const FVector3& pTarget);

		float length();

		static float dot(const FVector3& pLeft, const FVector3& pRight);
		
		float dot(const FVector3& pRight);

		static float distance(const FVector3& pLeft, const FVector3& pRight);
		
		float distance(const FVector3& pRight);

		static FVector3 cross(const FVector3& pLeft, const FVector3& pRight);

		FVector3 cross(const FVector3& pRight);

		static FVector3 normalize(const FVector3& pTarget);

		FVector3 normalize();

		static FVector3 lerp(const FVector3& pA, const FVector3& pB, float pT);

		static float angleBetween(const FVector3& pFrom, const FVector3& pTo);
	};
}