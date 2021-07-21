#ifndef SHADER_BASE_H_
#define SHADER_BASE_H_

#include <d3d11.h>
#include <stdio.h>


class ShaderBase
{
public:
	/** Constructor */
	ShaderBase() :
		m_Size(0),
		m_Data(nullptr)
	{}

	/** Destructor */
	virtual ~ShaderBase() 
	{
		if (m_Data != nullptr)
		{
			delete[] m_Data;
		}
	}

	/** Shaderデータ(バイナリ)のゲッター */
	inline const char* GetData()
	{
		return m_Data;
	}

	/** Shaderのデータサイズのゲッター */
	inline long GetSize()
	{
		return m_Size;
	}

	/**
	* @brief シェーダ作成関数@n
	* 指定されたファイルからシェーダを作成する@n
	* 継承先では各シェーダを作成するように実装する
	* @return 作成結果 成功(true)
	* @param[in] device ID3D11Device
	* @param[in] file_name 読み込み対象のファイル名
	*/
	virtual bool Create(ID3D11Device* device, const char*file_name) = 0;
protected:

	/**
	* @brief シェーダ読み込み関数@n
	* 指定されたファイルを読み込む@
	* ファイルはcso(コンパイル済み)を対象としているので@n
	* hlslファイルを読み込むとエラーとして扱う
	* @return 読み込み結果 成功(true)
	* @param[in] file_name 読み込み対象のファイル名
	*/
	bool Load(const char*file_name);

protected:
	char* m_Data;	//!< @brief シェーダデータ(バイナリ)
	long m_Size;	//!< @brief データのサイズ
};


//シェーダー読み込み用　（使いやすい形に加工する）
class VertexShader : public ShaderBase
{
public:
	/** Constructor */
	VertexShader() :
		m_Interface(nullptr)
	{}

	/** Destructor */
	virtual ~VertexShader()
	{
		if (m_Interface != nullptr)
		{
			m_Interface->Release();
			m_Interface = nullptr;
		}
	}

	/**
	* @brief VertexShader作成関数@n
	* 指定されたファイルからVertexShaderを作成する@n
	* @return 作成結果 成功(true)
	* @param[in] device ID3D11Device
	* @param[in] file_name 読み込み対象のファイル名
	*/
	virtual bool Create(ID3D11Device* device, const char* file_name)
	{
		// ファイル読み込み
		if (Load(file_name) == false)
		{
			return false;
		}

		// VertexShader作成
		if (FAILED(device->CreateVertexShader(
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
	inline ID3D11VertexShader* GetShaderInterface()
	{
		return m_Interface;
	}
private:
	ID3D11VertexShader* m_Interface;		//!< @brief VertexShaderInterface
};
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
	virtual bool Create(ID3D11Device* device, const char* file_name)
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
	ID3D11GeometryShader* m_Interface;		//!< @brief GeometryShaderInterface
};
class PixelShader : public ShaderBase
{
public:
	/** Constructor */
	PixelShader() :
		m_Interface(nullptr)
	{}

	/** Destructor */
	virtual ~PixelShader()
	{
		if (m_Interface != nullptr)
		{
			m_Interface->Release();
			m_Interface = nullptr;
		}
	}

	/**
	* @brief PixelShader作成関数@n
	* 指定されたファイルからPixelShaderを作成する@n
	* @return 作成結果 成功(true)
	* @param[in] device ID3D11Device
	* @param[in] file_name 読み込み対象のファイル名
	*/
	virtual bool Create(ID3D11Device* device, const char* file_name)
	{
		// シェーダの読み込み
		if (Load(file_name) == false)
		{
			return false;
		}

		// PixelShaderの作成
		if (FAILED(device->CreatePixelShader(
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

	/** PixelShaderInterfaceのゲッター */
	inline ID3D11PixelShader* GetShaderInterface()
	{
		return m_Interface;
	}
private:
	ID3D11PixelShader* m_Interface;			//!< @brief PixelShaderInterface
};
#endif
