#include "ResourceManager.h"




void ResourceManager::SetModel(std::string keyName, FbxModel* model,bool local) {
	if (local) {
		modelLKey.push_back(keyName);
		modelL[keyName] = model;
	}
	else {
		modelGKey.push_back(keyName);
		modelG[keyName] = model;
	}
}
void ResourceManager::SetImage(std::string keyName, Image* image, bool local) {
	if (local) {
		imageLKey.push_back(keyName);
		imageL[keyName]=image;
	}
	else {
		imageGKey.push_back(keyName);
		imageG[keyName]=image;
	}

}
void ResourceManager::SetBGM(std::string keyName, Sound_MP3* mp3, bool local) {
	if (local) {
		bgmLKey.push_back(keyName);
		bgmL[keyName] = mp3;
	}
	else {
		bgmGKey.push_back(keyName);
		bgmG[keyName] = mp3;
	}
}
void ResourceManager::SetSE(std::string keyName, Sound_MP3* mp3, bool local) {
	if (local) {
		seLKey.push_back(keyName);
		seL[keyName] = mp3;
	}
	else {
		seGKey.push_back(keyName);
		seG[keyName] = mp3;
	}
}
void ResourceManager::SetShaderV(std::string keyName, VertexShaderData* vs) {
	shaderVKey.push_back(keyName);
	shaderV[keyName] = vs;
}
void ResourceManager::SetShaderG(std::string keyName, GeometryShaderData* gs) {
	shaderGKey.push_back(keyName);
	shaderG[keyName] = gs;
}
void ResourceManager::SetShaderP(std::string keyName, PixelShaderData* ps) {
	shaderPKey.push_back(keyName);
	shaderP[keyName] = ps;
}

FbxModel* ResourceManager::GetModel(std::string keyName, bool local) {
	if (local) {
		return modelL[keyName];
	}
	else {
		return modelG[keyName];
	}
}
Image* ResourceManager::GetImage(std::string keyName, bool local) {
	if (local) {
		return imageL[keyName];
	}
	else {
		return imageG[keyName];
	}
}
Sound_MP3* ResourceManager::GetBGM(std::string keyName, bool local) {
	if (local) {
		return bgmL[keyName];
	}
	else {
		return bgmG[keyName];
	}
}
Sound_MP3* ResourceManager::GetSE(std::string keyName, bool local) {
	if (local) {
		return seL[keyName];
	}
	else {
		return seG[keyName];
	}
}
VertexShaderData* ResourceManager::GetShaderV(std::string keyName) {
	return shaderV[keyName];
}
GeometryShaderData* ResourceManager::GetShaderG(std::string keyName) {
	return shaderG[keyName];
}
PixelShaderData* ResourceManager::GetShaderP(std::string keyName) {
	return shaderP[keyName];
}

void ResourceManager::AllDeleteLocal() {
	for (int i = 0; i < modelL.size(); i++) {
		delete modelL[modelLKey[i]];
		modelL.erase(modelLKey[i]);
		modelLKey[i].erase(i);
	}
	for (int i = 0; i < imageL.size(); i++) {

		delete imageL[imageLKey[i]];
		imageL.erase(imageLKey[i]);
		imageLKey[i].erase(i);
	}
	for (int i = 0; i < bgmL.size(); i++) {
		delete bgmL[bgmLKey[i]];
		bgmL.erase(bgmLKey[i]);
		bgmLKey[i].erase(i);
	}
	for (int i = 0; i < seL.size(); i++) {
		delete seL[seLKey[i]];
		seL.erase(seLKey[i]);
		seLKey[i].erase(i);
	}


}
void ResourceManager::AllDeleteGrobal() {
	for (int i = 0; i < modelG.size(); i++) {
		delete modelG[modelGKey[i]];
		modelG.erase(modelGKey[i]);
		modelGKey[i].erase(i);
	}
	for (int i = 0; i < imageG.size(); i++) {
		delete imageG[imageGKey[i]];
		imageG.erase(imageGKey[i]);
		imageGKey[i].erase(i);
	}
	for (int i = 0; i < bgmG.size(); i++) {
		delete bgmG[bgmGKey[i]];
		bgmG.erase(bgmGKey[i]);
		bgmGKey[i].erase(i);
	}
	for (int i = 0; i < seG.size(); i++) {
		delete seG[seGKey[i]];
		seG.erase(seGKey[i]);
		seGKey[i].erase(i);
	}

}
