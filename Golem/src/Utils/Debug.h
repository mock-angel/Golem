/*
 * Debug.h
 *
 *  Created on: 10-May-2022
 *      Author: anantha
 */

#ifndef UTILS_DEBUG_H_
#define UTILS_DEBUG_H_

#include <string>
#include <iostream>
namespace Debug
{
	// void log(std::string text);
	// void log(std::string first, std::string second);

	template <class... Args>
	void log(Args... args)
	{
		(std::cout << ... << args) << "\n";
	}
} // namespace Debug

#endif /* UTILS_DEBUG_H_ */
