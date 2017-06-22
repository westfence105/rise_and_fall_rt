#pragma once

#ifndef rise_and_fall_Unit_h
#define rise_and_fall_Unit_h

#include <vector>
#include <stdexcept>

#include "GL/gl.h"

#include "boost/python.hpp"

#include "soundbag/Point.hpp"

namespace rise_and_fall {
	typedef soundbag::Point<double> coord_type;

	struct Stage;

	struct Bullet {
		enum class BulletType {
			AP, HE, APHE, TORPEDO
		} type;
		int pen;
		int dmg;

		Bullet();
		Bullet( BulletType _type, int _pen, int _dmg );
	};

	struct FlyingBullet {
		Bullet bullet;
		coord_type target;
		coord_type pos;
		int speed;

		FlyingBullet(
				const Bullet& b, 
				const coord_type& tgt,
				const coord_type& p,
				int s
			);
		void update(double delta);
	};

	struct Unit {
		struct Module {
			std::string name;
			int hp_max;
			int hp;
			int armor;
			coord_type pos;
			int width;
			int length;
			double angle;

			GLuint vbo_line[2];
			GLuint vbo_fill[2];

			std::vector<GLfloat> line_data;
			std::vector<GLfloat> fill_data;
			std::vector<GLuint> line_index;
			std::vector<GLuint> fill_index;

		public:
			Module();
			Module( const boost::python::dict& d );
			Module( const Module& );
			virtual ~Module();

			double health() const;

			void genBuffers();
			void draw();

			bool hit( const coord_type& p, const FlyingBullet& b );
			
			//For Python
			void setLineData( const boost::python::object& l );
			void setFillData( const boost::python::object& l );
			void setLineIndex( const boost::python::object& l );
			void setFillIndex( const boost::python::object& l );

		};

		class Engine : public Module {
			double m_max_speed;
			double m_min_speed;
			double m_accel;
			double m_decel;

		public:
			Engine();
			Engine( const boost::python::dict& d );

			double max_speed() const;
			double min_speed() const;
			double accel() const;
			double decel() const;
		};

		struct Armament : public Module {
			enum class ArmamentType {
				CANNON, TORPEDO
			} type;

			Bullet bullet;
			int velocity;
			int range;
			int num;
			double load;
			double load_time;
			double rotation;
			double rotate_min;
			double rotate_max;
			double rotate_speed;
			coord_type aimed;
			coord_type aim;

			Armament();
			Armament( const boost::python::dict& d );

			void update( double delta );
		};

		struct ArmamentGroup {
			std::string name;
			std::vector<Armament> arms;
			coord_type aim;

			ArmamentGroup();
			ArmamentGroup( const std::string&, const boost::python::object& );
			
			void shot( Stage& stage, const coord_type& unit_pos );
			void update( double delta );
			int getReadyCount() const;

			std::vector<Armament>::iterator begin();
			std::vector<Armament>::iterator end();
		};

		std::string type;
		std::string name;
		coord_type pos;
		double face;
		double speed;
		double turn_pef;

		enum class TurnType {
			TURNING_RADIUS, SPIN_TURN, SHIP_TURN,
		} turn_type;

		Module body;
		Engine engine;

	protected:
		Stage* stage;

		std::vector<ArmamentGroup> arms;
		size_t current_arm;
		double target_speed;
		double target_angle;

	public:
		Unit();
		Unit( const boost::python::dict& d );
		~Unit();

		void enter( Stage* _stage );
		void retreat();

		bool alive() const;
		size_t getArmCount() const;
		ArmamentGroup& getCurrentArm();
		coord_type getCurrentAim() const;
		int getReadyCount() const;
		void nextArm();
		void moveAim( double x_diff, double y_diff );
		void pivotAim( double distance_diff, double rotation );
		void shot();
		void accel( double v );
		void turn( double v );

		bool hit( const coord_type& p, const FlyingBullet& b );

		void genBuffers();
		void draw();
		void update( double delta );

		//for python
		std::vector<ArmamentGroup>::iterator arms_begin();
		std::vector<ArmamentGroup>::iterator arms_end();
	};
}

#endif