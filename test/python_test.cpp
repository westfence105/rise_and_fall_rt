#pragma execution_character_set("utf-8")

#include "gtest/gtest.h"

#include "battle/Unit.hpp"
#include "battle/Stage.hpp"

using namespace soundbag;
using namespace rise_and_fall;

namespace bpy = boost::python;

extern "C" {
	void initrise_and_fall();
}

int main( int argc, char** argv ){
	testing::InitGoogleTest( &argc, argv );
	PyImport_AppendInittab("rise_and_fall",initrise_and_fall);
	Py_Initialize();
	return RUN_ALL_TESTS();
}

TEST(python_test,unit){
	try{
		auto ns = bpy::import("__main__").attr("__dict__");
		bpy::exec(
			"# -*- coding: utf-8 -*-" "\n"
			"import rise_and_fall" "\n"
			"module = rise_and_fall.Module({"
				"'name':'Body',"
				"'hp':10000,"
				"'armor':102,"
				"'pos':(0,0),"
				"'width':23,"
				"'length':132,"
				"'angle':0 })" "\n"
			, ns );
		Unit::Module& module = bpy::extract<Unit::Module&>(ns["module"]);
		EXPECT_EQ( module.name, "Body" );
		EXPECT_EQ( module.hp, 10000 );
		EXPECT_EQ( module.hp_max, 10000 );
		EXPECT_EQ( module.armor, 102 );
		EXPECT_EQ( module.pos, coord_type( 0, 0 ) );
		EXPECT_EQ( module.width, 23 );
		EXPECT_EQ( module.length, 132 );
		EXPECT_EQ( module.angle, 0 );

		bpy::exec(
			"engine = rise_and_fall.Engine({"
				"'hp':5000," //継承のテスト
				"'max_speed': 30,"
				"'min_speed': -5,"
				"'accel': 10,"
				"'decel':  2 })\n"
			, ns );
		Unit::Engine& engine = bpy::extract<Unit::Engine&>(ns["engine"]);
		EXPECT_EQ( engine.hp, 5000 );
		EXPECT_EQ( engine.max_speed(), 30 );
		EXPECT_EQ( engine.min_speed(), -5 );
		EXPECT_EQ( engine.accel(), 10 );
		EXPECT_EQ( engine.decel(),  2 );

		bpy::exec(
			"arm = rise_and_fall.Armament({"
				"'pos':(50,0),"
				"'type':rise_and_fall.ArmamentType.CANNON,"
				"'bullet':"
					"rise_and_fall.Bullet("
						"rise_and_fall.BulletType.AP,102,200),"
				"'velocity':730,"
				"'range':13700,"
				"'num':2,"
				"'load_time':60,"
				"'rotation':0,"
				"'rotate_max': 150,"
				"'rotate_min':-150,"
				"'rotate_speed': 5 })\n"
			, ns );
		Unit::Armament& arm = bpy::extract<Unit::Armament&>( ns["arm"] );
		EXPECT_EQ( arm.type, Unit::Armament::ArmamentType::CANNON );
		EXPECT_EQ( arm.bullet.type, Bullet::BulletType::AP );
		EXPECT_EQ( arm.bullet.pen, 102 );
		EXPECT_EQ( arm.bullet.dmg, 200 );
		EXPECT_EQ( arm.velocity, 730 );
		EXPECT_EQ( arm.range, 13700 );
		EXPECT_EQ( arm.num, 2 );
		EXPECT_EQ( arm.load, 60 );
		EXPECT_EQ( arm.load_time, 60 );
		EXPECT_EQ( arm.rotation, 0 );
		EXPECT_EQ( arm.rotate_max,  150 );
		EXPECT_EQ( arm.rotate_min, -150 );
		EXPECT_EQ( arm.rotate_speed, 5 );

		bpy::exec(
			"arm2 = arm" "\n"
			"arm2.pos.x = -50" "\n"
			"arm2.angle = -180" "\n"

			"a_grp = rise_and_fall.ArmamentGroup("
				"'30.5cm Twin Main-Battery', [ arm, arm2 ])" "\n"
			, ns );
		Unit::ArmamentGroup& a_grp = bpy::extract<Unit::ArmamentGroup&>( ns["a_grp"] );
		EXPECT_EQ( a_grp.name, "30.5cm Twin Main-Battery" );
		ASSERT_EQ( a_grp.arms.size(), 2 );
		EXPECT_EQ( a_grp.arms[1].pos.x, -50 );
		EXPECT_EQ( a_grp.arms[1].angle, -180 );

		bpy::exec(
			"unit = rise_and_fall.Unit({"
				"'type':'Shikishima Class BattleShip',"
				"'name':'Mikasa',"
				"'pos':(334,9800),"
				"'face':-135,"
				"'turn_pef':10,"
				"'turn_type':rise_and_fall.TurnType.SHIP_TURN,"
				"'body':module,"
				"'engine':engine,"
				"'arms':[a_grp] })" "\n"
			, ns );
		Unit& unit = bpy::extract<Unit&>( ns["unit"] );
		EXPECT_EQ( unit.type, "Shikishima Class BattleShip" );
		EXPECT_EQ( unit.name, "Mikasa" );
		EXPECT_EQ( unit.pos.x, 334 );
		EXPECT_EQ( unit.pos.y, 9800 );
		EXPECT_EQ( unit.face, -135 );
		EXPECT_EQ( unit.turn_pef, 10 );
		EXPECT_EQ( unit.turn_type, Unit::TurnType::SHIP_TURN );
		EXPECT_EQ( unit.body.hp, 10000 );
		EXPECT_EQ( unit.body.armor, 102 );
		EXPECT_EQ( unit.engine.accel(), 10 );
		EXPECT_EQ( unit.getArmCount(), 1 );

		bpy::exec(
			"unit_alive = unit.alive" "\n"
			"arm_count = unit.arm_count" "\n"
			, ns );
		EXPECT_TRUE( bpy::extract<bool>( ns["unit_alive"] ) );
		EXPECT_EQ( bpy::extract<size_t>( ns["arm_count"] ), 1 );

		unit.pos.x = 330;
		EXPECT_EQ( (coord_type)bpy::extract<coord_type>( ns["unit"].attr("pos") ), unit.pos );
	
		Stage stage;
		ns["stage"] = boost::ref(stage);
		bpy::exec(
				"stage.append_unit( unit )" "\n"
				"unit_count = stage.unit_count" "\n"
			, ns );
		ASSERT_EQ( stage.getUnitCount(), 1 );
		EXPECT_EQ( stage.getCurrentUnit().name, unit.name );
	}
	catch(bpy::error_already_set& e){
		PyErr_Print();
		FAIL();
	}
	catch(...){
		FAIL() << "Unknown Exception";
	}
}