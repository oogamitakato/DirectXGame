#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

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

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.f, 10.f);

	//キャラクターの大元
	worldTransform_[Root].Initialize();
	//脊髄
	worldTransform_[Spine].translation_ = {0, 4.5f, 0};
	worldTransform_[Spine].parent_ = &worldTransform_[0];
	worldTransform_[Spine].Initialize();

	//上半身
	//胸
	worldTransform_[Chest].translation_ = { 0, -1.0f, 0};
	worldTransform_[Chest].parent_ = &worldTransform_[Spine];
	worldTransform_[Chest].Initialize();

	//頭
	worldTransform_[Head].translation_ = { 0, 3.0f, 0};
	worldTransform_[Head].parent_ = &worldTransform_[Chest];
	worldTransform_[Head].Initialize();

	//左腕
	worldTransform_[ArmL].translation_ = {3.0f, 0, 0};
	worldTransform_[ArmL].parent_ = &worldTransform_[Chest];
	worldTransform_[ArmL].Initialize();

	//右腕
	worldTransform_[ArmR].translation_ = {-3.0f, 0, 0};
	worldTransform_[ArmR].parent_ = &worldTransform_[Chest];
	worldTransform_[ArmR].Initialize();

	//下半身
	//尻
	worldTransform_[Hip].translation_ = {0, -4.0f, 0};
	worldTransform_[Hip].parent_ = &worldTransform_[Spine];
	worldTransform_[Hip].Initialize();

	//左足
	worldTransform_[LegL].translation_ = {3.0f, -3.0f, 0};
	worldTransform_[LegL].parent_ = &worldTransform_[Hip];
	worldTransform_[LegL].Initialize();

	//右足
	worldTransform_[LegR].translation_ = {-3.0f, -3.0f, 0};
	worldTransform_[LegR].parent_ = &worldTransform_[Hip];
	worldTransform_[LegR].Initialize();

	//for (size_t i = 0; i < _countof(worldTransform_); i++) {

	//	//X,Y,Z方向のスケーリングを設定
	//	worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};

	//	//X,Y,Z軸周りの回転角を設定
	//	worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};

	//	//X,Y,Z軸周りの平行移動を設定
	//	worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

	//	//ワールドトランスフォームの初期化
	//	worldTransform_[i].Initialize();
	//}

	////カメラ視点座標を設定
	viewProjection_.eye = {0, 0, -20};

	////カメラ注視点座標を設定
	//viewProjection_.target = {10, 0, 0};

	////カメラ上方向ベクトルを設定(右上45度指定)
	//viewProjection_.up = {cosf(XM_PI / 4.0f), sinf(XM_PI / 4.0f), 0.0f};

	//カメラ垂直方向視野を設定
	//viewProjection_.fovAngleY = XMConvertToRadians(10.0f);

	//アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;

	//ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	

	////スプライトの今の座標を取得
	//XMFLOAT2 position = sprite_->GetPosition();

	////移動した座標をスプライトに反映
	//sprite_->SetPosition(position);

	////スペースキーを押した瞬間
	//if (input_->TriggerKey(DIK_SPACE)) {
	//	//音楽停止
	//	audio_->StopWave(voiceHandle_);
	//}

	////視点移動処理
	//{
	//	//視点の移動ベクトル
	//	XMFLOAT3 move = {0, 0, 0};

	//	//視点の移動速さ
	//	const float kEyeSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W)) {
	//		move = {0, 0, kEyeSpeed};
	//	} else if (input_->PushKey(DIK_S)) {
	//		move = {0, 0, -kEyeSpeed};
	//	}

	//	//視点移動(ベクトルの加算)
	//	viewProjection_.eye.x += move.x;
	//	viewProjection_.eye.y += move.y;
	//	viewProjection_.eye.z += move.z;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//}

	////注視点移動処理
	//{
	//	//注視点の移動ベクトル
	//	XMFLOAT3 move = {0, 0, 0};

	//	//注視点の移動速さ
	//	const float kTargetSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_LEFT)) {
	//		move = {-kTargetSpeed, 0, 0};
	//	} else if (input_->PushKey(DIK_RIGHT)) {
	//		move = {kTargetSpeed, 0, 0};
	//	}

	//	//注視点移動(ベクトルの加算)
	//	viewProjection_.target.x += move.x;
	//	viewProjection_.target.y += move.y;
	//	viewProjection_.target.z += move.z;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();
	//}

	////上方向回転処理
	//{
	//	//上方向の回転速さ[ラジアン/frame]
	//	const float kUpRotSpeed = 0.05f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_SPACE)) {
	//		viewAngle += kUpRotSpeed;
	//		//2πを超えたら0に戻す
	//		viewAngle = fmodf(viewAngle, XM_2PI);
	//	}

	//	//上方向のベクトルを計算(半径1の円周上の座標)
	//	viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();
	//}

	//Fov変更処理
	//{
	//	//Wキーで視野角が広がる
	//	if (input_->PushKey(DIK_W)) {
	//		viewProjection_.fovAngleY += 0.01f;
	//		viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
	//	//Sキーで視野角が狭まる
	//	} else if (input_->PushKey(DIK_S)) {
	//		viewProjection_.fovAngleY -= 0.01f;
	//		viewProjection_.fovAngleY = max(viewProjection_.fovAngleY,0.01f);
	//	}
	//	
	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//}

	////クリップ距離変更処理
	//{
	//	//上下キーでニアクリップ距離を増減
	//	if (input_->PushKey(DIK_UP)) {
	//		viewProjection_.nearZ += 0.1f;
	//	} else if (input_->PushKey(DIK_DOWN)) {
	//		viewProjection_.nearZ -= 0.1f;
	//	}

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();
	//}
	
	//キャラクターの移動処理
	{
		//キャラクターの移動ベクトル
		XMFLOAT3 move = {0, 0, 0};

		//キャラクターの移動速さ
		const float kCharacterSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kCharacterSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kCharacterSpeed, 0, 0};
		}

		//注視点移動(ベクトルの加算)
		worldTransform_[Root].translation_.x += move.x;
		worldTransform_[Root].translation_.y += move.y;
		worldTransform_[Root].translation_.z += move.z;
	}

	//上半身回転処理
	{
		//上半身の回転速さ[ラジアン/flame]
		const float kChestRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U)) {
			worldTransform_[Chest].rotation_.y -= kChestRotSpeed;
		} else if (input_->PushKey(DIK_I)) {
			worldTransform_[Chest].rotation_.y += kChestRotSpeed;
		}
	}

	//下半身回転処理
	{
		//下半身の回転速さ[ラジアン/flame]
		const float kHipRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_J)) {
			worldTransform_[Hip].rotation_.y += kHipRotSpeed;
		} else if (input_->PushKey(DIK_K)) {
			worldTransform_[Hip].rotation_.y -= kHipRotSpeed;
		}
	}

	//大元から順に更新
	worldTransform_[Root].UpdateMatrix();
	worldTransform_[Spine].UpdateMatrix();
	worldTransform_[Chest].UpdateMatrix();
	worldTransform_[Head].UpdateMatrix();
	worldTransform_[ArmL].UpdateMatrix();
	worldTransform_[ArmR].UpdateMatrix();
	worldTransform_[Hip].UpdateMatrix();
	worldTransform_[LegL].UpdateMatrix();
	worldTransform_[LegR].UpdateMatrix();

	//デバッグ用表示
	/*debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));

	debugText_->SetPos(50, 130);
	debugText_->Printf(
	  "nearZ:%f", viewProjection_.nearZ);*/

	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"Root:(%f,%f,%f)",
		worldTransform_[Root].translation_.x,
		worldTransform_[Root].translation_.y,
		worldTransform_[Root].translation_.z);
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
	//for (size_t i = 0; i < _countof(worldTransform_); i++) {

	//	model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	//}

	//model_->Draw(worldTransform_[Root], viewProjection_, textureHandle_);
	//model_->Draw(worldTransform_[Spine], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Chest], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[ArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[ArmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Hip], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[LegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[LegR], viewProjection_, textureHandle_);

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
