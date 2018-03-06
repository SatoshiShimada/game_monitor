#ifndef _CAPTURE_H_
#define _CAPTURE_H_

#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
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
	QList<QCameraInfo> getCameras(void);
public slots:
	void setCamera(const QCameraInfo &);
private slots:
	void startCamera();
	void stopCamera();
	void updateCameraDevice(QAction *);
	void updateCaptureMode();
	void setExposureCompensation(int);
	void imageSaved(int, const QString &);
	void updateRecordTime(void);
public slots:
	void record();
	void pause();
	void stop();
	void displayRecorderError(void);
	void displayCameraError(void);
	void displayCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString);
signals:
	void updateRecordTimeSignal(QString);
private:
	QList<QCameraInfo> availableCameras;
	QScopedPointer<QCamera> m_camera;
	QScopedPointer<QCameraImageCapture> m_imageCapture;
	QScopedPointer<QMediaRecorder> m_mediaRecorder;
	QCameraViewfinder *viewfinder;
	QImageEncoderSettings m_imageSettings;
	QAudioEncoderSettings m_audioSettings;
	QVideoEncoderSettings m_videoSettings;
	QString m_videoContainerFormat;
	bool m_isCaptureingImage;
	bool m_applicationExiting;
};

#endif // _CAPTURE_H_

