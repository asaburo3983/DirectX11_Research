#pragma once

#include <d3d11.h>
#include "ShaderBase.h"
//=====================================================================//
//! Geometryシェーダ情報を保持するクラス
//=====================================================================//
class GeometryShader : public ShaderBase
{
public:
	/** Constructor */
	GeometryShader() :
		m_Interface(nullptr)
	{}

	/** Destructor */
	virtual ~GeometryShader()
	{
		if (m_Interface != nullptr)
		{
			m_Interface->Release();
			m_Interface = nullptr;
		}
	}

	/**
	* @brief GeometryShader作成関数@n
	* 指定されたファイルからGeometryShaderを作成する@n
	* @return 作成結果 成功(true)
	* @param[in] device ID3D11Device
	* @param[in] file_name 読み込み対象のファイル名
	*/
	virtual bool Create(ID3D11Device* device, const char*file_name)
	{
		// ファイル読み込み
		if (Load(file_name) == false)
		{
			return false;
		}

		// GeometryShader作成
		if (FAILED(device->CreateGeometryShader(
			m_Data,					// 生成元となるデータ
			m_Size,					// データのサイズ
			/*
				ID3D11ClassLinkageのポインタを設定
					ID3D11ClassLinkageはシェーダの実行関数を
					動的に切り替えることができる
					それによって、条件分岐をシェーダ内に書かなくてもすむので、
					シェーダの速度低下を抑えることができる。
					今回は使用しないのでnullptr
			*/
			nullptr,
			&m_Interface)))			// 作成後のInterface格納先
		{
			return false;
		}

		return true;
	}

	/** ShaderInterfaceのゲッター */
	inline ID3D11GeometryShader* GetShaderInterface()
	{
		return m_Interface;
	}
private:
	ID3D11GeometryShader *m_Interface;		//!< @brief GeometryShaderInterface
};
