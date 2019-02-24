#ifndef ASPECT_RATIO_PIXMAP_LABEL_H
#define ASPECT_RATIO_PIXMAP_LABEL_H

#include <QtGui>
#include <QtCore>
#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class AspectRatioPixmapLabel : public QLabel
{
	Q_OBJECT
public:
	AspectRatioPixmapLabel(QWidget *parent = 0);
	virtual int heightForWidth(int width) const;
	virtual QSize sizeHint() const;
	QPixmap scaledPixmap() const;
public slots:
	void setPixmap(const QPixmap &);
	void resizeEvent(QResizeEvent *);
private:
	QPixmap pix;
};

#endif // ASPECT_RATIO_PIXMAP_LABEL_H

