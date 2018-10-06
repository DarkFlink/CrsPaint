#ifndef COURSEWORKABOUTWIDGET_H
#define COURSEWORKABOUTWIDGET_H

#include <QWidget>

namespace Ui {
class CourseWorkAboutWidget;
}

class CourseWorkAboutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CourseWorkAboutWidget(QWidget *parent = 0);
    ~CourseWorkAboutWidget();

private:
    Ui::CourseWorkAboutWidget *ui;
};

#endif // COURSEWORKABOUTWIDGET_H
