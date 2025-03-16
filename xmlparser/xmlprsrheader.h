
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
