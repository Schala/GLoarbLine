/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "CDragAndDroppable.h"

/* -------------------------------------------------------------------------- */

void CDragAndDropHandler::HandleDrag(CDragAndDroppable */* inDD */, const SMouseMsgData& /* inInfo */)
{
	// nothing to do
}
void CDragAndDropHandler::HandleSetDragAction(CDragAndDroppable *inDD, const SDragMsgData& inInfo)
{
	#pragma unused(inDD, inInfo)
}
bool CDragAndDropHandler::HandleDrop(CDragAndDroppable */* inDD */, const SDragMsgData& /* inInfo */)
{
	return false;
}

/* -------------------------------------------------------------------------- */
#pragma mark -

CDragAndDroppable::CDragAndDroppable()
{
	mDragAndDropHandler = nil;
}

CDragAndDroppable::CDragAndDroppable(CDragAndDropHandler *inHandler)
{
	mDragAndDropHandler = inHandler;
}

void CDragAndDroppable::SetDragAndDropHandler(CDragAndDropHandler *inHandler)
{
	mDragAndDropHandler = inHandler;
}

CDragAndDropHandler *CDragAndDroppable::GetDragAndDropHandler() const
{
	return mDragAndDropHandler;
}



