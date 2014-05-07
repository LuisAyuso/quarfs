#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


Camera::Camera (const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up, Renderer& renderer)
    : pos(pos), up(up), lookAt(lookAt), renderer(renderer), camMat (){
        update();
}
Camera::~Camera(){
}

void Camera::update(){
    camMat = glm::lookAt (pos, lookAt, up);
    renderer.updateCamera(camMat);
}

void Camera::keyUp(){
    glm::vec3 to = lookAt-pos;
    to = glm::normalize(to);
    pos.z += 0.1;
    lookAt.z += 0.1;
    update();
}
void Camera::keyDown(){
    pos.z -= 0.1;
    lookAt.z -= 0.1;
    update();
}
void Camera::keyRight(){
    pos.x -= 0.1;
    lookAt.x -= 0.1;
    update();
}
void Camera::keyLeft(){
    pos.x += 0.1;
    lookAt.x += 0.1;
    update();
}
void Camera::mouseDiff(float x, float y){

    glm::vec3 to = lookAt-pos;
    to = glm::normalize(to);
    to = glm::rotateX(to, y);
    to = glm::rotateY(to, x);

    lookAt = to+pos;
    update();

   // std::cout << x << "," << y << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


void shotFrame(const TreeNode& tree) {

    static unsigned numElems = 0;
    static unsigned vao = 0;

    if (!numElems){
        std::vector<glm::vec3> posList;
        std::set<unsigned> vaos;
        struct DrawVisitor : public ConstTreeVisitor <DrawVisitor>{
            std::vector<glm::vec3>& list;
            std::set<unsigned>& vaos;
            DrawVisitor(std::vector<glm::vec3>& list, std::set<unsigned>& vaos) 
                : list(list), vaos(vaos) {}
            bool visitElem (const DrawNode& elem){
                list.push_back(elem.getPos());
                vaos.insert(elem.getVao());
				return true;
            }
        } vis(posList, vaos);
        vis.traverseTree(tree);

        numElems =  posList.size();

        float positions[numElems*3];
        float* index = positions;
        for(auto e: posList){
            index[0] = e.x;
            index[1] = e.y;
            index[2] = e.z;
            index +=3;
        }

        // this might be smarter in the future, but now, only one figure
        assert(vaos.size()==1);

        // need to load the vao
        vao = *vaos.begin();

        glBindVertexArray (vao);

        unsigned ibo;
        glGenBuffers(1, &ibo);  // instances buffer object
        glBindBuffer(GL_ARRAY_BUFFER, ibo);

        // fill with data
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*numElems, positions, GL_STATIC_DRAW);

        // set up generic attrib pointers
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));

        // a attrib divisor of 1 means that attribute 2 will advance once
        // every instance (0 would mean once per vertex)
        glVertexAttribDivisor(3, 1);

        glBindBuffer(GL_ARRAY_BUFFER,0);
        assert(glGetError() == GL_NO_ERROR);
    }

    assert(vao);
    glBindVertexArray (vao);
    glDrawElementsInstanced(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0, numElems);
}


void shotFrame(const VaoList& list){

	for(const auto& elem : list){
		if (elem.numInstances > 0){
			glBindVertexArray (elem.vao);
			glDrawElementsInstanced(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0, elem.numInstances);
		}
	}
	glBindVertexArray (0);
}
