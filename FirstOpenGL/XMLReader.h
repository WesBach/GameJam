#ifndef _XMLReader_HG_
#define _XMLReader_HG_

#define PUGIXML_WCHAR_MODE

#include <string>
#include <pugixml.hpp>
#include <map>
#include <vector>
class XMLReader {
public:
	std::map<std::wstring, std::vector<std::wstring>> languageMap;
	void load_languages_from_xml(std::wstring input_xml_file);
};


#endif // !_XMLReader_HG_