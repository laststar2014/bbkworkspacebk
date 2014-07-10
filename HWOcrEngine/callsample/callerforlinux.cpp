#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hw_ocrapi.h"

int main(int argc, char *argv[])
{
	printf("TEST Project!\n");

	int nRtnVal = STATUS_OK;
	int aActLang[3];
	int nWidth;
    int nHeight;
	int nFileLen;
	int iSkewAngle;
	int iBlockCount;
    int count;
	char aDBpath[256];
	FILE *fpImg = NULL;
	TColorType iImgType;
	unsigned char *pRawImg = NULL;
	unsigned char *iBinImage = NULL;
	unsigned char pReturn[2];
	TOCRBlockInfo *iBlock = NULL;
	TOCRTextRgnInfo *iTextRgn = NULL;
	TOCRLayoutSetting iSettings;
	TOCRRecognizeSetting  iRecSettings;



    if (argc < 4)
    {
        printf("Chinese OCR Recognition one raw(GRAY) image.\n");
        printf("argv[1]: The raw image's file name.\n");
        printf("argv[2]: The raw image's width.\n");
        printf("argv[3]: The raw image's height.\n");
    }
	


	//Load Image;
    nWidth =  atoi(argv[2]);
    nHeight =  atoi(argv[3]);
    nFileLen = nWidth*nHeight;

    //nWidth = 595;
    //nHeight = 183;
    //nFileLen = nWidth*nHeight;

    pRawImg = (unsigned char*)malloc(nFileLen);
    if (pRawImg == NULL)
    {
        nRtnVal = STATUS_NO_ENOUGH_RAM;
        goto clearup;
    }
    fpImg = fopen(argv[1], "rb");
    if (fpImg == NULL)
    {
        nRtnVal = STATUS_FAILED;
        goto clearup;
    }
    count = fread(pRawImg, 1, nFileLen, fpImg);
    if (count != nFileLen)
    {
        nRtnVal = STATUS_FAILED;
        goto clearup;
    }
    fclose(fpImg);
    fpImg = NULL;

	//Load OCR's DB.  Please change the directory to your dbs location.
    sprintf(aDBpath, "F:/work/program dir/program dir/HWOcrEngine/database/");

	//initialize the engine
	printf("HWOCR_InitializeEngine!\n");
	nRtnVal = HWOCR_InitializeEngine(NULL, aDBpath, 0, NULL, NULL );

	if ( nRtnVal==STATUS_OK )
	{
		//set language to Chinese mixed with English
		aActLang[0] = LANG_CN;
		aActLang[1] = LANG_EN;//
		aActLang[2] = -1;

		printf("HWOCR_SetActiveLanguage!\n");
		nRtnVal = HWOCR_SetActiveLanguage(aActLang);
	 }
	 
 
	 if ( nRtnVal == STATUS_OK )
	 {
        
	 	//layout analyze
		//Do not do the calibration, or else the coordinates of the characters will not correspond to their original coordinates 
		iSettings.iSkew = 0;//EFalse;
		iImgType = EGray256;
		printf("HWOCR_LayoutAnalyze!\n");
 	 	nRtnVal = HWOCR_LayoutAnalyze( pRawImg, nWidth, nHeight,iImgType,
	 	iSettings,&iBinImage,&iBlock,&iBlockCount);
	 	
	 }


	 if (nRtnVal==STATUS_OK)
     {
		//do the recognition
		printf("HWOCR_Recognize!\n"); 
		nRtnVal = HWOCR_Recognize( iBinImage, iRecSettings, iBlock, iBlockCount, &iTextRgn,  NULL);
    }
	
	 //Debug: output the result
	if (nRtnVal==STATUS_OK)
	{
		printf("SaveResult!\n");

		//save results
		char name[127];
		 //Please change the directory to your desired location.
		sprintf( name, "HWOCR_result.txt");
		FILE *fp = fopen( name, "wb");
		
		//save unicode
		unsigned char pReturn[2];
		pReturn[0] = 0xff;
		pReturn[1] = 0xfe;
		fwrite( (char*)pReturn, sizeof(char), 2, fp);

		if ( iTextRgn )
		for ( int i=0; i<iBlockCount; i++ )
		{
		   for ( int j=0; j<iTextRgn[i].iLineCount; j++ )
		   {
				fwrite( iTextRgn[i].iLines[j].iText, 2, iTextRgn[i].iLines[j].iCharCount, fp);

				pReturn[0] = 0x0d;
				pReturn[1] = 0x00;
				fwrite( (char*)pReturn, sizeof(char), 2, fp);
				pReturn[0] = 0x0a;
				pReturn[1] = 0x00;
				fwrite( (char*)pReturn, sizeof(char), 2, fp);
		   }
		   pReturn[0] = 0x0d;
		   pReturn[1] = 0x00;
		   fwrite( (char*)pReturn, sizeof(char), 2, fp);
		   pReturn[0] = 0x0a;
		   pReturn[1] = 0x00;
		   fwrite( (char*)pReturn, sizeof(char), 2, fp);

		}
        fclose(fp);
	} 
clearup:
    HWOCR_ExitEngine();
	if (iBinImage)
	{
		HWOCR_FreeBinImage(iBinImage);
		iBinImage = NULL;
	}
	if (iBlock)
	{
		HWOCR_FreeBlocks(iBlock);
		iBlock = NULL;
	}
	if (iTextRgn)
	{
		HWOCR_FreeTextRgns(iTextRgn, iBlockCount);
		iTextRgn = NULL;
	}

	if(pRawImg)
	{
		free(pRawImg);
		pRawImg = NULL;
	}
	if(nRtnVal == STATUS_OK)
		printf("TEST Project OK!\n");
	else
		printf("TEST Project Fail!\n");
    return nRtnVal;
}

