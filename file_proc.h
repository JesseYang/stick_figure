#pragma once

#include <string>
#include <sstream>
#include <fstream>


namespace file_proc {
	#define STD_FIG_DIR "data/stdFigs/"
	#define STD_FEATURE_DIR "data/stdFeatures/"
	#define TEST_FIG_DIR "data/testFigs/"

	enum file_type {
		STD_FIG = 1,
		STD_FEATURE = 2,
		TEST_FIG = 3
	};
	std::string getPathByName(std::string, file_type);
	std::string readFile(std::string);
}