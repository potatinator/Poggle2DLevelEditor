#include <glad/glad.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "game.h"
#include "peg.h"
#include "ui.h"

Game2D::Game2D(unsigned int width, unsigned int height) : width(width), height(height), keys(), mousePos(0.0, 0.0), wireframe(false), debug(false){}

Game2D::~Game2D(){
    //cleanup
}

void Game2D::init(){
    //load

    objects.push_back(
        new TexturedGameObject(
            glm::vec2(0.0f, 0.0f), // position
            Texture2D("./resources/grid.png", true),
            glm::vec2(800.0f, 600.0f), // scale
            8
    ));    
}
void Game2D::update(float dt){
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.0f, 10.0f);

    

    for(GameObject* object : objects){
        object->setDebug(debug);
        object->setView(view);
        object->update(dt);
    }
    for(Peg* object : pegs){
        if(object != nullptr){
            object->setDebug(debug);
            object->setView(view);
            object->update(dt);
        }
    }
}

void Game2D::render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if(wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    for(GameObject* object : objects){
        object->draw();
    }
    for(Peg* object : pegs){
        if(object != nullptr){
            object->draw();
        }
    }
}

void Game2D::processInputs(){
    debug = keys[GLFW_KEY_S];
    wireframe = keys[GLFW_KEY_W] || this->debug;

    if(leftMouse && !leftLast){
        pegs.push_back(new Peg(
            mousePos
        ));
    }
    if(rightMouse && !rightLast){
        for(int i=0; i<pegs.size(); i++){
            Peg* p = pegs.at(i);
            if(p == nullptr){
                continue;
            }
            if(glm::length(p->position+p->scale/glm::vec2(2.0) - mousePos) < p->scale.x/2.0){
                pegs.at(i) = nullptr;
            }
        }
    }
    if(keys[GLFW_KEY_C] && !oldKeys[GLFW_KEY_C] && keys[GLFW_KEY_LEFT_SHIFT]){
        for(int i=0; i<pegs.size(); i++){
            pegs.at(i) = nullptr;
        }
    }
    if(keys[GLFW_KEY_Q] && !oldKeys[GLFW_KEY_Q]){
        save.save(pegs);
    }
    if(keys[GLFW_KEY_E] && !oldKeys[GLFW_KEY_E]){
        string s;
        cout << "enter save file to load: " << endl;
        cin >> s;
        std::vector<Peg*> tmp = save.load(s);
        pegs = tmp;
    }

    rightLast = rightMouse;
    leftLast = leftMouse;
    for(int i=0; i<sizeof(keys)/sizeof(bool); i++){
        oldKeys[i] = keys[i];
    }
}