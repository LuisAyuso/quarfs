
#include "world/world_cache.h"
#include "render/renderer.h"
#include "world/world.h"
#include "input/listener.h"

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <set>

namespace {

		struct VAOUpdater : public DrawingVisitor <VAOUpdater>{

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
		struct SectorCollector : public LevelDrawingVisitor <SectorCollector>{

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

				unsigned& vao = map[&node];
				if (!vao){
						// new node!
					std::cout << "new node found" << std::endl;
					vao = DrawNode::getNewCubeVAO();
				}

				// if did changed
				//if(node->updated()){
				
					// colect the contained objects
					std::vector<glm::vec3> positions;
					VAOUpdater up(positions);
					up.traverseTree(node);
					std::cout << " with: " << positions.size() << " elements" << std::endl;
								  
					// execute the VAO computation
				//}
				return false;
			}
			void visitElem (const DrawNode& node ){}
					
			std::vector<unsigned> operator()(const World& w){
				
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

//
//        numElems =  posList.size();
//
//        float positions[numElems*3];
//        float* index = positions;
//        for(auto e: posList){
//            index[0] = e.x;
//            index[1] = e.y;
//            index[2] = e.z;
//            index +=3;
//        }
//

void WorldCache::updateMap(const World& w){

		SectorCollector col(1,map);
		col.traverseTree(w.getTree());
}


void WorldCache::updateMap(const TreeNode& n){
}

VaoList WorldCache::getWhatToDraw(){
	SectorCollector col(4,map);
	return col(world);
}


