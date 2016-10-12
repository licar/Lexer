#pragma once

#include "stdafx.h"
#include "TokenAndAnalisator.h"

using namespace std;


CAnalisator::CAnalisator(
	string const & operatorsPath,
	string const & mathOperatorsPath,
	string const & separatorsPath,
	string const & variblesPath,
	string const & controlSignsPath)
{
	m_currNumStr = 0;
	m_currPositionStr = 0;

	LoadLexem(operatorsPath,
		mathOperatorsPath,
		separatorsPath,
		variblesPath,
		controlSignsPath);
	Scan();
}

CAnalisator::~CAnalisator()
{
};

bool CAnalisator::Init(string const &path, Type type)
{
	ifstream file;
	file.open(path);

	if (!file)
	{
		return false;
	}

	while (!file.eof())
	{
		string view, lexem;
		file >> view >> lexem;

		switch (type)
		{
		case Separators:
			m_separators[view] = lexem;
			break;
		case Operators:
			m_operators[view] = lexem;
			break;
		case MathOperators:
			m_mathOperators[view] = lexem;
			break;
		case VarTypes:
			m_varType[view] = lexem;
			break;
		case ControlSigns:
			m_controlSigns[view] = lexem;
		}
	}
	return true;
}

bool CAnalisator::LoadLexem(string const & operatorsPath,
	string const & mathOperatorsPath,
	string const & separatorsPath,
	string const & variblesPath,
	string const & controlSignsPath)
{
	if (!Init(operatorsPath, Operators) ||
		!Init(separatorsPath, Separators) ||
		!Init(variblesPath, VarTypes) ||
		!Init(mathOperatorsPath, MathOperators),
		!Init(controlSignsPath, ControlSigns))
	{
		return false;
	}
	return true;
}


void CAnalisator::AddToken(string const &text, string const &type)
{
	Token token;
	token.strNumber = m_currNumStr;
	token.type = type;
	token.text = text;
	m_tokens.push_back(token);
}

void CAnalisator::WriteTokens(string const &outputName)
{
	ofstream file;
	file.open(outputName);

	for (auto token : m_tokens)
	{
		file << token.strNumber << " " <<
			token.type << " " << token.text << endl;
	}
}

bool CAnalisator::IsSeparator(string const &str) const
{
	return m_separators.find(str)
		!= m_separators.end();
}

bool CAnalisator::IsMathOperator(string const &str) const
{
	return m_mathOperators.find(str)
		!= m_mathOperators.end();
}

bool CAnalisator::IsBinaryMathOperator(string str, string &res)
{
	if (m_currNumStr + 1 == m_currStr.length())
	{
		return false;
	}

	str.push_back(m_currStr[m_currPositionStr + 1]);

	if (IsMathOperator(str))
	{
		res = str;
		return true;
	}
	return false;
}

bool CAnalisator::IsOperator(string const &str)
{
	return m_operators.find(str) != m_operators.end();
}

bool CAnalisator::IsDigit(char ch) const
{
	return (ch <= '9' && ch >= '0');
}

bool CAnalisator::IsConst(string const &str) const
{
	for (auto ch : str)
	{
		if (!IsDigit(ch))
		{
			return false;
		}
	}
	return true;
}

bool CAnalisator::IsItAllConst(string number)
{
	char const DOT = '.';
	char const SPACE = ' ';
	number += m_currStr[m_currPositionStr];

	if (!IsDigit(m_currStr[m_currPositionStr + 1]))
	{
		number.push_back(m_currStr[m_currPositionStr + 1]);
		AddToken(number, "ERROR");
		return false;
	}

	string buf;
	buf += m_currStr[m_currPositionStr + 1];

	if (IsSeparator(buf))
	{
		number.push_back(m_currStr[m_currPositionStr + 1]);
		AddToken(number, "ERROR");
		return false;
	}

	for (size_t i = m_currPositionStr + 1; i != m_currStr.length(); ++i)
	{
		buf.clear();
		buf.push_back(m_currStr[i]);

		if (m_currStr[i] == DOT)
		{
			number.push_back(m_currStr[i]);
			AddToken(number, "ERROR");
			return false;
		}

		else if (IsSeparator(buf) || m_currStr[i] == SPACE || m_currStr[i] == '\n')
		{
			AddToken(number, "CONST_FLOAT");
			m_currPositionStr = i;
			m_currElem = buf;
			return true;
		}

		else if (!(IsDigit(m_currStr[i])))
		{
			number.push_back(m_currStr[i]);
			AddToken(number, "ERROR");
			return false;
		}
		number.push_back(m_currStr[i]);
	}
	AddToken(buf, m_separators[buf]);
	return true;
}

bool CAnalisator::IsControlSign(string const &str)
{
	return m_controlSigns.find(str) != m_controlSigns.end();
}

bool CAnalisator::IsChar()
{
	string buf = m_currStr.substr(m_currPositionStr + 1, 2);

	if ((IsControlSign(buf)) && (m_currStr[m_currPositionStr + 3] == '\''))
	{
		AddToken("'" + buf + "'", "CONST_STRING");
		m_currPositionStr += 3;
		return true;
	}
	else if (m_currStr[m_currPositionStr + 2] == '\'')
	{
		string bufBuf;
		bufBuf.push_back(buf[0]);
		AddToken("'" + bufBuf + "'", "CONST_STRING");
		m_currPositionStr += 2;
		return true;
	}
	AddToken("'" + buf, "ERROR");
	return false;
}

bool CAnalisator::IsVarType(string const &str) const
{
	return m_varType.find(str)
		!= m_varType.end();
}

bool CAnalisator::IsId(string const &str) const
{
	if ((static_cast<int>(str[0]) > 64 && static_cast<int>(str[0]) < 91 || static_cast<int>(str[0]) > 96 && static_cast<int>(str[0]) < 123))
	{
		return true;
	}
	return false;
}


bool  CAnalisator::DefineToken(string const &str)
{
	if (IsMathOperator(str))
	{
		string res;
		if (IsBinaryMathOperator(str, res))
		{
			AddToken(res, m_mathOperators[res]);
			m_currPositionStr += 1;
			return true;
		}
		AddToken(str, m_mathOperators[str]);
		return true;
	}

	else if (IsVarType(str))
	{
		AddToken(str, m_varType[str]);
		return true;
	}

	else if (IsOperator(str))
	{
		if (m_currElem != ".")
		{
			AddToken(str, m_operators[str]);
			return true;
		}
		AddToken(str + ".", "ERROR");
		return false;
	}

	else if (IsConst(str))
	{
		if (m_currStr[m_currPositionStr] == '.')
		{
			if (!IsItAllConst(str))
			{
				return false;
			}
			return true;
		}
		else
		{
			AddToken(str, "CONST");
			return true;
		}
	}


	else if (IsId(str))
	{
		AddToken(str, "IDENTIFIER");
		return true;
	}

	AddToken(str, "ERROR");
	return false;
}


bool CAnalisator::IsString()
{
	for (size_t i = m_currPositionStr + 1; i != m_currStr.length(); ++i)
	{
		if (m_currStr[i] == '\"')
		{
			AddToken("\"" + m_currStr.substr(m_currPositionStr + 1,
				i - m_currPositionStr), "STRING");
			m_currPositionStr = i;
			return true;
		}
	}
	AddToken("\"" + m_currStr.substr(m_currPositionStr + 1,
		m_currStr.length() - m_currPositionStr + 1), "ERROR");
	return false;
}

void CAnalisator::AddSpaceToString()
{
	m_currStr.push_back(' ');
}

void CAnalisator::SkipTab()
{
	if (m_currStr[0] != '\t' || m_currStr.length() == 0)
	{
		return;
	}

	for (size_t i = 1; i < m_currStr.length(); ++i)
	{
		if (m_currStr[i] != '\t')
		{
			m_currStr = m_currStr.substr(i, m_currStr.length() - i);
			return;
		}
	}
}

void CAnalisator::SkipManyComment()
{
	while (!cin.eof())
	{
		size_t found = m_currStr.find("*/");

		if (found != m_currStr.npos)
		{
			m_currStr = m_currStr.substr(found + 1, m_currStr.length());
			m_currPositionStr = 0;
			return;
		}
		getline(cin, m_currStr);
		AddSpaceToString();
		SkipTab();
		++m_currNumStr;
	}
}

void CAnalisator::SkipSingleComment()
{
	//AddToken("comment", "SINGLE_Cs
	m_currPositionStr = m_currStr.length() - 1;
}


void CAnalisator::IsComment()
{
	if (m_currStr[m_currPositionStr + 1] == '/')
	{
		//AddToken("//", m_controlSigns["//"]);
		SkipSingleComment();
		return;
	}
	if (m_currStr[m_currPositionStr + 1] == '*')
	{
		//AddToken("/*", m_controlSigns["/*"]);
		SkipManyComment();
		return;
	}
	AddToken("/", m_separators["/"]);
	return;
}

void CAnalisator::Scan()
{
	string buf;
	const string SPACE = " ";
	const string STR_END = "\0";
	const string DOT = ".";

	while (!cin.eof())
	{
		++m_currNumStr;
		getline(cin, m_currStr);
		AddSpaceToString();
		SkipTab();

		buf = "";
		for (m_currPositionStr = 0; m_currPositionStr <
			m_currStr.length(); ++m_currPositionStr)
		{
			m_currElem.clear();
			m_currElem += m_currStr[m_currPositionStr];

			if (m_currElem == SPACE || m_currElem == STR_END
				|| IsSeparator(m_currElem) || IsMathOperator(m_currElem))
			{
				if (buf.length() != 0)
				{
					if (!DefineToken(buf))
					{
						return;
					}
				}

				if (m_currElem == SPACE || m_currElem == STR_END);

				else if (IsSeparator(m_currElem))
				{
					if (m_currElem == "\'")
					{
						if (!IsChar()) return;;
					}
					else if (m_currElem == "\"")
					{
						if (!IsString()) return;
					}
					else if (m_currElem == "/")
					{
						IsComment();
					}
					else
					{
						AddToken(m_currElem, m_separators[m_currElem]);
					}
				}

				else if (IsMathOperator(m_currElem))
				{
					if (!DefineToken(m_currElem)) return;
				}

				buf.clear();
			}
			else
			{
				buf += m_currElem;
			}
		}
	}
}

Tokens CAnalisator::GetTokens()
{
	return m_tokens;
}