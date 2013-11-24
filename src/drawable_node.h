#pragma once
#include <vector>

/**
 * this an octree node, it divides space in 8 sub spaces
 */
template <typename T>
class OcNode {

//  std::array<OcNode<T>,8> childs;  // WRONG. incomplete type
    std::vector<OcNode<T>> childs;     // this can be a vector BUT not an array?  interesting
    std::vector<T> elems;
    bool leaf;

    unsigned x,y;
    unsigned w,h;

public:

    OcNode()
        : leaf(true), childs(0)
    {}

    bool isLeaf()const {
        return leaf;
    }

    std::vector<T>& getElements(){
        return elems;
    }

    const std::vector<T>& getElements() const{
        return elems;
    }

    void addElement (const T& elem){
        if (leaf){
            elems.push_back(elem);
        // // if condition to split. 
        // // // create 8 childs, distribute elements
        // // // not leaf anymore
        // // else
        // // // add to the list
        // if not leaf, forward to the right child
        }
    }
};







template <typename BASE, typename E>
class DrawingVisitor {

    public:

        // default behavior of non implemented functions
        void visitElem (const E& elem){
        }

        void TraverseTree(const OcNode<E>& node ){
            if (node.isLeaf()){
                for(const auto& e : node.getElements())
                    static_cast<BASE*>(this)->visitElem(e);
            }
            // TODO: recursive descend
        }
};
