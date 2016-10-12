#pragma once

#include "stdafx.h";
using namespace std;

struct Token
{
	string type;
	string text;
	size_t strNumber;
};

typedef vector<Token> Tokens;

class CAnalisator
{
public:
	CAnalisator::CAnalisator(string const & operatorsPath,
		string const & mathOperatorsPath,
		string const & separatorsPath,
		string const & variblesPath,
		string const & controlSignsPath);

	~CAnalisator();

	enum Type
	{
		MathOperators,
		UnaryMath,
		BinaryMath,
		Operators,
		Separators,
		VarTypes,
		Constants,
		Identificators,
		ControlSigns,
		Errors
	};

	enum ConstType
	{
		FloatConst,
		IntConst,
		ErrConst
	};

	bool Init(string const &path, Type type);
	bool LoadLexem(string const & operatorsPath,
		string const & mathOperatorsPath,
		string const & separatorsPath,
		string const & variblesPath,
		string const & controlSignsPath);
	void AddToken(string const &text, string const &type);

	bool IsId(string const &str) const;
	bool IsSeparator(string const &str) const;
	bool IsVarType(string const &str) const;
	bool IsMathOperator(string const &str) const;
	bool IsOperator(string const &str);
	bool IsControlSign(string const &str);
	bool IsConst(string const &str) const;
	bool IsDigit(char ch) const;
	bool IsString();
	bool IsBinaryMathOperator(string str, string &res);
	bool IsChar();
	bool IsItAllConst(string number);
	void IsComment();
	bool DefineToken(string const &str);
	void Scan();

	Tokens GetTokens();
	//size_t GetCurrNumStr() const;
	//string GetCurrString() const;
	//size_t GetCurrPosInStr() const;

	void SkipTab();
	void WriteTokens(string const &outputName);
	void AddSpaceToString();
	void SkipManyComment();
	void SkipSingleComment();


private:
	size_t m_currNumStr;
	size_t m_currPositionStr;
	string m_currStr;
	string m_currElem;
	Tokens m_tokens;

	map<string, string> m_mathOperators;
	map<string, string> m_operators;
	map<string, string> m_separators;
	map<string, string> m_varType;
	map<string, string> m_controlSigns;
};
