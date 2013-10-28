/** openbook-glutter.cpp - Initialise GLUT, handle display functions. **/

#include "openbook2.h"

// Further redeclarations mysteriously required by C++.
int Glutter::mFocalWin;
list<Umbrella*> Glutter::mUmbrellaList;
bool Glutter::mPostedRedisplay;
float Glutter::mLastUpdateTime;
int Glutter::mTextureCount;
Texture* Glutter::mTextures;
GLuint* Glutter::mTextureId;

void Glutter::AddUmbrella(Umbrella* xiUmbrella)
{
    Logger::Log("Entering Glutter::AddUmbrella...", LOG_DEBUG);
    mUmbrellaList.push_back(xiUmbrella);
}

void Glutter::Finish()
{
    Logger::Log("Entering Glutter::Finish...", LOG_DEBUG);

    // clean up the game window

    if (!TEST_MODE)
    {
        glutHideWindow();
        glutLeaveGameMode();
        glutDestroyWindow(mFocalWin);
        glutMainLoopEvent();
    }
    else
    {
        glutLeaveMainLoop();
    }

}

void Glutter::Resize(int xiWidth, int xiHeight)
{
    Logger::Log("Entering Glutter::Resize...", LOG_DEBUG);

    const float lAr = (float) xiWidth / (float) xiHeight;

    glViewport(0, 0, xiWidth, xiHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-lAr, lAr, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Glutter::Display(void)
{
    Logger::Log("Entering Glutter::Display...", LOG_DEBUG);

    // erase and push
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    // process list of umbrellas
    list<Umbrella*>::iterator lCurrentUmbrella;
    for (lCurrentUmbrella = mUmbrellaList.begin(); lCurrentUmbrella != mUmbrellaList.end(); lCurrentUmbrella++)
    {
        // call the display function for each umbrella registered with the Glutter
        Logger::Log("Displaying an Umbrella...", LOG_DEBUG);
        (*lCurrentUmbrella)->display();
    }

    // pop 'n swap
    glPopMatrix();
    glutSwapBuffers();

    // reset the redisplay flag, if set
    mPostedRedisplay = false;
}

void Glutter::SpecialKey(int xiAlternateKey, int x, int y)
{
    Logger::Log("Entering Glutter::SpecialKey...", LOG_DEBUG);

    // process list of umbrellas
    bool lTaken;
    list<Umbrella*>::iterator lCurrentUmbrella;
    for (lCurrentUmbrella = mUmbrellaList.begin(); lCurrentUmbrella != mUmbrellaList.end(); lCurrentUmbrella++)
    {
        // call the display function for each umbrella registered with the Glutter
        Logger::Log("Passing (special) Key to an Umbrella...", LOG_DEBUG);
        if ((*lCurrentUmbrella)->key(xiAlternateKey))
        {
            Logger::Log("Key taken.", LOG_DEBUG);
            lTaken = true;
        }
    }
}

void Glutter::Key(unsigned char xiKey, int x, int y)
{
    Logger::Log("Entering Glutter::Key...", LOG_DEBUG);

    bool lTaken = false;
    if (!lTaken)
    {
        Logger::Log("Passing Key to Frotzer...", LOG_DEBUG);
        Frotzer::AddInputChar(xiKey);
    }
}

void Glutter::AddText(string* xiText)
{
    Logger::Log("Entering Glutter::AddText...", LOG_DEBUG);
    bool lUsed = false;

    // process list of umbrellas
    list<Umbrella*>::iterator lCurrentUmbrella;
    for (lCurrentUmbrella = mUmbrellaList.begin();
         lCurrentUmbrella != mUmbrellaList.end() && !lUsed;
         lCurrentUmbrella++)
    {
        Logger::Log("Adding Text to an Umbrella...", LOG_DEBUG);

        // add text, and test to see if it was used
        if ((*lCurrentUmbrella)->addText(xiText))
        {
            lUsed = true;
        }
    }
}

void Glutter::AddChar(char xiChar)
{
    Logger::Log("Entering Glutter::AddChar...", LOG_DEBUG);
    bool lUsed = false;

    // process list of umbrellas
    list<Umbrella*>::iterator lCurrentUmbrella;
    for (lCurrentUmbrella = mUmbrellaList.begin();
         lCurrentUmbrella != mUmbrellaList.end() && !lUsed;
         lCurrentUmbrella++)
    {
        Logger::Log("Adding a char to an Umbrella...", LOG_DEBUG);

        // add text, and test to see if it was used
        if ((*lCurrentUmbrella)->addChar(xiChar))
        {
            lUsed = true;
        }
    }
}

void Glutter::Mouse(int button, int state, int x, int y)
{
    Logger::Log("Entering Glutter::Mouse...", LOG_DEBUG);

    /*
    if (state == GLUT_UP && button == GLUT_LEFT_BUTTON)
        pm->start();

    if (state == GLUT_UP && button == GLUT_RIGHT_BUTTON)
        quitTheApp();
    */
}

void Glutter::Redisplay(void)
{
    if (!mPostedRedisplay)
    {
        glutPostRedisplay();
        mPostedRedisplay = true;
    }
}

void Glutter::Idle(void)
{
    // do not log animation events, for now
    //Logger::Log("Entering Glutter::Idle...", LOG_DEBUG);

    // calculate time difference
    float lTimeNow = glutGet(GLUT_ELAPSED_TIME);

    // animate and trigger redisplay
    Animate((lTimeNow - mLastUpdateTime) / 1000.0);

    mLastUpdateTime = lTimeNow;
}

void Glutter::Animate(float xiSecs)
{
    // do not log animation events, for now
    //Logger::Log("Entering Glutter::Animate...", LOG_DEBUG);

    list<Umbrella*>::iterator lCurrentUmbrella;
    for (lCurrentUmbrella = mUmbrellaList.begin(); lCurrentUmbrella != mUmbrellaList.end(); lCurrentUmbrella++)
    {
        //Logger::Log("Animating an Umbrella...", LOG_DEBUG);

        // add text, and test to see if it was used
        (*lCurrentUmbrella)->animate(xiSecs);
    }

}

void Glutter::ProcessEvent()
{
    Logger::Log("Entering Glutter::ProcessEvent...", LOG_DEBUG);
    glutMainLoopEvent();
}

void Glutter::ProcessEvents()
{
    Logger::Log("Entering Glutter::ProcessEvents...", LOG_DEBUG);
    glutMainLoop();
}

/* Program entry point */
void Glutter::Initialise()
{
    Logger::Log("Entering Glutter::Initialise (no parameters)...", LOG_DEBUG);

    // initialise glut with no arguments
    int gargc = 0;
    char* gargv[0];
    gargv[0] = "";
    Initialise(gargc, gargv);
}

void Glutter::Initialise(int argc, char *argv[])
{
    Logger::Log("Entering Glutter::Initialise (parameters)...", LOG_DEBUG);

    // initialise glut with no arguments (for now)
    int gargc = 0;
    char* gargv[0];
    gargv[0] = "";
    glutInit(&gargc, gargv);

    // graphics window init
    char* game_mode_string = "1280x800:16";
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    //glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT);

    if (TEST_MODE)
    {
        // window mode, keep mouse
        glutGameModeString(game_mode_string);
        mFocalWin = glutCreateWindow("test window");
    }
    else
    {
        // game mode and hide mouse
        mFocalWin = glutEnterGameMode();
        glutSetCursor(GLUT_CURSOR_NONE);
    }

    // callbacks
    glutReshapeFunc(Resize);
    glutDisplayFunc(Display);
    glutKeyboardFunc(Key);
    glutSpecialFunc(SpecialKey);
    glutMouseFunc(Mouse);
    glutIdleFunc(Idle);

    // parameters
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    InitialiseLighting();

    // final internals
    mPostedRedisplay = true;
    mLastUpdateTime = glutGet(GLUT_ELAPSED_TIME);

    LoadTextures();
}

void Glutter::InitialiseLighting()
{
    Logger::Log("Entering Glutter::InitialiseLighting...", LOG_DEBUG);

    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    GLfloat light_ambient[]  = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 8.0, 0.0, 1.0, 1.0 }; // behind right shoulder

    GLfloat paper_ambient[]  = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat paper_diffuse[]  = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat paper_specular[] = { 0.0, 0.0, 0.0, 0.0 };
    GLfloat no_emissions[]   = { 0.0, 0.0, 0.0, 0.0 };
    GLfloat high_shininess[] = { 100.0 };

    // enable lighting and materials (basic)
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, paper_ambient);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, paper_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, paper_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emissions);
}

void Glutter::RenderStyle(eRenderStyle xiStyle)
{
    //Logger::Log("Entering Glutter::RenderStyle...", LOG_DEBUG);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    switch(xiStyle)
    {
        case STYLE_TEXTURES_ON:
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;

        case STYLE_TEXTURES_OFF:
            glDisable(GL_TEXTURE_2D);
            break;

        case STYLE_SMOOTH_POLY:
            glEnable(GL_POLYGON_SMOOTH);
            glEnable(GL_BLEND);
            break;

        case STYLE_JAGGY_POLY:
            glDisable(GL_POLYGON_SMOOTH);
            glDisable(GL_BLEND);
            break;

        case STYLE_SMOOTH_LINE:
            glEnable(GL_POINT_SMOOTH);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_BLEND);
            break;

        case STYLE_JAGGY_LINE:
            glDisable(GL_POINT_SMOOTH);
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_BLEND);
            break;
    }
}

void Glutter::LoadTextures()
{
    Logger::Log("Entering Glutter::LoadTextures...", LOG_DEBUG);

    Logger::Log("Loading textures...", LOG_NB);
    mTextureCount = 0;

    int lTEXTURES = 2;
    mTextures = new Texture[lTEXTURES];
    mTextureId = new GLuint[lTEXTURES];

    mTextureId[TEXTURE_PAPER] = LoadTexture("textures\\paper.bmp");
    mTextureId[TEXTURE_COVER] = LoadTexture("textures\\cover.bmp");

}

void Glutter::BindTexture(eTexture xiTextureId)
{
    glBindTexture(GL_TEXTURE_2D, mTextureId[xiTextureId]);
}

// returns the ID of the texture loaded
int Glutter::LoadTexture(char* xiTextureFilename)
{
    Logger::Log("Entering Glutter::LoadTexture...", LOG_DEBUG);

    // pop image into Texture struct
    try
    {
        BMP::ImageLoad(xiTextureFilename, &mTextures[mTextureCount]);

        GLuint lTextureId; // the ID of the texture we're creating

        // generate and bind the image
        glGenTextures (1, &lTextureId);
        glBindTexture (GL_TEXTURE_2D, lTextureId);

        glTexImage2D (
            GL_TEXTURE_2D, 0, 3,
            mTextures[mTextureCount].sizeX,
            mTextures[mTextureCount].sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE,
            mTextures[mTextureCount].data);

        mTextureCount++;
        return lTextureId;
    }
    catch (char* e)
    {
        Logger::Log(e, LOG_ERROR);
        throw "Glutter::LoadTexture failed.";
    }
    catch (...)
    {
        throw "Glutter::LoadTexture failed.";
    }
}
