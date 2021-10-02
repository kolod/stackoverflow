#pragma once

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QIntValidator>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	QSerialPort *serialport;
	QTimer timer;
	quint64 counter;

private slots:
	void enumeratePorts();
	void enumerateBaudRates();
	void send();
	void read();
	void start();
};
