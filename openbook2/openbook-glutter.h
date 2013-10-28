/** openbook-glutter.h - Glut handler - includes callback functions. **/

class Glutter
{
    public:
    static void Initialise(int argc, char *argv[]);
    static void Initialise();
    static void InitialiseLighting();
    static void Finish();

    static void Resize(int xiWidth, int xiHeight);
    static void Redisplay();
    static void Display();

    static void AddUmbrella(Umbrella* xiUmbrella);

    static void Mouse(int button, int state, int x, int y);
    static void Key(unsigned char key, int x, int y);
    static void SpecialKey(int key,    int x, int y);

    static void ProcessEvents();
    static void ProcessEvent();
    static void Idle();

    static void Animate(float xiSecs);

    static void AddText(string* xiText);
    static void AddChar(char xiChar);

    static void RenderStyle(eRenderStyle xiStyle);

    static void LoadTextures();
    static int LoadTexture(char* xiTextureFile);
    static void BindTexture(eTexture xiTextureId);

    static int mTextureCount;
    static Texture* mTextures;
    static GLuint* mTextureId;

    private:
    static int mFocalWin;
    static list<Umbrella*> mUmbrellaList;
    static bool mPostedRedisplay;

    static float mLastUpdateTime;

};
