#include "OfflineProgramTool.h"
#pragma execution_character_set("utf-8")

OfflineProgramTool::OfflineProgramTool(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	versionNumber = "V4.3.2";
	openNCfileName = "D:/";
	openDatfileName = "D:/";
	saveNCfileName = "D:/";
	saveDatfileName = "D:/";

	ui.dockWidget->setHidden(true);
	ui.widget_Content_1->setHidden(false);
	ui.widget_Content_2->setHidden(true);
	openFileMode = OpenFileMode::NC;


	QLabel* state_msg = new QLabel(this);
	state_msg->setMinimumSize(state_msg->sizeHint());
	state_msg->setAlignment(Qt::AlignHCenter);
	state_msg->setText("Offline Program Tool "+ versionNumber+"    ");
	ui.statusBar->addPermanentWidget(state_msg);

	ui.tableWidget_AlreadyNCFile->setColumnCount(1);
	ui.tableWidget_AlreadyNCFile->setRowCount(1000);
	for (int i = 0; i < ui.tableWidget_AlreadyNCFile->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget_AlreadyNCFile->columnCount(); j++)
		{
			ui.tableWidget_AlreadyNCFile->setItem(i, j, new QTableWidgetItem(""));
		}
	}
	ui.tableWidget_AlreadyNCFile->horizontalHeader()->hide();
	ui.tableWidget_AlreadyNCFile->horizontalHeader()->setMinimumSectionSize(1920);

	ui.tableWidget_newNC->setColumnCount(1);
	ui.tableWidget_newNC->setRowCount(1000);
	for (int i = 0; i < ui.tableWidget_newNC->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget_newNC->columnCount(); j++)
		{
			ui.tableWidget_newNC->setItem(i, j, new QTableWidgetItem(""));
		}
	}
	ui.tableWidget_newNC->horizontalHeader()->hide();
	ui.tableWidget_newNC->horizontalHeader()->setMinimumSectionSize(1920);


	ui.tableWidget_srcFiles->setColumnCount(1);
	ui.tableWidget_srcFiles->setRowCount(1000);
	for (int i = 0; i < ui.tableWidget_srcFiles->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget_srcFiles->columnCount(); j++)
		{
			ui.tableWidget_srcFiles->setItem(i, j, new QTableWidgetItem(""));
		}
	}
	ui.tableWidget_srcFiles->horizontalHeader()->hide();
	ui.tableWidget_srcFiles->horizontalHeader()->setMinimumSectionSize(1920);


	ui.tableWidget_outSrcFiles->setColumnCount(1);
	ui.tableWidget_outSrcFiles->setRowCount(1000);
	for (int i = 0; i < ui.tableWidget_outSrcFiles->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget_outSrcFiles->columnCount(); j++)
		{
			ui.tableWidget_outSrcFiles->setItem(i, j, new QTableWidgetItem(""));
		}
	}
	ui.tableWidget_outSrcFiles->horizontalHeader()->hide();
	ui.tableWidget_outSrcFiles->horizontalHeader()->setMinimumSectionSize(1920);


	ui.comboBox_1->addItems(QStringList() << " X " << " Y " << " Z " );
	ui.comboBox_1->setCurrentIndex(0);
	ui.comboBox_2->addItems(QStringList() << " X " << " Y " << " Z " );
	ui.comboBox_2->setCurrentIndex(1);
	ui.comboBox_3->addItems(QStringList() << " X " << " Y " << " Z " );
	ui.comboBox_3->setCurrentIndex(2);

	ui.lineEdit_angle_1->setText("0");
	ui.lineEdit_angle_2->setText("0");
	ui.lineEdit_angle_3->setText("0");
	ui.lineEdit_A_X->setText("0");
	ui.lineEdit_A_Y->setText("0");
	ui.lineEdit_A_Z->setText("0");
	ui.lineEdit_A_A->setText("0");
	ui.lineEdit_A_B->setText("0");
	ui.lineEdit_A_C->setText("0");
	ui.lineEdit_B_X->setText("0");
	ui.lineEdit_B_Y->setText("0");
	ui.lineEdit_B_Z->setText("0");
	ui.lineEdit_B_A->setText("0");
	ui.lineEdit_B_B->setText("0");
	ui.lineEdit_B_C->setText("0");

	ui.spinBox_littleNumber->setValue(3);

    connect(ui.tbn_openAlreadyNCFile, &QToolButton::clicked, this, &OfflineProgramTool::ReadOldNCFile);
	connect(ui.tbn_openAlreadyDATFile, &QToolButton::clicked, this, &OfflineProgramTool::ReadOldDATFile);
	connect(ui.tbn_generate, &QToolButton::clicked, this, &OfflineProgramTool::GenerateNewNC);
	connect(ui.tbn_output, &QToolButton::clicked, this, &OfflineProgramTool::SaveNewNCFile);
	connect(ui.tbn_chooseInverse, &QToolButton::clicked, this, &OfflineProgramTool::ChooseInverse);
	QScrollBar* QSB_left = ui.tableWidget_AlreadyNCFile->verticalScrollBar();
	QScrollBar* QSB_right = ui.tableWidget_newNC->verticalScrollBar();
	QScrollBar* QSB_left_2 = ui.tableWidget_srcFiles->verticalScrollBar();
	QScrollBar* QSB_right_2 = ui.tableWidget_outSrcFiles->verticalScrollBar();
	connect(QSB_left,  &QScrollBar::valueChanged, this, &OfflineProgramTool::LeftScroll);
	connect(QSB_right, &QScrollBar::valueChanged, this, &OfflineProgramTool::RightScroll);
	connect(QSB_left_2, &QScrollBar::valueChanged, this, &OfflineProgramTool::LeftScroll);
	connect(QSB_right_2, &QScrollBar::valueChanged, this, &OfflineProgramTool::RightScroll);
	connect(ui.toolButton_offlineProgramModel, &QToolButton::clicked, this, &OfflineProgramTool::OfflineProgramToolModel);
	connect(ui.toolButton_other, &QToolButton::clicked, this, &OfflineProgramTool::OtherModel);
	connect(ui.tbn_about, &QToolButton::clicked, this, &OfflineProgramTool::About);
	connect(ui.tbn_choose, &QToolButton::clicked, this, &OfflineProgramTool::Choose);
	connect(ui.tbn_turnLeft, &QToolButton::clicked, this, &OfflineProgramTool::NewNCTurnLeft);
    connect(ui.tbn_A_AutoPaste, &QToolButton::clicked, [&]() 
        {
			QClipboard* clipboard = QApplication::clipboard();
			QString originalText = clipboard->text();
			qDebug() << originalText;
			std::vector<double> data;
			QString pattern = "^((-?\\d+)(\\.\\d+)?)(\\s*)((-?\\d+)(\\.\\d+)?)(\\s*)((-?\\d+)(\\.\\d+)?)(\\s*)((-?\\d+)(\\.\\d+)?)(\\s*)((-?\\d+)(\\.\\d+)?)(\\s*)((-?\\d+)(\\.\\d+)?)$";
			QRegExp rx(pattern);
			int pos = originalText.indexOf(rx);
			qDebug() << pos;
			if (pos >= 0)
			{
				QStringList list = rx.capturedTexts();
				qDebug() << list;
				ui.lineEdit_A_X->setText(list[1]);
				ui.lineEdit_A_Y->setText(list[5]);
				ui.lineEdit_A_Z->setText(list[9]);
				ui.lineEdit_A_A->setText(list[13]);
				ui.lineEdit_A_B->setText(list[17]);
				ui.lineEdit_A_C->setText(list[21]);;
			}
			else
			{
				QMessageBox::warning(this, "错误", " 格式错误", QMessageBox::Ok);
			}

        });
	connect(ui.tbn_B_AutoPaste, &QToolButton::clicked, [&]()
		{
			QClipboard* clipboard = QApplication::clipboard();
			QString originalText = clipboard->text();
			qDebug() << originalText;
			std::vector<double> data;
			QString pattern = "^((-?\\d+)(\\.\\d+)?)(\\s*)((-?\\d+)(\\.\\d+)?)(\\s*)((-?\\d+)(\\.\\d+)?)(\\s*)((-?\\d+)(\\.\\d+)?)(\\s*)((-?\\d+)(\\.\\d+)?)(\\s*)((-?\\d+)(\\.\\d+)?)$";
			QRegExp rx(pattern);
			int pos = originalText.indexOf(rx);
			qDebug() << pos;
			if (pos >= 0)
			{
				QStringList list = rx.capturedTexts();
				qDebug() << list;
				ui.lineEdit_B_X->setText(list[1]);
				ui.lineEdit_B_Y->setText(list[5]);
				ui.lineEdit_B_Z->setText(list[9]);
				ui.lineEdit_B_A->setText(list[13]);
				ui.lineEdit_B_B->setText(list[17]);
				ui.lineEdit_B_C->setText(list[21]);;
			}
			else
			{
				QMessageBox::warning(this, "错误", " 格式错误", QMessageBox::Ok);
			}
		});
	connect(ui.tbn_AutoPaste_RPY, &QToolButton::clicked, [&]()
		{
			QClipboard* clipboard = QApplication::clipboard();
			QString originalText = clipboard->text();
			qDebug() << originalText;
			std::vector<double> data;
			QString pattern = "^((-?\\d+)(\\.\\d+)?)(\\s)((-?\\d+)(\\.\\d+)?)(\\s)((-?\\d+)(\\.\\d+)?)$";
			QRegExp rx(pattern);
			int pos = originalText.indexOf(rx);
			if (pos >= 0)
			{
				QStringList list = rx.capturedTexts();
				qDebug() << list;
				ui.lineEdit_angle_1->setText(list[1]);
				ui.lineEdit_angle_2->setText(list[5]);
				ui.lineEdit_angle_3->setText(list[9]);
			}
			else
			{
				QMessageBox::warning(this, "错误", " 格式错误", QMessageBox::Ok);
			}
		});

	connect(ui.tbn_ImportMoreFiles, &QToolButton::clicked, this, &OfflineProgramTool::ImportMoreFiles_SRC);
	connect(ui.tbn_generateSRC, &QToolButton::clicked, this, &OfflineProgramTool::GenerateNewSRC);
	connect(ui.tbn_SetSaveFiles, &QToolButton::clicked, this, &OfflineProgramTool::SetSRCSaveFiles);
	connect(ui.tbn_openSRCFiles, &QToolButton::clicked, this, &OfflineProgramTool::OpenSRCFiles);
	connect(ui.tbn_deleteRow, &QToolButton::clicked, this, &OfflineProgramTool::DeleteRow);
	connect(ui.tbn_addFiles, &QToolButton::clicked, this, &OfflineProgramTool::AddSRCFiles);

	connect(ui.tbn_checkUpdate, &QToolButton::clicked, this, &OfflineProgramTool::CheckVersionUpdate);

	manager = new QNetworkAccessManager(this);
	bool isr = connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ReplyFinished(QNetworkReply*)));

	connect(ui.lineEdit_A_X, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_A_Y, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_A_Z, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_A_A, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_A_B, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_A_C, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);

	connect(ui.lineEdit_B_X, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_B_Y, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_B_Z, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_B_A, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_B_B, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_B_C, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);

	connect(ui.lineEdit_angle_1, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_angle_2, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);
	connect(ui.lineEdit_angle_3, &QLineEdit::editingFinished, this, &OfflineProgramTool::InputSafeCheck);

}

OfflineProgramTool::~OfflineProgramTool()
{}

void OfflineProgramTool::Choose()
{
	if (openFileMode == OpenFileMode::NC)
	{
		QList<QTableWidgetItem*> selectedItems = ui.tableWidget_AlreadyNCFile->selectedItems();
		for (size_t i = 0; i < selectedItems.size(); i++)
		{
			QString ncText = selectedItems.at(i)->text();
			qDebug() << ncText;
			//正则表达式提取数据
			std::vector<double> data;
			XYZABC point;
			QStringList list;
			QString pattern = "(.*)(X=)((-?\\d+)(\\.\\d+)?)(\\s)(Y=)((-?\\d+)(\\.\\d+)?)(\\s)(Z=)((-?\\d+)(\\.\\d+)?)(\\s)(A=)((-?\\d+)(\\.\\d+)?)(\\s)(B=)((-?\\d+)(\\.\\d+)?)(\\s)(C=)((-?\\d+)(\\.\\d+)?)(.*)";
			QRegExp rx(pattern);
			int pos = ncText.indexOf(rx);
			qDebug() << pos;
			if (pos < 0)
			{
				selectedItems.at(i)->setSelected(false);
				continue;
			}
			
		}
	}
	else if (openFileMode == OpenFileMode::DAT)
	{
		QList<QTableWidgetItem*> selectedItems = ui.tableWidget_AlreadyNCFile->selectedItems();
		for (size_t i = 0; i < selectedItems.size(); i++)
		{
			QString ncText = selectedItems.at(i)->text();
			qDebug() << ncText;
			//正则表达式提取数据
			std::vector<double> data;
			XYZABC point;
			QStringList list;
			QString pattern = "(.*)(X )((-?\\d+)(\\.\\d+)?)(,Y )((-?\\d+)(\\.\\d+)?)(,Z )((-?\\d+)(\\.\\d+)?)(,A )((-?\\d+)(\\.\\d+)?)(,B )((-?\\d+)(\\.\\d+)?)(,C )((-?\\d+)(\\.\\d+)?)(.*)";
			QRegExp rx(pattern);
			int pos = ncText.indexOf(rx);
			qDebug() << pos;
			if (pos < 0)
			{
				selectedItems.at(i)->setSelected(false);
				continue;
			}

		}
	}
}

void OfflineProgramTool::ChooseInverse()
{
	for (size_t i = 0; i < ui.tableWidget_AlreadyNCFile->rowCount(); i++)
	{
		ui.tableWidget_AlreadyNCFile->item(i, 0)->setSelected(!ui.tableWidget_AlreadyNCFile->item(i, 0)->isSelected());
	}
}

void OfflineProgramTool::ReadOldNCFile()
{
	QString str = QFileDialog::getOpenFileName(this, "打开NC文件", openNCfileName, "(*.txt)");
	ui.lineEdit_alreadyNCFilePosition->setText(str);
	if (str == "")
	{
		QMessageBox::warning(this, "错误", "文件打开失败", QMessageBox::Ok);
		return;
	}
	openNCfileName = str;
	openFileMode = OpenFileMode::NC;
	oldNCfileContent.clear();
	ui.lineEdit_outputFile->setText("");
	ui.tableWidget_AlreadyNCFile->setColumnCount(1);
	ui.tableWidget_AlreadyNCFile->setRowCount(1000);
	for (int i = 0; i < ui.tableWidget_AlreadyNCFile->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget_AlreadyNCFile->columnCount(); j++)
		{
			ui.tableWidget_AlreadyNCFile->setItem(i, j, new QTableWidgetItem(""));
		}
	}
	ui.tableWidget_newNC->setColumnCount(1);
	ui.tableWidget_newNC->setRowCount(1000);
	for (int i = 0; i < ui.tableWidget_newNC->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget_newNC->columnCount(); j++)
		{
			ui.tableWidget_newNC->setItem(i, j, new QTableWidgetItem(""));
		}
	}

	QFile file(str);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	while (!file.atEnd())
	{
		QByteArray line = file.readLine();
		QString str(line);
		std::string stritem = str.toStdString();
		stritem.erase(std::remove(stritem.begin(), stritem.end(), '\n'), stritem.end());
		str= str.fromStdString(stritem);
		oldNCfileContent.push_back(str);
	}
	file.close();
	ui.tableWidget_AlreadyNCFile->setRowCount(oldNCfileContent.size());
	ui.tableWidget_newNC->setRowCount(oldNCfileContent.size());
	for (size_t i = 0; i < oldNCfileContent.size(); i++)
	{
		ui.tableWidget_AlreadyNCFile->item(i, 0)->setText(oldNCfileContent[i]);
		//G00匹配
		QStringList list;
		QString pattern = "(.*)(G00)( X=)((-?\\d+)(\\.\\d+)?)(\\s)(Y=)((-?\\d+)(\\.\\d+)?)(\\s)(Z=)((-?\\d+)(\\.\\d+)?)(\\s)(A=)((-?\\d+)(\\.\\d+)?)(\\s)(B=)((-?\\d+)(\\.\\d+)?)(\\s)(C=)((-?\\d+)(\\.\\d+)?)(.*)";
		QRegExp rx(pattern);
		int pos = oldNCfileContent[i].indexOf(rx);
		qDebug() << pos;
		if (pos >= 0)
		{
			list = rx.capturedTexts();
			qDebug() << list;
			ui.lineEdit_A_X->setText(list[4]);
			ui.lineEdit_A_Y->setText(list[9]);
			ui.lineEdit_A_Z->setText(list[14]);
			ui.lineEdit_A_A->setText(list[19]);
			ui.lineEdit_A_B->setText(list[24]);
			ui.lineEdit_A_C->setText(list[29]);
		}
	}
	QApplication::restoreOverrideCursor();
}

void OfflineProgramTool::ReadOldDATFile()
{
	QString str = QFileDialog::getOpenFileName(this, "打开DAT文件", openDatfileName, "(*.dat)");
	ui.lineEdit_alreadyNCFilePosition->setText(str);
	if (str == "")
	{
		QMessageBox::warning(this, "错误", "文件打开失败", QMessageBox::Ok);
		return;
	}
	openDatfileName = str;
	openFileMode = OpenFileMode::DAT;
	oldDATfileContent.clear();
	ui.lineEdit_outputFile->setText("");
	ui.tableWidget_AlreadyNCFile->setColumnCount(1);
	ui.tableWidget_AlreadyNCFile->setRowCount(1000);
	for (int i = 0; i < ui.tableWidget_AlreadyNCFile->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget_AlreadyNCFile->columnCount(); j++)
		{
			ui.tableWidget_AlreadyNCFile->setItem(i, j, new QTableWidgetItem(""));
		}
	}
	ui.tableWidget_newNC->setColumnCount(1);
	ui.tableWidget_newNC->setRowCount(1000);
	for (int i = 0; i < ui.tableWidget_newNC->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget_newNC->columnCount(); j++)
		{
			ui.tableWidget_newNC->setItem(i, j, new QTableWidgetItem(""));
		}
	}

	QFile file(str);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	while (!file.atEnd())
	{
		QByteArray line = file.readLine();
		QString str(line);
		std::string stritem = str.toStdString();
		stritem.erase(std::remove(stritem.begin(), stritem.end(), '\n'), stritem.end());
		str = str.fromStdString(stritem);
		oldDATfileContent.push_back(str);
	}
	file.close();
	ui.tableWidget_AlreadyNCFile->setRowCount(oldDATfileContent.size());
	ui.tableWidget_newNC->setRowCount(oldDATfileContent.size());
	for (size_t i = 0; i < oldDATfileContent.size(); i++)
	{
		ui.tableWidget_AlreadyNCFile->item(i, 0)->setText(oldDATfileContent[i]);
	}
	QApplication::restoreOverrideCursor();

}

void OfflineProgramTool::GenerateNewNC()
{
	QList<QTableWidgetItem*> selectedItems = ui.tableWidget_AlreadyNCFile->selectedItems();
	for (size_t i = 0; i < ui.tableWidget_AlreadyNCFile->rowCount(); i++)
	{
		ui.tableWidget_AlreadyNCFile->item(i, 0)->setBackground(QBrush(Qt::white));
	}
	for (size_t i = 0; i < selectedItems.size(); i++)
	{
		ui.tableWidget_AlreadyNCFile->item(selectedItems.at(i)->row(), 0)->setBackground(QBrush(Qt::cyan));
	}
	std::vector<int> selectedRowNumbers;
	for (size_t i = 0; i < selectedItems.size(); i++)
	{
		qDebug() << selectedItems.at(i)->row();
		selectedRowNumbers.push_back(selectedItems.at(i)->row());
		QString ncText = selectedItems.at(i)->text();
		qDebug() << ncText;
		//正则表达式提取数据
		std::vector<double> data;
		XYZABC point;
		QStringList list;
		if (openFileMode == OpenFileMode::NC)
		{
			QString pattern = "(.*)(X=)((-?\\d+)(\\.\\d+)?)(\\s)(Y=)((-?\\d+)(\\.\\d+)?)(\\s)(Z=)((-?\\d+)(\\.\\d+)?)(\\s)(A=)((-?\\d+)(\\.\\d+)?)(\\s)(B=)((-?\\d+)(\\.\\d+)?)(\\s)(C=)((-?\\d+)(\\.\\d+)?)(.*)";
			QRegExp rx(pattern);
			int pos = ncText.indexOf(rx);
			qDebug() << pos;
			if (pos >= 0)
			{
				list = rx.capturedTexts();
				qDebug() << list;
				point.x = list.at(3).toDouble();
				point.y = list.at(8).toDouble();
				point.z = list.at(13).toDouble();
				point.a = list.at(18).toDouble();
				point.b = list.at(23).toDouble();
				point.c = list.at(28).toDouble();
			}
			else
			{
				QMessageBox::warning(this, "错误", " 选择的NC格式错误 ", QMessageBox::Ok);
				for (size_t i = 0; i < ui.tableWidget_AlreadyNCFile->rowCount(); i++)
				{
					ui.tableWidget_AlreadyNCFile->item(i, 0)->setBackground(QBrush(Qt::white));
				}
				return;
			}
		}
		else if (openFileMode == OpenFileMode::DAT)
		{
			QString pattern = "(.*)(X )((-?\\d+)(\\.\\d+)?)(,Y )((-?\\d+)(\\.\\d+)?)(,Z )((-?\\d+)(\\.\\d+)?)(,A )((-?\\d+)(\\.\\d+)?)(,B )((-?\\d+)(\\.\\d+)?)(,C )((-?\\d+)(\\.\\d+)?)(.*)";
			QRegExp rx(pattern);
			int pos = ncText.indexOf(rx);
			qDebug() << pos;
			if (pos >= 0)
			{
				list = rx.capturedTexts();
				qDebug() << list;
				point.x = list.at(3).toDouble();
				point.y = list.at(7).toDouble();
				point.z = list.at(11).toDouble();
				point.a = list.at(15).toDouble();
				point.b = list.at(19).toDouble();
				point.c = list.at(23).toDouble();
			}
			else
			{
				QMessageBox::warning(this, "错误", " 选择的DAT格式错误 ", QMessageBox::Ok);
				for (size_t i = 0; i < ui.tableWidget_AlreadyNCFile->rowCount(); i++)
				{
					ui.tableWidget_AlreadyNCFile->item(i, 0)->setBackground(QBrush(Qt::white));
				}
				return;
			}
		}
		
		qDebug() << point.x <<" "<< point.y << " " << point.z << " " << point.a << " " << point.b << " " << point.c;
		XYZABC point2Base_oldXYZABC = { point.x ,point.y ,point.z ,point.a ,point.b ,point.c };
		Eigen::Matrix4d point2Base_oldMatrix = W_RPY(point2Base_oldXYZABC);

		XYZABC oldPart2BaseXYZABC = { ui.lineEdit_A_X ->text().toDouble(),ui.lineEdit_A_Y->text().toDouble(),ui.lineEdit_A_Z->text().toDouble(),ui.lineEdit_A_A->text().toDouble(),ui.lineEdit_A_B->text().toDouble(),ui.lineEdit_A_C->text().toDouble() };
		XYZABC newPart2BaseXYZABC = { ui.lineEdit_B_X->text().toDouble(),ui.lineEdit_B_Y->text().toDouble(),ui.lineEdit_B_Z->text().toDouble(),ui.lineEdit_B_A->text().toDouble(),ui.lineEdit_B_B->text().toDouble(),ui.lineEdit_B_C->text().toDouble() };

		Eigen::Matrix4d point2Base_newMatrix = W_RPY(newPart2BaseXYZABC) * W_RPY(oldPart2BaseXYZABC).inverse() * point2Base_oldMatrix;
		

		//TCP
		int angleAxis_1, angleAxis_2, angleAxis_3;
		angleAxis_1 = ui.comboBox_1->currentIndex();
		angleAxis_2 = ui.comboBox_2->currentIndex();
		angleAxis_3 = ui.comboBox_3->currentIndex();

		double angle_1, angle_2, angle_3;
		angle_1 = ui.lineEdit_angle_1->text().toDouble();
		angle_2 = ui.lineEdit_angle_2->text().toDouble();
		angle_3 = ui.lineEdit_angle_3->text().toDouble();

		Eigen::Matrix4d Axis_1_RotMatrix, Axis_2_RotMatrix, Axis_3_RotMatrix;
		switch (angleAxis_1)
		{
		case 0:
			Axis_1_RotMatrix << 1, 0, 0, 0,
				0, cos(angle_1 * 3.14159265 / 180), -sin(angle_1 * 3.14159265 / 180), 0,
				0, sin(angle_1 * 3.14159265 / 180), cos(angle_1 * 3.14159265 / 180), 0,
				0, 0, 0, 1;
			break;
		case 1:
			Axis_1_RotMatrix << cos(angle_1 * 3.14159265 / 180), 0, sin(angle_1 * 3.14159265 / 180), 0,
				0, 1, 0, 0,
				-sin(angle_1 * 3.14159265 / 180), 0, cos(angle_1 * 3.14159265 / 180), 0,
				0, 0, 0, 1;
			break;
		case 2:
			Axis_1_RotMatrix << cos(angle_1 * 3.14159265 / 180), -sin(angle_1 * 3.14159265 / 180), 0, 0,
				sin(angle_1 * 3.14159265 / 180), cos(angle_1 * 3.14159265 / 180), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1;
			break;
		default:
			Axis_1_RotMatrix << 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1;
			break;
		}
		switch (angleAxis_2)
		{
		case 0:
			Axis_2_RotMatrix << 1, 0, 0, 0,
				0, cos(angle_2 * 3.14159265 / 180), -sin(angle_2 * 3.14159265 / 180), 0,
				0, sin(angle_2 * 3.14159265 / 180), cos(angle_2 * 3.14159265 / 180), 0,
				0, 0, 0, 1;
			break;
		case 1:
			Axis_2_RotMatrix << cos(angle_2 * 3.14159265 / 180), 0, sin(angle_2 * 3.14159265 / 180), 0,
				0, 1, 0, 0,
				-sin(angle_2 * 3.14159265 / 180), 0, cos(angle_2 * 3.14159265 / 180), 0,
				0, 0, 0, 1;
			break;
		case 2:
			Axis_2_RotMatrix << cos(angle_2 * 3.14159265 / 180), -sin(angle_2 * 3.14159265 / 180), 0, 0,
				sin(angle_2 * 3.14159265 / 180), cos(angle_2 * 3.14159265 / 180), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1;
			break;
		default:
			Axis_2_RotMatrix << 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1;
			break;
		}
		switch (angleAxis_3)
		{
		case 0:
			Axis_3_RotMatrix << 1, 0, 0, 0,
				0, cos(angle_3 * 3.14159265 / 180), -sin(angle_3 * 3.14159265 / 180), 0,
				0, sin(angle_3 * 3.14159265 / 180), cos(angle_3 * 3.14159265 / 180), 0,
				0, 0, 0, 1;
			break;
		case 1:
			Axis_3_RotMatrix << cos(angle_3 * 3.14159265 / 180), 0, sin(angle_3 * 3.14159265 / 180), 0,
				0, 1, 0, 0,
				-sin(angle_3 * 3.14159265 / 180), 0, cos(angle_3 * 3.14159265 / 180), 0,
				0, 0, 0, 1;
			break;
		case 2:
			Axis_3_RotMatrix << cos(angle_3 * 3.14159265 / 180), -sin(angle_3 * 3.14159265 / 180), 0, 0,
				sin(angle_3 * 3.14159265 / 180), cos(angle_3 * 3.14159265 / 180), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1;
			break;
		default:
			Axis_3_RotMatrix << 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1;
			break;
		}
		point2Base_newMatrix = point2Base_newMatrix * Axis_1_RotMatrix * Axis_2_RotMatrix * Axis_3_RotMatrix;

		XYZABC point2Base_newXYZABC = W_T2xyzabc(point2Base_newMatrix);
		QString newNCstr;
		if (openFileMode == OpenFileMode::NC)
		{
			newNCstr = list[1] + list[2] + QString::number(point2Base_newXYZABC.x, 'f', ui.spinBox_littleNumber->value()) + list[6] + list[7] + QString::number(point2Base_newXYZABC.y, 'f', ui.spinBox_littleNumber->value()) + list[11] + list[12] + QString::number(point2Base_newXYZABC.z, 'f', ui.spinBox_littleNumber->value()) + list[16] + list[17] + QString::number(point2Base_newXYZABC.a, 'f', ui.spinBox_littleNumber->value()) + list[21] + list[22] + QString::number(point2Base_newXYZABC.b, 'f', ui.spinBox_littleNumber->value()) + list[26] + list[27] + QString::number(point2Base_newXYZABC.c, 'f', ui.spinBox_littleNumber->value()) + list[list.count() - 1];
		}
		else if (openFileMode == OpenFileMode::DAT)
		{
			newNCstr = list[1] + list[2] + QString::number(point2Base_newXYZABC.x, 'f', ui.spinBox_littleNumber->value()) + list[6] + QString::number(point2Base_newXYZABC.y, 'f', ui.spinBox_littleNumber->value()) + list[10] + QString::number(point2Base_newXYZABC.z, 'f', ui.spinBox_littleNumber->value()) + list[14] + QString::number(point2Base_newXYZABC.a, 'f', ui.spinBox_littleNumber->value()) + list[18] + QString::number(point2Base_newXYZABC.b, 'f', ui.spinBox_littleNumber->value()) + list[22] + QString::number(point2Base_newXYZABC.c, 'f', ui.spinBox_littleNumber->value()) + list[list.count() - 1];
		}

		QTableWidgetItem* item = new QTableWidgetItem(newNCstr);
		ui.tableWidget_newNC->setItem(selectedItems.at(i)->row(), 0, item);
		ui.tableWidget_newNC->item(selectedItems.at(i)->row(), 0)->setBackground(QBrush(Qt::yellow));
	}

	for (size_t j = 0; j < ui.tableWidget_AlreadyNCFile->rowCount(); j++)
	{
		bool isRight = false;
		for (size_t k = 0; k < selectedRowNumbers.size(); k++)
		{
			if (j == selectedRowNumbers[k])
			{
				isRight = true;
				break;
			}
		}
		if (isRight == false)
		{
			QTableWidgetItem* newItem = ui.tableWidget_AlreadyNCFile->item(j, 0)->clone();
			ui.tableWidget_newNC->setItem(j, 0, newItem);
		}
		//检查G00
		QStringList list;
		QString pattern = "(.*)(G00)( X=)((-?\\d+)(\\.\\d+)?)(\\s)(Y=)((-?\\d+)(\\.\\d+)?)(\\s)(Z=)((-?\\d+)(\\.\\d+)?)(\\s)(A=)((-?\\d+)(\\.\\d+)?)(\\s)(B=)((-?\\d+)(\\.\\d+)?)(\\s)(C=)((-?\\d+)(\\.\\d+)?)(.*)";
		QRegExp rx(pattern);
		int pos = ui.tableWidget_AlreadyNCFile->item(j, 0)->text().indexOf(rx);
		qDebug() << pos;
		if (pos >= 0)
		{
			list = rx.capturedTexts();
			qDebug() << list;
			QTableWidgetItem* newItem = ui.tableWidget_AlreadyNCFile->item(j, 0)->clone();
			QString newG00Str = list[1]+"G00 X=" + ui.lineEdit_B_X->text() + " Y=" + ui.lineEdit_B_Y->text() + " Z=" + ui.lineEdit_B_Z->text() + " A=" + ui.lineEdit_B_A->text() + " B=" + ui.lineEdit_B_B->text() + " C=" + ui.lineEdit_B_C->text();
			newItem->setText(newG00Str);
			ui.tableWidget_newNC->setItem(j, 0, newItem);
		}
	}
}

void OfflineProgramTool::NewNCTurnLeft()
{
	for (size_t i = 0; i < ui.tableWidget_AlreadyNCFile->rowCount(); i++)
	{
		ui.tableWidget_AlreadyNCFile->item(i, 0)->setBackground(QBrush(Qt::white));
	}
	for (size_t i = 0; i < ui.tableWidget_newNC->rowCount(); i++)
	{
		ui.tableWidget_newNC->item(i, 0)->setBackground(QBrush(Qt::white));
	}
	for (size_t i = 0; i < ui.tableWidget_newNC->rowCount(); i++)
	{
		ui.tableWidget_AlreadyNCFile->item(i, 0)->setText(ui.tableWidget_newNC->item(i, 0)->text());
		ui.tableWidget_newNC->item(i, 0)->setText("");
	}
	ui.lineEdit_alreadyNCFilePosition->setText("");
	ui.lineEdit_outputFile->setText("");
}

void OfflineProgramTool::SaveNewNCFile()
{
	std::vector<QString> newNCFile;
	for (size_t i = 0; i < ui.tableWidget_newNC->rowCount(); i++)
	{
		newNCFile.push_back(ui.tableWidget_newNC->item(i,0)->text());
	}

	QString str;
	if (openFileMode == OpenFileMode::NC)
	{
		saveNCfileName = openNCfileName;
		str = QFileDialog::getSaveFileName(this, "Save NC file", saveNCfileName, "(*.txt)");
		if (str == "")
		{
			return;
		}
		saveNCfileName = str;
	}
	else if (openFileMode == OpenFileMode::DAT)
	{
		saveDatfileName = openDatfileName;
		str = QFileDialog::getSaveFileName(this, "Save DAT file", saveDatfileName, "(*.dat)");
		if (str == "")
		{
			return;
		}
		saveDatfileName = str;
	}
	QFile filename(str);
	if (!filename.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "错误", "保存失败", QMessageBox::Ok);
		return;
	}
	else
	{
		QTextStream textStream(&filename);
		for (size_t i = 0; i < newNCFile.size(); i++)
		{
			textStream << newNCFile[i]<<"\n";
		}
	}
	QMessageBox::information(this, "保存文件", "保存成功", QMessageBox::Ok);
	filename.close();

	ui.lineEdit_outputFile->setText(str);

}

void OfflineProgramTool::LeftScroll()
{
	QScrollBar* QSB_left = ui.tableWidget_AlreadyNCFile->verticalScrollBar();
	QScrollBar* QSB_right = ui.tableWidget_newNC->verticalScrollBar();
	QScrollBar* QSB_left_2 = ui.tableWidget_srcFiles->verticalScrollBar();
	QScrollBar* QSB_right_2 = ui.tableWidget_outSrcFiles->verticalScrollBar();
	QSB_right->setValue(QSB_left->value());
	QSB_right_2->setValue(QSB_left_2->value());
}

void OfflineProgramTool::RightScroll()
{
	QScrollBar* QSB_left = ui.tableWidget_AlreadyNCFile->verticalScrollBar();
	QScrollBar* QSB_right = ui.tableWidget_newNC->verticalScrollBar();
	QScrollBar* QSB_left_2 = ui.tableWidget_srcFiles->verticalScrollBar();
	QScrollBar* QSB_right_2 = ui.tableWidget_outSrcFiles->verticalScrollBar();
	QSB_left->setValue(QSB_right->value());
	QSB_left_2->setValue(QSB_right_2->value());
}

void OfflineProgramTool::OfflineProgramToolModel()
{
	ui.widget_Content_1->setHidden(false);
	ui.widget_Content_2->setHidden(true);
}

void OfflineProgramTool::OtherModel()
{
	ui.widget_Content_1->setHidden(true);
	ui.widget_Content_2->setHidden(false);
}

void OfflineProgramTool::About()
{

	QString ReadMe = "软件说明：\n软件为离线编程二次生成NC工具，由南京航空航天大学机电学院田威老师课题组王忠康开发，联系方式2998963449@qq.com\n\n使用说明\n此软件主要作为离线编程软件的辅助使用，面向于大规模NC文件的批量修改，坐标转换使用；\n\n最新版本请参考GitHub仓库：https://github.com/junlin49/OfflineProgramTool \n\n欢迎star和issue";
	ui.textEdit->setText(ReadMe);
	ui.dockWidget->setHidden(isDockWidgetVisual);
	isDockWidgetVisual = !isDockWidgetVisual;

}

Eigen::Matrix4d OfflineProgramTool::W_RPY(XYZABC v)
{

	//转弧度
	v.a = v.a * 3.14159265 / 180;
	v.b = v.b * 3.14159265 / 180;
	v.c = v.c * 3.14159265 / 180;
	Eigen::Vector4d column1 = { cos(v.a) * cos(v.b), sin(v.a) * cos(v.b), -sin(v.b), 0 };
	Eigen::Vector4d column2 = { cos(v.a) * sin(v.b) * sin(v.c) - sin(v.a) * cos(v.c), sin(v.a) * sin(v.b) * sin(v.c) + cos(v.a) * cos(v.c), cos(v.b) * sin(v.c), 0 };
	Eigen::Vector4d column3 = { cos(v.a) * sin(v.b) * cos(v.c) + sin(v.a) * sin(v.c), sin(v.a) * sin(v.b) * cos(v.c) - cos(v.a) * sin(v.c), cos(v.b) * cos(v.c), 0 };
	Eigen::Vector4d column4 = { v.x, v.y, v.z, 1 };
	Eigen::Matrix4d T;
	T << column1, column2, column3, column4;
	return T;
}

XYZABC OfflineProgramTool::W_T2xyzabc(Eigen::Matrix4d T)
{
	XYZABC v;
	v.x = T(0, 3);
	v.y = T(1, 3);
	v.z = T(2, 3);
	v.b = atan2(-T(2, 0), abs(sqrt(T(0, 0) * T(0, 0) + T(1, 0) * T(1, 0))));
	v.a = atan2(T(1, 0) / cos(v.b), T(0, 0) / cos(v.b));
	v.c = atan2(T(2, 1) / cos(v.b), T(2, 2) / cos(v.b));

	v.b = v.b * 180 / 3.14159265;
	v.a = v.a * 180 / 3.14159265;
	v.c = v.c * 180 / 3.14159265;


	return v;
}

void OfflineProgramTool::ImportMoreFiles_SRC()
{
	QStringList str = QFileDialog::getOpenFileNames(this, "选择SRC文件", "D:/", "(*.src)");
	if (str.size() == 0)
	{
		QMessageBox::warning(this, "错误", "文件打开失败", QMessageBox::Ok);
		return;
	}

	ui.tableWidget_srcFiles->setColumnCount(1);
	ui.tableWidget_srcFiles->setRowCount(str.size());
	for (int i = 0; i < ui.tableWidget_srcFiles->rowCount(); i++)
	{
		for (int j = 0; j < ui.tableWidget_srcFiles->columnCount(); j++)
		{
			ui.tableWidget_srcFiles->setItem(i, j, new QTableWidgetItem(""));
		}
	}

	for (size_t i = 0; i < str.size(); i++)
	{
		ui.tableWidget_srcFiles->setItem(i, 0, new QTableWidgetItem(str.at(i)));
	}
}

void OfflineProgramTool::GenerateNewSRC()
{
	if (srcSaveFileDir == "")
	{
		QMessageBox::warning(this, "错误", "未设置保存文件路径", QMessageBox::Ok);
		return;
	}
	ui.tableWidget_outSrcFiles->setRowCount(ui.tableWidget_srcFiles->rowCount());

	for (int k = 0; k < ui.tableWidget_outSrcFiles->rowCount(); k++)
	{
		for (int h = 0; h < ui.tableWidget_outSrcFiles->columnCount(); h++)
		{
			ui.tableWidget_outSrcFiles->setItem(k, h, new QTableWidgetItem(""));
		}
	}
	for (size_t i = 0; i < ui.tableWidget_srcFiles->selectedItems().size(); i++)
	{
		QString str = ui.tableWidget_srcFiles->selectedItems().at(i)->text();
		//找到文件名字
		QStringList strlist = str.split("/");
		QString fileNameLast = strlist[strlist.size() - 1];


		QFile file(str);
		std::vector<QString> fileContent;
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		while (!file.atEnd())
		{
			QByteArray line = file.readLine();
			QString str(line);
			std::string stritem = str.toStdString();
			stritem.erase(std::remove(stritem.begin(), stritem.end(), '\n'), stritem.end());
			QString str_content = str.fromStdString(stritem);
			fileContent.push_back(str_content);
			qDebug() << str_content;
		}
		file.close();
		QApplication::restoreOverrideCursor();

		std::vector<QString> new_fileContent = fileContent;
		std::vector<int> rowNumberV_needTriger;
		int rowNumber_needOut;
		for (size_t j = 0; j < fileContent.size(); j++)
		{
			//寻找lin关键字，前一行加入trigger，lin一行最后要有C_DIS
			QString pattern = "(LIN)(\\s)(.*)";
			QRegExp rx(pattern);
			int pos = fileContent[j].indexOf(rx);
			if (pos >= 0)
			{
				QStringList list = rx.capturedTexts();
				qDebug() << list;
				rowNumberV_needTriger.push_back(j);
				if (list[3] == "P1")
				{
					new_fileContent[j] = list[0] + " C_DIS";
				}
			}

			//寻找RSI_CREATE，前一行加入$OUT[60] = FALSE
			pattern = "(.*)(RSI_CREATE)(.*)";
			QRegExp rx2(pattern);
			pos = fileContent[j].indexOf(rx2);
			if (pos >= 0)
			{
				QStringList list = rx.capturedTexts();
				qDebug() << list;
				rowNumber_needOut = j;
			}

		}

		QString triggerBoolFalse = "FALSE";
		QString triggerBoolTrue = "TRUE";
		for (size_t j = 0; j < rowNumberV_needTriger.size(); j++)
		{
			if (j%2 == 0)
			{
				QString insertContent = "TRIGGER WHEN DISTANCE=1 DELAY=0 DO $OUT[60]=" + triggerBoolTrue;
				new_fileContent.insert(new_fileContent.begin() + rowNumberV_needTriger[j] + j, insertContent);
			}
			else
			{
				QString insertContent = "TRIGGER WHEN DISTANCE=1 DELAY=0 DO $OUT[60]=" + triggerBoolFalse;
				new_fileContent.insert(new_fileContent.begin() + rowNumberV_needTriger[j] + j, insertContent);
			}
		}

		new_fileContent.insert(new_fileContent.begin() + rowNumber_needOut, "$OUT[60] = FALSE");



		
		for (size_t j = 0; j < new_fileContent.size(); j++)
		{
			qDebug() << new_fileContent[j];
		}

		QString saveFile = srcSaveFileDir +"/gai_" + fileNameLast;
		QFile filename(saveFile);
		if (!filename.open(QIODevice::ReadWrite | QIODevice::Text))
		{
			QMessageBox::warning(this, "错误", "生成失败", QMessageBox::Ok);
			return;
		}
		else
		{
			QTextStream textStream(&filename);
			for (size_t i = 0; i < new_fileContent.size(); i++)
			{
				textStream << new_fileContent[i]<<"\n";
			}
		}
		filename.close();

		ui.tableWidget_outSrcFiles->item(ui.tableWidget_srcFiles->selectedItems().at(i)->row(), 0)->setText(saveFile);


	}
	QMessageBox::information(this, "保存文件", "保存成功", QMessageBox::Ok);
}

void OfflineProgramTool::SetSRCSaveFiles()
{
	QString str = QFileDialog::getExistingDirectory(this, "Save SRC file", "D:/");
	if (str == "")
	{
		return;
	}
	srcSaveFileDir = str;
	ui.lineEdit_SRCSavePosition->setText(srcSaveFileDir);
}

void OfflineProgramTool::OpenSRCFiles()
{
	if (srcSaveFileDir == "")
	{
		QMessageBox::warning(this, "错误", "未生成文件", QMessageBox::Ok);
		return;
	}
	QDesktopServices::openUrl(QUrl::fromLocalFile(srcSaveFileDir));
}

void OfflineProgramTool::DeleteRow()
{
	for (size_t i = 0; i < ui.tableWidget_srcFiles->selectedItems().size(); i++)
	{
		ui.tableWidget_srcFiles->removeRow(ui.tableWidget_srcFiles->selectedItems().at(i)->row());
	}
	
}

void OfflineProgramTool::AddSRCFiles()
{
	QStringList str = QFileDialog::getOpenFileNames(this, "选择SRC文件", "D:/", "(*.src)");
	if (str.size() == 0)
	{
		QMessageBox::warning(this, "错误", "文件打开失败", QMessageBox::Ok);
		return;
	}
	ui.tableWidget_srcFiles->setColumnCount(1);
	int oriSize = ui.tableWidget_srcFiles->rowCount();
	ui.tableWidget_srcFiles->setRowCount(str.size() + ui.tableWidget_srcFiles->rowCount());


	for (size_t i = 0; i < str.size(); i++)
	{
		ui.tableWidget_srcFiles->setItem(i+oriSize, 0, new QTableWidgetItem(str.at(i)));
	}

}

void OfflineProgramTool::CheckVersionUpdate()
{
	QNetworkRequest quest;
	quest.setUrl(QUrl("https://raw.githubusercontent.com/junlin49/OfflineProgramTool/main/update.json")); //包含最新版本软件的下载地址
	QString agent = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.132 Safari/537.36";
	quest.setHeader(QNetworkRequest::UserAgentHeader, agent);
	manager->get(quest);    //发送get网络请求

}

void OfflineProgramTool::ReplyFinished(QNetworkReply* reply)
{
	QString str = reply->readAll();//读取接收到的数据
	qDebug() << str;

	//json数据解析
	Json::Reader reader;
	Json::Value root;
	VersionUpdateInfor infor;
	if (reader.parse(str.toStdString(), root))
	{
		infor.versionNumber = root["LatestVerison"].asString();
		infor.url = root["Url"].asString();
		infor.updateTime = root["UpdateTime"].asString();
		infor.releaseNote = root["ReleaseNote"].asString();


		if (infor.versionNumber > versionNumber.toStdString())
		{
			std::string updateText = "检测到新版本！\n" + infor.versionNumber + "\n" + "更新时间：" + infor.updateTime + "\n" + "更新说明：" + infor.releaseNote;
			int result = QMessageBox::warning(this, "检查更新", QString::fromStdString(updateText),"下载","不更新");
			if (result == 0)
			{
				QDesktopServices::openUrl(QUrl(QString::fromStdString(infor.url)));
			}
		}
		else
		{
			QMessageBox::information(this, "检查更新", "当前已经是最新版本!");
		}
	}
	else
	{
		QMessageBox::information(this, "检查更新", "更新失败!");
	}
	reply->deleteLater();
}

void OfflineProgramTool::InputSafeCheck()
{
	QString str = "background-color: rgb(255, 255, 0)";

	if (ui.lineEdit_A_X->text().toDouble() != 0)
	{
		ui.lineEdit_A_X->setStyleSheet(str);
	}
	if (ui.lineEdit_A_Y->text().toDouble() != 0)
	{
		ui.lineEdit_A_Y->setStyleSheet(str);
	}
	if (ui.lineEdit_A_Z->text().toDouble() != 0)
	{
		ui.lineEdit_A_Z->setStyleSheet(str);
	}
	if (ui.lineEdit_A_A->text().toDouble() != 0)
	{
		ui.lineEdit_A_A->setStyleSheet(str);
	}
	if (ui.lineEdit_A_B->text().toDouble() != 0)
	{
		ui.lineEdit_A_B->setStyleSheet(str);
	}
	if (ui.lineEdit_A_C->text().toDouble() != 0)
	{
		ui.lineEdit_A_C->setStyleSheet(str);
	}
	if (ui.lineEdit_B_X->text().toDouble() != 0)
	{
		ui.lineEdit_B_X->setStyleSheet(str);
	}
	if (ui.lineEdit_B_Y->text().toDouble() != 0)
	{
		ui.lineEdit_B_Y->setStyleSheet(str);
	}				
	if (ui.lineEdit_B_Z->text().toDouble() != 0)
	{
		ui.lineEdit_B_Z->setStyleSheet(str);
	}
	if (ui.lineEdit_B_A->text().toDouble() != 0)
	{
		ui.lineEdit_B_A->setStyleSheet(str);
	}
	if (ui.lineEdit_B_B->text().toDouble() != 0)
	{
		ui.lineEdit_B_B->setStyleSheet(str);
	}				
	if (ui.lineEdit_B_C->text().toDouble() != 0)
	{
		ui.lineEdit_B_C->setStyleSheet(str);
	}

	if (ui.lineEdit_angle_1->text().toDouble() != 0)
	{
		ui.lineEdit_angle_1->setStyleSheet(str);
	}
	if (ui.lineEdit_angle_2->text().toDouble() != 0)
	{
		ui.lineEdit_angle_2->setStyleSheet(str);
	}
	if (ui.lineEdit_angle_3->text().toDouble() != 0)
	{
		ui.lineEdit_angle_3->setStyleSheet(str);
	}


}