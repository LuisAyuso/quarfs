

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

template <typename BASE, typename E>
class DrawingVisitor {

    public:

        // default behavior of non implemented functions
        void visitElem (const E& elem){
        }

        void traverseTree(const TreeNode& node ){
            if (node.isLeaf()){
                for(const auto& e : node.getElements())
                    static_cast<BASE*>(this)->visitElem(e);
            }
            else{
                for(const auto& n : node.getChildren())
                    traverseTree(n);

            }
        }
};


std::ostream& operator<<(std::ostream& out, const TreeNode& tn);
class World{

    TreeNode nodeTree;

public:

    World();
    ~World();

    bool update();
    const TreeNode& getTree() const { return nodeTree; }
};
