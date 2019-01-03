#pragma once
#include "ISubsystem.h"
#include "../../Resource/IResource.h"

class ResourceManager : public ISubsystem
{
public:
	ResourceManager(class Context *context);
	virtual ~ResourceManager();

	void Initialize() override;

	template<typename T> T* Load(const string &filePath);
	template<typename T> T* GetResourceFromName(const string &name);
	template<typename T> T* GetResourceFromPath(const string &path);
	template<typename T> vector<T*> GetResourcesFromType();
	vector<IResource*> GetAllResource();

	void RegisterResource(IResource *resource);
	void RegisterDirectory(const ResourceType &type, const string &directory);

private:
	typedef vector<IResource *> ResourceGroup;
	map<ResourceType, ResourceGroup> resourceGroups; //map¾È¿¡ vector
	map<ResourceType, string> resourceDirectories;
};

template<typename T>
inline T * ResourceManager::Load(const string & filePath)
{
	return NULL;
}

template<typename T>
inline T * ResourceManager::GetResourceFromName(const string & name)
{
	for (auto resource : resourceGroups[IResource::DeduceResourceType<T>()]) {
		if (resource->GetResourceName() == name) return static_cast<T*>(resource);
	}
	return nullptr;
}

template<typename T>
inline T * ResourceManager::GetResourceFromPath(const string & path)
{
	for (auto resource : resourceGroups[IResource::DeduceResourceType<T>()]) {
		if (resource->GetResourcePath() == path) return static_cast<T*>(resource);
	}
	return nullptr;
}

template<typename T>
inline vector<T*> ResourceManager::GetResourcesFromType()
{
	ResourceType type = IResource::DeduceResourceType<T>();
	vector<T*> tempResources;

	for (auto resource : resourceGroups[type]) {
		if (resource->GetResourceType() != type) continue;
		tempResources.emplace_back(static_cast<T*>(resource));
	}
	return tempResources;
}
