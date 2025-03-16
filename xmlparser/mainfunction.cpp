#include <iostream>
#include <pugixml.hpp>
#include <cstring>
#include <pqxx/pqxx>
#include <cstdlib>
#include <string>
#include "xmlprsrheader.h"
#include <fstream>


using std::cout;
using std::endl;


std::string statutename;

std::unique_ptr<pqxx::connection> conn;

//to do
// discern naming: instead of art_/para/lbl/lbl - make columns with each
// handle empty articles
// handle transitional articles

int main() {

    std::string dbdata = getdbdata();
    cout << "got data \n";

    // Database connection attempt
    try {
        cout << "attempt to connect \n";
        conn = std::make_unique<pqxx::connection>(dbdata);
        if (conn->is_open()) {
            std::cout << "Connected to database: " << conn->dbname() << std::endl;
        }
        else {
            std::cerr << "Failed to connect to the database." << std::endl;
            return -1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
        return -1;
    }

    // Clear input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Type the abbreviation of the norm as it is typed in your file (fe zpo): ";
    std::cin >> statutename;
    std::string fileget;

    fileget = "C:\\lexicode\\" + statutename + ".xml";
    const char* filepath = fileget.c_str();

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileget.c_str());

    try {
        pqxx::work txn(*conn);
        std::string create_tables_query =
            "CREATE TABLE IF NOT EXISTS " + statutename + R"(_main_table (
                   id UUID DEFAULT uuid_generate_v4() PRIMARY KEY,
                   eid TEXT,
                   text TEXT,
                   futureproof TEXT
                   );
            CREATE TABLE IF NOT EXISTS )" + statutename + R"(_relations (
                  id UUID DEFAULT uuid_generate_v4() PRIMARY KEY,
                  connex_origin UUID NOT NULL,
                  connex_end UUID NOT NULL,
                  placeholder UUID NOT NULL
                  );
            CREATE TABLE IF NOT EXISTS )" + statutename + R"(_constituent_elements (
                  id UUID DEFAULT uuid_generate_v4() PRIMARY KEY,
                  connex_origin UUID NOT NULL,
                  connex_end UUID NOT NULL,
                  rank INT NOT NULL,
                  placeholder UUID NOT NULL
                  );)";

        txn.exec(create_tables_query);
        txn.commit();
        std::cout << "Tables successfully created" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }




    if (!result) {
        std::cerr << "Failed to load file: " << result.description() << "\n";
        return -1;
    }

    pugi::xml_node root = doc.document_element(); 
    if (root) {
        std::cout << "Root element name: " << root.name() << "\n";

    }
    else {
        std::cerr << "No root element found.\n";
    }
    std::string statute_main_table = statutename + "_main_table";

    get_articles(root, *conn, statute_main_table);


 
std::cin.get();
return 0;
}
