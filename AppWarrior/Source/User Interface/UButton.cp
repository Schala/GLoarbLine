/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */



#include "UButton.h"



void UButton_Draw(TImage inImage, const SRect& inBounds, const SButtonInfo& inInfo);

void UButton_DrawFocused(TImage inImage, const SRect& inBounds, const SButtonInfo& inInfo);

void UButton_DrawHilited(TImage inImage, const SRect& inBounds, const SButtonInfo& inInfo);

void UButton_DrawDisabled(TImage inImage, const SRect& inBounds, const SButtonInfo& inInfo);



void _SetVirtualOrigin(TImage inImage, const SPoint& inVirtualOrigin);

void _GetVirtualOrigin(TImage inImage, SPoint& outVirtualOrigin);

void _ResetVirtualOrigin(TImage inImage);



/* -------------------------------------------------------------------------- */



void UButton::Draw(TImage inImage, const SRect& inBounds, const SButtonInfo& inInfo)

{

	// get virtual origin

	SPoint stVirtualOrigin;

	_GetVirtualOrigin(inImage, stVirtualOrigin);



	if (stVirtualOrigin.IsNull())

		UButton_Draw(inImage, inBounds, inInfo);

	else

	{

		// reset virtual origin

		_ResetVirtualOrigin(inImage);



		// recalculate rect

		SRect stBounds = inBounds;

		stBounds.MoveBack(stVirtualOrigin.x, stVirtualOrigin.y);

	

		// draw

		UButton_Draw(inImage, stBounds, inInfo);



		// restore virtual origin

		_SetVirtualOrigin(inImage, stVirtualOrigin);

	}

}



void UButton::DrawFocused(TImage inImage, const SRect& inBounds, const SButtonInfo& inInfo)

{

	// get virtual origin

	SPoint stVirtualOrigin;

	_GetVirtualOrigin(inImage, stVirtualOrigin);



	if (stVirtualOrigin.IsNull())

		UButton_DrawFocused(inImage, inBounds, inInfo);

	else

	{

		// reset virtual origin

		_ResetVirtualOrigin(inImage);



		// recalculate rect

		SRect stBounds = inBounds;

		stBounds.MoveBack(stVirtualOrigin.x, stVirtualOrigin.y);

	

		// draw

		UButton_DrawFocused(inImage, stBounds, inInfo);



		// restore virtual origin

		_SetVirtualOrigin(inImage, stVirtualOrigin);

	}

}



void UButton::DrawHilited(TImage inImage, const SRect& inBounds, const SButtonInfo& inInfo)

{

	// get virtual origin

	SPoint stVirtualOrigin;

	_GetVirtualOrigin(inImage, stVirtualOrigin);



	if (stVirtualOrigin.IsNull())

		UButton_DrawHilited(inImage, inBounds, inInfo);

	else

	{

		// reset virtual origin

		_ResetVirtualOrigin(inImage);



		// recalculate rect

		SRect stBounds = inBounds;

		stBounds.MoveBack(stVirtualOrigin.x, stVirtualOrigin.y);

	

		// draw

		UButton_DrawHilited(inImage, stBounds, inInfo);



		// restore virtual origin

		_SetVirtualOrigin(inImage, stVirtualOrigin);

	}

}



void UButton::DrawDisabled(TImage inImage, const SRect& inBounds, const SButtonInfo& inInfo)

{

	// get virtual origin

	SPoint stVirtualOrigin;

	_GetVirtualOrigin(inImage, stVirtualOrigin);



	if (stVirtualOrigin.IsNull())

		UButton_DrawDisabled(inImage, inBounds, inInfo);

	else

	{

		// reset virtual origin

		_ResetVirtualOrigin(inImage);



		// recalculate rect

		SRect stBounds = inBounds;

		stBounds.MoveBack(stVirtualOrigin.x, stVirtualOrigin.y);

	

		// draw

		UButton_DrawDisabled(inImage, stBounds, inInfo);



		// restore virtual origin

		_SetVirtualOrigin(inImage, stVirtualOrigin);

	}

}



