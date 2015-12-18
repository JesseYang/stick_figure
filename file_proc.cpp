#include "file_proc.h"

namespace file_proc {
	std::string getPathByName(std::string name, file_type type)
	{
		std::stringstream ss;
		if (type == STD_FEATURE) {
			ss << STD_FEATURE_DIR << name;
		}
		else if (type == STD_FIG) {
			ss << STD_FIG_DIR << name << ".jpg";
		}
		else if (type == TEST_FIG) {
			ss << TEST_FIG_DIR << name << ".jpg";
		}
		return ss.str();
	}

	std::string readFile(std::string fileName) {
		std::stringstream ss;

		std::fstream f(fileName);
		if (f.is_open()) {
			std::string line;
			while (getline(f, line)) {
				ss << line << std::endl;
			}

			f.close();
		}

		return ss.str();
	}
}
