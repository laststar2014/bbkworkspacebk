/*--------------------------------------------------------------------------
                Embedded OCR Engine for embedded platform v1.0
Copyright (c) 2006£¬HanWang Technology Co., Ltd. All Rights Reserved.
                
    File:       hw_ocrapi.h

    Description:
                Include file for embedded OCR module.

    Author:     Hanwang Embedded OCR Group
    History:              
                Nov.7, 2006     Created

---------------------------------------------------------------------------*/
#ifndef HWOCRAPI_H
#define HWOCRAPI_H

//  INCLUDES
#include "ocrcommon.h"

// FORWARD DECLARATIONS
#ifdef __cplusplus
#define WINDLL	 extern "C"
#else
#define WINDLL   extern
#endif

#define WINAPI

#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------------------------ */
/*  Language                                                                */
/* ------------------------------------------------------------------------ */
#define  LANG_NONE		0
#define  LANG_CN		1


#define  LANG_EN        101

/* ------------------------------------------------------------------------ */
/*  Error Code                                                              */
/* ------------------------------------------------------------------------ */
#define STATUS_OK                   0       //no error
#define STATUS_BADIMAGE             -1001   //bad image or unsupported format
#define STATUS_BADLANGUAGE          -1002   //unsupported language
#define STATUS_BADRGN               -1003   //bad layout region
#define STATUS_NO_ENOUGH_RAM        -1004   //no enough memory
#define STATUS_BADPARAMETER         -1005   //wrong parameter
#define STATUS_NOTSETLANGUAGE       -1006   //not set any language
#define STATUS_BADDICTFILE          -1007   //bad dictionary files
#define STATUS_FAILED               -1100   //the other errors

/**
@brief ´íÎó±àÂë <0 ·¢Éú´íÎó ·ñÔòÕýÈ·
*/
/** ÅÐ¶ÏÓï¾ä */
#define FAILED(iStatus) ((iStatus) < 0)
#define SUCCEEDED(iStatus) ((iStatus) >= 0)


/*--------------------------------------------------------------------------*/
/*      Typedef's                                                           */
/*--------------------------------------------------------------------------*/
typedef int  TStatus;  

// Description: Provide a callback function to implement the cancel function,
//              and allocate necessary memory.  
// Parameters:  aFuncIsCanceled - (In) callback function name for IsCanceled flag
//				aDBPath - (In) db file path, including back slash
//              aParam - (In) for later usage
//				pfnHeapAlloc	- (In) callback function for malloc()//Not to be used in this edition
//				pfnHeapFree		- (In) call back function for free()//Not to be used in this edition
// Return:      none
WINDLL TStatus WINAPI HWOCR_InitializeEngine( IsCanceled aFuncIsCanceled, 
                                              char *aDBPath, 
                                              void *aParam,
                                              BCRHeapAlloc  pfnHeapAlloc,
                                              BCRHeapFree   pfnHeapFree);


// Description: Free the memory allocated by InitializeEngine(),
//   Calls to EngineExit() must correspond to prior calls to InitializeEngine()  
// Parameters:  none                
// Return:      none
WINDLL void  WINAPI HWOCR_ExitEngine();

// Description: Layout analyze before recognition
// Parameters:  aImage - (In) the input image
//				saving length of a scan line(not 4-bytes-align):
//EGray2:  (aWidth+7)/8 bytes  
//EGray8:  aWidth bytes  
//EColor16M: aWidth*3 bytes

//
//				aWidth - (In) width of aImage in pixels
//				aHeight - (In) height of aImage in pixels
//				aColor - (In) color type for aImage
//              aBinImage - (Out) the binarized image data,
//                  which will be used in recognize() function
//              aSettings - (In) the settings
//              aBlock - (Out) block info array
//              aBlockCount - (Out) block count
// Return:      STATUS_OK if successful. Other err number will be defined by vendor
//              If the process is cancelled, return STATUS_OK 
WINDLL TStatus WINAPI  HWOCR_LayoutAnalyze( const BYTE *aImage, 
													   const int aWidth,
													   const int aHeight,
													   TColorType  aColor,
													   const TOCRLayoutSetting aSettings,
														 BYTE **aBinImage, 
														 TOCRBlockInfo **aBlock, 
														 int *aBlockCount);

// Description: Free data structure returned by LayoutAnalyze
// Parameters:  aBlock - (In) the pointer returned by LayoutAnalyze
// Return:      none
WINDLL void  WINAPI HWOCR_FreeBlocks(TOCRBlockInfo *aBlock);

// Description: Free image returned by LayoutAnalyze
// Parameters:  aBinImage - (In) the pointer returned by LayoutAnalyze
// Return:      none
WINDLL void WINAPI HWOCR_FreeBinImage(BYTE *aBinImage);


// Description: Recognize Image 
// Parameters:  aBinImage - (In) the binarized image returned from LayoutAnalyze().
//				aWidth - (In) width of aImage in pixels
//				aHeight - (In) height of aImage in pixels
//				aColor - (In) color type for aImage

//              aSettings - (In) the specified settings
//				aBlock - (In) the specified region array to be recognized
//              aBlockCount - (In) count of blocks users want to recognize
//              aTextRgn - (Out) the recognized result data structure array, whose item count is aBlockCount,in unicode
//              aFuncRecogProgress - (In) callback function name for RecogProgress
// Return:      STATUS_OK if successful. Other err number will be defined by vendor
WINDLL TStatus WINAPI HWOCR_Recognize( BYTE *aBinImage, const TOCRRecognizeSetting aSettings,
                             const TOCRBlockInfo *aBlock, const int aBlockCount, TOCRTextRgnInfo **aTextRgn,  RecogProgress aFuncRecogProgress);


// Description: Free data structure returned by Recognize
// Parameters:  aTextRgn - (In) the pointer returned by Recognize
//              aBlockCount - (In) count of items in aTextRgn array
// Return:      none
WINDLL void WINAPI HWOCR_FreeTextRgns(TOCRTextRgnInfo *aTextRgn,const int aBlockCount );



// Description: Set the language in the engine
// Parameters:  aLanguages- (In) not more than 2 languages, should end with -1
// Return:      STATUS_OK if successful. Other err number will be defined by vendor
WINDLL TStatus WINAPI HWOCR_SetActiveLanguage(int aLanguages[3]);


// Description: Get the installed language. This funtions should be called after HWOCR_Refresh()
// Parameters:  iLanguages - (Out)list of installed languages, end with -1
// Return:      STATUS_OK if successful. Other err number will be defined by vendor
WINDLL TStatus WINAPI HWOCR_GetInstalledLanguages (int aLanguages[10]);

// Description: Refresh the installed lanaguages
// Return:      STATUS_OK if successful. Other err number will be defined by vendor
WINDLL TStatus WINAPI HWOCR_Refresh(char *aDBPath);


// Description: Recognize specified regions 
// Parameters:  aImage - (In) the input image
//				aWidth - (In) width of aImage in pixels
//				aHeight - (In) height of aImage in pixels
//				aColor - (In) color type for aImage

//              aLayoutInfo - (In) recognition region information
//              
//              aTextRgn - (Out) the recognized result data structure,in unicode
//              aFuncRecogProgress - (In) callback function name for RecogProgress
// Return:      STATUS_OK if successful. Other err number will be defined by vendor
WINDLL TStatus WINAPI HWOCR_RecognizeBlock(const BYTE *aImage, 
													   const int aWidth,
													   const int aHeight,
													   TColorType  aColor,
													  TOCRLayoutBlockInfo aLayoutInfo,
												 TOCRTextRgnInfo **aTextRgn, RecogProgress aFuncRecogProgress);


#ifdef __cplusplus
}
#endif


#endif
