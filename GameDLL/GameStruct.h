#ifndef __GITASKTAO_GAMEDLL_CORE_COMMON_GAMESTRUCT_H__
#define __GITASKTAO_GAMEDLL_CORE_COMMON_GAMESTRUCT_H__


#define UI������ַ			0xD954E0
#define UINameƫ��			0x94
#define UI����ƫ��1			0x1E4
#define ����HPƫ��			0x1F0
#define ����MAXHPƫ��		0x1E8
#define UI�ı�ƫ��			0xC
#define UIEDITƫ��			0x1E0
#define UI����TEXTƫ��		0x29C
#define ��������ƫ��			0x16C
#define ����IDƫ��			0x8
#define ���������ַ			0xD94FE8
#define �������ƫ��			0x2A0
#define ��ǰ��ͼ��ַ			0xD95518
#define �Ƿ�ս���л�ַ		0xD9ADC4
#define ����ս���˵�UIƫ��	0x40
#define ����HPƫ��			0x3F8
#define ����MAXHPƫ��		0x3FC
#define �������Ի�ַ			0xD98F74
#define ��������ƫ��			0xE4
#define ��·��ַ				0xD930B4
#define ������ַ				0xD956FC
#define ����CALL				0x714120
#define Hook���1			0x71415B
#define �ָ�VirtualProtect  0x77A40028
#define NTDLL��ַ			0x77A20000
#define ���﹥��ID�麯��ƫ�� 0xE4
#define ս��CALL				0x4E1800



///----���������ƫ��-------------------------
#define ս�����Ƿ����ƫ��	0x54


#define ReadDWORD(Addr) MyTools::CCharacter::ReadDWORD(Addr)
#define ReadBYTE(Addr)  MyTools::CCharacter::ReadBYTE(Addr)

enum class em_VariableName
{
	NewPeekMessageAddr,
};

enum em_SkillId
{

};

struct Point
{
	DWORD X, Y;

	Point()
	{
		X = Y = NULL;
	}

	Point(_In_ DWORD dwX, _In_ DWORD dwY) : X(dwX), Y(dwY)
	{

	}

	bool operator == (CONST Point& Point_) CONST
	{
		return this->X == Point_.X && this->Y == Point_.Y;
	}

	bool operator != (CONST Point& Point_) CONST
	{
		return this->X != Point_.X || this->Y != Point_.Y;
	}
};

#endif // !__GITASKTAO_GAMEDLL_CORE_COMMON_GAMESTRUCT_H__
