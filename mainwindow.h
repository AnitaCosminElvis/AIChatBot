#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <Qtimer>
#include <QSettings>
#include <QStringList>
#include <Qlist>
#include <QMap>
#include <QTime>
#include <QCryptographicHash>
#include <QChar>
#include "aimlparser.h"
#include "qstartdialog.h"

#define TYPE_HERE_TEXT  "Type Here ..."
#define AIML_DIRECTORY  "AIML"
#define LEGAL_AGE       18

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool Initialize();

protected:
    void showEvent(QShowEvent* pEvent);
    void closeEvent(QCloseEvent* pEvent);
public slots:
    void OnTimeoutSlot();
    void OnCloseSlot();
    void OnInsertMessageSlot();
    void OnFinishedGettingDataSlot(QString qsName, qint32 nAge, QString qsOccupation);

private:
    void WriteMessage(QString);
    void PushMessage(QString);
    void SaveChatHistory();
    void LoadChatHistory();

private:
    Ui::MainWindow *        ui;
    QStartDialog*           m_pQStartDialog;
    AIMLParser*             m_pAimlParser;
    QRegExp*                m_pQRegExp;
    QTimer                  m_QTimer;
    QString                 m_qsName;
    QString                 m_qsOccupation;
    qint32                  m_nAge;
    bool                    m_bIsInitialized;
    QMap<QString,QString>   m_qChatHistoryMap;

};

#endif // MAINWINDOW_H
