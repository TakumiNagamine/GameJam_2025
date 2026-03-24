#include "DxLib.h"
#include "Utility/InputManager.h"
#include "Utility/PadInputManager.h"
#include "Utility/ResourceManager.h"
#include "Utility/FPS.h"
#include "Scene/SceneManager.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	// ウィンドウタイトルの設定
	SetMainWindowText("MemoryChime");
	// アイコンの設定
	SetWindowIconID(333);

	ChangeWindowMode(TRUE);

	//画面サイズ指定
	SetGraphMode(1280, 720, 32);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager* manager = nullptr;
	int result = 0;

	try
	{
		// SceneManagerのオブジェクト化
		manager = new SceneManager();

		// SceneManagerの初期化処理
		manager->Initialize();

		// キーボード入力制御のインスタンスを取得
		InputManager* input = InputManager::GetInstance();
		// パッド入力制御のインスタンスを取得
		PadInputManager* pad_input = PadInputManager::GetInstance();

		FPS fps;
		//ループ前にFPS計測を初期化
		fps.Reset_FPS();

		// マウスカーソル非表示
		SetMouseDispFlag(FALSE);

		while (ProcessMessage() != -1 && manager->LoopCheck())
		{
			// FPS固定
			fps.FPSUpdate();

			// 入力更新処理
			input->Update();
			pad_input->Update();

			ClearDrawScreen();

			// シーン更新処理
			manager->Update();

			// FPS描画
			//fps.Display_FPS();

			ScreenFlip();

			if (input->GetKeyInputState(KEY_INPUT_ESCAPE) == eInputState::eRelease
				|| pad_input->GetButtonInputState(XINPUT_BUTTON_BACK) == ePadInputState::eRelease)
			{
				// ESCAPEキーかBACKボタンで終了
				break;
			}
		}
	}
	catch (std::string& error_text)
	{
		OutputDebugString(error_text.c_str());
		result = -1;
	}

	// キーボード入力制御の終了処理
	InputManager::DeleteInstance();
	// パッド入力制御の終了処理
	InputManager::DeleteInstance();
	// リソース管理の終了処理
	ResourceManager::DeleteInstance();

	if (manager != nullptr)
	{
		manager->Finalize();
		// メモリの開放
		delete manager;
	}

	DxLib_End();

	return 0;
}