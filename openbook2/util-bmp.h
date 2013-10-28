/** util-bmp.h - bmp handler **/

/* Image type - contains height, width, and data */
struct Texture {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};

class BMP
{
    public:
    static void ImageLoad(char* xiFilename, Texture* xoTexture);
    static unsigned int getint(FILE* xiFilePointer);
    static unsigned int getshort(FILE* xiFilePointer);
};
