#ifndef MYSTAIRS_H
#define MYSTAIRS_H

unsigned int stairsVertexCount=24;

float stairsVertices[]={
	-1,-1,-1,
	1,-1,-1,
	1,1,-1,
	-1,1,-1,

	/*-1,-1,1,
	1,-1,1,
	1,1,1,
	-1,1,1,*/

	-1,-1,-1,
	1,-1,-1,
	1,1,1,
	-1,1,1,

	-1,1,-1,
	1,1,-1,
	1,1,1,
	-1,1,1,

	//-1,-1,-1,
	//-1,-1,1,
	//-1,1,1,
	//-1,1,-1,

	//1,-1,-1,
	//1,-1,1,
	//1,1,1,
	//1,1,-1,
};

float stairsColors[]={
	1,0,0, 1,0,0, 1,0,0, 1,0,0,
	0,1,0, 0,1,0, 0,1,0, 0,1,0,
	0,0,1, 0,0,1, 0,0,1, 0,0,1,
	1,1,0, 1,1,0, 1,1,0, 1,1,0,
	0,1,1, 0,1,1, 0,1,1, 0,1,1,
	1,1,1, 1,1,1, 1,1,1, 1,1,1
};

float stairsNormals[]={
    0,0,-1,
    0,0,-1,
    0,0,-1,
    0,0,-1,
    
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,1,
    
    0,-1,0,
    0,-1,0,
    0,-1,0,
    0,-1,0,
    
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    
    -1,0,0,
    -1,0,0,
    -1,0,0,
    -1,0,0,
    
    1,0,0,
    1,0,0,
    1,0,0,
    1,0,0,
};

float stairsTexCoords[]={
    0,0, 1,0, 1,1, 0,1,
    0,0, 1,0, 1,1, 0,1,
    0,0, 1,0, 1,1, 0,1,
    0,0, 1,0, 1,1, 0,1,
    0,0, 1,0, 1,1, 0,1,
    0,0, 1,0, 1,1, 0,1,
};

#endif