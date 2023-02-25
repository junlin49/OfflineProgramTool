#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OfflineProgramTool.h"
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <QFiledialog.h>
#include <QClipboard.h>
#include <qdebug.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <qfile.h>
#include <qtablewidget.h>
#include "CustomTypes.h"
#include <QMessageBox>
#include <QScrollBar.h>
#include <qscreen.h>
#include <qdesktopservices.h>
#include <qdockwidget.h>
#include <qtextbrowser.h>
#include <qlayout.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "json_tool.h"
#include <json/json.h>

#pragma execution_character_set("utf-8")

class OfflineProgramTool : public QMainWindow
{
    Q_OBJECT

public:
    OfflineProgramTool(QWidget *parent = nullptr);
    ~OfflineProgramTool();
    void ReadOldNCFile();
    void GenerateNewNC();
    void NewNCTurnLeft();
    XYZABC W_T2xyzabc(Eigen::Matrix4d T);
    Eigen::Matrix4d W_RPY(XYZABC v);
    void SaveNewNCFile();
    void LeftScroll();
    void RightScroll();
    void Choose();
    void ChooseInverse();
    void OfflineProgramToolModel();
    void OtherModel();
    void About();
    void ReadOldDATFile();

    void ImportMoreFiles_SRC();
    void GenerateNewSRC();
    void SetSRCSaveFiles();
    void OpenSRCFiles();
    void DeleteRow();
    void AddSRCFiles();

    void CheckVersionUpdate();

public slots:
    void ReplyFinished(QNetworkReply* reply);

    void InputSafeCheck();

private:
    Ui::OfflineProgramToolClass ui;
    std::vector<QString> oldNCfileContent;
    std::vector<QString> oldDATfileContent;
    OpenFileMode openFileMode;
    QString versionNumber;
    QString openNCfileName, openDatfileName, saveNCfileName, saveDatfileName;
    QString srcSaveFileDir;
    bool isDockWidgetVisual = false;

    QNetworkAccessManager* manager;		//定义网络请求对象

};
