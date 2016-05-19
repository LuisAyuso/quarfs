

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


template <typename F, typename ... ARGS>
void visit_depth(const TreeNode& node, F&& f, ARGS&&... args){
    f(node, std::forward<ARGS>(args)...);
    for (const auto& child : node.getChildren())
        visit_depth(child, std::forward<F>(f));
}

template <typename F, typename ... ARGS>
void visit_depth(TreeNode& node, F&& f, ARGS&&... args){
    f(node, std::forward<ARGS>(args)...);
    for (auto& child : node.getChildren())
        visit_depth(child, std::forward<F>(f));
}

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
