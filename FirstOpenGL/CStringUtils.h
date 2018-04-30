#ifndef _CStringUtils_HG_
#define _CStringUtils_HG_

#include <string>

class CStringUtils
{
public:
	static std::string trim( std::string &text );
	static std::string trimLeft( std::string &text );
	static std::string trimRight( std::string &text );
	static bool bIsWhiteSpace( char theChar );
	static std::wstring ASCIIToUnicodeQnD( const std::string &ASCIIString );
	static std::string UnicodeToASCII_QnD( const std::wstring &UnicodeString );
	static std::string toLowerASCII( std::string &text );
	static std::string toUpperASCII( std::string &text );
	static std::wstring toLowerUnicode( std::wstring &text );
	static std::wstring toUpperUnicode( std::wstring &test );
};

#endif
