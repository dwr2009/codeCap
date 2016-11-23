#include "clickedLabel.h"   
#include <QtDebug>
void ClickedLabel::mousePressEvent ( QMouseEvent * ev )   
{   
	Q_UNUSED(ev);
	//qDebug ()  << "Send Clicked ";
	emit Clicked();   
}  
