// CalculatorParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <memory>
#include "lex.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"
#include "svm_codes.h"
#include "svm.hpp"
#include "codegen.h"

int main()
{
	using namespace lex;
	using namespace parser;
	using namespace runtime::StackVM;

	fstream fs;
	fs.open("source.txt", fstream::in);
	Lexer lexer(fs);

	Parser parser(lexer);
	parser.parse();
	fs.close();

	if (!parser.hasError())
	{
		codegen::CodeGen cg(parser);
		cg.generate();

		auto state = make_unique<State>(cg.pack.variablesSize, cg.pack.constant);
		auto nvm = make_unique<StackVM<vector<Instruction>::iterator> >(cg.pack.instructions.begin());
		nvm->setState(state.get());
		nvm->execute();
	}
	else
	{
		for (auto i = parser.getMessages().begin(); i != parser.getMessages().end(); ++i)
		{
			std::cout << *i;
		}
	}




	string ch;
	cin >> ch;
    return 0;
}

