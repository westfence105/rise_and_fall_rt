#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <iterator>

#include "soundbag/bits.hpp"
#include "soundbag/zlib_tool.hpp"

#include "RF_Map.hpp"

using namespace soundbag;
using namespace rise_and_fall;

namespace sbz = soundbag::zlib_tool;

constexpr byte RF_Map::TERRAIN_SEA;
constexpr byte RF_Map::TERRAIN_FLAT;	
constexpr byte RF_Map::TERRAIN_HILL;
constexpr byte RF_Map::TERRAIN_MOUNTAIN;
constexpr byte RF_Map::TERRAIN_ALPINE;

RF_Map::RF_Map(){
	m_width = 0;
	m_height = 0;
	tex = 0;
	vbo = 0;
}

RF_Map::RF_Map( const RF_Map& b ){
	m_width = b.m_width;
	m_height = b.m_height;
	m_data.reserve( b.m_data.size() );
	m_image.reserve( b.m_image.size() );
	std::copy( b.m_data.cbegin(), b.m_data.cend(), std::back_inserter(m_data) );
	std::copy( b.m_image.cbegin(), b.m_image.cend(), std::back_inserter(m_image) );
	tex = 0;
	vbo = 0;
}

RF_Map::RF_Map( const std::string& filename )
		throw( std::invalid_argument )
{
	std::basic_ifstream<uint8_t> ifs( filename, std::ios::binary );
	if( ifs.fail() ){
		throw std::invalid_argument("Failed to open map file.");
	}

	load( std::vector<byte>( std::istreambuf_iterator<byte>(ifs), std::istreambuf_iterator<byte>() ) );
}

RF_Map::RF_Map( const std::vector<byte>& data ){
	load( data );
}

void RF_Map::load( const std::vector<byte>& data_in ){
	constexpr size_t MINIMUM_FILESIZE = 4 + 4;
	if( data_in.size() < MINIMUM_FILESIZE ){
		throw std::invalid_argument("Too small data given.");
	}
	auto iter = data_in.begin();

	bool compressed;
	constexpr std::array<byte,4> MAGIC_NUMBER_CM = { 3, 'r', 'f', 'm' }; //Compressed
	constexpr std::array<byte,4> MAGIC_NUMBER_NC = { 3, 'r', 'f', 'M' }; //Not compressed
	if( std::equal( iter, iter + 4, MAGIC_NUMBER_CM.begin() ) ){
		compressed = true;
	}
	else if( std::equal( iter, iter + 4, MAGIC_NUMBER_NC.begin() ) ){
		compressed = false;
	}
	else {
		throw std::invalid_argument("Invalid data magick-number.");
	}
	iter += 4;

	m_width  = decodeBytes<uint16_t>( iter );
	m_height = decodeBytes<uint16_t>( iter );

	bool data_init = false;
	bool img_init  = false;
	while( (!( data_init && img_init )) && ( ( 2 + 4 ) < ( data_in.end() - iter ) ) ){
		const byte type = *(iter++);
		const byte format = *(iter++);
		const uint32_t data_size = decodeBytes<uint32_t>( iter, false );
		if( data_size < data_in.end() - iter ){
			throw std::invalid_argument("Invalid chunk size");
		}
		if( type == 'd' && data_init == false ){
			if( format == 0 ){
				m_data.reserve(data_size);
				std::copy( iter, iter + data_size, std::back_inserter(m_data) );
			}
			else if( format == 1 ){
				sbz::decompress( m_data, iter, iter + data_size );
			}
			data_init = true;
		}
		else if( type == 'g' && img_init == false ){
			if( ( format & 0x0F ) == 0 ){
				m_image.reserve(data_size);
				std::copy( iter, iter + data_size, std::back_inserter(m_image) );
			}
			else if( ( format & 0x0F ) == 1 ){
				sbz::decompress( m_image, iter, iter + data_size );
			}
			img_init = true;
		}
		else {
			throw std::invalid_argument("Invalid chunk.");
		}
		iter += data_size;
	}

	if( !data_init ){
		throw std::invalid_argument("Map data chunk is not found.");
	}
	else if( m_data.size() != m_width*m_height ){
		throw std::invalid_argument("Map Data size mismatch");
	}

	if( !img_init ){
		static const std::vector< std::vector<byte> > colors = {
			{ 0, 0, 0xFF },
			{ 0xDC, 0xDC, 0x7F },
			{ 0, 0xB0, 0 },
			{ 0, 0x60, 0x1A },
			{ 0x80, 0x80, 0x80 }
		};
		m_image.reserve( m_width * m_height * 3 );
		for( int j = 0; j < m_height; ++j ){
			for( int i = 0; i < m_width; ++i ){
				const byte b = m_data[i+(m_height-j-1)*m_width];
				if( b < colors.size() ){
					std::copy( colors[b].cbegin(), colors[b].cend(), std::back_inserter(m_image) );
				}
			}
		}
	}
	else if( m_image.size() != m_width*m_height*3 ){
		throw std::invalid_argument("Map Image size mismatch");
	}
}

RF_Map::~RF_Map(){

}