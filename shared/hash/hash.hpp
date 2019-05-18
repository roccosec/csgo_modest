#pragma once

namespace shared::hash
{
	/// FNV-1a constants.
	enum : uint32_t
	{
		FNV1A_PRIME = 0x1000193,
		FNV1A_BASIS = 0x811C9DC5
	};

	/// compile-time strlen.
	constexpr size_t ct_strlen( const char* str )
	{
		size_t out = 1;

		for ( ; str[ out ] != '\0'; ++out );

		return out;
	}

	/// hash data.
	constexpr uint32_t fnv1a_32( const uint8_t * data, const size_t len )
	{
		uint32_t out = FNV1A_BASIS;

		for ( size_t i = 0; i < len; ++i )
			out = ( out ^ data[ i ] ) * FNV1A_PRIME;

		return out;
	}

	/// hash c-style string.
	constexpr uint32_t fnv1a_32( const char* str )
	{
		uint32_t out = FNV1A_BASIS;
		size_t len = ct_strlen( str );

		for ( size_t i = 0; i < len; ++i )
			out = ( out ^ str[ i ] ) * FNV1A_PRIME;

		return out;
	}

	/// hash C++-style string (runtime only).
	__forceinline uint32_t fnv1a_32( const std::string & str )
	{
		return fnv1a_32( ( uint8_t* )str.c_str(), str.size() );
	}
}

/// weird hacky fix for hashes not working when passing to functions as an arg / etc. fuck you msvc
/// always use this for your compile-time hashes.
#define HASH( str ) \
       [ ]( ) { \
           constexpr hash32_t ret = hash::fnv1a_32( str ); \
           return ret; \
       }( )
