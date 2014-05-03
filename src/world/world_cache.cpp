
#include "world/world_cache.h"
#include "render/renderer.h"
#include "world/world.h"
#include "input/listener.h"

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <set>

namespace {

		void updateVaoWithPositions (const InstancedVao& ivao, const std::vector<glm::vec3> posList) {
			assert(ivao.vao);

			size_t numElem = ivao.numInstances*3;
			float positions[numElem];
			float* index = positions;
			for(auto e: posList){
				index[0] = e.x;
				index[1] = e.y;
				index[2] = e.z;
				index +=3;
			}

			glBindVertexArray (ivao.vao);

			unsigned ibo;
			glGenBuffers(1, &ibo);  // instances buffer object
			glBindBuffer(GL_ARRAY_BUFFER, ibo);

			// fill with data
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numElem, positions, GL_STATIC_DRAW);

			// set up generic attrib pointers
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));

			// a attrib divisor of 1 means that attribute 2 will advance once
			// every instance (0 would mean once per vertex)
			glVertexAttribDivisor(3, 1);

			glBindBuffer(GL_ARRAY_BUFFER,0);
			assert(glGetError() == GL_NO_ERROR);

			std::cout << "updated buffer for vao: " << ivao.vao << " size: " << numElem*sizeof(GLfloat) << std::endl;
		}



		struct VAOUpdater : public ConstTreeVisitor <VAOUpdater>{

			std::vector<glm::vec3>& list;

			VAOUpdater(std::vector<glm::vec3>& list) 
				: list(list){}

			bool visitElem (const DrawNode& elem){
				list.push_back(elem.getPos());
				return true;
			}
		};
		
		/**
		 * this visitor structure will traverse the tree spliting
		 * the number of sub regions selected. 
		 * will spawn a N childs and beyond this number a VAO
		 * will be generated.
		 */
		struct SectorCollector : public LevelTreeVisitor <SectorCollector>{

			unsigned cutLevel;
			VaoMap& map;

			SectorCollector(unsigned cutLevel,VaoMap& map) 
			: cutLevel(cutLevel), map(map)
			{
				std::cout << "cuting at " << cutLevel << std::endl;
			}

			bool visitNode (const TreeNode& node ){
				// here, count the level, see if stop or not
				std::cout << "visite a node" << std::endl;
				if(getLevel() < cutLevel) return true; 

				std::cout << "reached cut" << std::endl;

				InstancedVao& elem = map[node.getId()];

				// if did changed
				if(node.wasUpdated() || !elem.vao){
				
					if (!elem.vao){ // new node!
						std::cout << "new node found" << std::endl;
						elem.vao = DrawNode::getNewCubeVAO();
					}

					// colect the contained objects
					std::vector<glm::vec3> positions;
					VAOUpdater up(positions);
					up.traverseTree(node);
					elem.numInstances = positions.size();
					std::cout << " with: " << positions.size() << " elements" << std::endl;
								  
					// copy the buffer to GPU
					updateVaoWithPositions(elem, positions);
				}
				return false;
			}
			void visitElem (const DrawNode& node ){}
					
			std::vector<InstancedVao> operator()(const World& w){
				
				// traverse
				traverseTree(w.getTree());
	
				// compute the list
				VaoList res;
				for (auto& fit : map){
					// if updated, recompute
					res.push_back(fit.second);
				}
				return res;
			}
		};

		unsigned updateTreeDao(const DrawNode& node){
			return 0;
		}

}  // anon namespace

void WorldCache::updateMap(const World& w){

		SectorCollector col(1,map);
		col.traverseTree(w.getTree());

		std::cout << "cached the world with " << map.size() << " vaos" << std::endl;
}


void WorldCache::updateMap(const TreeNode& n){
}

VaoList WorldCache::getWhatToDraw(){
	SectorCollector col(4,map);
	return col(world);
}


