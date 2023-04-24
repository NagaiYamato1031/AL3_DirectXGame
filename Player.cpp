#include "Player.h"

#include "Vector3.h"
#include "Matrix4x4.h"
#include <cassert>
#include "ImGuiManager.h"

#include "Mymath.h"

void Player::Initialze(Model* model, uint32_t textureHandle) {
	// NULL ポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {

	// 移動ベクトルの設定
	Vector3 move = {0, 0, 0};

	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 座標加算(ベクトルの加算)
	worldTransform_.translation_ = Mymath::Add(worldTransform_.translation_, move);

	// 移動限界座標
	const float kMoveLimitX = 32.5f;
	const float kMoveLimitY = 17.5f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.matWorld_ = Mymath::MakeAffineMatrix4x4(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);


	// 行列を定数バッファに転送	
	worldTransform_.TransferMatrix();
	ImGui::Begin("Player");
	float* translate[3] = {
	    &worldTransform_.translation_.x, &worldTransform_.translation_.y,
	    &worldTransform_.translation_.z};
	ImGui::SliderFloat3("translate", *translate, -50.0f, 50.0f);
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewProjection) {
	// 3D モデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
