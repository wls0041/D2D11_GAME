#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(Context * context) : ISubsystem(context)
{
}

ResourceManager::~ResourceManager()
{
	//map<type, vector<>> --> resourceGroups
	//pair<type, vector<>> --> resourceGroup
	for (auto resourceGroup : resourceGroups) {
		for (auto resource : resourceGroup.second) SAFE_DELETE(resource);
	}
}

void ResourceManager::Initialize()
{
	RegisterDirectory(ResourceType::Texture, "../_Assets/Texture/");
	RegisterDirectory(ResourceType::Shader, "../_Assets/Shader/");
	RegisterDirectory(ResourceType::Mesh, "../_Assets/Mesh/");
	RegisterDirectory(ResourceType::Animation, "../_Assets/Animation/");
	RegisterDirectory(ResourceType::Audio, "../_Assets/Audio/");
	RegisterDirectory(ResourceType::Material, "../_Assets/Material/");
}

vector<IResource*> ResourceManager::GetAllResource()
{
	return vector<IResource*>();
}

void ResourceManager::RegisterResource(IResource * resource)
{
	assert(resource);
	resourceGroups[resource->GetResourceType()].emplace_back(resource);
}

void ResourceManager::RegisterDirectory(const ResourceType & type, const string & directory)
{
	resourceDirectories[type] = directory;
}
