#include "CStringUtils.h"
#include <algorithm>
#include <sstream>
#include <locale.h>	// for toLower, etc. in ASCII
#include <wctype.h>	// For toLower, etc. in Unicode

//static 
std::string CStringUtils::trim( std::string &text )
{
	std::string trimmed = text;
	CStringUtils::trimLeft( trimmed );
	CStringUtils::trimRight( trimmed );
	text = trimmed;
	return trimmed;
}

//static 
std::string CStringUtils::trimLeft( std::string &text )
{
	std::string trimmed;
	bool bFoundForFirstChar = false;
	//for ( std::string::size_type index = 0; index != text.size(); index++ )
	for ( std::string::iterator itChar = text.begin(); itChar != text.end(); itChar++ )
	{
		char curChar = *itChar;
		if ( bFoundForFirstChar ) 
		{
			trimmed += curChar;
		}
		else if ( ! CStringUtils::bIsWhiteSpace( curChar ) ) 
		{
			trimmed += curChar;
			bFoundForFirstChar = true;
		}
	}
	text = trimmed;
	return trimmed;
}

//static 
std::string CStringUtils::trimRight( std::string &text )
{
	std::string trimmed;
	bool bFoundForFirstChar = false;
	//for ( std::string::size_type index = 0; index != text.size(); index++ )
	for ( std::string::reverse_iterator itChar = text.rbegin(); itChar != text.rend(); itChar++ )
	{
		char curChar = *itChar;
		if ( bFoundForFirstChar ) 
		{
			trimmed += curChar;
		}
		else if ( ! CStringUtils::bIsWhiteSpace( curChar ) ) 
		{
			trimmed += curChar;
			bFoundForFirstChar = true;
		}
	}
	std::reverse( trimmed.begin(), trimmed.end() );
	text = trimmed;
	return trimmed;
}


//static
bool CStringUtils::bIsWhiteSpace( char theChar )
{
	if ( ( theChar == ' ' ) || ( theChar == '\n' ) || ( theChar == '\t') )
	{	// TODO: Any others?
		return true;
	}
	return false;
}


// NOTE: There are WAY better (i.e. safer!) ways to do this. See MultiByteToWideChar(), etc. in windows
// But this is "good enough" for now (and works file converting just the ASCII characters)
//static
std::wstring CStringUtils::ASCIIToUnicodeQnD( const std::string &ASCIIString )
{
	std::wstringstream ss;
	for ( std::string::const_iterator itChar = ASCIIString.begin(); itChar != ASCIIString.end(); itChar++ )
	{
		char tempChar = *itChar;
		wchar_t tempCharUni = static_cast< wchar_t >( tempChar );
		ss << tempCharUni;
	}
	return ss.str();
}

// NOTE: There are WAY better (i.e. safer!) ways to do this. See MultiByteToWideChar(), etc. in windows
// But this is "good enough" for now (and works file converting just the ASCII characters)
//static
std::string CStringUtils::UnicodeToASCII_QnD( const std::wstring &UnicodeString )
{
	std::stringstream ssReturnASCII;
	for ( std::wstring::const_iterator itChar = UnicodeString.begin(); itChar != UnicodeString.end(); itChar++ )
	{
		char theChar = static_cast<char>( *itChar );
		ssReturnASCII << theChar;
	}
	return ssReturnASCII.str();
}

//	towlower() function from <wctype.h>
//static 
std::string CStringUtils::toLowerASCII( std::string &text )
{
	for ( std::string::size_type index = 0; index != text.npos; index++ )
	{
		text[index] = tolower( text[index] );
	}
	return text;
}

//static 
std::string CStringUtils::toUpperASCII( std::string &text )
{
	for ( std::string::size_type index = 0; index != text.npos; index++ )
	{
		text[index] = toupper( text[index] );
	}
	return text;
}

//static 
std::wstring CStringUtils::toLowerUnicode( std::wstring &text )
{
	for ( std::wstring::size_type index = 0; index != text.npos; index++ )
	{
		text[index] = towlower( text[index] );
	}
	return text;
}

//static 
std::wstring CStringUtils::toUpperUnicode( std::wstring &text )
{
	for ( std::wstring::size_type index = 0; index != text.npos; index++ )
	{
		text[index] = towupper( text[index] );
	}
	return text;
}

