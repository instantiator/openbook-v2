/** openbook-printlogic.h - Handles assigning text to Umbrellas. **/

class PrintLogic
{
    public:
    static void Initialise();
    static void Finish();

    static void AddUmbrella(Umbrella* xiUmbrella);

    static void AddText(string* xiText);
    static void AddChar(char xiChar);

    static void BufferAddChar(char xiChar);
    static void AddBuffer();

    private:
    static list<Umbrella*> mUmbrellaList;
    static string* mStringBuffer;
};
