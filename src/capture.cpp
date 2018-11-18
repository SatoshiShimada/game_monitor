#include <QMediaService>
#include <QMediaRecorder>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QMediaMetaData>

#include "capture.h"

Q_DECLARE_METATYPE(QCameraInfo)

Capture::Capture() : m_isCaptureingImage(false), m_applicationExiting(false)
{
	availableCameras = QCameraInfo::availableCameras();
	setCamera(QCameraInfo::defaultCamera());
}

Capture::~Capture()
{
}

QList<QCameraInfo> Capture::getCameras(void)
{
	return availableCameras;
}

void Capture::setCamera(const QCameraInfo &cameraInfo)
{
	m_camera.reset(new QCamera(cameraInfo));

	//connect(m_camera.data(), QOverload<QCamera::Error>::of(&QCamera::error), this, &Capture::displayCameraError);

	m_mediaRecorder.reset(new QMediaRecorder(m_camera.data()));
	connect(m_mediaRecorder.data(), &QMediaRecorder::stateChanged, this, &Capture::updateRecorderState);

	connect(m_mediaRecorder.data(), &QMediaRecorder::durationChanged, this, &Capture::updateRecordTime);
	//connect(m_mediaRecorder.data(), QOverload<QMediaRecorder::Error>::of(&QMediaRecorder::error), this, &Capture::displayRecorderError);

	m_mediaRecorder->setMetaData(QMediaMetaData::Title, QVariant(QLatin1String("Test Title")));

	viewfinder = new QCameraViewfinder;
	viewfinder->hide();
	m_camera->setViewfinder(viewfinder);

	updateRecorderState(m_mediaRecorder->state());

	//connect(m_camera.data(), QOverload<QCamera::LockStatus, QCamera::LockChangeReason>::of(&QCamera::lockStatusChanged), this, &Camera::updateLockStatus);

	updateCaptureMode();
	m_camera->start();
}

void Capture::updateCameraDevice(QAction *action)
{
	setCamera(qvariant_cast<QCameraInfo>(action->data()));
}

void Capture::updateCaptureMode(void)
{
	QCamera::CaptureModes captureMode = QCamera::CaptureVideo;
	if(m_camera->isCaptureModeSupported(captureMode))
		m_camera->setCaptureMode(captureMode);
}

void Capture::setExposureCompensation(int index)
{
	m_camera->exposure()->setExposureCompensation(index * 0.5);
}

void Capture::startCamera(void)
{
	m_camera->start();
}

void Capture::stopCamera(void)
{
	m_camera->stop();
}

void Capture::record(void)
{
	m_mediaRecorder->record();
	viewfinder->show();
}

void Capture::pause(void)
{
	m_mediaRecorder->pause();
}

void Capture::stop(void)
{
	m_mediaRecorder->stop();
	viewfinder->hide();
}

void Capture::setFilename(QString filename)
{
	QDir name(filename);
	m_mediaRecorder->setOutputLocation(QUrl(name.absolutePath()));
}

void Capture::displayCameraError(void)
{
	QMessageBox::warning(this, tr("Camera Error"), m_camera->errorString());
}

void Capture::displayRecorderError(void)
{
	QMessageBox::warning(this, tr("Capture Error"), m_mediaRecorder->errorString());
}

void Capture::updateRecordTime(void)
{
	QString str = QString("Record %1 sec").arg(m_mediaRecorder->duration() / 1000);
	emit updateRecordTimeSignal(str);
}

void Capture::updateRecorderState(QMediaRecorder::State state)
{
	switch(state) {
	case QMediaRecorder::StoppedState:
		emit updateRecordButtonMessage(QString("Record"));
		break;
	case QMediaRecorder::PausedState:
		break;
	case QMediaRecorder::RecordingState:
		emit updateRecordButtonMessage(QString("Stop record"));
		break;
	}
}

