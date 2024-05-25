#pragma once

#include <DirectXMath.h>
#include <math.h>
#include <string>


#ifdef IS_DEBUG
void GuiMatrix(const DirectX::XMMATRIX& matrix, const std::string& name) noexcept;
#endif // IS_DEBUG

/*Gets the value by index in the DirectX::XMMATRIX*/
#define MatAt(mat,i) (*((float*)&mat + i))
namespace GraphicsFundament 
{
	float Clamp(float min, float max, float value) noexcept;
	float Clamp01(float value) noexcept;
	float Random01() noexcept;
	float RandomMinus11() noexcept;
	
	class Vector2D {
	public:
		Vector2D(float x, float y) noexcept;
		Vector2D(float a) noexcept;
		Vector2D() noexcept;

		float Length() const noexcept;
		Vector2D Normalized() const noexcept;
		void Normalize() noexcept;
		Vector2D Clamped(float min, float max) const noexcept;
		Vector2D Clamped01() const noexcept;
		void Clamp(float min, float max) noexcept;
		void Clamp01() noexcept;

		Vector2D operator+ (const Vector2D& o) noexcept;
		Vector2D operator- (const Vector2D& o) noexcept;
		Vector2D operator* (const Vector2D& o) noexcept;
		Vector2D operator* (const float a) noexcept;
		Vector2D operator/ (const Vector2D& o) noexcept;
		Vector2D operator/ (const float a) noexcept;
		Vector2D operator+= (const Vector2D& o) noexcept;
		Vector2D operator-= (const Vector2D& o) noexcept;
		Vector2D operator*= (const Vector2D& o) noexcept;
		Vector2D operator*= (const float a) noexcept;
		Vector2D operator/= (const Vector2D& o) noexcept;
		Vector2D operator/= (const float a) noexcept;

		float& r() noexcept;
		float& g() noexcept;

		float x;
		float y;
	};

	class Vector3D 
	{
	public:
		Vector3D(float x, float y, float z) noexcept;
		Vector3D(float a) noexcept;
		Vector3D() noexcept;

		float Length() const noexcept;
		Vector3D Normalized() const noexcept;
		void Normalize() noexcept;
		Vector3D Clamped(float min, float max) const noexcept;
		Vector3D Clamped01() const noexcept;
		void Clamp(float min, float max) noexcept;
		void Clamp01() noexcept;

		Vector3D operator+ (const Vector3D& o) noexcept;
		Vector3D operator- (const Vector3D& o) noexcept;
		Vector3D operator* (const Vector3D& o) noexcept;
		Vector3D operator* (const float a) noexcept;
		Vector3D operator/ (const Vector3D& o) noexcept;
		Vector3D operator/ (const float a) noexcept;
		Vector3D operator+= (const Vector3D& o) noexcept;
		Vector3D operator-= (const Vector3D& o) noexcept;
		Vector3D operator*= (const Vector3D& o) noexcept;
		Vector3D operator*= (const float a) noexcept;
		Vector3D operator/= (const Vector3D& o) noexcept;
		Vector3D operator/= (const float a) noexcept;

		float& r() noexcept;
		float& g() noexcept;
		float& b() noexcept;

		float x;
		float y;
		float z;
	};

	class Vector4D
	{
	public:
		Vector4D(float x, float y, float z, float w) noexcept;
		Vector4D(float a) noexcept;
		Vector4D() noexcept;

		static Vector4D one() noexcept;
		static Vector4D zero() noexcept;
		static Vector4D white() noexcept;
		static Vector4D black() noexcept;
		static Vector4D red() noexcept;
		static Vector4D green() noexcept;
		static Vector4D blue() noexcept;
		static Vector4D yellow() noexcept;
		static Vector4D cyan() noexcept;
		static Vector4D magenta() noexcept;

		float Length() const noexcept;
		Vector4D Normalized() const noexcept;
		void Normalize() noexcept;
		Vector4D Clamped(float min, float max) const noexcept;
		Vector4D Clamped01() const noexcept;
		void Clamp(float min, float max) noexcept;
		void Clamp01() noexcept;

		Vector4D operator +(const Vector4D& o) noexcept;
		Vector4D operator -(const Vector4D& o) noexcept;
		Vector4D operator *(const Vector4D& o) noexcept;
		Vector4D operator *(const float a) noexcept;
		Vector4D operator /(const Vector4D& o) noexcept;
		Vector4D operator /(const float a) noexcept;
		Vector4D operator +=(const Vector4D& o) noexcept;
		Vector4D operator -=(const Vector4D& o) noexcept;
		Vector4D operator *=(const Vector4D& o) noexcept;
		Vector4D operator *=(const float a) noexcept;
		Vector4D operator /=(const Vector4D& o) noexcept;
		Vector4D operator /=(const float a) noexcept;

		float& r() noexcept;
		float& g() noexcept;
		float& b() noexcept;
		float& a() noexcept;

		float x;
		float y;
		float z;
		float w;
	};

	DirectX::XMMATRIX MakeTransformMatrix(const Vector3D& position, const Vector3D& rotationRPY, const Vector3D& scale) noexcept;
	DirectX::XMMATRIX MakeNormalMatrix(const DirectX::XMMATRIX& transform) noexcept;
}



