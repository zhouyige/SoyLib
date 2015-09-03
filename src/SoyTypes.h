#pragma once

typedef	signed char			int8;
typedef	unsigned char		uint8;
typedef	signed short		int16;
typedef	unsigned short		uint16;


//	earlier as used by some TARGET_X specific smart pointers
struct NonCopyable {
	NonCopyable & operator=(const NonCopyable&) = delete;
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable() = default;
};


#if defined(_MSC_VER) && !defined(TARGET_WINDOWS)
#define TARGET_WINDOWS
#endif

#if defined(TARGET_IOS)
#include "SoyTypes_CoreFoundation.h"
#elif defined(TARGET_ANDROID)
#include "SoyTypes_Android.h"
#elif defined(TARGET_OSX)
#include "SoyTypes_CoreFoundation.h"
#elif defined(TARGET_WINDOWS)
#include "SoyTypes_Windows.h"
#else
#error no TARGET_XXX defined
#endif


//	all platforms
#include <stdio.h>
#include <assert.h>
#include <memory>
#include <string>
#include <type_traits>
#include <sstream>


//	clang(?) macro for testing features missing on android
#if !defined(__has_feature)
#define __has_feature(x)	FALSE
#endif


//	forward declarations
namespace Soy
{
	class TVersion;
}

//	array forward declaration
template<typename TYPE>
class ArrayInterface;



//	when casting integers down, get rid of warnings using this, so we can add a check later if it EVER comes up as a problem
template<typename SMALLSIZE,typename BIGSIZE>
inline SMALLSIZE size_cast(BIGSIZE Size)
{
	//	gr: do value check
	return static_cast<SMALLSIZE>( Size );
}




namespace std
{
#define DISALLOW_EVIL_CONSTRUCTORS(x)
//#include "chromium/stack_container.h"
};


#define sizeofarray(ARRAY)	( sizeof(ARRAY)/sizeof((ARRAY)[0]) )

//	set a standard RTTI macro
#if defined(__cpp_rtti) || defined(GCC_ENABLE_CPP_RTTI) || __has_feature(cxx_rtti)
#define ENABLE_RTTI
#endif




class ofFilePath
{
public:
	static std::string		getFileName(const std::string& Filename,bool bRelativeToData=true);
};



inline std::string ofToDataPath(const std::string& LocalPath,bool FullPath=false)	{	return LocalPath;	}


namespace Soy
{
	//	speed up large array usage with non-complex types (structs, floats, etc) by overriding this template function (or use the macro)
	template<typename TYPE>
	inline bool IsComplexType()	
	{
		//	default complex for classes
		return true;	
	}

	//	speed up allocations of large arrays of non-complex types by skipping construction& destruction (placement new, means data is always uninitialised)
	template<typename TYPE>
	inline bool DoConstructType()	
	{
		//	by default we want to construct classes, structs etc. 
		//	Only types with no constructor we don't want constructed for speed reallys
		return true;	
	}

	template<typename A,typename B>
	inline bool DoComplexCopy()
	{
		if ( std::is_same<A,B>::value )
			return IsComplexType<A>();
		else
			return true;
	}
	

	std::string		DemangleTypeName(const char* name);
#if !defined(ENABLE_RTTI)
	std::string		AllocTypeName();
#endif
	
	
	//	readable name for a type (alternative to RTTI) which means we don't need to use DECLARE_TYPE_NAME
	template<typename TYPE>
	inline const std::string& GetTypeName()
	{
#if defined(ENABLE_RTTI)
		static std::string TypeName = DemangleTypeName( typeid(TYPE).name() );
#else
		static std::string TypeName = AllocTypeName();
#endif
		return TypeName;
	}

	template<typename TYPE>
	inline const std::string& GetTypeName(const TYPE& Object)
	{
		return GetTypeName<TYPE>();		
	}
	

	
	//	auto-define the name for this type for use in the memory debug
#define DECLARE_TYPE_NAME(TYPE)	\
	namespace Soy \
	{	\
		template<>		\
		inline const std::string& GetTypeName<TYPE>()	{	static std::string TypeName = #TYPE;	return TypeName;	} \
	};
	
#define DECLARE_TYPE_NAME_AS(TYPE,NAME)	\
	namespace Soy \
	{	\
		template<>	\
		inline const std::string& GetTypeName<TYPE>()	{	static std::string TypeName = NAME;	return TypeName ;	} \
	};
	
	//	speed up use of this type in our arrays when resizing, allocating etc
	//	declare a type that can be memcpy'd (ie. no pointers or ref-counted objects that rely on =operators or copy constructors)
#define DECLARE_NONCOMPLEX_TYPE(TYPE)	\
	DECLARE_TYPE_NAME(TYPE)	\
	namespace Soy \
	{	\
		template<>	\
		inline bool IsComplexType<TYPE>()	{	return false;	} \
	};
	
	//	speed up allocation of this type in our heaps...
	//	declare a non-complex type that also requires NO construction (ie, will be memcpy'd over or fully initialised when required)
#define DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE(TYPE)	\
	DECLARE_NONCOMPLEX_TYPE(TYPE)	\
	namespace Soy \
	{	\
		template<>	\
		inline bool DoConstructType<TYPE>()	{	return false;	}	\
	};
	

} // namespace Soy

//	some generic noncomplex types
DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE( float );
DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE( int );
DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE( char );
DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE( int8 );
DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE( uint8 );
DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE( int16 );
DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE( uint16 );
//DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE( int32 );	//	int
DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE( uint32 );
DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE( int64 );
DECLARE_NONCOMPLEX_NO_CONSTRUCT_TYPE( uint64 );

DECLARE_TYPE_NAME_AS( std::string, "text" );



class TCrc32
{
public:
	static const uint32_t	Crc32Table[256];

public:
    TCrc32() { Reset(); }
    ~TCrc32() throw() {}
    void Reset() { _crc = (uint32_t)~0; }
    void AddData(const uint8_t* pData, const size_t length)
    {
        uint8_t* pCur = (uint8_t*)pData;
        auto remaining = length;
        for (; remaining--; ++pCur)
            _crc = ( _crc >> 8 ) ^ Crc32Table[(_crc ^ *pCur) & 0xff];
    }
    const uint32_t GetCrc32() { return ~_crc; }

private:
    uint32_t _crc;
};


namespace Soy
{
	namespace Private
	{
		uint32	GetCrc32(const char* Data,size_t DataSize);
	};
	
	template<typename TYPE>
	inline uint32	GetCrc32(const ArrayInterface<TYPE>& Array)
	{
		return Private::GetCrc32( Array.GetArray(), Array.GetDataSize() );
	}

};


namespace Soy
{
	namespace Platform
	{
		bool		Init();
		bool		InitThread();
		void		ShutdownThread();
		
#if defined(TARGET_ANDROID)
		bool		HasJavaVm();
		JNIEnv&		Java();
#endif

		int					GetLastError();
		std::string			GetErrorString(int Error);
		inline std::string	GetLastErrorString()	{	return GetErrorString( GetLastError() );	}
	}
};

template<typename TYPE>
class ArrayBridge;

namespace Soy
{
	bool		FileToArray(ArrayBridge<char>& Data,std::string Filename,std::ostream& Error);
	inline bool	FileToArray(ArrayBridge<char>&& Data,std::string Filename,std::ostream& Error)	{	return FileToArray( Data, Filename, Error );	}
	inline bool	LoadBinaryFile(ArrayBridge<char>& Data,std::string Filename,std::ostream& Error)	{	return FileToArray( Data, Filename, Error );	}
	bool		ReadStream(ArrayBridge<char>& Data, std::istream& Stream, std::ostream& Error);
	bool		ReadStream(ArrayBridge<char>&& Data, std::istream& Stream, std::ostream& Error);
	bool		ReadStreamChunk( ArrayBridge<char>& Data, std::istream& Stream );
	inline bool	ReadStreamChunk( ArrayBridge<char>&& Data, std::istream& Stream )	{	return ReadStreamChunk( Data, Stream );		}
	bool		StringToFile(std::string Filename,std::string String);
	bool		FileToString(std::string Filename,std::string& String);
	bool		FileToString(std::string Filename,std::string& String,std::ostream& Error);
	bool		FileToStringLines(std::string Filename,ArrayBridge<std::string>& StringLines,std::ostream& Error);
	inline bool	FileToStringLines(std::string Filename,ArrayBridge<std::string>&& StringLines,std::ostream& Error)	{	return FileToStringLines( Filename, StringLines, Error );	}
}


namespace Soy
{
	//	http://www.adp-gmbh.ch/cpp/common/base64.html
	void		base64_encode(ArrayBridge<char>& Encoded,const ArrayBridge<char>& Decoded);
	void		base64_decode(const ArrayBridge<char>& Encoded,ArrayBridge<char>& Decoded);
};




class Soy::TVersion
{
public:
	TVersion() :
		mMajor	( 0 ),
		mMinor	( 0 )
	{
	}
	TVersion(size_t Major,size_t Minor) :
		mMajor	( Major ),
		mMinor	( Minor )
	{
	}
	explicit TVersion(std::string VersionStr,const std::string& Prefix="");
	
	//	gr: throw if the minor is going to overflow
	size_t	GetHundred() const;
	
public:
	size_t	mMajor;
	size_t	mMinor;
};

namespace Soy
{
inline std::ostream& operator<<(std::ostream &out,const Soy::TVersion& in)
{
	out << in.mMajor << '.' << in.mMinor;
	return out;
}
}



