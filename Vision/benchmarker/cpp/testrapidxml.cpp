#include <iostream>
#include <sstream>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>

using namespace rapidxml;

int 
parse(char * xml) {
	
	
	

    //Parse the original document

    rapidxml::xml_document<> doc;
    doc.parse<0>(xml);
    std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";
    xml_node<> *node = doc.first_node()->first_node();
    xml_node<> *node2 = doc.first_node()->last_node()->first_node()->first_node();
    std::cout << "Name of my first node is: " << node->name() << "\n";
    std::cout << "Name of my first node is: " << node2->name() << "\n" << "value" << node2->value() << std::endl;
    node2=node2->next_sibling();
    std::cout << "Name of my first node is: " << node2->name() << "\n" << "value" << node2->value() << std::endl;
	

	xml_attribute<> *atr=node->first_attribute();
	std::cout << "Name of my first node is: " << atr->name() << "\n" << "value" << atr->value() << std::endl;

    //Insert something
    /*rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, "author", "John Doe");
    doc.first_node()->append_node(node);*/
    
    

    std::stringstream ss;
    ss <<*doc.first_node();
    std::string result_xml = ss.str();
    //std::cout <<result_xml<<std::endl;
    
    return 0;
}


// read contents of file "bla" into buf, discarding newlines
int main(int argc, char* argv[]) {
	
std::string buf;
std::string line;
std::ifstream in("test.xml");

while(std::getline(in,line))
	buf += line;
	
char * xml=(char*)malloc(buf.length()+1);

strcpy(xml,buf.c_str());

parse(xml);
return 0;
}

