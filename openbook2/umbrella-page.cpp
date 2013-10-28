/** umbrella-page.cpp - The Page object - responsible for lines and display. **/

#include "openbook2.h"

Page::Page(Umbrella* xiParent, int xiPageNumber) : Umbrella(xiParent)
{
    Logger::Log("Initialising a Page object (default params)...", LOG_DEBUG);
    initialise(7.9, 11.9, xiPageNumber);
}

Page::Page(Umbrella* xiParent, float xiWidth, float xiHeight, int xiPageNumber) : Umbrella(xiParent)
{
    Logger::Log("Initialising a Page object (specified params)...", LOG_DEBUG);
    initialise(xiWidth, xiHeight, xiPageNumber);
}

void Page::animate(float xiSecs)
{
    //Logger::Log("Entering Page.animate...", LOG_DEBUG);

    // step through each line object, invoking its animate method
    list<Line*>::iterator lCurrentLine;
    for (lCurrentLine = mLines.begin(); lCurrentLine != mLines.end(); lCurrentLine++)
    {
        (*lCurrentLine)->animate(xiSecs);
    }

    // qqLGW: bin this soon
    mNumberLine->animate(xiSecs);

    // restore visibility state
    if (mNewLineVisibilityState < 0.0)
        mNewLineVisibilityState += (mNewLineVisibilitySpeed * xiSecs);

}

void Page::display()
{
    //Logger::Log("Entering Page.display...", LOG_DEBUG);
    glPushMatrix(); // far spine

    Glutter::RenderStyle(STYLE_JAGGY_POLY);
    Glutter::RenderStyle(STYLE_TEXTURES_ON);
    Glutter::BindTexture(TEXTURE_PAPER);

    float lTextureIterationsX = 3.0;
    float lTextureIterationsY = lTextureIterationsX * (mHeight / mWidth);

    // render
    glColor3f(1.0, 1.0, 0.9);
    glBegin(GL_QUADS);
      glNormal3f(0.0, 0.0, -1.0);

      glTexCoord2f (0.0, 0.0);
      glVertex3f   (0.0, -mHeight, 0.0);

      glTexCoord2f (0.0, lTextureIterationsY);
      glVertex3f   (0.0, 0.0, 0.0);

      glTexCoord2f (lTextureIterationsX, lTextureIterationsY);
      glVertex3f   (mWidth, 0.0, 0.0);

      glTexCoord2f (lTextureIterationsX, 0.0);
      glVertex3f   (mWidth, -mHeight, 0.0);
    glEnd();

    Glutter::RenderStyle(STYLE_TEXTURES_OFF);

    // come forwards a little
    glTranslated(0.0, 0.0, 0.02);

    // drop by height of first line
    glTranslated(mMargin, -(mMargin + (*mLines.begin())->height()), 0.0);

    // step through each line object, invoking its display method
    list<Line*>::iterator lCurrentLine;
    for (lCurrentLine = mLines.begin(); lCurrentLine != mLines.end(); lCurrentLine++)
    {
        (*lCurrentLine)->display();
    }

    if (mNumberLine != NULL)
    {
        glTranslated((mWidth/2) - ((mMargin + mNumberLine->stringWidth()/2)), 0.0, 0.0);
        mNumberLine->display();
    }
    glPopMatrix(); // far spine
}

void Page::displayFrame()
{
    glPushMatrix(); // far spine

    glColor3f(1.0, 1.0, 0.8);

    glBegin(GL_QUADS);
      // far end
      glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(0.0,    0.0, 0.0);
      glVertex3f(mWidth, 0.0, 0.0);
      glVertex3f(mWidth, 0.0, mStackHeight);
      glVertex3f(0.0,    0.0, mStackHeight);

      // near end
      glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(0.0,    -mHeight, 0.0);
      glVertex3f(mWidth, -mHeight, 0.0);
      glVertex3f(mWidth, -mHeight, mStackHeight);
      glVertex3f(0.0,    -mHeight, mStackHeight);

      // left side
      glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0,      0.0);
      glVertex3f(0.0, -mHeight, 0.0);
      glVertex3f(0.0, -mHeight, mStackHeight);
      glVertex3f(0.0, 0.0,      mStackHeight);

      // right side
      glNormal3f(1.0, 0.0, 0.0);
      glVertex3f(mWidth, 0.0,      0.0);
      glVertex3f(mWidth, -mHeight, 0.0);
      glVertex3f(mWidth, -mHeight, mStackHeight);
      glVertex3f(mWidth, 0.0,      mStackHeight);
    glEnd();

    glPopMatrix();  // far spine
}

void Page::initialise(float xiWidth, float xiHeight, int xiPageNumber)
{
    Logger::Log("Entering Page.initialise...", LOG_DEBUG);

    mWidth  = xiWidth;
    mHeight = xiHeight;
    mMargin = 0.4;

    mStackHeight = -0.5;

    mNewLineVisibilityState = 0.0;
    mNewLineVisibilitySpeed = 2.0;

    // create blank first line
    mLines.push_back(new Line(this, 0.0, mWidth - (mMargin*2)));
    mLastLine = *mLines.begin();

    if (xiPageNumber > 0)
    {
        // set up line number
        stringstream lStringStream;
        lStringStream << xiPageNumber;
        string* lNumberString = new string(lStringStream.str());
        mNumberLine = new Line(this, mHeight - (mMargin * 2) - (mLastLine->height() * 1.5), mWidth - (mMargin*2));
        mNumberLine->addText(lNumberString);
    }
    else
    {
        mNumberLine = NULL;
    }
}

void Page::setNotLastNow()
{
    Logger::Log("Entering Page.setNotLastNow()...", LOG_DEBUG);

    string* lDots = new string("...");
    mNumberLine->addText(lDots);
    delete lDots;

    Glutter::Redisplay();
}

bool Page::newLine()
{
    Logger::Log("Entering Page.newLine...", LOG_DEBUG);

    float lLinesHeight = mHeight - (mMargin * 2) - (mLastLine->height() * 2);

    if (mLastLine->bottom() + mLastLine->height() < lLinesHeight)
    {
        Line* lNewLine = new Line(this, mLastLine->bottom(), mWidth - (mMargin*2), mNewLineVisibilityState);
        mLines.push_back(lNewLine);
        mLastLine = lNewLine;

        mNewLineVisibilityState -= 0.05;
        return true;
    }
    else
    {
        return false;
    }
}

bool Page::addText(string* xiText)
{
    Logger::Log("Entering Page.addText (string)...", LOG_DEBUG);
    while (!mLastLine->addText(xiText))
    {
        // attempt to add to a new line
        Logger::Log("Adding remainder of text to a new line.", LOG_DEBUG);
        if (!newLine())
        {
            // if we can't add another line (out of space!)
            // bin out. the string now contains the remainder.
            return false;
        }
    }

    // lies (for now)
    return true;
}

bool Page::addChar(char xiChar)
{
    Logger::Log("Entering Page.addChar...", LOG_DEBUG);

    if (xiChar == '\n')
    {
        Logger::Log("Newline character received.", LOG_DEBUG);
        if (!newLine())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    while (!mLastLine->addChar(xiChar))
    {
        Logger::Log("Adding char to a new line.", LOG_DEBUG);
        if (!newLine())
        {
            return false;
        }
    }

    return true;
}

float Page::getStackHeight()
{
    return mStackHeight;
}
