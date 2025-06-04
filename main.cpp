#include <Novice.h>
#include "Matrix.h"
#include <cmath>

const char kWindowTitle[] = "LC1C_15_ニューマンヨシノリ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	Matrix* matrix = new Matrix;

	// クロス積を作る
	Vector3 v1{ 1.2f, -3.9f, 2.5f };
	Vector3 v2{ 2.8f, 0.4f, -1.3f };
	Vector3 cross = matrix->Cross(v1, v2);

	Vector3 rotate{};
	Vector3 translate{};

	// カメラの位置
	Vector3 cameraPosition = { 0.0f, 0.0f, 10.0f };
	// 三角形の頂点
	Vector3 kLocalVerticals[3] = { {.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {-1.0f, 1.0f, 0.0f}};

	// 各種行列
	Matrix4x4 worldMatrix = {};
	Matrix4x4 cameraMatrix = {};
	Matrix4x4 viewMatrix = {};
	Matrix4x4 projectionMatrix = {};
	// WVPMatrixを作る
	Matrix4x4 worldViewProjectionMatrix = {};
	// ViewportMatrixを作る
	Matrix4x4 viewportMatrix = {};
	// Screen空間へと頂点を変換する
	Vector3 screenVerticals[3];
	for (uint32_t i = 0; i < 3; ++i) {
		// NDCまで変換
		Vector3 ndcVertex = {};
		// Viewport変換を行ってScreen空間へ
		screenVerticals[i] = {};
	}

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// 前に動かす
		if (keys[DIK_W]) {
			translate.z += 0.1f;
		}
		// 後ろに動かす
		if (keys[DIK_S]) {
			translate.z -= 0.1f;
		}

		// 左に動かす
		if (keys[DIK_A]) {
			translate.x -= 0.1f;
		}
		// 右に動かす
		if (keys[DIK_D]) {
			translate.x += 0.1f;
		}

		// Y軸回転させる
		rotate.y += 0.03f;

		// 各種行列の計算
		worldMatrix = matrix->MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
		cameraMatrix = matrix->MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, cameraPosition);
		viewMatrix = matrix->Inverse(cameraMatrix);
		projectionMatrix = matrix->MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		// WVPMatrixを作る
		worldViewProjectionMatrix = matrix->Multiply(worldMatrix, matrix->Multiply(viewMatrix, projectionMatrix));
		// ViewportMatrixを作る
		viewportMatrix = matrix->MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		for (uint32_t i = 0; i < 3; ++i) {
			// NDCまで変換
			Vector3 ndcVertex = matrix->Transform(kLocalVerticals[i], worldViewProjectionMatrix);
			// Viewport変換を行ってScreen空間へ
			screenVerticals[i] = matrix->Transform(ndcVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// クロス積
		matrix->VectorScreenPrintf(0, 0, cross, "Cross");

		// 描画
		Novice::DrawTriangle(
			int(screenVerticals[0].x), int(screenVerticals[0].y), int(screenVerticals[1].x), int(screenVerticals[1].y),
			int(screenVerticals[2].x), int(screenVerticals[2].y), RED, kFillModeSolid
		);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
