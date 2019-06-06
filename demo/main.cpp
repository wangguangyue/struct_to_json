// struct_to_json.cpp: 定义应用程序的入口点。
//

#include <iostream>

#include "struct_to_json.hpp"

class abc {};

int main()
{
	char str[5] = { 'a','b',0,'c' };
	const char* const astr = str;

	std::cout << to_string(str)<< std::endl;

	std::cout << to_string(astr) << std::endl;

	getchar();

	return 0;
}
