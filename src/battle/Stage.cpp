#include "Stage.hpp"

using namespace soundbag;
using namespace rise_and_fall;

namespace bpy = boost::python;

Stage::Stage(){

}

Stage::~Stage(){
	for( auto& u : units ){
		u.retreat();
	}
}

void Stage::genBuffers(){
	for( auto& unit : units ){
		unit.genBuffers();
	}
}

void Stage::draw(){
	for( auto& unit : units ){
		unit.draw();
	}
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

std::vector<Unit>::iterator Stage::units_begin(){
	return units.begin();
}

std::vector<Unit>::iterator Stage::units_end(){
	return units.end();
}