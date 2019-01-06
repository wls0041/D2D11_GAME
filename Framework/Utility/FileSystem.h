#pragma once

//���Ͽ� ���� ���, Ȯ����, �̸� ���� �ٷ�� Ŭ����
class FileSystem
{
public:
	static const string GetFileNameFromFilePath(const string &path);
	static const string GetIntactFileNameFromFilePath(const string &path);
	static const string GetDirectoryFromFilePath(const string &path);
	static const string GetExtensionFromFilePath(const string &path);

	static const string ToUpper(const string &lower);
	static const string ToLower(const string &upper);
	static const string ToString(const wstring &wstr);
	static const wstring ToWstring(const string &str);
};