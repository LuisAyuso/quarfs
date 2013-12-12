
#include "world/world.h"
#include "world/noise.h"


///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7

TreeNode::TreeNode (){
    // needs to be here, but should never be called
    assert(false);
}


TreeNode::TreeNode (int x, int y, int w, int h, unsigned level)
    : x(x), y(y), w(w), h(h), level(level), leaf(true)
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
            childs.push_back( TreeNode(x      , y      , halfH, halfW, level+1));
            childs.push_back( TreeNode(x      , y+halfH, halfH, halfW, level+1));
            childs.push_back( TreeNode(x+halfW, y      , halfH, halfW, level+1));
            childs.push_back( TreeNode(x+halfW, y+halfH, halfH, halfW, level+1));

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

const std::vector<DrawNode> TreeNode::getElements() const{
    return elems;
};
std::vector<DrawNode> TreeNode::getElements(){
    return elems;
};
const std::vector<TreeNode> TreeNode::getChildren() const{
    return childs;
}

std::ostream& operator<<(std::ostream& out, const TreeNode& tn){
    out << "TN[(" << tn.level << ") " << tn.x << "," << tn.y << ", " << tn.w << "x" << tn.h << "] with: " << tn.elems.size() << (tn.leaf? " LEAF":"");
    return out;
}

///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7

///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7

World::World()
        : nodeTree(-1024, -1024, 2048 , 2048, 0)
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


    for (float i =-400; i < 400; ++i){
        for (float j =-400; j < 400; ++j){
            const auto& e = DrawNode (i, perlin(i,j),j);
            nodeTree.addElement(e);
        }
    }
    std::cout << " map done " << std::endl;

}

World::~World(){
}

bool World::update(){
    return false;
}

