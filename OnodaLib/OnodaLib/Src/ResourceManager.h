#pragma once

#include <string>
#include <unordered_map>

#include "FbxModel.h"
#include "DXTK11Lib.h"

using namespace DXTK11LIB;
//すべてのリソース(Model、Image、Sound)をここで管理している


class ResourceManager {
public:
	static ResourceManager* GetInstance() {
		static ResourceManager rm;
		return &rm;
	}

private:

	std::vector<std::string> modelLKey;//メモリを食いずらいがロードが必要　キー文字列保存用の可変配列 
	std::vector<std::string> modelGKey;//メモリを食うが、ロードが不要

	std::vector<std::string> imageLKey;
	std::vector<std::string> imageGKey;

	std::vector<std::string> bgmLKey;
	std::vector<std::string> bgmGKey;

	std::vector<std::string> seLKey;
	std::vector<std::string> seGKey;

	std::vector<std::string> shaderVKey;//消すほどでかいデータでもないのでGrobalのみ
	std::vector<std::string> shaderGKey;
	std::vector<std::string> shaderPKey;

	std::unordered_map<std::string, FbxModel*> modelL;//シーン移動の際に消すモデル
	std::unordered_map<std::string, FbxModel*> modelG;//ゲーム内で消さないモデル
	std::unordered_map<std::string, Image*> imageL;
	std::unordered_map<std::string, Image*> imageG;
	std::unordered_map<std::string, Sound_MP3*> bgmL;
	std::unordered_map<std::string, Sound_MP3*> bgmG;
	std::unordered_map<std::string, Sound_MP3*> seL;
	std::unordered_map<std::string, Sound_MP3*> seG;

	std::unordered_map<std::string, VertexShaderData*> shaderV;
	std::unordered_map<std::string, GeometryShaderData*> shaderG;
	std::unordered_map<std::string, PixelShaderData*> shaderP;
public:

	void SetModel(std::string keyName, FbxModel* model, bool local=true);//モデルのセットと削除
	void SetImage(std::string keyName, Image* image, bool local = true);
	void SetBGM(std::string keyName, Sound_MP3* mp3, bool local = true);
	void SetSE(std::string keyName, Sound_MP3* mp3, bool local = true);
	void SetShaderV(std::string keyName, VertexShaderData* vs);
	void SetShaderG(std::string keyName, GeometryShaderData* gs);
	void SetShaderP(std::string keyName, PixelShaderData* ps);
	
	FbxModel* GetModel(std::string keyName="NULL", bool local=true);//モデルのセットと削除
	Image* GetImage(std::string keyName = "NULL", bool local=true);
	Sound_MP3* GetBGM(std::string keyName = "NULL", bool local=true);
	Sound_MP3* GetSE(std::string keyName = "NULL", bool local=true);
	VertexShaderData* GetShaderV(std::string keyName);
	GeometryShaderData* GetShaderG(std::string keyName);
	PixelShaderData* GetShaderP(std::string keyName);

	
	void AllDeleteLocal();//シーン移動時処理
	void AllDeleteGrobal();//ゲーム終了時処理


};