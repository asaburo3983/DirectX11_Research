#include "DXTK11Lib.h"
#include "input.h"
namespace DXTK11LIB {

	float imageExtend = 1.0f;//全画像の倍率

	std::unique_ptr<DirectX::SpriteBatch> fontBatch;//フォント用の共通変数
	std::unique_ptr<DirectX::SpriteFont> fontSprite;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	float alphaDefault = 1.0f;//全体のデフォルトアルファ値

	ID3D11DeviceContext* context;
	ID3D11Device* device;


	//初期処理
	void InitDXTK11Lib() {
		context = DirectGraphics::GetInstance()->GetContext();
		device = DirectGraphics::GetInstance()->GetDevice();
		//フォント読み込み
		fontBatch = std::make_unique<DirectX::SpriteBatch>(context);
		fontSprite = std::make_unique<DirectX::SpriteFont>(device, L"Font/myfile.spritefont");
		//画像準備
		m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	}

	//２D描画系//////////////////////////////

	//文字描画
	void DrawString(const wchar_t* str, int x, int y, StrColor _color, float size, float angle) {

		DirectX::XMVECTORF32 color;
		for (int i = 0; i < 4; i++) {
			color.f[i] = _color.rgba[i];
		}

		//描画
		fontBatch->Begin();

		fontSprite->DrawString(fontBatch.get(), str, DirectX::XMFLOAT2(x * imageExtend, y * imageExtend), color, 0, DirectX::XMFLOAT2(0, 0), size * imageExtend);

		fontBatch->End();
	}
	void DrawString(const char* str, int x, int y, StrColor _color, float size, float angle) {

		size_t tmpsize = strlen(str);//char型をWCHARに変換
		int  tmpsizei = tmpsize + 1;
		WCHAR* tmpwbuf = new WCHAR[tmpsizei];
		mbstowcs_s(0, tmpwbuf, tmpsizei, str, _TRUNCATE);

		DrawString(tmpwbuf, x, y, _color, size, angle);

		delete tmpwbuf;
	}

	//FPSを描画
	int frame = 0;
	unsigned long base_time = 0.0;
	unsigned long prev = 0.0;
	unsigned long current = 0.0;
	char	buf[20];
	WCHAR wbuf[20];
	void DrawFPS() {
		// 時間取得
		base_time = timeGetTime();


		float intarval = 0.0;

		// フレームレート出力
		if ((frame + 1) % 60 == 0)
		{
			current = timeGetTime();

			intarval = 60000.0f / (current - prev);

			sprintf_s(buf, sizeof(buf), "FPS : %.1f", intarval);
			prev = current;
		}
		frame++;
		mbstowcs_s(0, wbuf, 20, buf, _TRUNCATE);
		DrawString(wbuf, 0, 0, StrColor{ 1,0,0,1 });

	}

	//全体のデフォルトα値をセット
	void SetDefaultAlpha(float _alpha) {
		alphaDefault = _alpha;
	}
	//全体の追加の拡大率をセット
	void SetDefaultImageExtend(float _extend) {
		imageExtend = _extend;
	}
	float GetDefaultImageExtend() {
		return imageExtend;
	}
	//////////////////////////////
	//2D画像//////////////////////////////
	Image::Image(const wchar_t* filePath) {//画像ロード コンストラクタ
		Load(filePath);
	}
	void Image::Load(const wchar_t* filePath) {//画像ロード　
		//画像描画追加分
		

		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		
		//画像読み込み										↓これが悪いときはファイル名がすべて大文字だったらダメ
		DirectX::CreateWICTextureFromFile(device, filePath, resource.GetAddressOf(), m_texture.ReleaseAndGetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		DX::ThrowIfFailed(resource.As(&texture));

		//画像データをとる
		CD3D11_TEXTURE2D_DESC textureDesc;
		texture->GetDesc(&textureDesc);

		textureCenter = DirectX::SimpleMath::Vector2(textureDesc.Width / 2, textureDesc.Height / 2);

		width = textureDesc.Width;//画像サイズの格納
		height = textureDesc.Height;

	}

	void Image::Load(const char* filePath) {//画像ロード　
		size_t tmpsize = strlen(filePath);//char型をWCHARに変換
		int  tmpsizei = tmpsize + 1;
		WCHAR* tmpwbuf = new WCHAR[tmpsizei];
		mbstowcs_s(0, tmpwbuf, tmpsizei, filePath, _TRUNCATE);

		Load(tmpwbuf);

		delete tmpwbuf;
	}

	void Image::LoadShader(const char* filePath) {
		m_PixelShader = new PixelShader();
		m_PixelShader->Create(device, filePath);
	}
	void Image::Draw(int _x, int _y, float _angle, float _size, float _alpha, int _center, bool shader) {

		
		x = _x * imageExtend;
		y = _y * imageExtend;
		sizeX = _size * imageExtend;
		sizeY = _size * imageExtend;

		center = _center;
		angle = _angle;
		////シェーダーを無効化にしている これがないと別のシェーダーが適応されてバグる
		// VSに頂点シェーダを設定
		context->VSSetShader(NULL, NULL, 0);
		// GSにジオメトリ・シェーダを設定
		context->GSSetShader(NULL, NULL, 0);
		// RSにラスタライザ・ステート・オブジェクトを設定
		context->RSSetState(NULL);
		// PSにピクセル・シェーダを設定
		context->PSSetShader(NULL, NULL, 0);

		//未入力の際デフォルトのα値を入れる
		if (_alpha == -1000.0123f)
			_alpha = alphaDefault;
		//画像描画
		
		if (shader) {
			ID3D11Buffer* cb;
			cb = DirectGraphics::GetInstance()->GetConstantBuffer();
			context->UpdateSubresource(DirectGraphics::GetInstance()->GetConstantBuffer(), 0, NULL, &m_ConstantBufferData, 0, 0);

			m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, [=]
				{
					context->PSSetShader(m_PixelShader->GetShaderInterface(), NULL, 0);
					context->PSSetConstantBuffers(0,1, &cb);
					//context->PSSetShaderResources(0,1, nullptr);
				});
		}
		else {
			m_spriteBatch->Begin();
		}
		m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), NULL, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, textureCenter * _center, sizeX);
		m_spriteBatch->End();
	}
	void Image::DrawEX(int _x, int _y, float _angle, float _sizeX, float _sizeY, float _alpha, int _center, bool LR_TURN, bool UD_TURN, bool shader, RECT drawArea) {
		x = _x * imageExtend;
		y = _y * imageExtend;

		center = _center;
		sizeX = _sizeX * imageExtend;
		sizeY = _sizeY * imageExtend;
		angle = _angle;
		//シェーダーを無効化にしている これがないと別のシェーダーが適応されてバグる
		// VSに頂点シェーダを設定
		context->VSSetShader(NULL, NULL, 0);
		// GSにジオメトリ・シェーダを設定
		context->GSSetShader(NULL, NULL, 0);
		// RSにラスタライザ・ステート・オブジェクトを設定
		context->RSSetState(NULL);
		// PSにピクセル・シェーダを設定
		context->PSSetShader(NULL, NULL, 0);

		//未入力の際デフォルトのα値を入れる
		if (_alpha == -1000.0123f)
			_alpha = alphaDefault;

		DirectX::SimpleMath::Vector2 texCenter = textureCenter;
		if (drawArea.left == -1) {
			drawArea.left = 0;
			drawArea.top = 0;
			drawArea.right = width;
			drawArea.bottom = height;

		}


		const RECT* rect=&drawArea;

		if (shader) {
			m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, [=]
				{
					context->PSSetShader(m_PixelShader->GetShaderInterface(), NULL, 0);
					//context->PSSetConstantBuffers(0,0,nullptr);
					//context->PSSetShaderResources(0,1, nullptr);
				});
		}
		else {
			m_spriteBatch->Begin();
		}


		if (!LR_TURN && !UD_TURN) {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), rect, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, texCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_None);
		}

		else if (LR_TURN) {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y),rect , DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, texCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_FlipHorizontally);

		}
		else if (UD_TURN) {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), rect, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, texCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_FlipVertically);

		}
		else {
			m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(x, y), rect, DirectX::Colors::White * _alpha, _angle / 180.0 * 3.1415, texCenter * _center, DirectX::XMFLOAT2(_sizeX * imageExtend, _sizeY * imageExtend), DirectX::SpriteEffects_FlipBoth);

		}

		m_spriteBatch->End();

	}

	bool Image::Hit(int _x, int _y) {//画像内に入った当たり判定 回転してない四角い画像だけ当たり判定をとれる
		int minX, maxX, minY, maxY;
		if (center == 1) {
			maxX = x + (width / 2 * sizeX);
			minX = x - (width / 2 * sizeX);
			maxY = y + (height / 2 * sizeY);
			minY = y - (height / 2 * sizeY);
		}
		else {
			maxX = x + (width * sizeX);
			minX = x;
			maxY = y + (height * sizeY);
			minY = y;
		}

		//当たり判定内
		if (minX <= _x && _x <= maxX && minY <= _y && _y <= maxY) {
			return true;
		}
		return false;
	}

	void Image::DebugDraw(int speed,float angleSpeed,float scaleSpeed,int dataPosX,int dataPosY) {
		KeyInput* KEY = KeyInput::GetInstance();

		Draw(debugX, debugY, debugAngle, debugSize);

		char buf[256];
		WCHAR wbuf[256];
		sprintf_s(buf, sizeof(buf), "POS:X:%d:Y:%d", debugX, debugY);
		mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
		DXTK11LIB::DrawString(wbuf, dataPosX + 0, dataPosY + 30, DXTK11LIB::StrColor{ 1,0,0,1 });
		sprintf_s(buf, sizeof(buf), "ANGLE:%.2f", angle);
		mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
		DXTK11LIB::DrawString(wbuf, dataPosX + 0, dataPosY + 60, DXTK11LIB::StrColor{ 1,0,0,1 });
		sprintf_s(buf, sizeof(buf), "SCALE:%.2f", debugSize);
		mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
		DXTK11LIB::DrawString(wbuf, dataPosX + 0, dataPosY + 90, DXTK11LIB::StrColor{ 1,0,0,1 });

		//拡縮
		if (KEY->key[DIK_LSHIFT] >= 1 && KEY->key[DIK_LCONTROL] >= 1) {
			if (KEY->key[DIK_W] >= 1) {
				debugSize += scaleSpeed;
			}
			if (KEY->key[DIK_S] >= 1) {
				debugSize -= scaleSpeed;
			}
			if (KEY->key[DIK_D] >= 1) {
				debugSize += scaleSpeed;
			}
			if (KEY->key[DIK_A] >= 1) {
				debugSize -= scaleSpeed;
			}
			return;
		}

		//移動
		if (KEY->key[DIK_LSHIFT]>=1) {
			if (KEY->key[DIK_W] >= 1) {
				debugY -= speed;
			}
			if (KEY->key[DIK_S] >= 1) {
				debugY += speed;
			}
			if (KEY->key[DIK_D] >= 1) {
				debugX += speed;
			}
			if (KEY->key[DIK_A] >= 1) {
				debugX -= speed;
			}
			return;
		}
		//回転
		if (KEY->key[DIK_LCONTROL] >= 1) {
			if (KEY->key[DIK_D] >= 1) {
				debugAngle+=angleSpeed;
			}
			if (KEY->key[DIK_A] >= 1) {
				debugAngle-=angleSpeed;
			}
			return;
		}

	}
	//////////////////////////////
	//音楽//////////////////////////////

	Sound_MP3::Sound_MP3(const wchar_t* filePath) {
		Load(filePath);
	}
	void Sound_MP3::Load(const wchar_t* filePath) {

		MFPCreateMediaPlayer(filePath, FALSE, 0, NULL, NULL, &g_pPlayer);

	}
	void Sound_MP3::Load(const char* filePath) {
		size_t tmpsize = strlen(filePath);//char型をWCHARに変換
		int  tmpsizei = tmpsize + 1;
		WCHAR* tmpwbuf = new WCHAR[tmpsizei];
		mbstowcs_s(0, tmpwbuf, tmpsizei, filePath, _TRUNCATE);

		MFPCreateMediaPlayer(tmpwbuf, FALSE, 0, NULL, NULL, &g_pPlayer);
		delete tmpwbuf;
	}
	void Sound_MP3::Play(bool loop) {

		PROPVARIANT vPositionValue;
		PropVariantInit(&vPositionValue);
		vPositionValue.vt = VT_I8;
		vPositionValue.hVal.QuadPart = 0; // 1秒 (10000000[100nsec]) の位置から再生する;

		if (!loop) {
			g_pPlayer->SetPosition(
				MFP_POSITIONTYPE_100NS,
				&vPositionValue
			);
		}
		g_pPlayer->GetState(&state);
		if (state == MFP_MEDIAPLAYER_STATE_STOPPED || state == MFP_MEDIAPLAYER_STATE_EMPTY)
		{
			g_pPlayer->Play();

		}
	}
	void Sound_MP3::Stop() {
		g_pPlayer->GetState(&state);
		if (state != MFP_MEDIAPLAYER_STATE_STOPPED) {
			g_pPlayer->Stop();
		}
	}
	void Sound_MP3::Volume(float vol) {
		g_pPlayer->SetVolume(vol);
	}
	//////////////////////////////
}