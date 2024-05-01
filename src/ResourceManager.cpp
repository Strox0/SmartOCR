#include "ResourceManager.h"
#include <Windows.h>
#include <memory>

ResourceManager::ResourceManager(int resc_id, const wchar_t* resc_type) : m_id(resc_id), m_type(resc_type)
{
	if (GetResource())
		m_error = true;
}

ResourceManager::~ResourceManager()
{
	if (m_raii && !m_error)
		Free();
}

void* ResourceManager::Get()
{
	return mp_resc;
}

unsigned long ResourceManager::Size()
{
	return m_size;
}

bool ResourceManager::Error()
{
	return m_error;
}

void ResourceManager::Free()
{
	free(mp_resc);
}

void ResourceManager::Owned(bool state)
{
	m_raii = state;
}

bool ResourceManager::GetResource()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	if (hModule == NULL)
		return true;

	HRSRC hRes = FindResourceW(hModule, MAKEINTRESOURCEW(m_id), m_type);
	if (hRes == NULL)
	{
		DWORD err = GetLastError();
		return true;
	}

	HGLOBAL hMem = LoadResource(NULL, hRes);
	if (hMem == NULL)
		return true;

	DWORD dwSize = SizeofResource(NULL, hRes);
	LPCVOID lpResc = LockResource(hMem);
	if (lpResc == nullptr)
		return true;

	mp_resc = malloc(dwSize);
	if (mp_resc == nullptr)
		return true;
	memcpy(mp_resc, lpResc, dwSize);
	m_size = dwSize;

	return false;
}
