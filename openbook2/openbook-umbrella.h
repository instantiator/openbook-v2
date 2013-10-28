/** openbook-umbrella.h - The umbrella object - from which Book, Page and Line inherit. **/

class Umbrella
{
    public:
    Umbrella(Umbrella* xiParent);
    virtual ~Umbrella();

    virtual void display();
    virtual void animate(float xiSecs);

    virtual bool addText(string* xiString);
    virtual bool addChar(char xiChar);

    virtual bool key(int xiChar);

    private:
    Umbrella* mParentUmbrella;
};
