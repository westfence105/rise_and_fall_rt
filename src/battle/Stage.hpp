#pragma once

#ifndef rise_and_fall_Stage_h
#define rise_and_fall_Stage_h

#include <vector>
#include <deque>

#include "Unit.hpp"

namespace rise_and_fall {
	struct Stage {
		std::string name;
		std::string map_name;

		std::deque<FlyingBullet> bullets;

	private:
		std::vector<Unit> units;
		size_t current_unit;

	public:
		Stage();
		~Stage();

		void genBuffers();
		void draw();
		void update( uint32_t delta );

		Unit& getCurrentUnit();
		size_t getUnitCount() const;
		size_t getCurrentUnitIndex() const;

		//For python
		void appendUnit( const boost::python::dict& d );
		void appendUnit( const Unit& unit );
		std::vector<Unit>::iterator units_begin();
		std::vector<Unit>::iterator units_end();
	};
}

#endif