#ifndef _CAPTURE_H_
#define _CAPTURE_H_

#include <QCamera>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QScopedPointer>

#include <QtWidgets>
#include <QWidget>
#include <QString>

class Capture : public QWidget
{
	Q_OBJECT
public:
	Capture();
	~Capture();
	void setFilename(QString);
private slots:
	void setCamera(const QCameraInfo &);
	void startCamera();
	void stopCamera();
	void updateCameraDevice(QAction *);
	void updateCaptureMode();
	void setExposureCompensation(int);
	void imageSaved(int, const QString &);
public slots:
	void record();
	void pause();
	void stop();
private:
	QScopedPointer<QCamera> m_camera;
	QScopedPointer<QCameraImageCapture> m_imageCapture;
	QScopedPointer<QMediaRecorder> m_mediaRecorder;

	QImageEncoderSettings m_imageSettings;
	QAudioEncoderSettings m_audioSettings;
	QVideoEncoderSettings m_videoSettings;
	QString m_videoContainerFormat;
	bool m_isCaptureingImage;
	bool m_applicationExiting;
};

#endif // _CAPTURE_H_

