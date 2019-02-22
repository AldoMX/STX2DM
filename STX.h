// STX.h: archivo de encabezado principal para la aplicación PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error incluye 'stdafx.h' antes de incluir este archivo para PCH
#endif

#include "resource.h"		// Símbolos principales


// CSTXApp:
// Consulte la sección STX.cpp para obtener información sobre la implementación de esta clase
//

class CSTXApp : public CWinApp
{
public:
	CSTXApp();

// Reemplazos
	public:
	virtual BOOL InitInstance();

// Implementación

	DECLARE_MESSAGE_MAP()
};

extern CSTXApp theApp;

namespace stx
{ 
    enum TRACK
    {
        TR_PRACTICE = 0,
        TR_NORMAL,
        TR_HARD,
        TR_NIGHTMARE,
        TR_CRAZY,
        TR_FULLDOUBLE,
        TR_HALFDOUBLE,        
        TR_DIVISION,
        TR_LIGHTS,
        TR_COUNT,
        TR_FORCEDWORD = 0x7FFFFFFF
    };

    //-------------------
    // The first thing in a STX file
    struct header_t
    {
        unsigned char mFourcc[4]; // STF4 magic number
        unsigned int  mPad[14];   // Only zeros here
        char mTitle[64];          // ANSI Zero terminated string (check if can be UTF-8)
        char mArtist[64];         // ANSI Zero terminated string (check if can be UTF-8)
        char mStepAuthor[64];     // ANSI Zero terminated string (check if can be UTF-8)
        unsigned int mOffsets[TR_COUNT];  // offset from start of file to each track
    };

    //-------------------
    // Each track is made of two main part, this simple header, follow by N blocks, where N = SUM( mBlocks )
    struct track_t
    {
        unsigned int mDifficulty;    
        unsigned int mBlocks[50];
    };

    //-------------------
    // A Helper to read the STX file
    struct packed_block_t
    {
        unsigned int mSize;
        packed_block_t* Next()
        { return reinterpret_cast<packed_block_t*>(reinterpret_cast<unsigned char*>(this+1)+mSize); }
        const packed_block_t* Next() const
        { return reinterpret_cast<const packed_block_t*>(reinterpret_cast<const unsigned char*>(this+1)+mSize); }
    };

    struct block_t
    {
        float        mBpm;
        unsigned int mBeatPerMeasure;
        unsigned int mBeatSplit;
                 int mDelay; // N * 1s/100, can be neg

        struct DivisionSet
        {
            unsigned int mPerfect[2];
            unsigned int mGreat[2];
            unsigned int mGood[2];
            unsigned int mBad[2];
            unsigned int mMiss[2];
            unsigned int mStepG[2];
            unsigned int mStepW[2];
            unsigned int mStepA[2];
            unsigned int mStepB[2];
            unsigned int mStepC[2];
        } mDivisionSet;

        unsigned int mSpeed; // store as int(Speed*1000.f)
		//unsigned int mSplitDelay;
		//unsigned int mComboMultiplier;
		//		 int mLifeMultiplier; // store as int(Multiplier*1000.f)
		//		 int mScoreMultiplier; // store as int(Multiplier*1000.f)
		//unsigned int mNoteSkin[3];
		unsigned int mPad[7];
        unsigned int mBlockLength; // In beatSplit
    };

    struct row_t
    {
		unsigned char   mRow[13];
    };
}