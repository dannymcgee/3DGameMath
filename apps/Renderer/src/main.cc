#include <iostream>
#include <string>

#include <fmt/format.h>


auto main() -> int
{
	std::cout << "What's your name? ";

	std::string name;
	std::getline(std::cin, name);

	fmt::print("Hello, {}!\n", name);

	return 0;
}
