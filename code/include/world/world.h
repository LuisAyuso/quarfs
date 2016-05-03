

#pragma once

#include <iostream>
#include <assert.h>

#include "world/cube.h"

namespace quarfs{

class TreeNode{
public:
	typedef unsigned idType;

private:
	idType id;
    int x, y, w, h;

    std::vector<TreeNode> childs;     // this can be a vector BUT not an array?  interesting
    std::vector<DrawNode> elems;	// TODO: the VAO should no be stored here, here just possitions

    bool leaf;
	bool changed;

	static idType idCount;
public: 

    TreeNode ();
    TreeNode (int x, int y, int w, int h);

	idType getId() const {return id;}

    void addElement (const DrawNode& elem);
    void distributeElement (const DrawNode& elem);
    bool isLeaf()const ;

	bool wasUpdated() const;
	void setBaseline();

    const std::vector<DrawNode> getElements() const;
    std::vector<DrawNode>& 		getElements();

    const std::vector<TreeNode> getChildren() const;

    friend std::ostream& operator<<(std::ostream& out, const TreeNode& tn);
};

std::ostream& operator<<(std::ostream& out, const TreeNode& tn);


template <typename BASE> 
struct ConstTreeVisitor {
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
struct PrunableTreeVisitor {
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
class LevelTreeVisitor {
private:
	unsigned level;

public:
	LevelTreeVisitor()
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


class WorldFactory;

class World{

    TreeNode nodeTree;
	unsigned width, height;

    World(); // only constructible by factory

public:
    ~World();

	unsigned getWidth()  const {return width;}
	unsigned getHeight() const {return height;}

    bool update();
    const TreeNode& getTree() const { return nodeTree; }

	friend WorldFactory;
    friend std::ostream& operator<<(std::ostream& out, const World&);
};

std::ostream& operator<<(std::ostream& out, const World& );

class WorldFactory{
	public:
	static World getSmallWorld();
	static World getEmptyWorld() { return World(); }
};

} //namespace quarfs{
