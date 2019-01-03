#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(Context * context)
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Initialize()
{
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
