#pragma once
#include <atomic>
#include <mutex>
#include <d3d9.h>
#include <dwmapi.h>
#include <xmmintrin.h>
#include <array>
#include <vector>
#include <cstdlib>
#include <random>
#include <direct.h>
#include <fstream>
#include <string>
#include <sstream>
#include <numbers>

#include "../render/ImGui/imgui.h"
#include "../render/ImGui/imgui_impl_dx11.h"
#include "../render/ImGui/imgui_impl_win32.h"
#include "../cheat/driver/driver.hpp"

inline int Width = GetSystemMetrics(SM_CXSCREEN);
inline int Height = GetSystemMetrics(SM_CYSCREEN);

class Vector2 {
public:
	Vector2() : x(0.f), y(0.f) {}
	Vector2(double _x, double _y) : x(_x), y(_y) {}
	~Vector2() {}

	double x;
	double y;
};

class Vector3 {
public:
	Vector3() : x(0.f), y(0.f), z(0.f) {}

	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	~Vector3() {}

	double x;
	double y;
	double z;

	inline double Dot(Vector3 v) {
		return x * v.x + y * v.y + z * v.z;
	}

	inline double Distance(Vector3 v) {
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline double Length() {
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 operator+(Vector3 v) {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v) {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(double flNum) { return Vector3(x * flNum, y * flNum, z * flNum); }
};

struct FPlane : Vector3 {
	double W;

	FPlane() : W(0) {}
	FPlane(double W) : W(W) {}
};

class FMatrix {
public:
	double m[4][4];
	FPlane XPlane, YPlane, ZPlane, WPlane;

	FMatrix() : XPlane(), YPlane(), ZPlane(), WPlane() {}
	FMatrix(FPlane XPlane, FPlane YPlane, FPlane ZPlane, FPlane WPlane)
		: XPlane(XPlane), YPlane(YPlane), ZPlane(ZPlane), WPlane(WPlane) {
	}

	D3DMATRIX ToD3DMATRIX() const {
		D3DMATRIX Result;
		Result._11 = XPlane.x; Result._12 = XPlane.y; Result._13 = XPlane.z; Result._14 = XPlane.W;
		Result._21 = YPlane.x; Result._22 = YPlane.y; Result._23 = YPlane.z; Result._24 = YPlane.W;
		Result._31 = ZPlane.x; Result._32 = ZPlane.y; Result._33 = ZPlane.z; Result._34 = ZPlane.W;
		Result._41 = WPlane.x; Result._42 = WPlane.y; Result._43 = WPlane.z; Result._44 = WPlane.W;
		return Result;
	}
};

struct FQuat {
	double x;
	double y;
	double z;
	double w;
};

struct FTransform final {
public:
	FQuat Rotation;
	Vector3 Translation;
	uint8_t Pad_1C[0x8];
	Vector3 Scale3D;
	uint8_t Pad_2C[0x8];

public:
	PFORCEINLINE FMatrix ToMatrixWithScale() const {
		const Vector3 Scale(
			(Scale3D.x == 0.0) ? 1.0 : Scale3D.x,
			(Scale3D.y == 0.0) ? 1.0 : Scale3D.y,
			(Scale3D.z == 0.0) ? 1.0 : Scale3D.z);

		const double x2 = Rotation.x + Rotation.x;
		const double y2 = Rotation.y + Rotation.y;
		const double z2 = Rotation.z + Rotation.z;
		const double xx2 = Rotation.x * x2;
		const double yy2 = Rotation.y * y2;
		const double zz2 = Rotation.z * z2;
		const double yz2 = Rotation.y * z2;
		const double wx2 = Rotation.w * x2;
		const double xy2 = Rotation.x * y2;
		const double wz2 = Rotation.w * z2;
		const double xz2 = Rotation.x * z2;
		const double wy2 = Rotation.w * y2;

		FMatrix Matrix = FMatrix();

		Matrix.WPlane.x = Translation.x;
		Matrix.WPlane.y = Translation.y;
		Matrix.WPlane.z = Translation.z;

		Matrix.XPlane.x = (1.0 - (yy2 + zz2)) * Scale.x;
		Matrix.YPlane.y = (1.0 - (xx2 + zz2)) * Scale.y;
		Matrix.ZPlane.z = (1.0 - (xx2 + yy2)) * Scale.z;

		Matrix.ZPlane.y = (yz2 - wx2) * Scale.z;
		Matrix.YPlane.z = (yz2 + wx2) * Scale.y;

		Matrix.YPlane.x = (xy2 - wz2) * Scale.y;
		Matrix.XPlane.y = (xy2 + wz2) * Scale.x;

		Matrix.ZPlane.x = (xz2 + wy2) * Scale.z;
		Matrix.XPlane.z = (xz2 - wy2) * Scale.x;

		Matrix.XPlane.W = 0.0f;
		Matrix.YPlane.W = 0.0f;
		Matrix.ZPlane.W = 0.0f;
		Matrix.WPlane.W = 1.0f;

		return Matrix;
	}
};

struct Camera {
	Vector3 Location;
	Vector3 Rotation;
	float FieldOfView;
};

template<class type>
class tarray {
public:
	tarray() : data(nullptr), count(std::int32_t()), maxx(std::int32_t()) {}
	tarray(type* data, std::int32_t count, std::int32_t maxx) : data(data), count(count), maxx(maxx) {}

	const bool is_valid() const noexcept {
		return !(this->data == nullptr);
	}

	const std::int32_t size() const noexcept {
		return this->count;
	}

	type& operator[](std::int32_t index) noexcept {
		return this->data[index];
	}

	const type& operator[](std::int32_t index) const noexcept {
		return this->data[index];
	}

	bool is_valid_index(std::int32_t index) const noexcept {
		return index < this->size();
	}

	type* data;
	std::int32_t count;
	std::int32_t maxx;
};


inline D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2) {
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

inline D3DMATRIX Matrix(Vector3 rot) {
	float radPitch = (rot.x * float(std::numbers::pi) / 180.f);
	float radYaw = (rot.y * float(std::numbers::pi) / 180.f);
	float radRoll = (rot.z * float(std::numbers::pi) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = 0;
	matrix.m[3][1] = 0;
	matrix.m[3][2] = 0;
	matrix.m[3][3] = 1.f;

	return matrix;
}

void DrawCornerBox(int X, int Y, int W, int H, const ImColor color, int thickness) {
	float lineW = (W / 3);
	float lineH = (H / 3);

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), color, thickness);
}