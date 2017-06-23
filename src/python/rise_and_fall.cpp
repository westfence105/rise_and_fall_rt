#define BOOST_PYTHON_STATIC_LIB
#include "boost/python.hpp"

#include "battle/Unit.hpp"
#include "battle/Stage.hpp"

using namespace soundbag;
using namespace rise_and_fall;

namespace bpy = boost::python;

BOOST_PYTHON_MODULE(rise_and_fall){
	bpy::class_<coord_type>("Point")
			.def_readwrite("x",&coord_type::x)
			.def_readwrite("y",&coord_type::y)
		;
	bpy::enum_<Bullet::BulletType>("BulletType")
			.value("AP",Bullet::BulletType::AP)
			.value("HE",Bullet::BulletType::HE)
			.value("APHE",Bullet::BulletType::APHE)
			.value("TORPEDO",Bullet::BulletType::TORPEDO)
		;
	bpy::class_<Bullet>("Bullet")
			.def(bpy::init<Bullet::BulletType,int,int>())
			.def_readwrite("pen",&Bullet::pen)
			.def_readwrite("dmg",&Bullet::dmg)
		;
	bpy::class_<Unit::Module>("Module")
			.def(bpy::init<const bpy::dict&>())
			.def_readwrite("name",&Unit::Module::name)
			.def_readwrite("hp",&Unit::Module::hp)
			.def_readwrite("armor",&Unit::Module::armor)
			.def_readwrite("pos",&Unit::Module::pos)
			.def_readwrite("width",&Unit::Module::width)
			.def_readwrite("length",&Unit::Module::length)
			.def_readwrite("angle",&Unit::Module::angle)
			.add_property("health",&Unit::Module::health)
			.def("setLineData",&Unit::Module::setLineData)
			.def("setFillData",&Unit::Module::setFillData)
			.def("setLineIndex",&Unit::Module::setLineIndex)
			.def("setFillIndex",&Unit::Module::setFillIndex)
		;
	bpy::class_<Unit::Engine,bpy::bases<Unit::Module> >("Engine")
			.def(bpy::init<const bpy::dict&>())
			.add_property("max_speed",&Unit::Engine::max_speed)
			.add_property("min_speed",&Unit::Engine::min_speed)
			.add_property("accel",&Unit::Engine::accel)
			.add_property("decel",&Unit::Engine::decel)
		;
	bpy::enum_<Unit::Armament::ArmamentType>("ArmamentType")
			.value("CANNON",Unit::Armament::ArmamentType::CANNON)
			.value("TORPEDO",Unit::Armament::ArmamentType::TORPEDO)
		;
	bpy::class_<Unit::Armament,bpy::bases<Unit::Module> >("Armament")
			.def(bpy::init<const bpy::dict&>())
			.def_readwrite("type",&Unit::Armament::type)
			.def_readwrite("bullet",&Unit::Armament::bullet)
			.def_readwrite("velocity",&Unit::Armament::velocity)
			.def_readwrite("range",&Unit::Armament::range)
			.def_readwrite("num",&Unit::Armament::num)
			.def_readwrite("load",&Unit::Armament::load)
			.def_readwrite("load_time",&Unit::Armament::load_time)
			.def_readwrite("rotation",&Unit::Armament::rotation)
			.def_readwrite("rotate_min",&Unit::Armament::rotate_min)
			.def_readwrite("rotate_max",&Unit::Armament::rotate_max)
			.def_readwrite("rotate_speed",&Unit::Armament::rotate_speed)
		;
	bpy::class_<Unit::ArmamentGroup>("ArmamentGroup")
			.def(bpy::init<const std::string&,const bpy::object&>())
			.def("__iter__",
				bpy::range(
					&Unit::ArmamentGroup::begin,
					&Unit::ArmamentGroup::end
				)
			)
			.def_readonly("name",&Unit::ArmamentGroup::name)
			.def_readwrite("aim",&Unit::ArmamentGroup::aim)
			.add_property("ready_count",&Unit::ArmamentGroup::getReadyCount)
		;
	bpy::enum_<Unit::TurnType>("TurnType")
			.value("TURNING_RADIUS",Unit::TurnType::TURNING_RADIUS)
			.value("SPIN_TURN",Unit::TurnType::SPIN_TURN)
			.value("SHIP_TURN",Unit::TurnType::SHIP_TURN)
		;
	bpy::class_<Unit>("Unit")
			.def(bpy::init<const bpy::dict&>())
			.def_readonly("type",&Unit::type)
			.def_readonly("name",&Unit::name)
			.def_readonly("pos",&Unit::pos)
			.def_readonly("face",&Unit::face)
			.def_readonly("speed",&Unit::speed)
			.def_readonly("turn_pef",&Unit::turn)
			.def_readonly("turn_type",&Unit::turn_type)
			.def_readonly("body",&Unit::body)
			.def_readonly("engine",&Unit::engine)
			.add_property("alive",&Unit::alive)
			.add_property("arm_count",&Unit::getArmCount)
			.add_property("aim",&Unit::getCurrentAim)
			.def("current_arm",&Unit::getCurrentArm,bpy::return_value_policy<bpy::copy_non_const_reference>())
			.def("next_arm",&Unit::nextArm)
			.def("move_aim",&Unit::moveAim)
			.def("pivot_aim",&Unit::pivotAim)
			.add_property("ready_count",&Unit::getReadyCount)
			.def("shot",&Unit::shot)
			.def("accel",&Unit::accel)
			.def("turn",&Unit::turn)
			.def("arms.__iter__",bpy::range(
									&Unit::arms_begin,
									&Unit::arms_end
								))
		;
}