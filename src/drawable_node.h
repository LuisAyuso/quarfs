#pragma once
#include <array>
#include <vector>


/**
 * this an octree node, it divides space in 8 sub spaces
 */
template <typename T>
class OcNode : public T {

    std::array<OcNode<T>, 8> childs;
    std::vector<T> elems;
    bool _leaf;

    unsigned x,y;
    unsigned w,h;

public:

    OcNode()
        : _leaf(true)
    {}

    bool leaf(){
        return _leaf;
    }

    std::vector<T>& getElements(){
        return elems;
    }

    const std::vector<T>& getElements() const{
        return elems;
    }

    void addElement (const T& elem){
        // if leaf
        // // if condition to split. 
        // // // create 8 childs, distribute elements
        // // else
        // // // add to the list
        // if not leaf, forward to the right child
    }
};







template <typename T>
class DrawingVisitor : public T{

    private:

        // default behavior of non implemented functions

    public:


        void TraverseTree(){
        }
};
