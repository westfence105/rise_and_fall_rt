#include "GL/glew.h"

#include "Unit.hpp"
#include "Stage.hpp"

using namespace soundbag;
using namespace rise_and_fall;


void Stage::genBuffers(){
	for( auto& unit : units ){
		unit.genBuffers();
	}
}

void Unit::genBuffers(){
	///Not implemented
}

void Stage::draw(){
	for( auto& unit : units ){
		unit.draw();
	}
}

void Unit::draw(){
	body.draw();
	engine.draw();
	for( auto& a_grp : arms ){
		for( auto& arm : a_grp.arms ){
			arm.draw();
		}
	}
}

void Unit::Module::draw(){
	if( vbo_fill[0] * vbo_fill[1] * vbo_line[0] * vbo_line[1] == 0 ){
		return;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_fill[0] );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_fill[1] );
	glVertexPointer( 3, GL_FLOAT, ( 3 + 4 ) * sizeof(float), 0 );
	glColorPointer(  4, GL_FLOAT, ( 3 + 4 ) * sizeof(float), (void*)( sizeof(float) * 3 ) );
	glDrawElements( GL_TRIANGLES, fill_index.size(), GL_UNSIGNED_INT, 0 );

	glBindBuffer(GL_ARRAY_BUFFER, vbo_line[0] );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_line[1] );
	glVertexPointer( 3, GL_FLOAT, ( 3 + 4 ) * sizeof(float), 0 );
	glColorPointer(  4, GL_FLOAT, ( 3 + 4 ) * sizeof(float), (void*)( sizeof(float) * 3 ) );
	glDrawElements( GL_TRIANGLES, line_index.size(), GL_UNSIGNED_INT, 0 );

	glBindBuffer(GL_ARRAY_BUFFER, 0 );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}