

#pragma once

#include <iostream>
#include <assert.h>

#include "world/cube.h"


class TreeNode{

    int x, y, w, h;
    unsigned level;

    std::vector<TreeNode> childs;     // this can be a vector BUT not an array?  interesting
    std::vector<DrawNode> elems;

    bool leaf;

    public: 

    TreeNode ();
    TreeNode (int x, int y, int w, int h, unsigned level);

    void addElement (const DrawNode& elem);
    void distributeElement (const DrawNode& elem);
    bool isLeaf()const ;
    const std::vector<DrawNode> getElements() const;
    std::vector<DrawNode> getElements();
    const std::vector<TreeNode> getChildren() const;

    friend std::ostream& operator<<(std::ostream& out, const TreeNode& tn);
};



template <typename BASE> 
struct DrawingVisitor {
    void traverseTree(const TreeNode& node ){
        if (node.isLeaf()){
            for(const auto& e : node.getElements()){
                static_cast<BASE*>(this)->visitElem(e);
			}
        }
        else{
            for(const auto& n : node.getChildren()){
			   	traverseTree(n);
			}
        }
    }
};


template <typename BASE> 
struct PrunableDrawingVisitor {
    bool traverseTree(const TreeNode& node ){
        if (node.isLeaf()){
            for(const auto& e : node.getElements()){
                if (!static_cast<BASE*>(this)->visitElem(e)) return false;
			}
        }
        else{
            for(const auto& n : node.getChildren()){
                if (!traverseTree(n)) return false;
			}
        }
		return true;
    }
};

template <typename BASE, bool VisitLeaves = false> 
class LevelDrawingVisitor {
private:
	unsigned level;

public:
	LevelDrawingVisitor()
	:level(0) {}

   void traverseTree(const TreeNode& node ){
	   if (static_cast<BASE*>(this)->visitNode(node)){
			level++;
	   		if (VisitLeaves && node.isLeaf()){
	   			for(const auto& e : node.getElements()){
	   				static_cast<BASE*>(this)->visitElem(e);
	   			}
	   		}
	   		else{
	   			for(const auto& n : node.getChildren()){
	   				traverseTree(n);
	   			}
	   		}
	   }
   }
   unsigned getLevel()const {return level;}
};

std::ostream& operator<<(std::ostream& out, const TreeNode& tn);
class World{

    TreeNode nodeTree;

	unsigned width, height;

public:

    World();
    ~World();

	unsigned getWidth()  const {return width;}
	unsigned getHeight() const {return height;}

    bool update();
    const TreeNode& getTree() const { return nodeTree; }
};

