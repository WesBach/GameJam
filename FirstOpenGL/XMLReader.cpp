#include "XMLReader.h"

void XMLReader::load_languages_from_xml(std::wstring input_xml_file) {
	//TODO:: set up xml to have 5 different languages
	//Read the languages in 
	//10 items for the "menu"
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(input_xml_file.c_str());
	pugi::xml_node_iterator it = doc.children().begin();
	for (it; it != doc.children().end(); it++)
	{
		pugi::xml_node_iterator it2 = it->children().begin();
		for (it2; it2 != it->children().end(); it2++) {
			//fprintf(stdout, "Node name: %s\n", it2->name());
			pugi::xml_node_iterator it3 = it2->children().begin();
			for (it3; it3 != it2->children().end(); it3++) {
				languageMap[it2->first_attribute().value()].push_back(it3->child_value());
			}
		}
	}
}

