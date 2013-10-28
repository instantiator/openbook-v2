/** umbrella-line.h - The Line object - responsible for characters and display. **/

class Line : public Umbrella
{
    public:
    Line(Umbrella* xiParent);
    Line(Umbrella* xiParent, float y, float xiWidth);
    Line(Umbrella* xiParent, float y, float xiWidth, float xiVisibility);

    void display();
    void animate(float xiSecs);

    bool addText(string* xiText);
    bool addChar(char xiChar);
    void removeChars(int xiNumberToRemove);

    float height();
    float bottom();

    float stringWidth();

    private:
    void initialise(float y, float width, float visibility);
    float stringWidth(string* xiText);

    float  mY;
    float  mWidth;
    string mText;
    void*  mFont;
    float  mScale;

    int   mFixedChars;
    float mVisibility;
    float mVisibilitySpeed;
};
