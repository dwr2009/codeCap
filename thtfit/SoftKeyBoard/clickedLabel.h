#ifndef CLICKEDLABEL_H_   
#define CLICKEDLABEL_H_   
#include <QtWidgets/QLabel>   
#include <QtWidgets/QWidget>   
class ClickedLabel : public QLabel   
{   
    Q_OBJECT   
signals:   
   // void Clicked(ClickedLabel* clicked);   
    void Clicked();   
public:   
    ClickedLabel(QWidget *parent=0): QLabel(parent),m_str("")   
    {   
        setText(m_str);   
    };   
    ~ClickedLabel() {};   
protected:   
    void mousePressEvent (QMouseEvent *);   
private:   
    QString m_str;   
};   
#endif /* CLICKEDLABEL_H_ */
