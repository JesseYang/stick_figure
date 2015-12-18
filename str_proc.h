#pragma once

#include <vector>
#include <sstream>

namespace str_proc {
	std::vector<std::string> &split(const std::string &, char, std::vector<std::string> &);
	std::vector<std::string> split(const std::string &, char);
}