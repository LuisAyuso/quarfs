
#include "world/world.h"
#include "world/noise.h"


///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7
namespace quarfs {

TreeNode::idType TreeNode::idCount = 0;

TreeNode::TreeNode (){
    // needs to be here, but should never be called
    assert(false);
}


TreeNode::TreeNode (int x, int y, int w, int h)
    : id(idCount++), x(x), y(y), w(w), h(h), leaf(true), changed(false)
{}


void TreeNode::addElement (const DrawNode& elem){

   // std::cout << "\nin node: " << *this << std::endl;
    assert( elem.x() >= x);
    assert( elem.z() >= y);
    assert( elem.x() < x+w);
    assert( elem.z() < y+h);

    if(leaf){
        // if condition to split
        if (elems.size() >= 100){
            int halfW = w/2;
            int halfH = h/2;

            leaf = false;

            // note this is quadtree, but will change
            childs.push_back( TreeNode(x      , y      , halfH, halfW ));
            childs.push_back( TreeNode(x      , y+halfH, halfH, halfW ));
            childs.push_back( TreeNode(x+halfW, y      , halfH, halfW ));
            childs.push_back( TreeNode(x+halfW, y+halfH, halfH, halfW ));

            for (const DrawNode& e : elems){
                distributeElement(e);
            }
            elems.clear();
            assert(elems.empty());

            distributeElement(elem);
        }
        else{
            elems.push_back(elem);
        }
    }
    else{
        distributeElement(elem);
    }

    assert(elems.size()<= 100);
}

void TreeNode::distributeElement (const DrawNode& elem){
    int halfW = w/2;
    int halfH = h/2;

 //   std::cout << "elem: " << elem << std::endl;
 //   std::cout << "dist: " << *this << std::endl;
 //   std::cout <<  (x+halfW) << " " << (y+halfH) << " : " << halfW << "," << halfH << std::endl;

    if (elem.x() < x+halfW){
        if (elem.z() < y+halfH){
            childs[0].addElement(elem);
        }
        else{
            childs[1].addElement(elem);
        }
    }
    else{
        if (elem.z() < y+halfH){
            childs[2].addElement(elem);
        }
        else{
            childs[3].addElement(elem);
        }
    }
}

bool TreeNode::isLeaf()const {
    return leaf ;
}

bool TreeNode::wasUpdated() const{
	return changed;
}

void TreeNode::setBaseline(){
	changed = true;
	for(auto& c : childs)
		c.setBaseline();
}


const std::vector<DrawNode> TreeNode::getElements() const{
    return elems;
};
std::vector<DrawNode>& TreeNode::getElements(){
    return elems;
};
const std::vector<TreeNode> TreeNode::getChildren() const{
    return childs;
}

std::ostream& operator<<(std::ostream& out, const TreeNode& tn){
    out << "TN[" << tn.x << "," << tn.y << ", " << tn.w 
		<< "x" << tn.h << "] " 
		<< "with: " << tn.elems.size() << (tn.leaf? " LEAF":"");
    return out;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

World::World()
        : nodeTree(-2048, -2048, 4096 , 4096 )
{

//  nodeTree.addElement(DrawNode ( 0.0f, 0.0f, 0.0f));
//  nodeTree.addElement(DrawNode ( 3.0f, 0.0f, 0.0f));
//  nodeTree.addElement(DrawNode ( 0.0f, 3.0f, 0.0f));
//  nodeTree.addElement(DrawNode ( 0.0f, 0.0f, 3.0f));
//  nodeTree.addElement(DrawNode ( 5.0f, 0.0f, 0.0f));
//  nodeTree.addElement(DrawNode ( 0.0f, 5.0f, 0.0f));
//  nodeTree.addElement(DrawNode ( 0.0f, 0.0f, 5.0f));
//  nodeTree.addElement(DrawNode ( 0.0f, 7.0f, 0.0f));
//  nodeTree.addElement(DrawNode ( 0.0f, 0.0f, 7.0f));
//  nodeTree.addElement(DrawNode ( 0.0f, 0.0f, 9.0f));
}

World::~World(){
}

bool World::update(){
    return false;
}

std::ostream& operator<<(std::ostream& out, const World& w){
	out << "world with base node-tree: \n\t" << w.nodeTree;
	return out;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

World WorldFactory::getSmallWorld(){
	World w;
	w.width = 0;
   	w.height= 0;
    for (float i =0; i < 2000; ++i){
		w.width++;
        for (float j =0; j < 2000; ++j){
			w.height++;
		    w.nodeTree.addElement(DrawNode(i,0,j));
        }
    }
    std::cout << " map done: " << w.nodeTree.getElements().size() << std::endl;
	std::cout << w.nodeTree << std::endl;
	return w;
}

}//nameaspace quarfs {
