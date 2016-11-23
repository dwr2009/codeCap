#ifndef INPUTPANELCONTEXT_H
#define INPUTPANELCONTEXT_H
#include <QInputContext>
#include "inputwidget.h"

class MyInputPanelContext : public QInputContext
{
	Q_OBJECT

public:
	MyInputPanelContext();
	~MyInputPanelContext();
	bool filterEvent(const QEvent* event);
	QString identifierName();
	QString language();
	bool isComposing() const;
	void reset();
private slots:
	void sendCharacter(QString character);
	void BackspaceContent();
	void Escape();
	void sendKeyValue(int key_value);
private:
	void updatePosition();
private:
	InputWidget *inputPanel;
};

#endif // INPUTPANELCONTEXT_H

