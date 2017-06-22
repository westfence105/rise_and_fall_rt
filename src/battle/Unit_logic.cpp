#include <cmath>

#include "Unit.hpp"
#include "Stage.hpp"

using namespace soundbag;
using namespace rise_and_fall;

namespace bpy = boost::python;

size_t Unit::getArmCount() const {
	return arms.size();
}

coord_type Unit::getCurrentAim() const {
	if( current_arm < arms.size() ){
		return arms[current_arm].aim;
	}
	else {
		return pos;
	}
}

void Unit::nextArm(){
	++current_arm;
	if( arms.size() <= current_arm ){
		current_arm = 0;
	}
}

void Unit::moveAim( double x_diff, double y_diff ){
	if( current_arm < arms.size() ){
		auto& arm = arms[current_arm];
		arm.aim.x += x_diff;
		arm.aim.y += y_diff;
	}
}

void Unit::pivotAim( double distance_diff, double angle_diff ){
	if( current_arm < arms.size() ){
		auto& arm = arms[current_arm];
		const double rx = arm.aim.x - pos.x;
		const double ry = arm.aim.y - pos.y;
		double distance = sqrt( rx * rx - ry * ry ) + distance_diff;
		double angle = atan2( ry, rx ) + angle_diff / 180 * M_PI;
		arm.aim.x = cos(angle) * distance;
		arm.aim.y = sin(angle) * distance;
	}
}

int Unit::ArmamentGroup::getReadyCount() const {
	int ret = 0;
	for( const auto& arm : arms ){
		if( aim == arm.aimed && arm.load <= 0 ){
			ret += arm.num;
		}
	}
	return ret;
}

int Unit::getReadyCount() const {
	if( current_arm < arms.size() ){
		return arms[current_arm].getReadyCount();
	}
	else {
		return 0;
	}
}

void Unit::accel( double v ){
	target_speed += v;
	if( target_speed < engine.min_speed() ){
		target_speed = engine.min_speed();
	}
	else if( target_speed > engine.max_speed() ){
		target_speed = engine.max_speed();
	}
}

void Unit::turn( double v ){
	target_angle += v;
	while( target_angle < -180 ){
		target_angle += 360;
	}
	while( 180 < target_angle ){
		target_angle -= 360;
	}
}

void Unit::shot(){
	if( stage != NULL ){
		getCurrentArm().shot( *stage, pos );
	}
}

void Unit::ArmamentGroup::shot( Stage& stage, const coord_type& pos ){
	for( auto& a : arms ){
		if( a.aimed == aim ){
			stage.bullets.emplace_back( a.bullet, aim, pos, a.velocity );
			a.load = a.load_time;
		}
	}
}