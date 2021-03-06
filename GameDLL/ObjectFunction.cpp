#include "stdafx.h"
#include "ObjectFunction.h"
#include "GameUi.h"
#include <MyTools/Log.h>

#define _SELF L"ObjectFunction.cpp"
VOID CObjectFunction::AbstractMethod()
{

}

CGameUi* CObjectFunction::FindGameUi_For_StaticMap_By_MapKey(_In_ CONST std::wstring& wsKeyText)
{
	if (_MapGameUi.empty())
		RefreshStaticMapGameUi();

	std::lock_guard<std::mutex> Lock(_MtxGameUi);

	auto itr = _MapGameUi.find(wsKeyText);
	return itr == _MapGameUi.end() ? nullptr : &itr->second;
}

VOID CObjectFunction::RefreshStaticMapGameUi()
{
	std::lock_guard<std::mutex> Lock(_MtxGameUi);

	_MapGameUi.clear();
	GetGameUiList(_MapGameUi);
}

DWORD CObjectFunction::GetGameUiValue_By_MapKey(_In_ CONST std::wstring& wsKeyText, _In_ std::function<DWORD(CGameUi&)> GetValuePtr)
{
	auto pGameUi = FindGameUi_For_StaticMap_By_MapKey(wsKeyText);
	if (pGameUi == nullptr)
	{
		LOG_MSG_CF(L"UnExist Ui[%s]", wsKeyText.c_str());
		return 0;
	}

	return GetValuePtr(*pGameUi);
}

std::wstring CObjectFunction::GetGameUiText_By_MapKey(_In_ CONST std::wstring& wsKeyText, _In_ std::function<std::wstring(CGameUi&)> GetTextPtr)
{
	auto pGameUi = FindGameUi_For_StaticMap_By_MapKey(wsKeyText);
	if (pGameUi == nullptr)
	{
		LOG_MSG_CF(L"UnExist Ui[%s]", wsKeyText.c_str());
		return 0;
	}

	return GetTextPtr(*pGameUi);
}

BOOL CObjectFunction::IsShowGameUi_By_MapKey(_In_ CONST std::wstring& wsKeyText)
{
	auto pGameUi = FindGameUi_For_StaticMap_By_MapKey(wsKeyText);
	if (pGameUi == nullptr)
	{
		LOG_MSG_CF(L"UnExist Ui[%s]", wsKeyText.c_str());
		return 0;
	}

	return pGameUi->IsShow();
}

CONST std::wstring CObjectFunction::GetAttributeText_By_Key(_In_ DWORD dwAttributeTreeHead, _In_ CONST std::wstring& wsKey, _In_ BOOL bForcePass) CONST
{
	std::wstring wsValue;
	if (!FindValue_By_Key_In_GameTree(dwAttributeTreeHead, wsKey, wsValue))
	{
		if (bForcePass)
			return L"";

		FindValue_By_Key_In_GameTree(dwAttributeTreeHead, L"*", wsValue);
		LOG_MSG_CF(L"GetAttributeText_By_Key[%s] = FALSE", wsKey.c_str());
		return L"";
	}

	return wsValue;
}

DWORD CObjectFunction::GetAttributeValue_By_Key(_In_ DWORD dwAttributeTreeHead, _In_ CONST std::wstring& wsKey) CONST
{
	std::wstring wsValue;
	if (!FindValue_By_Key_In_GameTree(dwAttributeTreeHead, wsKey, wsValue))
	{
		LOG_MSG_CF(L"GetAttributeValue_By_Key[%s] = FALSE", wsKey.c_str());
		return 0;
	}

	return std::stoi(wsValue);
}
