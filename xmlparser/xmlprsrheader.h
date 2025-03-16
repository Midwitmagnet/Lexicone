
#pragma once

#include <iostream>
#include <pugixml.hpp>
#include <cstring>
#include <pqxx/pqxx>
#include <cstdlib>
#include <string>



extern std::string statutename;

extern std::unique_ptr<pqxx::connection> conn;



std::string getdbdata();


void get_articles(pugi::xml_node& root, pqxx::connection& conn, std::string table_name);

void processe_blockList_nodes(pugi::xml_node& blocklist_node, const std::string& intro_text, pqxx::stream_to& stream, int& processed_count, std::string& art_name);

std::string handle_inline(pugi::xml_node& p_node);

