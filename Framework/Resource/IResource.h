#pragma once
#include "stdafx.h"

enum class ResourceType : uint { Unknown, Texture, Shader, Mesh, Animation, Audio, Material };

class IResource //모든 리소스를 가지고 필요로 하는 클래스에 상속
{
public:
	template<typename T>
	static ResourceType DeduceResourceType();

public:
	IResource(class Context *context) : context(context) {

	}
	virtual ~IResource() {}

	const string &GetResourceName() const { return name; }
	const string &GetResourcePath() const { return filePath; }
	const ResourceType &GetResourceType() const { return resourceType; }
	
	void SetResourceName(const string &name) { this->name = name; }
	void SetResourcePath(const string &filePath) { this->filePath = filePath; }
	void SetResourceType(const ResourceType &type) { this->resourceType = type; }

	virtual void SaveToFile(const string &filePath) = 0;
	virtual void LoadFromFile(const string &filePath) = 0;


protected:
	class Context *context;
	
	string name;
	string filePath;
	ResourceType resourceType;
};

template<typename T>
inline ResourceType IResource::DeduceResourceType()
{
	string id = typeid(T).name(); //T타입 이름을 끄집어냄 ex)반환값 Class String
	ResourceType enumType = ResourceType::Unknown;

	if (id.find("Texture") != string::npos) enumType = ResourceType::Texture;
	else if (id.find("Mesh") != string::npos) enumType = ResourceType::Mesh;
	else if (id.find("Audio") != string::npos) enumType = ResourceType::Audio;
	else if (id.find("Shader") != string::npos) enumType = ResourceType::Shader;
	else if (id.find("Material") != string::npos) enumType = ResourceType::Material;
	else if (id.find("Animation") != string::npos) enumType = ResourceType::Animation;

	return enumType;
}
