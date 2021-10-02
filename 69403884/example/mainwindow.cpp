#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	counter = 0;
	serialport = new QSerialPort(this);

	ui->setupUi(this);
	ui->speed->setValidator(new QIntValidator(0, 10000000));

	enumerateBaudRates();
	enumeratePorts();

	timer.setInterval(1);

	connect(ui->refresh, &QPushButton::clicked, this, &MainWindow::enumeratePorts);
	connect(ui->start, &QPushButton::clicked, this, &MainWindow::start);
	connect(ui->stop, &QPushButton::clicked, &timer, &QTimer::stop);
	connect(&timer, &QTimer::timeout, this, &MainWindow::send);
	connect(serialport, &QSerialPort::readyRead, this, &MainWindow::read);
}

MainWindow::~MainWindow()
{
	delete ui;
	serialport->close();
	serialport->deleteLater();
}

void MainWindow::start() {
	if (!serialport->isOpen()) {
		counter = 0;
		ui->sent->setValue(0);
		ui->received->setValue(0);
		serialport->setPortName(ui->port->currentText());
		serialport->setBaudRate(ui->speed->currentText().toUInt());
		serialport->open(QIODevice::ReadWrite);
	}

	if (serialport->isOpen()) timer.start();
}

void MainWindow::send() {
	for (int i = 1000; i; i--) {
		serialport->write(QString("%1\n").arg(counter++).toLatin1());
		ui->sent->setValue(counter);
	}
}

void MainWindow::read() {
	while (serialport->canReadLine()) {
		serialport->readLine();
		ui->received->setValue(ui->received->value() + 1);
	}
}

void MainWindow::enumeratePorts()
{
	int id = 0;
	ui->port->clear();
	foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
		QString tooltip =
		  QObject::tr(
			"Port: %1\n"
			"Location: %2\n"
			"Description: %3\n"
			"Manufacturer: %4\n"
			"Vendor Identifier: %5\n"
			"Product Identifier: %6"
		  )
		  .arg(info.portName())
		  .arg(info.systemLocation())
		  .arg(info.description())
		  .arg(info.manufacturer())
		  .arg(info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString())
		  .arg(info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString());

		ui->port->addItem(info.portName());
		ui->port->setItemData(id, QVariant(tooltip), Qt::ToolTipRole);
		id++;
	}
}

void MainWindow::enumerateBaudRates()
{
	ui->speed->clear();
	foreach (qint32 BaudRate, QSerialPortInfo::standardBaudRates()) {
		ui->speed->addItem(QString("%1").arg(BaudRate), QVariant(BaudRate));
	}
}

