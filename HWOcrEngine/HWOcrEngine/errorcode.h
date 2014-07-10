#ifndef __COMMON_H__
#define __COMMON_H__
/**
@brief ´íÎó±àÂë <0 ·¢Éú´íÎó ·ñÔòÕýÈ·
*/
/** ÅÐ¶ÏÓï¾ä */
#define FAILED(iStatus) ((iStatus) < 0)
#define SUCCEEDED(iStatus) ((iStatus) >= 0)

/**@brief No error occur*/
#define STATUS_OK                   0x00000000L
/**No enough memory*/
#define STATUS_NOMEMORY             0x80000001L
/**One or more arguments are invalid */
#define STATUS_INVALIDARG           0x80000002L
/**No such interface supported*/
#define STATUS_NOINTERFACE          0x80000003L
/**Invalid pointer*/
#define STATUS_INVALIDPTR           0x80000004L
/**bad input image*/
#define STATUS_BADIMAGE             0x80000005L
/**bad dictionary file*/
#define STATUS_BADDICTFILE          0x80000006L
/**unsuport language*/
#define STATUS_BADLANGUAGE          0x80000007L
/*TODO Add ERROR CODE HERE*/
/**error in the DICT*/
#define STATUS_DICTERROR            0x80000008L
/**NO OCR result feed to Dict*/
#define STATUS_DICTNOINPUT          0x80000009L
/**Image binary error*/
#define STATUS_IMAGEBINERR          0x80000010L
/**user hit on blank region*/
#define STATUS_BLANKRGN             0x80000011L
/**Can not open the file*/
#define STATUS_UNOPENFILE           0x80000012L
/*HW_ENG20_SetHeap failed*/
#define STATUS_ENG20_UNSETHEAP      0x80000013L
/*Too many characters in the Card*/
#define STATUS_BCROVERFLOW			0x80000014L
/* User canceled*/
#define STATUS_USERCANCELED         0x80000015L
/* FILE IO ERROR */
#define STATUS_FILEERROR            0x80000016L
/**unexpected error, failed*/
#define STATUS_UNEXPECTED           0x8000FFFFL

/**
versions are 32 bit value layout as follow:
   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
  +---------------+---------------+---------------+---------------+
  |    major      |     minor     |     revision  |   build       |
  +---------------+---------------+---------------+---------------+    
*/
#define MakeVersion(major,minor,revision,build) \
    (((major) << 24) + ((minor) << 16) + ((revision) << 8) + (build))


#endif