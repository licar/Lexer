// LexicalAnalisator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "string"
#include "vector"
#include "iostream"
#include "TokenAndAnalisator.h"

using namespace std;

void PrintResult(Tokens const &result)
{
	for (size_t i = 0; i != result.size(); ++i)
	{
		cout << result[i].strNumber << " " << result[i].text << " " << result[i].type << endl;
	}
}

int main()
{	
	CAnalisator analisator("operators.txt", "mathOperators.txt", "separators.txt",
		"var.txt", "signs.txt");
	//PrintResult(analisator.GetTokens());
	analisator.WriteTokens("Output.txt");
	system("pause");
	return 0;
}

