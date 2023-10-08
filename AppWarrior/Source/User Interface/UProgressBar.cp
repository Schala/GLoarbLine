/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */



#include "UProgressBar.h"



void UProgressBar_Draw(TImage inImage, const SRect& inBounds, const SProgressBarInfo& inInfo);



void _SetVirtualOrigin(TImage inImage, const SPoint& inVirtualOrigin);

void _GetVirtualOrigin(TImage inImage, SPoint& outVirtualOrigin);

void _ResetVirtualOrigin(TImage inImage);



/* -------------------------------------------------------------------------- */



void UProgressBar::Draw(TImage inImage, const SRect& inBounds, const SProgressBarInfo& inInfo)

{

	// get virtual origin

	SPoint stVirtualOrigin;

	_GetVirtualOrigin(inImage, stVirtualOrigin);



	if (stVirtualOrigin.IsNull())

		UProgressBar_Draw(inImage, inBounds, inInfo);

	else

	{

		// reset virtual origin

		_ResetVirtualOrigin(inImage);



		// recalculate rect

		SRect stBounds = inBounds;

		stBounds.MoveBack(stVirtualOrigin.x, stVirtualOrigin.y);

	

		// draw

		UProgressBar_Draw(inImage, stBounds, inInfo);



		// restore virtual origin

		_SetVirtualOrigin(inImage, stVirtualOrigin);

	}

}

