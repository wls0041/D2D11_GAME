#include "stdafx.h"
#include "FileSystem.h"

/*
find()  - �־��� ���ڿ��� �����ϴ� ��ġ(������)
rfind() - �־��� ���ڿ��� �����ϴ� ��ġ(������)
find_first_of() - �־��� ���� �� �ϳ��� �ɸ��� ù ��° ��ġ
find_last_of()  - �־��� ���� �� �ϳ��� �ɸ��� ������ ��ġ
find_first_not_of() - �־��� ���ڵ��� �ƴ� ���ڰ� �ϳ��� �ɸ��� ù ��° ��ġ
find_last_not_of()  - �־��� ���ڵ��� �ƴ� ���ڰ� �ϳ��� �ɸ��� ������ ��ġ
*/

const string FileSystem::GetFileNameFromFilePath(const string & path)
{ 
	//c:\\SGA\\2D\\Tree.png  ---> Tree.png
	auto lastIndex = path.find_last_of("\\/"); // '\'�� �ϳ��� ��ɾ� ->'\\'�� ��� �ϳ��� \�� �ν���
	auto fileName = path.substr(lastIndex + 1, path.length());

	return fileName;
}

const string FileSystem::GetIntactFileNameFromFilePath(const string & path)
{
	//Tree.png  ---> Tree
	auto fileName = GetFileNameFromFilePath(path);
	auto lastIndex = fileName.find_last_of('.');
	auto intactFileName = path.substr(0, lastIndex);

	return intactFileName;
}

const string FileSystem::GetDirectoryFromFilePath(const string & path)
{
	//c:\\SGA\\2D\\Tree.png  ---> c:\\SGA\\2D\\ 
	auto lastIndex = path.find_last_of("\\/");
	auto directory = path.substr(0, lastIndex + 1);
	
	return directory;
}

const string FileSystem::GetExtensionFromFilePath(const string & path)
{
	//c:\\SGA\\2D\\Tree.png  ---> .png
	auto lastIndex = path.find_last_of('.');
	if (lastIndex != string::npos) return path.substr(lastIndex, path.length());

	return "";
}

const string FileSystem::ToUpper(const string & lower)
{
	string upper = "";
	for (const auto &character : lower) upper += toupper(character);
	
	return upper;
}

const string FileSystem::ToLower(const string & upper)
{
	string lower = "";
	for (const auto &character : upper) lower += tolower(character);

	return lower;
}

const string FileSystem::ToString(const wstring & wstr)
{
	string result = "";
	result.assign(wstr.begin(), wstr.end());
	
	return result;
}

const wstring FileSystem::ToWstring(const string & str)
{
	wstring result = L"";
	result.assign(str.begin(), str.end());
	
	return result;
}
