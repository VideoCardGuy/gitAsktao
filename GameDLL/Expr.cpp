#include "stdafx.h"
#include "Expr.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLEchoException.h>
#include "GameStruct.h"
#include "Monster.h"
#include "ObjectFunction.h"

#define _SELF L"Expr.cpp"
CExpr::CExpr()
{

}

CExpr::~CExpr()
{
	Release();
}

VOID CExpr::Release()
{

}

std::vector<MyTools::ExpressionFunPtr>& CExpr::GetVec()
{
	static std::vector<MyTools::ExpressionFunPtr> Vec =
	{
		{ std::bind(&CExpr::Help,this, std::placeholders::_1),L"Help" },
		{ std::bind(&CExpr::PrintUi,this, std::placeholders::_1),L"PrintUi" },
		{ std::bind(&CExpr::Test,this, std::placeholders::_1),L"Test" },
		{ std::bind(&CExpr::WatchNewUi,this, std::placeholders::_1),L"WatchNewUi" },
	};

	return Vec;
}

VOID CExpr::Help(CONST std::vector<std::wstring>&)
{
	auto& Vec = GetVec();
	for (CONST auto& itm : Vec)
		LOG_C_D(L"MethodName=%s", itm.wsFunName.c_str());
}

VOID TraverseUi(_In_ DWORD dwNode, _Out_ std::vector<DWORD>& Vec)
{
	DWORD dwLinkTableHead = ReadDWORD(dwNode + 0x1E4);
	for (DWORD dwObjectAddr = ReadDWORD(dwLinkTableHead);;dwObjectAddr = ReadDWORD(dwObjectAddr))
	{
		if (dwObjectAddr == dwLinkTableHead)
			break;

		if (ReadDWORD(dwObjectAddr + 0x8) == 0)
			break;

		Vec.push_back(dwObjectAddr);
	}
}

VOID CExpr::PrintUi(CONST std::vector<std::wstring>&)
{
	std::vector<DWORD> Vec;
	DWORD dwRoot = ReadDWORD(UI遍历基址);
	TraverseUi(dwRoot, Vec);
	
	LOG_C_D(L"Vec.size=%d", Vec.size());
	for (auto& itm : Vec)
	{
		DWORD dwObject = ReadDWORD(itm + 0x8);
		CHAR* pszText = reinterpret_cast<CHAR*>(dwObject + 0x94);
		std::wstring wsUiName = MyTools::CCharacter::ASCIIToUnicode(pszText);
		LOG_C_D(L"itm.Show=%d, Obj=%X,Name=%s", ReadBYTE(dwObject + 0x40), dwObject, wsUiName.c_str());


		std::vector<DWORD> VecObject;
		TraverseUi(dwObject, VecObject);
		for (auto& ChildUiObject : VecObject)
		{
			std::wstring wsChildUiName = MyTools::CCharacter::ASCIIToUnicode(reinterpret_cast<CHAR*>(ReadDWORD(ChildUiObject + 8) + 0x94));
			LOG_C_D(L"%s->[Base=%X,Obj=%X, Name=%s]", wsUiName.c_str(), ChildUiObject, ReadDWORD(ChildUiObject + 8), wsChildUiName.c_str());
		}
	}
}

VOID CExpr::WatchNewUi(CONST std::vector<std::wstring>& VecParam)
{
	static std::map<std::wstring, CGameUi> VecGameUi;
	if (VecParam.size() == 0)
	{
		LOG_C_D(L"WatchNewUi(0|1)");
		return;
	}

	if (VecParam.at(0) == L"0")
	{
		VecGameUi.clear();
		CObjectFunction::GetInstance().GetGameUiList(VecGameUi);
		LOG_C_D(L"CurrentUi.count=%d", VecGameUi.size());
	}
	else
	{
		std::map<std::wstring, CGameUi> tmpVecGameUi;
		CObjectFunction::GetInstance().GetGameUiList(tmpVecGameUi);

		// 判断新增和消逝的UI
		LOG_C_D(L"----------------新增的UI-------------------------");
		for (auto& NewGameUi : tmpVecGameUi)
		{
			auto itr = VecGameUi.find(NewGameUi.first);
			if (itr == VecGameUi.end())
			{
				LOG_C_D(L"NewUi.Name=%s, Obj=%X", NewGameUi.second.GetName().c_str(),NewGameUi.second.GetObj());
			}
		}

		LOG_C_D(L"----------------消逝的UI-------------------------");
		for (auto& OldGameUi : VecGameUi)
		{
			auto itr = tmpVecGameUi.find(OldGameUi.first);
			if (itr == tmpVecGameUi.end())
			{
				LOG_C_D(L"DesktoryUi.Name=%s, Obj=%X", OldGameUi.second.GetName().c_str(), OldGameUi.second.GetObj());
			}
		}
		VecGameUi.clear();
	}
}

VOID PrintEqui(DWORD dwAddr)
{
	std::queue<DWORD> QueNode;
	QueNode.push(ReadDWORD(ReadDWORD(dwAddr + 0x4) + 0x4));

	int nCount = 0;
	while (!QueNode.empty() && ++nCount < 1000)
	{
		dwAddr = QueNode.front();
		QueNode.pop();

		// 广度优先   深度优先怕递归……
		if (ReadBYTE(dwAddr + 0x45) == 0)
		{
			QueNode.push(ReadDWORD(dwAddr + 0x0));
			QueNode.push(ReadDWORD(dwAddr + 0x8));

			if (ReadBYTE(dwAddr + 0x10) != 0 && ReadBYTE(dwAddr + 0x10 + 0x10) != 0 && ReadBYTE(dwAddr + 0x10 + 0x10) < 32 && ReadBYTE(dwAddr + 0x2C) != 0)
			{
				CONST CHAR* pszKey = ReadBYTE(dwAddr + 0x10 + 0x14) == 0xF ? reinterpret_cast<CONST CHAR*>(dwAddr + 0x10) : reinterpret_cast<CONST CHAR*>(ReadDWORD(dwAddr + 0x10));
				CONST CHAR* pszValue = ReadBYTE(dwAddr + 0x2C + 0x14) == 0xF ? reinterpret_cast<CONST CHAR*>(dwAddr + 0x2C) : reinterpret_cast<CONST CHAR*>(ReadDWORD(dwAddr + 0x2C));
				LOG_C_D(L"dwAddr=%X,pszKey=%s,pszValue=%s", dwAddr, MyTools::CCharacter::ASCIIToUnicode(pszKey).c_str(), MyTools::CCharacter::ASCIIToUnicode(pszValue).c_str());
			}
		}
	}
}

VOID CExpr::Test(CONST std::vector<std::wstring>&)
{
	/*std::vector<CMonster> VecMonster;
	CObjectFunction::GetInstance().GetVecMonster(VecMonster);
	for (auto& itm : VecMonster)
	{
		LOG_C_D(L"NodeBase=%X, Index=%d, 怪物ID=%X, Name=%s",itm.GetNodeBase(), itm.GetIndex(), itm.GetId(), itm.GetName().c_str());
	}*/

	DWORD dwHead = ReadDWORD(ReadDWORD(ReadDWORD(背包基址) + 0x14 + 0x4) + 0x4);

	std::queue<DWORD> QueNode;
	QueNode.push(dwHead);

	int nCount = 0;
	while (!QueNode.empty() && ++nCount < 1000)
	{
		auto dwAddr = QueNode.front();
		QueNode.pop();


		// +10 = Object
		LOG_C_D(L"dwAddr=%X, +C=%d, +10=%X", dwAddr, ReadDWORD(dwAddr + 0xC), ReadDWORD(dwAddr + 0x10));
		PrintEqui(ReadDWORD(dwAddr + 0x10) + 0x4 + 0x4);


		// 广度优先   深度优先怕递归……
		if (ReadBYTE(dwAddr + 0x15) == 0)
		{
			QueNode.push(ReadDWORD(dwAddr + 0x0));
			QueNode.push(ReadDWORD(dwAddr + 0x8));
		}
	}
}
