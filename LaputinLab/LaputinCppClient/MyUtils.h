#pragma once
#include "pch.h"

template <typename T>
T GetCorrectNumber(std::string text, T min, T max)
{
	T parameter;
	std::cout << text;
	while (!(std::cin >> parameter) || parameter < min || parameter > max) {
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		std::cout << text;
	}
	return parameter;
}