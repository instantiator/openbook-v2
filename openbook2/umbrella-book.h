/** umbrella-book.h - The Book object - responsible for pages and display. **/

class Book : public Umbrella
{
    public:
    Book();
    Book(float xiWidth, float xiHeight, float xiXt, float xiYt, float xiZt, float xiX, float xiY, float xiZ);

    void display();
    void displayCover();
    void animate(float xiSecs);
    bool key(int xiChar);

    bool addText(string* xiText);
    bool addChar(char xiChar);

    private:
    void initialise(float xiWidth, float xiHeight, float xiXt, float xiYt, float xiZt, float xiX, float xiY, float xiZ);
    bool newPage();
    void createPage();

    void position();

    float mWidth;
    float mHeight;
    float mMargin;
    float mCoverMargin;
    float mX; float mXt;
    float mY; float mYt;
    float mZ; float mZt;

    float mPosition;     // state of 'turn' of the focal page - from 0.0 to 1.0
    float mResetSpeed;   // change of position per sec
    float mCurrentSpeed; // current speed

    int mPageCount;

    list<Page*> mPages;                 // pointer to the first page
    list<Page*>::iterator mLastPage;    // pointer to the last page
    list<Page*>::iterator mCurrentPage; // pointer to the focal page

    float mCoverPosition;               // state of 'turn' of the cover - 0.0 = closed, 1.0 = open
    float mCoverPositionT;              // target for cover position
    float mCoverPositionSpeed;          // rate of change of cover position, in degrees per second

    float mOpenAngle;                   // angle of an open book: in POSITIVE degrees
    Page* mInsidePage;                  // the page inside the cover
};

