// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageCameraShake.h"

UDamageCameraShake::UDamageCameraShake() {
	// --- 1. 時間の設定 ---
	// 0.15秒〜0.2秒程度の短い時間が「衝撃」には最適
	OscillationDuration = 0.2f;
	OscillationBlendInTime = 0.05f; // 素早く立ち上がる
	OscillationBlendOutTime = 0.15f; // 余韻を残さずピタッと止める

	// --- 2. 回転の揺れ (RotOscillation) ---
	// Pitch: 上下の首振り。ダメージ時の基本。
	RotOscillation.Pitch.Amplitude = 5.0f;  // 少し強めに設定
	RotOscillation.Pitch.Frequency = 35.0f; // 速いピッチで揺らす
	RotOscillation.Pitch.InitialOffset = EOO_OffsetRandom;

	// Yaw: 左右の首振り。横からの衝撃を表現。
	RotOscillation.Yaw.Amplitude = 2.0f;
	RotOscillation.Yaw.Frequency = 25.0f;
	RotOscillation.Yaw.InitialOffset = EOO_OffsetRandom;

	// --- 3. 位置の揺れ (LocOscillation) ---
	// Z: 上下のガクッとする動き。
	LocOscillation.Z.Amplitude = 10.0f;
	LocOscillation.Z.Frequency = 40.0f;
	LocOscillation.Z.InitialOffset = EOO_OffsetRandom;

	// --- 4. FOV（画角）の揺れ ---
	// 瞬間的にズームアウト/インさせることで「衝撃の強さ」を強調
	FOVOscillation.Amplitude = 2.0f;
	FOVOscillation.Frequency = 30.0f;
	FOVOscillation.InitialOffset = EOO_OffsetRandom;
}