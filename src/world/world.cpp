
#include <math.h>
#include "world/world.h"


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

inline double findnoise2(double x,double y) {
    int n=(int)x+(int)y*57;
    n=(n<<13)^n;
    int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
    return 1.0-((double)nn/1073741824.0);
}


inline double interpolate(double a,double b,double x) {
 double ft=x * 3.1415927;
 double f=(1.0-cos(ft))* 0.5;
 return a*(1.0-f)+b*f;
}

inline double noise(double x,double y){
    double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
    double floory=(double)((int)y);
    double s,t,u,v;//Integer declaration
    s=findnoise2(floorx,floory);
    t=findnoise2(floorx+1,floory);
    u=findnoise2(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
    v=findnoise2(floorx+1,floory+1);
    double int1=interpolate(s,t,x-floorx);//Interpolate between the values.
    double int2=interpolate(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
    return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}


inline double perlin(double x,double y) {
    int octaves=2;
    double p = 0.5;
    double zoom = 1;
    double getnoise =0;
    for(int a=0;a<octaves-1;a++) {
        double frequency = pow(2,a);//This increases the frequency with every loop of the octave.
        double amplitude = pow(p,a);//This decreases the amplitude with every loop of the octave.
        getnoise += noise(x*frequency/zoom,y/zoom*frequency)*amplitude;
    }
    return getnoise;
  }
///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7

World::World()
        : nodeTree(-128,-128, 256, 256, 0)
{

 // nodeTree.addElement(DrawNode (0.5f, 0.0f, 0.0f));
 // nodeTree.addElement(DrawNode (0.5f, 0.0f, 0.0f));
 // nodeTree.addElement(DrawNode (0.0f, 0.5f, 0.0f));
 // nodeTree.addElement(DrawNode (-1.0f, 0.0f, 0.0f));
 // nodeTree.addElement(DrawNode (.0f, 0.0f, 0.0f));
 // nodeTree.addElement(DrawNode (.0f, 0.0f, 1.0f));

  //  nodeTree.addElement(DrawNode (1.0f, 0.0f, -3.0f));
  //  nodeTree.addElement(DrawNode (1.0f, 0.0f, -2.0f));

    for (float i =-10; i < 10; ++i){
        std::cout << ".";
        for (float j =-10; j < 10; ++j){
            const auto& e = DrawNode (i, perlin(i,j),j);
      //      std::cout << e << std::endl;
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

