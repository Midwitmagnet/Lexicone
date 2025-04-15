#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include "header.h"
#include <pugixml.hpp>
#include <filesystem>


extern std::string statute;
extern std::string filelink;
extern std::string graphdb_repo;
extern std::string sr_container_no;
std::string sr_container_node;
std::string sr_container_url;

std::ofstream& file = get_file();



std::ofstream& get_file() {
	static std::ofstream helper_file;
	return helper_file;
}

int find_pos_of_last_substring(std::string text, std::string substring) {
	int lastpos = -1;
	int pos = 0;
	while (pos != std::string::npos) {
		lastpos = pos;
		pos = text.find(substring, +1);
	}
	return lastpos;
}

bool convert_data(std::string filepath, std::string graphdb_repo){
	std::cout << filepath << "\n";

	std::cout << graphdb_repo << "\n";


	sr_container_node = "<http://lexicone.com/containernode/" + sr_container_no;

	sr_container_url = "<http://lexicone.com/containernode/" + sr_container_no+"/";

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filelink.c_str());
	pugi::xml_node root = doc.document_element();
	if (root) {
		std::cout << "rootnaame is " << root.name()<<"\n";
	}
	else {
		std::cout << "failed to parse";
	}

	std::string eraserstring = ".xml";
	size_t position = statute.find(eraserstring);
	if (position != std::string::npos) {
		statute.erase(position, eraserstring.length());
	}

	get_file().open(graphdb_repo + "\\" + statute + ".ttl");
	std::cout << graphdb_repo + "\\" + statute + ".ttl \n";

	if (file.is_open()) {
		std::cout << "file_is_open \n";
		file << "@prefix ex: <http://lexicone.com/data#> .\n";
		file << "@prefix : <http://lexicone.com/schema#> .\n";
		file << sr_container_node << "> a ex:containernode .\n";
		iterate_over_file( root);

	}
	else {
		std::cout << "file is not open";
	}
	file.close();
	return true;

}