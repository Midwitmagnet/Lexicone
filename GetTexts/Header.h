#pragma once
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <pugixml.hpp>


extern std::vector<std::pair<std::string, std::string>> indexing_vector;
size_t write_data(char* ptr, size_t size, size_t nmemb, void* userdata);
extern std::string sr_container_url;
extern std::string sr_container_node;
extern std::string filelink;
extern std::ofstream& file;
extern std::string sr_container_no;
std::ofstream& get_file();

bool downloadFile(const std::string& url, const std::string& outfilename);

std::string stripquotes(const std::string& input);

int gettexts(std::string filepath);
int find_level(std::string eid);
void set_structure(pugi::xml_node& node);
std::string handle_inline(pugi::xml_node& p_node);
void write_structure_for_nodes(std::string& eId);
void process_blockList_nodes(pugi::xml_node& blocklist_node, const std::string& intro_text, std::string& art_name);
void write_article(std::string& eId, std::string& article_text);
bool convert_data(std::string filepath, std::string graphdb_repo);

void iterate_over_file(pugi::xml_node& node);
//asd