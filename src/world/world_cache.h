#pragma once


#include "world/world.h"
#include "render/renderer.h"
#include "world/world.h"
#include "input/listener.h"

#include <GL/glew.h> // include GLEW and new version of GL on Windows

#include <map>
#include <vector>

typedef std::map<TreeNode::idType, InstancedVao> VaoMap;

/**
 * the mission of the world cache is to avoid updating the buffers in the GPU
 * and computing the world once and once again
 * 	it keeps track of the GPU buffers created for the last iteration and 
 * 	reuses them if the corresponding segment of the world did not changed
 */
class WorldCache{

	// for each of the corresponding areas, we keep a map to store 
	// the VAO object that need to be drawn;
	VaoMap map;
	const World& world;
	
public:

	WorldCache (const World& w)
	:world(w) { 
		updateMap(world);
	}
	void updateMap(const World& n);
	void updateMap(const TreeNode& n);

	VaoList getWhatToDraw();
};
