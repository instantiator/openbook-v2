/** umbrella-line.cpp - The Line object - responsible for characters and display. **/

#include "openbook2.h"

Line::Line(Umbrella* xiParent) : Umbrella(xiParent)
{
    Logger::Log("Initialising a Line object (default params)...", LOG_DEBUG);
    initialise(0.0, 6.0, 0.0);
}

Line::Line(Umbrella* xiParent, float xiY, float xiWidth) : Umbrella(xiParent)
{
    Logger::Log("Initialising a Line object (specified params)...", LOG_DEBUG);
    initialise(xiY, xiWidth, 0.0);
}

Line::Line(Umbrella* xiParent, float xiY, float xiWidth, float xiVisibility) : Umbrella(xiParent)
{
    Logger::Log("Initialising a Line object (specified params, including visibility)...", LOG_DEBUG);
    initialise(xiY, xiWidth, xiVisibility);
}

void Line::initialise(float xiY, float xiWidth, float xiVisibility)
{
    Logger::Log("Entering Line.initialise...", LOG_DEBUG);

    mY     = xiY;     // position on page
    mWidth = xiWidth; // maximum width of line

    mText  = "";                // start with empty text
    mFont  = GLUT_STROKE_ROMAN; // there is, like, only 1 font in GLUT :(
    mScale = 0.0025;            // scale adjustment for display font

    mFixedChars      = 0;            // number of characters NOT TO ANIMATE
    mVisibility      = xiVisibility; // current state of transparency
    mVisibilitySpeed = 2.0;          // 0.5 secs to visible (=1.0)
}

void Line::animate(float xiSecs)
{
    //Logger::Log("Entering Line.animate...", LOG_DEBUG);

    if ((int)mText.length() > mFixedChars || mVisibility < 1.0)
    {
        mVisibility += (mVisibilitySpeed * xiSecs);
        Glutter::Redisplay();

        if (mVisibility > 1.0)
        {
            mVisibility = 1.0;
            mFixedChars = (int)mText.length();
        }
    }
}

void Line::display()
{
    //Logger::Log("Entering Line.display...", LOG_DEBUG);

    Glutter::RenderStyle(STYLE_SMOOTH_LINE);

    glPushMatrix(); // as received

    glTranslated(0.0, -mY, 0.0);                                  // shift down
    glScalef(mScale, mScale, 1.0);                                // scale down text

    glLineWidth(1.0);                                             // todo: alter by style

    float lShift;
    for (int lIndex = 0; lIndex < (int)mText.length(); lIndex++)
    {
        // select character
        char lChar = mText[lIndex];

        glNormal3f(0.0, 0.0, 1.0);

        // alter transparency
        if (lIndex < mFixedChars)
        {
            glColor4f(0.0, 0.0, 0.3, 1.0);
            glutStrokeCharacter(mFont, lChar);

        }
        else
        {
            if (mVisibility > 0.0)
            {
                lShift = 1.0 - mVisibility;
                glColor4f(0.0, 0.0, 0.3, mVisibility);
                glTranslatef(0.0, 0.0, lShift);
                glutStrokeCharacter(mFont, lChar);
                glTranslatef(0.0, 0.0, -lShift);

            }
        }

    }

    glPopMatrix(); // as received
}

bool Line::addText(string* xiText)
{
    Logger::Log("Entering Line.addText (string)...", LOG_DEBUG);

    if (stringWidth(xiText) > mWidth)
    {
        Logger::Log("String too long for line.", LOG_DEBUG);

        // calculating number of characters to include
        float lCurrentWidth = stringWidth(&mText);
        string lHyphenation("-");
        string lLeftText;
        string lRightText;
        int lLeftChars;
        for (lLeftChars = 0; lCurrentWidth < mWidth; lLeftChars++)
        {
            // calculate width of current n characters + string already present + a hyphen
            lLeftText = xiText->substr(0, lLeftChars);
            lCurrentWidth = stringWidth(&mText) + stringWidth(&lLeftText) + stringWidth(&lHyphenation);
        }

        lLeftChars-=2;

        // add the portion of text that fits
        lLeftText = xiText->substr(0, lLeftChars);
        lLeftText.append(lHyphenation);

        // attempt to add chopped off string to line
        if (addText(&lLeftText))
        {
        Logger::Log("Wrapping remainder of long string onto next line.", LOG_DEBUG);

            // alter string to remainder, and return failiure state
            lRightText = xiText->substr(lLeftChars, xiText->length() - lLeftChars);
            *xiText = lRightText;

            // trim chopped part of line if it begins with a space
            if ((*xiText)[0] == ' ')
            {
                Logger::Log("Trimming first character of string (space).", LOG_DEBUG);
                string lTrimText = xiText->substr(1, xiText->length()-1);
                xiText->swap(lTrimText);
            }

            // and force the second part onto the next line
            return false;
        }
        else
        {
            // induce addition to next line
            return false;
        }
    }

    // if adding the string will make the line too wide, return failiure state
    if ((stringWidth(&mText) + stringWidth(xiText)) > mWidth)
    {
        Logger::Log("Wrapping string onto next line.", LOG_DEBUG);

        // trim chopped part of line if it begins with a space
        if ((*xiText)[0] == ' ')
        {
            Logger::Log("Trimming first char of string (space).", LOG_DEBUG);
            string lTrimText = xiText->substr(1, xiText->length()-1);
            xiText->swap(lTrimText);
        }

        // return failiure state: force it on to the next line
        return false;
    }

    // success case: the string will fit, so concatenate the text

    // fix the previous characters
    // mFixedChars = (int)mText.length();

    // add the new characters
    mText = mText + *xiText;

    // reset animation only if complete
    if (mVisibility == 1.0)
        mVisibility = 0.0;

    return true;
}

bool Line::addChar(char xiChar)
{
    Logger::Log("Entering Line.addChar (char)...", LOG_DEBUG);

    if (xiChar == 8) // delete
    {
        Logger::Log("Removing a character from this line.", LOG_DEBUG);
        removeChars(1);
        return true;
    }
    else
    {
        Logger::Log("Adding a character to this line.", LOG_DEBUG);
        string* lAppendable = new string("");
        lAppendable->append(1, xiChar);
        bool addResult = addText(lAppendable);
        delete lAppendable;
        return addResult;
    }
}

void Line::removeChars(int xiNumberToRemove)
{
    Logger::Log("Entering Line.removeChars...", LOG_DEBUG);
    string lShorter = mText.substr(0, mText.length()-1);
    mText.swap(lShorter);
}

float Line::stringWidth()
{
    // Logger::Log("Calculating Line.stringWidth (member string)...", LOG_DEBUG);
    return stringWidth(&mText);
}

float Line::stringWidth(string* xiText)
{
    // Logger::Log("Calculating Line.stringWidth...", LOG_DEBUG);
    return glutStrokeLength(mFont, (const unsigned char*)xiText->c_str()) * mScale;
}

float Line::height()
{
    // Logger::Log("Calculating Line.height...", LOG_DEBUG);
    return glutStrokeHeight(mFont) * mScale;
}

float Line::bottom()
{
    return mY + height();
}
