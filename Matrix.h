#pragma once

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Matrix4x4
{
	float m[4][4];
};

class Matrix
{
public:
	// 行列の積
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	// アフィン変換
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	Matrix4x4 Inverse(const Matrix4x4& m);
	// 透視投影行列
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farclip);
	// ビュー変換行列
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
	// 座標変換
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
	// クロス積
	Vector3 Cross(const Vector3& v1, const Vector3& v2);
	// ベクトルをスクリーン描画
	void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

	// ゲッター
	const int GetWidth() { return kColumnWidth; }
	const int GetHeight() { return kRowHeight; }

private:
	const int kColumnWidth = 60;
	const int kRowHeight = 20;
};
