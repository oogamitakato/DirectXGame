﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	
	delete sprite_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//サウンドデータの読み込み
	soundDateHandle_ = audio_->LoadWave("se_sad03.wav");

	//音楽再生
	//voiceHandle_ = audio_->PlayWave(soundDateHandle_, true);

	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	//3Dモデルの生成
	model_ = Model::Create();

	//X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};

	//X,Y,Z軸周りの回転角を設定
	worldTransform_.rotation_ = {XM_PI / 4.0f, XM_PI / 4.0f, 0.0f};

	//X,Y,Z軸周りの平行移動を設定
	worldTransform_.translation_ = {10.0f, 10.0f, 10.0f};

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	
	//スプライトの今の座標を取得
	XMFLOAT2 position = sprite_->GetPosition();

	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);

	//変数の値をインクリメント
	value_++;
	//値を含んだ文字列
	std::string strDebug = std::string("translation:") + std::to_string(worldTransform_.scale_.x);

	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		//音楽停止
		audio_->StopWave(voiceHandle_);
	}

	//デバックテキストの表示
	//平行移動の表示
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "translation:(%f,%f,%f)",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z
	);
	//回転角の表示
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rocation:(%f,%f,%f)", 
		worldTransform_.rotation_.x,
		worldTransform_.rotation_.y,
		worldTransform_.rotation_.z
	);
	//スケーリングの表示
	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "scale:(%f,%f,%f)",
		worldTransform_.scale_.x,
		worldTransform_.scale_.y,
		worldTransform_.scale_.z
	);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
