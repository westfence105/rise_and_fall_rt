#include <cmath>

#include "Unit.hpp"

using namespace soundbag;
using namespace rise_and_fall;

namespace bpy = boost::python;

Bullet::Bullet( BulletType _type, int _pen, int _dmg ){
	type = _type;
	pen  = _pen;
	dmg  = _dmg;
}

Bullet::Bullet() : Bullet( BulletType::AP, 0, 0 ) {}

FlyingBullet::FlyingBullet( 
		const Bullet& b, 
		const coord_type& tgt,
		const coord_type& p,
		int s 
	) : bullet( b ), target( tgt ), pos( p ), speed( s )
{

}

Unit::Unit(){
	face = 0;
	speed = 0;
	turn_pef = 0;
	turn_type = TurnType::TURNING_RADIUS;
	current_arm = 0;
	target_speed = 0;
	target_angle = 0;
	stage = NULL;
}

Unit::Unit( const bpy::dict& d ) : Unit() {
	bpy::list keys = d.keys();
	for( int i = 0; i < bpy::len(keys); ++i ){
		std::string k = bpy::extract<std::string>( keys[i] );
		if( k == "type" ){
			type = bpy::extract<std::string>( d[k] );
		}
		else if( k == "name" ){
			name = bpy::extract<std::string>( d[k] );
		}
		else if( k == "pos" ){
			bpy::tuple t = bpy::extract<bpy::tuple>( d[k] );
			pos.x = bpy::extract<double>( t[0] );
			pos.y = bpy::extract<double>( t[1] );
		}
		else if( k == "face" ){
			face = bpy::extract<double>( d[k] );
			target_angle = face;
		}
		else if( k == "speed" ){
			speed = bpy::extract<double>( d[k] );
			if( target_speed == 0 ){
				target_speed = speed;
			}
		}
		else if( k == "turn_pef" ){
			turn_pef = bpy::extract<double>( d[k] );
		}
		else if( k == "turn_type" ){
			turn_type = bpy::extract<TurnType>( d[k] );
		}
		else if( k == "body" ){
			body = bpy::extract<Module>( d[k] );
		}
		else if( k == "engine" ){
			engine = bpy::extract<Engine>( d[k] );
		}
		else if( k == "arms" ){
			bpy::object l_arms = bpy::extract<bpy::object>( d[k] );
			std::vector<ArmamentGroup>(
				bpy::stl_input_iterator<ArmamentGroup>( l_arms ),
				bpy::stl_input_iterator<ArmamentGroup>()
			).swap(arms);
		}
		else if( k == "target_speed" ){
			target_speed = bpy::extract<double>( d[k] );
		}
	}
}

Unit::~Unit(){

}

Unit::Module::Module(){
	hp_max = hp = 0;
	armor = 0;
	pos = coord_type();
	width = 0;
	length = 0;
	angle = 0;
	vbo_line[0] = vbo_line[1] = 0;
	vbo_fill[0] = vbo_fill[1] = 0;
}

Unit::Module::Module( const bpy::dict& d ) : Module()
{
	bpy::list keys = d.keys();
	for( int i = 0; i < bpy::len(keys); ++i ){
		std::string k = bpy::extract<std::string>( keys[i] );
		if( k == "name" ){
			name = bpy::extract<std::string>( d[k] );
		}
		else if( k == "hp" ){
			hp = bpy::extract<int>( d[k] );
		}
		else if( k == "hp_max" ){
			hp_max = bpy::extract<int>( d[k] );
		}
		else if( k == "armor" ){
			armor = bpy::extract<int>( d[k] );
		}
		else if( k == "pos" ){
			bpy::tuple t = bpy::extract<bpy::tuple>( d[k] );
			pos.x = bpy::extract<double>( t[0] );
			pos.y = bpy::extract<double>( t[1] );
		}
		else if( k == "width" ){
			width = bpy::extract<int>( d[k] );
		}
		else if( k == "length" ){
			length = bpy::extract<int>( d[k] );
		}
		else if( k == "angle" ){
			angle = bpy::extract<double>( d[k] );
		}
	}
	if( hp_max < hp ){
		hp_max = hp;
	}
}

Unit::Module::Module( const Module& m ) : Module()
{
	name = m.name;
	hp = m.hp;
	hp_max = m.hp_max;
	armor = m.armor;
	pos = m.pos;
	width = m.width;
	length = m.length;
	angle = m.angle;
	line_data = m.line_data;
	fill_data = m.fill_data;
	line_index = m.line_index;
	fill_index = m.fill_index;
}

Unit::Module::~Module(){

}

Unit::Engine::Engine() : Engine( bpy::dict() ) {
}

Unit::Engine::Engine( const boost::python::dict& d )
	: Module(d)
{
	m_max_speed = m_min_speed = 0;
	m_accel = m_decel = 0;

	bpy::list keys = d.keys();
	for( int i = 0; i < bpy::len(keys); ++i ){
		std::string k = bpy::extract<std::string>( keys[i] );
		if( k == "min_speed" ){
			m_min_speed = bpy::extract<double>( d[k] );
		}
		else if( k == "max_speed" ){
			m_max_speed = bpy::extract<double>( d[k] );
		}
		else if( k == "accel" ){
			m_accel = bpy::extract<double>( d[k] );
		}
		else if( k == "decel" ){
			m_decel = bpy::extract<double>( d[k] );
		}
	}
}

Unit::Armament::Armament() 
	: Armament( bpy::dict() )
{
}

Unit::Armament::Armament( const boost::python::dict& d )
	  : Module(d)
{
	type = ArmamentType::CANNON;
	velocity = 0;
	range = 0;
	num = 0;
	load = load_time = -1;
	rotation = 0;
	rotate_min = rotate_max = rotate_speed = 0;
	aimed = aim = coord_type(-1,-1);

	bpy::list keys = d.keys();
	for( int i = 0; i < bpy::len(keys); ++i ){
		std::string k = bpy::extract<std::string>( keys[i] );
		if( k == "type" ){
			type = bpy::extract<ArmamentType>( d[k] );
		}
		else if( k == "bullet" ){
			bullet = bpy::extract<Bullet>( d[k] );
		}
		else if( k == "velocity" ){
			velocity = bpy::extract<int>( d[k] );
		}
		else if( k == "range" ){
			range = bpy::extract<int>( d[k] );
		}
		else if( k == "num" ){
			num = bpy::extract<int>( d[k] );
		}
		else if( k == "load_time" ){
			load_time = bpy::extract<double>( d[k] );
			load = load_time;
		}
		else if( k == "rotation" ){
			rotation = bpy::extract<double>( d[k] );
		}
		else if( k == "rotate_speed" ){
			rotate_speed = bpy::extract<double>( d[k] );
		}
		else if( k == "rotate_max" ){
			rotate_max = bpy::extract<double>( d[k] );
		}
		else if( k == "rotate_min" ){
			rotate_min = bpy::extract<double>( d[k] );
		}
		else if( k == "aimed" ){
			bpy::tuple t = bpy::extract<bpy::tuple>( d[k] );
			aimed.x = bpy::extract<double>( t[0] );
			aimed.y = bpy::extract<double>( t[1] );
		}
		else if( k == "aim" ){
			bpy::tuple t = bpy::extract<bpy::tuple>( d[k] );
			aim.x = bpy::extract<double>( t[0] );
			aim.y = bpy::extract<double>( t[1] );
		}
	}
}

Unit::ArmamentGroup::ArmamentGroup()
	: aim( -1, -1 )
{

}

Unit::ArmamentGroup::ArmamentGroup(
				const std::string& _name, 
				const bpy::object& l
		) 
	: name(_name ),
	  arms( bpy::stl_input_iterator<Armament>(l), 
	  	    bpy::stl_input_iterator<Armament>() ),
	  aim( -1, -1 )
{

}

void Unit::enter( Stage* s ){
	stage = s;
}

void Unit::retreat(){
	stage = NULL;
}

bool Unit::alive() const {
	return 0 < body.health();
}

Unit::ArmamentGroup& Unit::getCurrentArm(){
	if( arms.size() <= current_arm ){
		current_arm = 0;
	}
	if( arms.empty() ){
		arms.push_back(ArmamentGroup());
	}
	return arms[current_arm];
}

double Unit::Module::health() const {
	if( hp_max != 0 ){
		return (double)hp / hp_max;
	}
	else {
		return 0;
	}
}

double Unit::Engine::max_speed() const {
	return m_max_speed;
}

double Unit::Engine::min_speed() const {
	return m_min_speed;
}

double Unit::Engine::accel() const {
	return m_accel * health();
}

double Unit::Engine::decel() const {
	return m_decel;
}

void Unit::Module::setLineData( const bpy::object& l ){
	std::vector<GLfloat>(
		bpy::stl_input_iterator<GLfloat>( l ),
		bpy::stl_input_iterator<GLfloat>()
	).swap( line_data );
}

void Unit::Module::setFillData( const bpy::object& l ){
	std::vector<GLfloat>(
		bpy::stl_input_iterator<GLfloat>( l ),
		bpy::stl_input_iterator<GLfloat>()
	).swap( fill_data );
}

void Unit::Module::setLineIndex( const bpy::object& l ){
	std::vector<GLuint>(
		bpy::stl_input_iterator<GLuint>( l ),
		bpy::stl_input_iterator<GLuint>()
	).swap( line_index );
}

void Unit::Module::setFillIndex( const bpy::object& l ){
	std::vector<GLuint>(
		bpy::stl_input_iterator<GLuint>( l ),
		bpy::stl_input_iterator<GLuint>()
	).swap( fill_index );
}

std::vector<Unit::ArmamentGroup>::iterator Unit::arms_begin(){
	return arms.begin();
}

std::vector<Unit::ArmamentGroup>::iterator Unit::arms_end(){
	return arms.end();
}

std::vector<Unit::Armament>::iterator Unit::ArmamentGroup::begin(){
	return arms.begin();
}

std::vector<Unit::Armament>::iterator Unit::ArmamentGroup::end(){
	return arms.end();
}