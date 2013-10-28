/** openbook-umbrella.cpp - The umbrella object - from which Book, Page and Line inherit. **/

#include "openbook2.h"

Umbrella::Umbrella(Umbrella* xiParent)
{
    Logger::Log("Initialising an Umbrella object...", LOG_DEBUG);
    mParentUmbrella = xiParent;
}

Umbrella::~Umbrella()
{
    Logger::Log("Destroying an Umbrella object...", LOG_DEBUG);
}

bool Umbrella::addChar(char xiChar)
{
    Logger::Log("Entering Umbrella.addChar...", LOG_DEBUG);
    return true;
}
bool Umbrella::addText(string* xiString)
{
    Logger::Log("Entering Umbrella.addString(string*)...", LOG_DEBUG);
    return true;
}

void Umbrella::animate(float xiSecs)
{
    Logger::Log("Entering Umbrella.animate...", LOG_DEBUG);
}

void Umbrella::display()
{
    Logger::Log("Entering Umbrella.display...", LOG_DEBUG);
}

bool Umbrella::key(int xiChar)
{
    Logger::Log("Entering Umbrella.key...", LOG_DEBUG);
    return false;
}
