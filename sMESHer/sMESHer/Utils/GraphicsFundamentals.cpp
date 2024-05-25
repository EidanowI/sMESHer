#include "GparhicsFundamentals.h"

#ifdef IS_DEBUG
//void GuiMatrix(const DirectX::XMMATRIX& matrix, const std::string& name) noexcept {
//	//std::string name = "ShowMatrix" + std::to_string(rand() * rand());
//	ImGui::Begin(name.c_str());
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 0))).c_str());
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 4))).c_str());//4
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 8))).c_str());//8
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 12))).c_str());//12
//
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 1))).c_str());//1
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 5))).c_str());//5
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 9))).c_str());//9
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 13))).c_str());//13
//
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 2))).c_str());//2
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 6))).c_str());//6
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 10))).c_str());//10
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 14))).c_str());//14
//
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 3))).c_str());//3
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 7))).c_str());//7
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 11))).c_str());//11
//	ImGui::SameLine();
//	ImGui::BulletText(std::to_string((*((float*)&matrix + 15))).c_str());//15
//	ImGui::End();
//}
#endif // IS_DEBUG

float GraphicsFundament::Clamp01(float value) noexcept {
	return Clamp(0.0f, 1.0f, value);
}
float GraphicsFundament::Clamp(float min, float max, float value) noexcept {
	if (value > max) return max;
	if (value < min) return min;
	return value;
}
float GraphicsFundament::Random01() noexcept{
	return (float(rand() % RAND_MAX)) / (float)RAND_MAX;
}
float GraphicsFundament::RandomMinus11() noexcept {
	return ((float(rand() % RAND_MAX)) / (float)RAND_MAX - 0.5f) * 2.0f;
}



GraphicsFundament::Vector2D::Vector2D(float x, float y) noexcept : x(x), y(y) {}
GraphicsFundament::Vector2D::Vector2D(float a) noexcept : Vector2D(a, a) {}
GraphicsFundament::Vector2D::Vector2D() noexcept :Vector2D(0.0f) {}

float GraphicsFundament::Vector2D::Length() const noexcept {
	return sqrt(x * x + y * y);
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::Normalized() const noexcept {
	float length = this->Length();
	return Vector2D(x / length, y / length);
}
void GraphicsFundament::Vector2D::Normalize() noexcept {
	*this = this->Normalized();
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::Clamped(float min, float max) const noexcept {
	return Vector2D(
		GraphicsFundament::Clamp(min, max, x),
		GraphicsFundament::Clamp(min, max, y)
	);
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::Clamped01() const noexcept {
	return this->Clamped(0.0f, 1.0f);
}
void GraphicsFundament::Vector2D::Clamp(float min, float max) noexcept {
	*this = this->Clamped(min, max);
}
void GraphicsFundament::Vector2D::Clamp01() noexcept {
	*this = this->Clamped01();
}

GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator+ (const Vector2D& o) noexcept {
	return Vector2D(x + o.x, y + o.y);
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator- (const Vector2D& o) noexcept {
	return Vector2D(x - o.x, y - o.y);
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator* (const Vector2D& o) noexcept {
	return Vector2D(x * o.x, y * o.y);
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator* (const float a) noexcept {
	return Vector2D(x * a, y * a);
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator/ (const Vector2D& o) noexcept {
	return Vector2D(x / o.x, y / o.y);
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator/ (const float a) noexcept {
	return Vector2D(x / a, y / a);
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator+= (const Vector2D& o) noexcept {
	*this = *this + o;
	return *this;
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator-= (const Vector2D& o) noexcept {
	*this = *this - o;
	return *this;
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator*= (const Vector2D& o) noexcept {
	*this = *this * o;
	return *this;
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator*= (const float a) noexcept {
	*this = *this * a;
	return *this;
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator/= (const Vector2D& o) noexcept {
	*this = *this / o;
	return *this;
}
GraphicsFundament::Vector2D GraphicsFundament::Vector2D::operator/= (const float a) noexcept {
	*this = *this / a;
	return *this;
}

float& GraphicsFundament::Vector2D::r() noexcept 
{ 
	return x; 
}
float& GraphicsFundament::Vector2D::g() noexcept 
{ 
	return y; 
}



GraphicsFundament::Vector3D::Vector3D(float x, float y, float z) noexcept : x(x), y(y), z(z) {}
GraphicsFundament::Vector3D::Vector3D(float a) noexcept : Vector3D(a, a, a) {}
GraphicsFundament::Vector3D::Vector3D() noexcept :Vector3D(0.0f) {}

float GraphicsFundament::Vector3D::Length() const noexcept {
	return sqrt(x * x + y * y + z * z);
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::Normalized() const noexcept {
	float length = this->Length();
	return Vector3D(x / length, y / length, z / length);
}
void GraphicsFundament::Vector3D::Normalize() noexcept {
	*this = this->Normalized();
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::Clamped(float min, float max) const noexcept {
	return Vector3D(
		GraphicsFundament::Clamp(min, max, x),
		GraphicsFundament::Clamp(min, max, y),
		GraphicsFundament::Clamp(min, max, z)
	);
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::Clamped01() const noexcept {
	return this->Clamped(0.0f, 1.0f);
}
void GraphicsFundament::Vector3D::Clamp(float min, float max) noexcept {
	*this = this->Clamped(min, max);
}
void GraphicsFundament::Vector3D::Clamp01() noexcept {
	*this = this->Clamped01();
}

GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator+ (const Vector3D& o) noexcept {
	return Vector3D(x + o.x, y + o.y, z + o.z);
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator- (const Vector3D& o) noexcept {
	return Vector3D(x - o.x, y - o.y, z - o.z);
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator* (const Vector3D& o) noexcept {
	return Vector3D(x * o.x, y * o.y, z * o.z);
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator* (const float a) noexcept {
	return Vector3D(x * a, y * a, z * a);
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator/ (const Vector3D& o) noexcept {
	return Vector3D(x / o.x, y / o.y, z / o.z);
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator/ (const float a) noexcept {
	return Vector3D(x / a, y / a, z / a);
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator+= (const Vector3D& o) noexcept {
	*this = *this + o;
	return *this;
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator-= (const Vector3D& o) noexcept {
	*this = *this - o;
	return *this;
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator*= (const Vector3D& o) noexcept {
	*this = *this * o;
	return *this;
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator*= (const float a) noexcept {
	*this = *this * a;
	return *this;
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator/= (const Vector3D& o) noexcept {
	*this = *this / o;
	return *this;
}
GraphicsFundament::Vector3D GraphicsFundament::Vector3D::operator/= (const float a) noexcept {
	*this = *this / a;
	return *this;
}

float& GraphicsFundament::Vector3D::r() noexcept 
{ 
	return x; 
}
float& GraphicsFundament::Vector3D::g() noexcept 
{ 
	return y; 
}
float& GraphicsFundament::Vector3D::b() noexcept 
{ 
	return z; 
}



GraphicsFundament::Vector4D::Vector4D(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
GraphicsFundament::Vector4D::Vector4D(float a) noexcept : Vector4D(a, a, a, a) {}
GraphicsFundament::Vector4D::Vector4D() noexcept : Vector4D(0.0f) {}

GraphicsFundament::Vector4D GraphicsFundament::Vector4D::one() noexcept {
	return Vector4D(1.0f);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::zero() noexcept {
	return Vector4D(0.0f);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::white() noexcept {
	return Vector4D::one();
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::black() noexcept {
	return Vector4D::zero();
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::red() noexcept {
	return Vector4D(1.0f, 0.0f, 0.0f, 1.0f);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::green() noexcept {
	return Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::blue() noexcept {
	return Vector4D(0.0f, 0.0f, 1.0f, 1.0f);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::yellow() noexcept {
	return Vector4D(1.0f, 1.0f, 0.0f, 1.0f);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::cyan() noexcept {
	return Vector4D(0.0f, 1.0f, 1.0f, 1.0f);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::magenta() noexcept {
	return Vector4D(1.0f, 0.0f, 1.0f, 1.0f);
}

float GraphicsFundament::Vector4D::Length() const noexcept {
	return sqrt(x * x + y * y + z * z + w * w);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::Normalized() const noexcept {
	float length = this->Length();
	return Vector4D(x / length, y / length, z / length, w / length);
}
void GraphicsFundament::Vector4D::Normalize() noexcept {
	*this = this->Normalized();
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::Clamped(float min, float max) const noexcept {
	return Vector4D(
		GraphicsFundament::Clamp(min, max, x),
		GraphicsFundament::Clamp(min, max, y),
		GraphicsFundament::Clamp(min, max, z),
		GraphicsFundament::Clamp(min, max, w)
	);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::Clamped01() const noexcept {
	return this->Clamped(0.0f, 1.0f);
}
void GraphicsFundament::Vector4D::Clamp(float min, float max) noexcept {
	*this = this->Clamped(min, max);
}
void GraphicsFundament::Vector4D::Clamp01() noexcept {
	*this = this->Clamped01();
}

GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator+ (const Vector4D& o) noexcept {
	return Vector4D(x + o.x, y + o.y, z + o.z, w + o.w);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator- (const Vector4D& o) noexcept {
	return Vector4D(x - o.x, y - o.y, z - o.z, w - o.w);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator* (const Vector4D& o) noexcept {
	return Vector4D(x * o.x, y * o.y, z * o.z, w * o.w);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator* (const float a) noexcept {
	return Vector4D(x * a, y * a, z * a, w * a);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator/ (const Vector4D& o) noexcept {
	return Vector4D(x / o.x, y / o.y, z / o.z, w / o.w);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator/ (const float a) noexcept {
	return Vector4D(x / a, y / a, z / a, w / a);
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator+= (const Vector4D& o) noexcept {
	*this = *this + o;
	return *this;
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator-= (const Vector4D& o) noexcept {
	*this = *this - o;
	return *this;
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator*= (const Vector4D& o) noexcept {
	*this = *this * o;
	return *this;
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator*= (const float a) noexcept {
	*this = *this * a;
	return *this;
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator/= (const Vector4D& o) noexcept {
	*this = *this / o;
	return *this;
}
GraphicsFundament::Vector4D GraphicsFundament::Vector4D::operator/= (const float a) noexcept {
	*this = *this / a;
	return *this;
}

float& GraphicsFundament::Vector4D::r() noexcept
{ 
	return x; 
}
float& GraphicsFundament::Vector4D::g() noexcept 
{ 
	return y; 
}
float& GraphicsFundament::Vector4D::b() noexcept 
{ 
	return z; 
}
float& GraphicsFundament::Vector4D::a() noexcept 
{ 
	return w; 
}


DirectX::XMMATRIX GraphicsFundament::MakeTransformMatrix(const Vector3D& position, const Vector3D& rotationRPY, const Vector3D& scale) noexcept {
	return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(rotationRPY.x, rotationRPY.y, rotationRPY.z) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}
DirectX::XMMATRIX GraphicsFundament::MakeNormalMatrix(const DirectX::XMMATRIX& transform) noexcept {
	return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, transform));
}

