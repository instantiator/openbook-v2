/** openbook-printlogic.h - Handles assigning text to Umbrellas. **/

#include "openbook2.h"

// Further redeclarations mysteriously required by C++.
list<Umbrella*> PrintLogic::mUmbrellaList;
string* PrintLogic::mStringBuffer;

void PrintLogic::Initialise()
{
    Logger::Log("Entering PrintLogic::Initialise...", LOG_DEBUG);
    mStringBuffer = new string("");
}

void PrintLogic::AddUmbrella(Umbrella* xiUmbrella)
{
    Logger::Log("Entering PrintLogic::AddUmbrella...", LOG_DEBUG);
    mUmbrellaList.push_back(xiUmbrella);
}

void PrintLogic::Finish()
{
    Logger::Log("Entering PrintLogic::Initialise...", LOG_DEBUG);
    delete mStringBuffer;
}

void PrintLogic::AddText(string* xiText)
{
    Logger::Log("Entering PrintLogic::AddText...", LOG_DEBUG);

    // process list of umbrellas
    list<Umbrella*>::iterator lCurrentUmbrella;
    for (lCurrentUmbrella = mUmbrellaList.begin(); lCurrentUmbrella != mUmbrellaList.end(); lCurrentUmbrella++)
    {
        Logger::Log("Adding Text to an Umbrella...", LOG_DEBUG);
        if ((*lCurrentUmbrella)->addText(xiText))
        {
            Logger::Log("All text added to the Umbrella.", LOG_DEBUG);
        }
        else
        {
            Logger::Log("Text partially or not added to Umbrella.", LOG_DEBUG);
        }
    }

    Glutter::Redisplay();
}

void PrintLogic::AddChar(char xiChar)
{
    Logger::Log("Entering PrintLogic::AddChar...", LOG_DEBUG);

    // process list of umbrellas
    list<Umbrella*>::iterator lCurrentUmbrella;
    for (lCurrentUmbrella = mUmbrellaList.begin(); lCurrentUmbrella != mUmbrellaList.end(); lCurrentUmbrella++)
    {
        Logger::Log("Adding a char to an Umbrella...", LOG_DEBUG);
        if ((*lCurrentUmbrella)->addChar(xiChar))
        {
            Logger::Log("Char added to the Umbrella.", LOG_DEBUG);
        }
        else
        {
            Logger::Log("Char not added to Umbrella.", LOG_DEBUG);
        }
    }

    Glutter::Redisplay();
}

void PrintLogic::BufferAddChar(char xiChar)
{
    if (xiChar == '\n')
    {
        Logger::Log("Newline coming in through PrintLogic::BufferAddChar...", LOG_DEBUG);
        AddText(mStringBuffer);
        AddChar('\n');
    }
    else
    {
        mStringBuffer->append(1, xiChar);
    }
}

void PrintLogic::AddBuffer()
{
    Logger::Log("Entering PrintLogic::AddBuffer...", LOG_DEBUG);
    AddText(mStringBuffer);

    Logger::Log("Erasing buffer...", LOG_DEBUG);
    string lBlankString("");
    mStringBuffer->swap(lBlankString);
}
