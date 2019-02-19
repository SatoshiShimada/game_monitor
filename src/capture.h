#ifndef CAPTURE_H
#define CAPTURE_H

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
	void startCamera(void);
	void stopCamera(void);
	void updateCameraDevice(QAction *);
	void updateCaptureMode(void);
	void setExposureCompensation(int);
	void updateRecordTime(void);
	void updateRecorderState(QMediaRecorder::State);
public slots:
	void record(void);
	void pause(void);
	void stop(void);
	void displayRecorderError(void);
	void displayCameraError(void);
signals:
	void updateRecordTimeSignal(QString);
	void updateRecordButtonMessage(QString);
private:
	QList<QCameraInfo> availableCameras;
	QScopedPointer<QCamera> m_camera;
	QScopedPointer<QMediaRecorder> m_mediaRecorder;
	QCameraViewfinder *viewfinder;
	QImageEncoderSettings m_imageSettings;
	QAudioEncoderSettings m_audioSettings;
	QVideoEncoderSettings m_videoSettings;
	QString m_videoContainerFormat;
	bool m_isCaptureingImage;
	bool m_applicationExiting;
};

#endif // CAPTURE_H

