#include "SoyString.h"
#include <regex>
#include <sstream>
#include "array.hpp"
#include "bufferarray.hpp"
#include "heaparray.hpp"
#include "SoyDebug.h"
#include <regex>
#include "RemoteArray.h"



void Soy::StringToLower(std::string& String)
{
	std::transform( String.begin(), String.end(), String.begin(), ::tolower );
}

std::string Soy::StringToLower(const std::string& String)
{
	std::string LowerString = String;
	StringToLower( LowerString );
	return LowerString;
}


bool Soy::StringContains(const std::string& Haystack, const std::string& Needle, bool CaseSensitive)
{
	if (CaseSensitive)
	{
		//	gr: may need to see WHY this doesn't return npos....
		if ( Needle.empty() )
			return false;
		return (Haystack.find(Needle) != std::string::npos);
	}
	else
	{
		std::string HaystackLow = Haystack;
		std::string NeedleLow = Needle;
		Soy::StringToLower( HaystackLow );
		Soy::StringToLower( NeedleLow );
		return StringContains(HaystackLow, NeedleLow, true);
	}
}


bool Soy::StringMatches(const std::string& Haystack, const std::string& Needle, bool CaseSensitive)
{
	if (CaseSensitive)
	{
		return Haystack == Needle;
	}
	else
	{
		//	do a quick length check before switching to lowercase
		if ( Haystack.length() != Needle.length() )
			return false;
		std::string HaystackLow = Haystack;
		std::string NeedleLow = Needle;
		Soy::StringToLower( HaystackLow );
		Soy::StringToLower( NeedleLow );
		return StringMatches(HaystackLow, NeedleLow, true);
	}
}

bool Soy::StringBeginsWith(const std::string& Haystack, const std::string& Needle, bool CaseSensitive)
{
	if (CaseSensitive)
	{
		return (Haystack.find(Needle) == 0 );
	}
	else
	{
		std::string HaystackLow = Haystack;
		std::string NeedleLow = Needle;
		Soy::StringToLower( HaystackLow );
		Soy::StringToLower( NeedleLow );
		return StringBeginsWith(HaystackLow, NeedleLow, true);
	}
}


bool Soy::StringEndsWith(const std::string& Haystack, const std::string& Needle, bool CaseSensitive)
{
	std::regex_constants::syntax_option_type Flags = std::regex_constants::ECMAScript;
	if ( CaseSensitive )
		Flags |= std::regex::icase;
	
	//	escape some chars...
	std::stringstream Pattern;
	Pattern << ".*";
	for ( auto it=Needle.begin();	it!=Needle.end();	it++ )
	{
		auto Char = *it;
		if ( Char == '.' )
			Pattern << "\\";
		Pattern << Char;
	}
	Pattern << "$";
	
	std::smatch Match;
	std::string HaystackStr = Haystack;
	std::regex Regex( Pattern.str(), Flags );
	if ( std::regex_match( HaystackStr, Match, Regex ) )
		return true;

	return false;
}

std::string	Soy::StringJoin(const std::vector<std::string>& Strings,const std::string& Glue)
{
	//	gr: consider a lambda here?
	std::stringstream Stream;
	for ( auto it=Strings.begin();	it!=Strings.end();	it++ )
	{
		Stream << (*it);
		auto itLast = Strings.end();
		itLast--;
		if ( it != itLast )
			Stream << Glue;
	}
	return Stream.str();
}



std::string Soy::ArrayToString(const ArrayBridge<char>& Array)
{
	std::stringstream Stream;
	ArrayToString( Array, Stream );
	return Stream.str();
}

void Soy::ArrayToString(const ArrayBridge<char>& Array,std::stringstream& String)
{
	String.write( Array.GetArray(), Array.GetSize() );
}

void Soy::StringToArray(std::string String,ArrayBridge<char>& Array)
{
	auto CommandStrArray = GetRemoteArray( String.c_str(), String.length() );
	Array.PushBackArray( CommandStrArray );
}

void Soy::StringSplitByString(ArrayBridge<std::string>& Parts,const std::string& String,const std::string& Delim,bool IncludeEmpty)
{
	std::string::size_type Start = 0;
	while ( Start < String.length() )
	{
		auto End = Parts.IsFull() ? String.length() : String.find( Delim, Start );
		if ( End == std::string::npos )
			End = String.length();
		
		auto Part = String.substr( Start, End-Start );
		//std::Debug << "found [" << Part << "] in [" << String << "]" << std::endl;

		if ( IncludeEmpty || !Part.empty() )
		{
			//	gr: better pre-emptive full required!
			if ( Parts.IsFull() )
			{
				Parts.GetBack() += Part;
			}
			else
			{
				Parts.PushBack( Part );
			}
		}
		
		Start = End+Delim.length();
	}
}

void Soy::StringSplitByString(ArrayBridge<std::string>&& Parts,const std::string& String,const std::string& Delim,bool IncludeEmpty)
{
	StringSplitByString( Parts, String, Delim, IncludeEmpty );
}


bool Soy::StringSplitByString(std::function<bool(const std::string&)> Callback,const std::string& String,const std::string& Delim,bool IncludeEmpty)
{
	std::string::size_type Start = 0;
	while ( Start < String.length() )
	{
		auto End = String.find( Delim, Start );
		if ( End == std::string::npos )
			End = String.length();
		
		auto Part = String.substr( Start, End-Start );
		
		if ( IncludeEmpty || !Part.empty() )
		{
			if ( !Callback(Part) )
				return false;
		}
		
		Start = End+Delim.length();
	}
	
	return true;
}


std::string Soy::StreamToString(std::stringstream&& Stream)
{
	return Stream.str();
}

std::string Soy::StreamToString(std::ostream& Stream)
{
	std::stringstream TempStream;
	TempStream << Stream.rdbuf();
	return TempStream.str();
}

bool Soy::StringTrimLeft(std::string& String,char TrimChar)
{
	bool Changed = false;
	std::Debug << __func__ << " (" << String <<"," << TrimChar << ")" << std::endl;
	while ( !String.empty() )
	{
		if ( String[0] != TrimChar )
			break;
		String.erase( String.begin() );
		Changed = true;
	}
	std::Debug << __func__ << " ... " << String << std::endl;
	return Changed;
}

bool Soy::StringTrimRight(std::string& String,const ArrayBridge<char>& TrimChars)
{
	bool Changed = false;
	while ( !String.empty() )
	{
		auto tail = String.back();

		if ( !TrimChars.Find(tail) )
			break;

		String.pop_back();
	}
	return Changed;
}

void Soy::StringSplitByMatches(ArrayBridge<std::string>& Parts,const std::string& String,const std::string& MatchingChars,bool IncludeEmpty)
{
	//	gr; I think this is the only case where we erroneously return something
	//		if removed, and we split "", then we get 1 empty case
	if ( String.empty() )
		return;
	
	//	gr: dumb approach as regexp is a little fiddly
	std::stringstream Pending;
	for ( int i=0;	i<=String.length();	i++ )
	{
		bool Eof = ( i >= String.length() );
		
		if ( !Eof && MatchingChars.find(String[i]) == MatchingChars.npos )
		{
			Pending << String[i];
			continue;
		}
		
		//	pop part
		auto Part = Pending.str();
		Pending.str( std::string() );
		
		if ( !IncludeEmpty && Part.empty() )
			continue;
		
		Parts.PushBack( Part );
	}
}

void Soy::StringSplitByMatches(ArrayBridge<std::string>&& Parts,const std::string& String,const std::string& MatchingChars,bool IncludeEmpty)
{
	StringSplitByMatches( Parts, String, MatchingChars, IncludeEmpty );
}
	
void Soy::SplitStringLines(ArrayBridge<std::string>& StringLines,const std::string& String)
{
	StringSplitByMatches( StringLines, String, "\n\r", false );
}
	
void Soy::SplitStringLines(ArrayBridge<std::string>&& StringLines,const std::string& String)
{
	SplitStringLines( StringLines, String );
}

bool Soy::IsUtf8String(const std::string& String)
{
	for ( int i=0;	i<String.length();	i++ )
	{
		char c = String[i];
		if ( !IsUtf8Char( c ) )
			return false;
	}
	return true;
}

bool Soy::IsUtf8Char(char c)
{
	switch (static_cast<unsigned char>(c) )
	{
		case 0xC0:
		case 0xC1:
		case 0xF5:
		case 0xF6:
		case 0xF7:
		case 0xF8:
		case 0xF9:
		case 0xFA:
		case 0xFB:
		case 0xFC:
		case 0xFD:
		case 0xFE:
		case 0xFF:
			return false;
	}
	return true;
}
