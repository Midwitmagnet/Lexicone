//void artsearch(pugi::xml_node root, std::string attrstrg) {
//    for (pugi::xml_node child = root.first_child(); child != nullptr; child = child.next_sibling()) {
//        if (child.attribute("eId").value() == attrstrg) {
//            std::cout << child.child("content").child("p").child_value();
//            return;
//        }
//        artsearch(child, attrstrg);
//    }
//}
//
//void search_article(pugi::xml_node root) {
//    std::string a;
//    std::string p;
//    std::string attrstrg;
//
//    std::cout << "Enter article number (if its w. literal, enter articlenumber_lit: ";
//    std::cin >> a;
//
//    std::cout << "Enter paragraph number with _in before: ";
//    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//    std::getline(std::cin, p);
//
//    if (p.empty()) {
//        std::string attrstrg = "art_" + a;
//    }
//    else {
//
//        std::string attrstrg = "art_" + a + "/para" + p;
//    }
//
//    std::cout << attrstrg;
//
//    artsearch(root, attrstrg);
//
//}
//
//void art2sql(pugi::xml_node root) {
//    //vector(?) of structuring elements (chapters, titles etc.) - ptrs to the respective/from the respective
//    for (pugi::xml_node child = root.first_child(); child != nullptr; child = child.next_sibling()) {
//        // if (inhaltsstruktur) add join
//        if (std::string(child.name()) == "p") {
//
//            //shoot text to sql
//            //remove from vector
//            return;
//        }
//        art2sql(child);
//        //iterate through until finding a <p> node > content into sql
//        //go back to parent until you find paragraph - article pointer into sq
//        //go back further to find title etc -> pointer into sql
//    }
//}
//
//void article_query(const pugi::xml_node& queried_node, const std::string& artnumb, const std::string& parnumb) {
//    pugi::xml_node text_node = queried_node.child("p");
//
//
//    std::string query = "INSERT INTO " + statutename + " (artnumb, parnumb, text) VALUES ($1, $2, $3)";
//
//    conn->prepare("dynamic_insert", query);
//
//    std::string artname = artnumb;
//    std::string parname = parnumb;
//    std::string_view text = text_node.child_value();
//
//
//    pqxx::work tx(*conn);
//    tx.exec_prepared("dynamic_insert", artname, parname, text);
//
//    tx.commit();
//}
//
//void load_single_article(pugi::xml_node& root, std::string attrstrg, std::string artnumb, std::string parnumb) {
//    for (pugi::xml_node child = root.first_child(); child != nullptr; child = child.next_sibling()) {
//        if (child.name() == "content") {
//            pugi::xml_node helper_node = child.child("content");
//            article_query(helper_node, artnumb, parnumb);
//
//            return;
//        }
//        load_single_article(child, attrstrg, artnumb, parnumb);
//
//    }
//
//}
//
//
//
//headers stoarge
//
//void search_article(pugi::xml_node);
//
//void art2sql(pugi::xml_node);
//
//void set_environment();
//
//void create__meta__table();
//
//void single_art_sql(pugi::xml_node root);
//
//
//
//sql
//
//#include <iostream>
//#include <pugixml.hpp>
//#include <cstring>
//#include <pqxx/pqxx>
//#include <cstdlib>
//#include <string>
//#include "xmlprsrheader.h"
//#include <fstream>
//
//void article_query(const pugi::xml_node& queried_node, const std::string& artnumb, const std::string& parnumb) {
//    pugi::xml_node text_node = queried_node.child("p");
//
//
//    std::string query = "INSERT INTO " + statutename + " (artnumb, parnumb, text) VALUES ($1, $2, $3)";
//
//    conn->prepare("dynamic_insert", query);
//
//    std::string artname = artnumb;
//    std::string parname = parnumb;
//    std::string_view text = text_node.child_value();
//
//
//    pqxx::work tx(*conn);
//    tx.exec_prepared("dynamic_insert", artname, parname, text);
//
//    tx.commit();
//}
//
//
//void single_art_sql(pugi::xml_node root) {
//    std::string artnumb;
//    std::string parnumb;
//    std::string attrstrg;
//
//    std::cout << "Enter article number (if its w. literal, enter articlenumber_lit: ";
//    std::cin >> artnumb;
//
//    std::cout << "Enter paragraph number with _in before, leave empty if no paragraph: ";
//    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//    std::getline(std::cin, parnumb);
//
//    if (parnumb.empty()) {
//        std::string attrstrg = "art_" + artnumb;
//    }
//    else {
//
//        std::string attrstrg = "art_" + artnumb + "/para" + parnumb;
//    }
//
//    std::cout << attrstrg;
//
//    load_single_article(root, attrstrg, artnumb, parnumb);
//}
//
//void load_table(pugi;; xml_node root)
//
