#pragma once

#ifndef rise_and_fall_RF_Map_h
#define rise_and_fall_RF_Map_h

#include <stdexcept>
#include <vector>
#include <cstdint>
#include <memory>

#include "GL/gl.h"

typedef uint8_t byte;

namespace rise_and_fall {
	class RF_Map {
		uint32_t m_width;
		uint32_t m_height;
		std::vector<byte> m_data;
		std::vector<byte> m_image;
		GLuint tex;
		GLuint vbo;

	public:
		static constexpr byte TERRAIN_SEA		= 0;
		static constexpr byte TERRAIN_FLAT		= 1;
		static constexpr byte TERRAIN_HILL		= 2;
		static constexpr byte TERRAIN_MOUNTAIN	= 3;
		static constexpr byte TERRAIN_ALPINE	= 4;

	protected:
		void load( const std::vector<byte>& data_in );

	public:
		RF_Map( const std::string& filename ) throw(std::invalid_argument);
		RF_Map( const std::vector<byte>& data_in );
		RF_Map( const RF_Map& );
		~RF_Map();

	private:
		RF_Map();
	};
}

#endif