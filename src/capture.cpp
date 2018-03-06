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
	//connect(m_mediaRecorder.data(), &QMediaRecorder::stateChanged, this, &Camera::updateRecorderState);

	m_imageCapture.reset(new QCameraImageCapture(m_camera.data()));

	connect(m_mediaRecorder.data(), &QMediaRecorder::durationChanged, this, &Capture::updateRecordTime);
	//connect(m_mediaRecorder.data(), QOverload<QMediaRecorder::Error>::of(&QMediaRecorder::error), this, &Capture::displayRecorderError);

	m_mediaRecorder->setMetaData(QMediaMetaData::Title, QVariant(QLatin1String("Test Title")));

	viewfinder = new QCameraViewfinder;
	viewfinder->hide();
	m_camera->setViewfinder(viewfinder);

	//updateCameraState(m_camera->state());
	//updateLockStatus(m_camera->lockStatus(), QCamera::UserRequest);
	//updateRecorderState(m_mediaRecorder->state());

	//connect(m_imageCapture.data(), &QCameraImageCapture::readyForCaptureChanged, this, &Camera::readyForCapture);
	//connect(m_imageCapture.data(), &QCameraImageCapture::imageCaptured, this, &Camera::processCapturedImage);
	connect(m_imageCapture.data(), &QCameraImageCapture::imageSaved, this, &Capture::imageSaved);
	//connect(m_imageCapture.data(), QOverload<int, QCameraImageCapture::Error, const QString &>::of(&QCameraImageCapture::error), this, &Capture::displayCaptureError);

	//connect(m_camera.data(), QOverload<QCamera::LockStatus, QCamera::LockChangeReason>::of(&QCamera::lockStatusChanged), this, &Camera::updateLockStatus);

	//ui->captureWidget->setTabEnabled(0, (m_camera->isCaptureModeSupported(QCamera::CaptureStillImage)));
	//ui->captureWidget->setTabEnabled(1, (m_camera->isCaptureModeSupported(QCamera::CaptureVideo)));

	updateCaptureMode();
	m_camera->start();
}

void Capture::updateCameraDevice(QAction *action)
{
	setCamera(qvariant_cast<QCameraInfo>(action->data()));
}

void Capture::updateCaptureMode()
{
	QCamera::CaptureModes captureMode = QCamera::CaptureVideo;
	if(m_camera->isCaptureModeSupported(captureMode))
		m_camera->setCaptureMode(captureMode);
}

void Capture::setExposureCompensation(int index)
{
	m_camera->exposure()->setExposureCompensation(index * 0.5);
}

void Capture::imageSaved(int id, const QString &fileName)
{
	Q_UNUSED(id);
	Q_UNUSED(fileName);
	m_isCaptureingImage = false;
	if(m_applicationExiting)
		close();
}

void Capture::startCamera()
{
	m_camera->start();
}

void Capture::stopCamera()
{
	m_camera->stop();
}

void Capture::record()
{
	m_mediaRecorder->record();
	viewfinder->show();
}

void Capture::pause()
{
	m_mediaRecorder->pause();
}

void Capture::stop()
{
	m_mediaRecorder->stop();
	viewfinder->hide();
}

void Capture::setFilename(QString filename)
{
	QDir name(filename);
	m_mediaRecorder->setOutputLocation(QUrl(name.absolutePath()));
}

void Capture::displayCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
	Q_UNUSED(id);
	Q_UNUSED(error);
	QMessageBox::warning(this, tr("Image Capture Error"), errorString);
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

