#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hw_ocrapi.h"
#include <opencv2/opencv.hpp>



#define     ELEM(img, r, c) (CV_IMAGE_ELEM(img, unsigned char, r, c))
int main(int argc, char *argv[])
{

	IplImage* image = NULL;
	int nRtnVal = STATUS_OK;
	int aActLang[3];

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
	iRecSettings.iCandidate = NULL;
	char fn[64] = {0};
	//for(int l = 0; l < 580; l ++)
	{
		memset(fn,0,64);
		sprintf(fn,"image_%05d.bmp",485);
		image = cvLoadImage("img2.jpg"/*fn*/,0);
		int width = image->width;
		int height = image->height;
		uchar * ptr = new uchar[width*height];
		for (int i = 0; i < height; i ++)
		{
			for (int j = 0; j < width; j ++)
			{
				ptr[i*width + j ] = (uchar)ELEM(image,i,j);//uchar* elem = (uchar*)image->imageData +i*image->widthStep;//(uchar)ELEM(image,i,j);
				//cout<<(int)ptr[i*width + j ]<<" ";
			}
		}
		printf("TEST Project!\n");
	
		

		//Load Image;
		pRawImg = (unsigned char*)malloc(width*height);
		if (pRawImg == NULL)
		{
			nRtnVal = STATUS_NO_ENOUGH_RAM;
			goto clearup;
		}
		for (int i = 0; i < height; i ++)
		{
			for (int j = 0; j < width; j ++)
			{
				pRawImg[i*width + j ] = (uchar)ELEM(image,i,j);//uchar* elem = (uchar*)image->imageData +i*image->widthStep;//(uchar)ELEM(image,i,j);
			}
		}
	

		//Load OCR's DB.  Please change the directory to your dbs location.
		sprintf(aDBpath, "F:/work/program dir/program dir/HWOcrEngine/database/");

		//initialize the engine
		printf("HWOCR_InitializeEngine!\n");
		nRtnVal = HWOCR_InitializeEngine(NULL, aDBpath, 0, NULL, NULL );

		if ( nRtnVal==STATUS_OK )
		{
			//set language to Chinese mixed with English
			aActLang[0] = LANG_EN;//LANG_CN
			aActLang[1] = LANG_CN;//
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
 	 		nRtnVal = HWOCR_LayoutAnalyze( pRawImg, width, height,iImgType,
	 		iSettings,&iBinImage,&iBlock,&iBlockCount);
	 	
		 }


		 if (nRtnVal==STATUS_OK)
		 {
			//do the recognition
			printf("HWOCR_Recognize!\n"); 
			nRtnVal = HWOCR_Recognize( iBinImage, iRecSettings, iBlock, iBlockCount, &iTextRgn,  NULL);
		}
		//printf("%s",);
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
					//std::cout<<"result = "<<std::endl;
					fwrite( iTextRgn[i].iLines[j].iText, 2, iTextRgn[i].iLines[j].iCharCount, fp);
					for(int k = 0; k < iTextRgn[i].iLines[j].iCharCount; k ++)
					{
						//std::cout<<iTextRgn[i].iLines[j].iText[2*k];
						//std::cout<<"result = "<<iTextRgn[i].iLines[j].iText[2*k+1]<<std::endl;
					}
					//std::cout<<std::endl;
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
		getchar();
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

