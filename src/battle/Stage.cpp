#include "Stage.hpp"

using namespace soundbag;
using namespace rise_and_fall;

namespace bpy = boost::python;

Stage::Stage(){
	current_unit = 0;
}

Stage::~Stage(){
	for( auto& u : units ){
		u.retreat();
	}
}

size_t Stage::getUnitCount() const {
	return units.size();
}

size_t Stage::getCurrentUnitIndex() const {
	if( units.size() <= current_unit ){
		return 0;
	}
	return current_unit;
}

Unit& Stage::getCurrentUnit() {
	if( units.size() == 0 ){
		current_unit = 0;
		units.emplace_back();
		units[0].enter(this);
	}
	return units[current_unit];
}

void Stage::appendUnit( const bpy::dict& d ) {
	units.emplace_back( d );
	units.back().enter(this);
}

void Stage::appendUnit( const Unit& unit ){
	units.push_back( unit );
	units.back().enter(this);
}

std::vector<Unit>::iterator Stage::units_begin(){
	return units.begin();
}

std::vector<Unit>::iterator Stage::units_end(){
	return units.end();
}

void Stage::update( uint32_t delta ){
	double game_time = (double)delta / 1000;
	for( auto& unit : units ){
		unit.update(game_time);
	}
	for( auto& bullet : bullets ){
		bullet.update(game_time);
	}
}