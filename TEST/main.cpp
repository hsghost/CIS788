#include <iostream>
#include <regex>

void main()
{
	char exp[64] = "Num_Cells=1600";
	char *out = exp;
	//char exp[32] = "";

	if (std::regex_match(exp, out, std::regex("^(Num_Cells=)+?([0-9]+)$")))
	{
		for (unsigned i = 0; i < 32; i++)
		{
			std::cout << "[" << out[i] << "]" << std::endl;
		}
	}

	std::cin.get();
}