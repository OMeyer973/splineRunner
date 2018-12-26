#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/common.hpp>
#include <glimac/Sphere.hpp>
#include <GL/glew.h>
#include <iostream>
#include <splineengine/GameManager.hpp>
#include <splineengine/Settings.hpp>
#include <splineengine/GameObject.hpp>
#include <splineengine/Player.hpp>
#include <splineengine/Spline.hpp>
#include <glm/gtc/noise.hpp>
#include <glimac/Image.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Geometry.hpp>
#include <splineengine/Model.hpp>
#include <splineengine/Texture.hpp>
#include <splineengine/POVCamera.hpp>
#include <splineengine/TrackballCamera.hpp>
#include <splineengine/RenderManager.hpp>
#include <splineengine/Settings.hpp>
#include <splineengine/common.hpp>
#include <splineengine/CubeMap.hpp>

//fps counter
#include <time.h>
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 30;


using namespace splineengine;

// cmake ../splineRacer && make -j 4 && ./TP_splineTest/TP_splineTest_ingameMenu



int initial_time = time(NULL), final_time, frame_count;

// glm::vec3 spline.point(float t) {
//     return glm::vec3(
//         t+30*glm::perlin(glm::vec2(0.1*t)),
//         t-30*glm::perlin(glm::vec2(-0.1*t)),
//         t+30*glm::pe rlin(glm::vec2(0.1*t+100))
//     );
// }

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    glimac::SDLWindowManager windowManager(800, 600, "splineRacer");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * INITIALIZATION CODE
     *********************************/

    Settings& settings = Settings::instance();
    settings.appPath() = glimac::FilePath(argv[0]);
    GameManager& gameManager = GameManager::instance();
    glimac::Sphere sphere(2, 3, 2);
    Player player;
    Spline spline;

    // Création d'un seul VBO = contient les données
    GLuint vbo;
    glGenBuffers(1, &vbo);

    //Binding d'un VBO sur la cible GL_ARRAY_BUFFER: permet de la modifier
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //On peut à présent modifier le VBO en passant par la cible

    //Envoi des données
    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof (glimac::Geometry::Vertex), sphere.getDataPointer(), GL_STATIC_DRAW);
    //On utilise GL_STATIC_DRAW pour un buffer dont les données ne changeront jamais.

    //Débindage, pour éviter de remodifier le VBO par erreur.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Création du VAO (Vertex Array Object) = décrit les données dans le VBOs
    GLuint vao;
    glGenVertexArrays(1, &vao);

    //Binding du VAO
    glBindVertexArray(vao);

    //Activation des attributs de vertex
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_COORDINATE = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_COORDINATE);

    //Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //Spécification des attributs de vertex
    //glVertexAttribPointer(GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof (glimac::Geometry::Vertex), (const GLvoid*) offsetof(glimac::Geometry::Vertex, m_Position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof (glimac::Geometry::Vertex), (const GLvoid*) offsetof(glimac::Geometry::Vertex, m_Normal));
    glVertexAttribPointer(VERTEX_ATTR_COORDINATE, 2, GL_FLOAT, GL_FALSE, sizeof (glimac::Geometry::Vertex), (const GLvoid*) offsetof(glimac::Geometry::Vertex, m_TexCoords));

    //Débindage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /****

    PROGRAMME DE TEXTURES

    ***/



    glimac::FilePath applicationPath(argv[0]);

    Texture buttonContinue("Continue");

    buttonContinue.loadTexture();




    // Charger et compiler les shaders
    glimac::Program program = glimac::loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                applicationPath.dirPath() + "shaders/normals.fs.glsl");
    glimac::Program menu = glimac::loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                applicationPath.dirPath() + "shaders/tex3D.fs.glsl");

    glimac::Program skyBoxProgram = glimac::loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                applicationPath.dirPath() + "shaders/tex3D.fs.glsl");

    GLint MVPMatrixLocation = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLint MVMatrixLocation = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLint NormalMatrixLocation = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
    GLint textureLocation = glGetUniformLocation(program.getGLId(), "uTexture");

    Model planeModel("plane");
    GameObject planeObject(planeModel, spline);
    Texture planeTex("menu");



    Model menuModel("menu");

    Model skyBox("skybox");
    Texture skyBoxTex("skurt.png");
    skyBoxTex.loadTexture();


    glEnable(GL_DEPTH_TEST);


    std::vector<std::unique_ptr<Camera>> cameras; // Contains two pointers on camera
	cameras.emplace_back(new POVCamera());
	cameras.emplace_back(new TrackballCamera());
	int chosenCamera = TRACKBALL_CAMERA;

	RenderManager renderManager(*cameras[chosenCamera]);

	float rotateSpeed = 1.0f;
	float zoom = 1.0f;

    bool displayInGameMenu=false;
    // Application loop:
    bool done = false;
    while(!done) {
        //set tickers to 0 for framerate cap
        Uint32 startTime = SDL_GetTicks();
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            switch (e.type) {
                case SDL_QUIT :
                    done = true; // Leave the loop after this iteration
                    break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym==SDLK_q){ //going left
                    player.goingLeft() = 1.f;
                }
                if (e.key.keysym.sym==SDLK_d){//going right
                    player.goingLeft() = -1.f;
                }
                 if (e.key.keysym.sym==SDLK_z){//going up
                    player.goingUp() = 1.f;
                }
                if (e.key.keysym.sym==SDLK_s){//going down
                    player.goingUp() = -1.f;
                }
                if(e.key.keysym.sym==SDLK_ESCAPE && !displayInGameMenu){
                    displayInGameMenu = true;
                }else if(e.key.keysym.sym == SDLK_ESCAPE && displayInGameMenu ){
                    displayInGameMenu = false;
                }
                break;

            case SDL_KEYUP:
                if (e.key.keysym.sym==SDLK_q && player.goingLeft() > 0) {//stop going left
                    player.goingLeft() = 0.f;
                }
                if (e.key.keysym.sym==SDLK_d && player.goingLeft() < 0) {//stop going right
                    player.goingLeft() = 0.f;
                }
                if (e.key.keysym.sym==SDLK_z && player.goingUp() > 0) {//stop going up
                    player.goingUp() = 0.f;
                }
                if (e.key.keysym.sym==SDLK_s && player.goingUp() < 0){//stop going down
                    player.goingUp() = 0.f;
                    ;
                }
                break;
            }
        }


        /*********************************
         * RENDERING CODE
         *********************************/
        gameManager.update();
        //std::cout << gameManager.dtime() << std::endl;


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //calcul des view matrix, model matrix, projetion matrix
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(110.f), 4.f/3.f, 0.1f, 100.f);

        //glm::mat4 MVMatrix = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f ,-5.f));
        ///////////////////////////////////////////////////////////////////////
        // spline stuff


        glm::mat4 camMatrix = glm::mat4();
        //curve part
        glm::mat4 MVMatrix ;

        MVMatrix = camMatrix;
        //MVMatrix = glm::translate(camMatrix, spline.point(t));




        if(displayInGameMenu){
            menu.use(); // Indiquer a OpenGL de les utiliser

            //:cout << "menu" << std::endl;
            glBindTexture(GL_TEXTURE_2D, buttonContinue.getTextureID() );
            glUniform1i(textureLocation, 0);

            MVMatrix = glm::scale(MVMatrix, glm::vec3(0.2f,0.2f,0.2f));
            MVMatrix = glm::translate(MVMatrix, glm::vec3(0,0,-15));
            glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

            //on récupère les locations des variables uniformes dans les shaders
            GLint uMVPMatrixLocation = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
            GLint uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
            GLint uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
            //on envoie les matrices à la CG dans les variables uniformes
            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE,  glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE,  glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE,  glm::value_ptr(NormalMatrix));

            glBindVertexArray(menuModel.getVAO());

            //chargement des textures

            glDrawElements(GL_TRIANGLES, menuModel.geometry().getIndexCount(), GL_UNSIGNED_INT, 0); // Draw all meshes
            glBindVertexArray(0);
    		    glBindTexture(GL_TEXTURE_2D, 0);


            //std::cout << "debug" << std::endl
        }

        program.use();

        planeObject.scale() = glm::vec3(1);
        planeObject.sPosition() = glm::vec3(0);
        planeObject.rotation() = glm::vec3(.25*cos(2*windowManager.getTime()), .25*cos(.5*windowManager.getTime()), 0);

        renderManager.updateMVMatrix(*cameras[chosenCamera], planeObject.matrix());

        glBindTexture(GL_TEXTURE_2D, planeTex.getTextureID());
        glUniform1i(textureLocation, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(renderManager.projMatrix() * renderManager.MVMatrix()));
        glUniformMatrix4fv(MVMatrixLocation, 1, GL_FALSE, glm::value_ptr(renderManager.MVMatrix()));
        glUniformMatrix4fv(NormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(renderManager.normalMatrix()));

        planeObject.draw();

        //               TEST SKYBOX

        //chargement des textures
        //std::cout<<"oups"<<std::endl;

        skyBoxProgram.use();
        glBindTexture(GL_TEXTURE_2D, skyBoxTex.getTextureID() );
        glUniform1i(textureLocation, 0);

        MVMatrix = glm::scale(MVMatrix, glm::vec3(25,25,-25));
        MVMatrix = glm::translate(MVMatrix, glm::vec3(0,0,0));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        //on récupère les locations des variables uniformes dans les shaders
        GLint uMVPMatrixLocation = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
        GLint uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
        GLint uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
        //on envoie les matrices à la CG dans les variables uniformes
        glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE,  glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE,  glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE,  glm::value_ptr(NormalMatrix));

        glBindVertexArray(skyBox.getVAO());

        //chargement des textures

        glDrawElements(GL_TRIANGLES, skyBox.geometry().getIndexCount(), GL_UNSIGNED_INT, 0); // Draw all meshes
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);



        // FIN TEST SKYBOX

        //updating player inner variables (speed, position...)
        if(!displayInGameMenu){
            player.update(settings.deltaTime());
            program.use();
        }




        program.use(); // Indiquer a OpenGL de les utiliser
        camMatrix = spline.camMatrix(player.sPosition());

        for (float t=1; t<100; t+=0.05f) {

            //curve part

            MVMatrix = camMatrix * spline.matrix(glm::vec3(t,0,0));
            //MVMatrix = glm::translate(camMatrix, spline.point(t));
            MVMatrix = glm::scale(MVMatrix, glm::vec3(0.2));



            for (int i=0; i<3; ++i) {

                if (i==1) {
                    MVMatrix = glm::translate(MVMatrix, glm::vec3(3,0,0));
                }
                if (i==2) {
                    MVMatrix = glm::translate(MVMatrix, glm::vec3(0,3,0));
                }

                // end spline stuff
                ////////////////////////////////////////////////////////////////
                glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

                //on récupère les locations des variables uniformes dans les shaders
                GLint uMVPMatrixLocation = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
                GLint uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
                GLint uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
                //on envoie les matrices à la CG dans les variables uniformes
                glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE,  glm::value_ptr(ProjMatrix * MVMatrix));
                glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE,  glm::value_ptr(MVMatrix));
                glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE,  glm::value_ptr(NormalMatrix));

                //Binding du VAO
                glBindVertexArray(vao);

                glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

                glBindVertexArray(0);
            }



        }
        // Update the display
        windowManager.swapBuffers();
        //fps count
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
		if(elapsedTime <= FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}



    }

    return EXIT_SUCCESS;
}
