/*--------------------------------------------------------------------------
                Embedded OCR Engine for embedded platform v1.0
Copyright (c) 2006£¬HanWang Technology Co., Ltd. All Rights Reserved.
                
    File:       ocrcommon.h

    Description:
                common file for embedded OCR module.

    Author:     Hanwang Embedded OCR Group
    History:              
                Nov.7, 2006     Created

---------------------------------------------------------------------------*/
#ifndef OCRCOMMON_H
#define OCRCOMMON_H
 
/*--------------------------------------------------------------------------*/
/*      Typedef's                                                           */
/*--------------------------------------------------------------------------*/
typedef unsigned char		BYTE;
typedef int					BOOL;
//Data type define
///////////////////////////////////////////////////////////////////////////////
typedef void (*RecogProgress)( int nPercent); 
typedef int (*IsCanceled)();

typedef void* (*BCRHeapAlloc)( unsigned int lSize );
typedef void  (*BCRHeapFree)( void *memblock );

typedef struct _OCRRect //rectangle
{
	int left;
	int top;
	int right;
	int bottom;
} TOCRRECT;


enum TLayoutType   //layout text type 
{
    ETypeHorizontal, // TYPE_HORIZONTAL,
    ETypeVertical // TYPE_VERTICAL
};

enum TTextType   
{ 
    ETextMultiLine, //multi-text line
    ETextSingleLine //sigle text line
};
enum TImageBackground  //brightness for text background
{
    EBackgroundLight, // dark character with light background 
    EBackgroundDark,  //light character with dark background
    EBackgroundUnknown  // unknown
};


enum TColorType   //color type 
{
	EGray2 = 0,  //black and white image, 1 bit per pixel
    EGray256, // gray image, 8 bit per pixel
    EColor16M, // color image, 24 bit per pixel
};

struct TOCRTextLineInfo   // data structure of text line
{
    char   *iText;             // during layout analyze, this is null
    TOCRRECT    iRect;             // 

    TOCRRECT   *iCharRect;         // This is an array which returns the regions of every single character in this line
    int    iCharCount;         // How many characters in this line
};

struct TOCRBlockInfo          // data structure of layout infomation
{

    TOCRRECT       iRect;              // block coordinates 
    TLayoutType iType;              //vertical or horizontal
    int        iBlockId;           // block's ID

};


struct TOCRTextRgnInfo          // data structure of recognition result
{
    TOCRRECT       iRect;              // block coordinates 
    TLayoutType iType;              //vertical or horizontal
    int        iBlockId;           // block's ID

    TOCRTextLineInfo  *iLines;  
    int        iLineCount;         // Lines' count in each block
};


struct TOCRLayoutSetting         // data structure of layout settings
{
    enum TBrightness            // brightness
    {  
        ENormal, // Normal
        ELight,  // Light
        EDark  // Dark
    };

    int iSkew;                // switch of skew adjustment 1: do skew adjustment; 0: dont do it
    TBrightness iBrightness;    // switch of brightness adjustment
};

struct TOCRRecognizeSetting      // data structure of Recognition settings
{

    void* iCandidate;           // for later usage
};


struct TOCRLayoutBlockInfo // COCRLayoutBlockInfo
{
    TOCRRECT            iRect;  //block coordinates
    TLayoutType         iLayout;  //vertical or horzontal text
    TTextType           iText;    //mulitline or single line
    TImageBackground    iBackgroundColor;//brightness for text background
};



struct TRegionInfo
{
    TOCRRECT iRect;
    TImageBackground iBackgroundColor;
    
    enum TRegionType
    {
        EEmailAddress,
        ETelephoneNumber,
        EWWWAddress
    };
    
    TRegionType iType;
};

#endif
