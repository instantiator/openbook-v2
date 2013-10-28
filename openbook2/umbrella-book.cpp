/** umbrella-book.cpp - The Book object - responsible for pages and display. **/

#include "openbook2.h"

Book::Book() : Umbrella(NULL)
{
    Logger::Log("Initialising a Book object (default params)...", LOG_DEBUG);
    initialise(/* w,h */ 8.0, 12.0,
               /* resting positions: x,y,z */ 2.0, -3.9, -8.0,
               /* initial positions: x,y,z */ 2.0, -9.9, -1.0);
}

Book::Book(float xiWidth, float xiHeight,
           float xiX, float xiY, float xiZ,
           float xiStartX, float xiStartY, float xiStartZ) : Umbrella(NULL)
{
    Logger::Log("Initialising a Book object (specified params)...", LOG_DEBUG);
    initialise(xiWidth, xiHeight, xiX, xiY, xiZ, xiStartX, xiStartY, xiStartZ);
}

void Book::initialise(float xiWidth, float xiHeight, float xiXt, float xiYt, float xiZt, float xiX, float xiY, float xiZ)
{
    Logger::Log("Entering Book.initialise...", LOG_DEBUG);

    mWidth  = xiWidth;
    mHeight = xiHeight;

    // starting position
    mX      = xiX;
    mY      = xiY;
    mZ      = xiZ;

    // targets
    mXt     = xiXt;
    mYt     = xiYt;
    mZt     = xiZt;

    mMargin = 0.1;

    mPageCount = 0;

    // place 1 page in the pages list
    createPage();                    // add the initial page
    mCurrentPage = mPages.begin();   // the page that is visible

    mInsidePage = new Page(this, mWidth - mMargin, mHeight - (mMargin*2), 0);

    mPosition     = 0.0;             // zero page turn state
    mResetSpeed   = 1.0;             // 1 second to flip page
    mCurrentSpeed = 0.0;             // not moving

    mCoverPosition = 0.0;            // open book (awww...)
    mCoverPositionT = 1.0;           // open

    mOpenAngle = 170.0;              // specifically how open (180.0 = parallel)
}

void Book::display()
{
    // Logger::Log("Entering Book.display...", LOG_DEBUG);

    glPushMatrix(); // as received - origin
    position();     // move to far spine

    // this may NOT affect the position
    displayCover();

    glPushMatrix(); // far spine
    glRotatef(-mCoverPosition * mOpenAngle, 0.0, 1.0, 0.0); // rotate into position
    mInsidePage->display();                                 // display inside page
    glPopMatrix();  // far spine

    // display current page and stack
    // these do NOT affect the position
    (*mCurrentPage)->display();      // display the currently visible focal page with frame
    (*mCurrentPage)->displayFrame(); // display the paper stack beneath the page (illusory)

    // consider the possibility of displaying the page in amination
    if (mPosition > 0.0 && mCurrentPage != mPages.begin())
    {
        glPushMatrix(); // far spine

        glTranslatef(0.0, 0.0, 0.01);                      // raise fractionally above the page
        glRotatef(-mPosition * (mCoverPosition* mOpenAngle), 0.0, 1.0, 0.0); // rotate into position

        mCurrentPage--;                                    // select previous page
        (*mCurrentPage)->display();                        // display the previous page
        mCurrentPage++;                                    // reselect current page

        glPopMatrix(); // far spine
    }

    glPopMatrix(); // as received - origin
}

bool Book::addText(string* xiText)
{
    Logger::Log("Entering Book.addText...", LOG_DEBUG);
    while (!(*mLastPage)->addText(xiText))
    {
        newPage();
    }
    return true;
}

bool Book::addChar(char xiChar)
{
    Logger::Log("Entering Book.addChar...", LOG_DEBUG);
    while (!(*mLastPage)->addChar(xiChar))
    {
        newPage();
    }
    return true;
}

bool Book::newPage()
{
    Logger::Log("Entering Book.newPage...", LOG_DEBUG);
    (*mLastPage)->setNotLastNow();

    createPage();

    return true;
}

void Book::createPage()
{
    Logger::Log("Entering Book.firstPage...", LOG_DEBUG);

    // create a new page, update mLastPage
    mPages.push_back(new Page(this, mWidth - mMargin, mHeight - (mMargin*2), ++mPageCount));
    mLastPage = --mPages.end();
}

void Book::displayCover()
{
    glPushMatrix(); // far spine

    float lStackHeight = (*mCurrentPage)->getStackHeight();

    glTranslatef(0.0, mMargin, lStackHeight);   // move to BOOK far spine, bottom of stack

    glPushMatrix(); // bottom left of back page

    glColor3f(1.0, 0.0, 0.0);

    // back page
    glNormal3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(0.0,    0.0,      0.0);
        glVertex3f(mWidth, 0.0,      0.0);
        glVertex3f(mWidth, -mHeight, 0.0);
        glVertex3f(0.0,    -mHeight, 0.0);
    glEnd();

    // spine
    glNormal3f(-1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3f(0.0, 0.0,      0.0);
        glVertex3f(0.0, 0.0,      -lStackHeight);
        glVertex3f(0.0, -mHeight, -lStackHeight);
        glVertex3f(0.0, -mHeight, 0.0);
    glEnd();

    glTranslatef(0.0 ,0.0, -lStackHeight);                  // raise above stack
    glRotatef(-mCoverPosition * mOpenAngle, 0.0, 1.0, 0.0); // rotate into position
    glTranslatef(0.0, 0.0, 0.03);                           // 'raise' fractionally again

    Glutter::RenderStyle(STYLE_TEXTURES_ON);
    Glutter::BindTexture(TEXTURE_COVER);

    float lTextureIterationsX = 4.0;
    float lTextureIterationsY = lTextureIterationsX * (mHeight / mWidth);

    // front cover (open)
    glColor3f(1.0, 0.0, 0.0); // colour determined by texture

    glNormal3f(0.0, 0.0, -1.0);
    glBegin(GL_QUADS);
        glTexCoord2f (0.0, lTextureIterationsY);
        glVertex3f(0.0,    0.0,      0.0);

        glTexCoord2f (lTextureIterationsX, lTextureIterationsY);
        glVertex3f(mWidth, 0.0,      0.0);

        glTexCoord2f (lTextureIterationsX, 0.0);
        glVertex3f(mWidth, -mHeight, 0.0);

        glTexCoord2f (0.0, 0.0);
        glVertex3f(0.0,    -mHeight, 0.0);
    glEnd();

    Glutter::RenderStyle(STYLE_TEXTURES_OFF);

    glPopMatrix(); // bottom left of back page
    glPopMatrix(); // far spine

}

// moves to FAR SPINE
void Book::position()
{
    // Logger::Log("Entering Book.position...", LOG_DEBUG);

    glTranslatef (mX, mY, mZ);              /* back a bit, down a bit */
    glRotatef    (-30.0, 1.0, 0.0, 0.0);    /* rotate plane, qqLGW: -30.0 for normal conditions! */
    glRotatef    (7.0,   0.0, 0.0, 1.0);    /* turn book a little */
    glTranslatef (-mWidth/2, mHeight, 0.0); /* move to far spine */
}

bool Book::key(int xiChar)
{
    Logger::Log("Entering Book::key...", LOG_DEBUG);

    switch(xiChar)
    {
        case GLUT_KEY_RIGHT:
            Logger::Log("Page turn onwards! (requested)", LOG_DEBUG);

            if (mCurrentSpeed != mResetSpeed && mPosition != 0.0 && mPosition != 1.0)
            {
                mCurrentSpeed = mResetSpeed;
            }

            // start at 1/100th of a second in (helps reduce flicker)
            if ((mPosition == 0.0 || mPosition == 1.0) && mCurrentPage != mLastPage)
            {
                // alter current page NOW

                mCurrentPage++;
                mCurrentSpeed = mResetSpeed;
                mPosition = mCurrentSpeed * 0.01;
            }

            // post redisplay
            Glutter::Redisplay();

            return true;

        case GLUT_KEY_LEFT:
            Logger::Log("Page turn back! (requested)", LOG_DEBUG);
            if (mCurrentPage != mPages.begin() && mCurrentSpeed != -mResetSpeed)
            {
                // alter current page AFTERWARDS
                mCurrentSpeed = -mResetSpeed;

                // start at 1/100th of a second in (helps reduce flicker)
                if (mPosition == 0.0 || mPosition == 1.0)
                {
                    mPosition = 1.0 + (mCurrentSpeed * 0.01);
                }

                // post redisplay
                Glutter::Redisplay();
            }
            return true;

        case GLUT_KEY_DOWN:
            mOpenAngle += 3.0;
            if (mOpenAngle > 270.0) mOpenAngle = 270.0;
            Glutter::Redisplay();
            break;

        case GLUT_KEY_UP:
            mOpenAngle -= 3.0;
            if (mOpenAngle < 0.0) mOpenAngle = 0.0;
            Glutter::Redisplay();
            break;
    }

    return false;
}

void Book::animate(float xiSecs)
{
    //Logger::Log("Entering Book.animate...", LOG_DEBUG);

    // motion of the entire book
    if (fabs(mZ - mZt) > 0.01) { mZ = (mZ*40 + mZt) /41; } else { mZ = mZt; }
    if (fabs(mX - mXt) > 0.01) { mX = (mX*40 + mXt) /41; } else { mX = mXt; }
    if (fabs(mY - mYt) > 0.01) { mY = (mY*40 + mYt) /41; } else { mY = mYt; }

    if (fabs(mCoverPosition - mCoverPositionT) > 0.01)
    {
        mCoverPosition = (mCoverPosition*40 + mCoverPositionT) /41;
    }
    else
    {
        mCoverPosition = mCoverPositionT;
    }

    if (fabs(mX - mXt) > 0.01 || fabs(mY - mYt) > 0.01 || fabs(mZ - mZt) > 0.01)
    {
        Glutter::Redisplay();
    }

    // test for local animation (page-flip)
    if (mCurrentSpeed != 0.0)
    {
        float mPreviousPosition = mPosition;

        // adjust page position
        mPosition += (mCurrentSpeed * xiSecs);
        Glutter::Redisplay();

        float dPosition = mPosition - mPreviousPosition; // a rough guide
        if (dPosition < 0.0) dPosition = -dPosition;

        // stop animation at either extreme
        if (mCurrentSpeed > 0.0 && mPosition + dPosition > 1.0)
        {
            mPosition     = 0.0;
            mCurrentSpeed = 0.0;
        }

        if (mCurrentSpeed < 0.0 && mPosition - dPosition < 0.0)
        {
            mPosition     = 0.0;
            mCurrentSpeed = 0.0;

            if (mCurrentPage != mPages.begin())
            {
                mCurrentPage--;
            }
        }
    } // page flip animation

    // step through each page object, invoking its animate method
    list<Page*>::iterator lCurrentPage;
    for (lCurrentPage = mPages.begin(); lCurrentPage != mPages.end(); lCurrentPage++)
    {
        (*lCurrentPage)->animate(xiSecs);
    }

}
