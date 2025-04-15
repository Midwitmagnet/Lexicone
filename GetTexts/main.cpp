
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include "header.h"
#include <filesystem>


//to do for DB: transitory norms, marginals, date of legality
//inference: find explicit references

namespace fs = std::filesystem;

std::string statute;
std::string filelink;
std::string base_url_article = "<http://lexicone.com/article_node/";
std::string graphdb_repo;
std::string sr_container_no;


int main() {

	std::string folder_path;
	std::cout << "enter link to folder w data \n";
	std::cin >> folder_path;

	std::cout << "enter target folder for turtle \n";
	std::cin >> graphdb_repo;

	int command = 0;

	std::cout << "download texts? 1 for yes, 2 for no\n";
	std::cin >> command;

	if (command == 1) {
		int k = gettexts(folder_path);

	}

	int test_command;
	std::cout << "press 3 for test run";
	std::cin >> test_command;



	for (const auto& entry : fs::directory_iterator(folder_path)) {

		filelink = entry.path().string();

		statute = filelink.substr(filelink.rfind("\\") + 1);
		std::cout << statute<<"\n";

		sr_container_no = statute.substr(0, statute.length() - 4);
		std::cout << sr_container_no;

		if (convert_data(filelink, graphdb_repo)) {
			std::cout << "converted "<<statute<<"\n";
		}
		else {
			std::cout << "failed to convert " << statute;
		}

		if (test_command == 3) {
			break;
		}

	}

}

