#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "input/listener.h"
#include "shader.h"

namespace quarfs {

struct DebugQuad;
struct ShadowCaster;

struct InstancedVao{
	unsigned vao;
	unsigned numInstances;

	InstancedVao()
	:vao(0), numInstances(0) 
	{}
};
typedef std::vector<InstancedVao> VaoList;


class Renderer :public InputListener{

    // shaders
    std::vector<std::shared_ptr<Shader>> shader_programs;
    unsigned currentShader;
    
    // other objects
    glm::mat4 projection;
    glm::mat4 camera;
    
    mutable unsigned currentCount;

    // debug:
    DebugQuad* debugScreen;
    //ShadowCaster* shadowCaster;

    public:

        Renderer();
        Renderer(const Renderer&) = delete;
        Renderer(const Renderer&&) = delete;
        ~Renderer();

        /**
         * dont execute the initialization until context is ready
         */
        void init(float w, float h);

        void setPerspective (float w, float h);
        
        void beginDraw()const;

        void endDraw()const;

        void updateCamera (const glm::mat4& camera);

        void configureRender(float w, float h);

        // From input. to change shader
        void spacebar();
};

} // namespace quarfs
