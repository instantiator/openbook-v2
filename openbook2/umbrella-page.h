/** umbrella-page.h - The Page object - responsible for lines and display. **/

class Page : public Umbrella
{
    public:
    Page(Umbrella* xiParent, int xiPageNumber);
    Page(Umbrella* xiParent, float xiWidth, float xiHeight, int xiPageNumber);

    void display();
    void displayFrame();
    void animate(float xiSecs);

    bool addText(string* xiText);
    bool addChar(char xiChar);

    void setNotLastNow();

    float getStackHeight();

    private:
    void initialise(float xiWidth, float xiHeight, int xiPageNumber);

    bool newLine();

    float mWidth;
    float mHeight;
    float mMargin;
    float mStackHeight;

    float mNewLineVisibilityState;
    float mNewLineVisibilitySpeed;

    list<Line*> mLines;
    Line* mLastLine;
    Line* mNumberLine;
};

