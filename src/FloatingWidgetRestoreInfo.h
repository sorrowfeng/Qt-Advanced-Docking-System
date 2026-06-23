#ifndef FloatingWidgetRestoreInfoH
#define FloatingWidgetRestoreInfoH

#include <QList>

#include "ads_globals.h"

namespace ads
{
class CDockAreaWidget;
class CDockContainerWidget;

struct FloatingWidgetSourceRestoreInfo
{
	CDockContainerWidget* SourceContainer = nullptr;
	CDockAreaWidget* SourceDockArea = nullptr;
	bool FloatedEntireDockArea = false;
	CDockAreaWidget* RestoreNeighborArea = nullptr;
	DockWidgetArea RestoreInsertArea = InvalidDockWidgetArea;
	QList<int> SplitterSizes;
	int SourceTabIndex = -1;
};

} // namespace ads

#endif
