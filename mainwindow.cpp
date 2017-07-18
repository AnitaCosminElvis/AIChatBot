#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pQStartDialog = NULL;
    m_pAimlParser = NULL;
    m_pQRegExp = NULL;
    m_bIsInitialized = false;
    connect(ui->btnSendMessage, SIGNAL(pressed()),this,SLOT(OnInsertMessageSlot()),Qt::QueuedConnection);

    m_QTimer.setInterval(60000);
    m_QTimer.setSingleShot(true);

    connect(&m_QTimer, SIGNAL(timeout()),this,SLOT(OnTimeoutSlot()),Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    m_qChatHistoryMap.clear();

    if (NULL != m_pQStartDialog)
    {
        m_pQStartDialog->close();
        delete m_pQStartDialog;
        m_pQStartDialog = NULL;
    }

    if (NULL != m_pAimlParser)
    {
        m_pAimlParser->clear();
        delete m_pAimlParser;
        m_pAimlParser = NULL;
    }

    if (NULL != m_pQRegExp)
    {
        delete m_pQRegExp;
        m_pQRegExp = NULL;
    }

    delete ui;
}

bool MainWindow::Initialize()
{
    bool bIsInitialized = true;

    m_pQStartDialog = new QStartDialog(this);

    if (NULL == m_pQStartDialog)
    {
        bIsInitialized = false;
        return bIsInitialized;
    }

    bIsInitialized = connect(m_pQStartDialog,   SIGNAL(FinishedSignal(QString,qint32,QString)),
                             this,              SLOT(OnFinishedGettingDataSlot(QString,qint32,QString)),Qt::QueuedConnection);

    if (false == bIsInitialized)
    {
        return bIsInitialized;
    }

    bIsInitialized = connect(m_pQStartDialog,   SIGNAL(CloseApplication()),
                             this,              SLOT(OnCloseSlot()),Qt::QueuedConnection);

    if (false == bIsInitialized)
    {
        return bIsInitialized;
    }


    m_pAimlParser = new AIMLParser;

    if (NULL == m_pAimlParser)
    {
        bIsInitialized = false;
        return bIsInitialized;
    }

    m_pQRegExp = new QRegExp("&lt;(/*)html:(.+)&gt;");

    if (NULL == m_pAimlParser)
    {
        bIsInitialized = false;
        return bIsInitialized;
    }

    return bIsInitialized;
}

void MainWindow::showEvent(QShowEvent* pEvent)
{
    QMainWindow::showEvent(pEvent);

    if (NULL != m_pQStartDialog)
    {
        m_pQStartDialog->show();
    }
}

void MainWindow::closeEvent(QCloseEvent* pEvent)
{
    SaveChatHistory();
    QMainWindow::closeEvent(pEvent);
}


void MainWindow::OnFinishedGettingDataSlot(QString qsName, qint32 nAge, QString qsOccupation)
{
    QString qsAgeDirectory;
    QString qsOccupationDirectory;
    QString qsDirPath;

    m_qsName = qsName;
    m_nAge = nAge;
    m_qsOccupation = qsOccupation;

    if (LEGAL_AGE <= m_nAge)
    {
        qsAgeDirectory = QString("ADULT");
    }else{
        qsAgeDirectory = QString("NONADULT");
    }

    qsDirPath = AIML_DIRECTORY  + QString("/") + qsAgeDirectory;


    if (false == m_pAimlParser->loadAIMLSet(qsDirPath))
    {
        return ;
    }

    PushMessage(QString("My Name is ") + m_qsName);
    //WriteMessage(QString("Are you really a ") + m_qsOccupation + QString ("?"));
    PushMessage(QString("my occupation is ") + m_qsOccupation);

    LoadChatHistory();

    m_bIsInitialized = true;

}

void MainWindow::SaveChatHistory()
{
    QString qsPathByUserData;
    qsPathByUserData = qApp->applicationDirPath() + QString("/UserData/") + m_qsName
                        + m_qsOccupation + QString::number(m_nAge);
    QSettings qSettings(qsPathByUserData,QSettings::IniFormat);

    QList<QString>qList =  m_qChatHistoryMap.keys();

    foreach (const QString &key, qList)
    {
        qSettings.setValue(key,m_qChatHistoryMap.value(key));
        qSettings.sync();
    }

}

void MainWindow::LoadChatHistory()
{
    QString qsPathByUserData;
    qsPathByUserData = qApp->applicationDirPath() + QString("/UserData/") + m_qsName
            + m_qsOccupation + QString::number(m_nAge);
    QSettings qSettings(qsPathByUserData,QSettings::IniFormat);

    QStringList qsList = qSettings.allKeys();

    foreach (const QString &key, qsList)
    {
        m_qChatHistoryMap.insert(key,qSettings.value(key).toString());
    }
}

void MainWindow::OnTimeoutSlot()
{
    qint64 randomNumber;

    qsrand((uint)QTime::currentTime().msec());

    randomNumber = qrand() % (m_qChatHistoryMap.size() + 1);

    QList<QString>qList =  m_qChatHistoryMap.keys();

    for (int i = 0; i < qList.size(); i++)
    {
        if (i == randomNumber)
        {
            PushMessage(m_qChatHistoryMap.value(qList.at(i)));
        }
    }

}

void MainWindow::OnCloseSlot()
{
    this->close();
}

void MainWindow::WriteMessage(QString qsWriteInput)
{
    ui->te_chat->setTextColor(Qt::blue);
    ui->te_chat->insertPlainText("Bot> ");
    ui->te_chat->setTextColor(Qt::black);
    QString qsHtml = Qt::convertFromPlainText(qsWriteInput);
    qsHtml.replace(*m_pQRegExp, "<\\1\\2>"),
    ui->te_chat->insertHtml(qsHtml + "<br>");
    ui->te_chat->ensureCursorVisible();
}

void MainWindow::PushMessage(QString qsPushInput)
{
    //insert the bot's text
    QString qsBotResponse = m_pAimlParser->getResponse(qsPushInput);
    ui->te_chat->setTextColor(Qt::blue);
    ui->te_chat->insertPlainText("Bot> ");
    ui->te_chat->setTextColor(Qt::black);
    QString qsHtml = Qt::convertFromPlainText(qsBotResponse);
    qsHtml.replace(*m_pQRegExp, "<\\1\\2>"),
    ui->te_chat->insertHtml(qsHtml + "<br>");
    ui->te_chat->ensureCursorVisible();
}

void MainWindow::OnInsertMessageSlot()
{
    if ((NULL == m_pAimlParser) || (false == m_bIsInitialized))
    {
        return;
    }

    if (true == ui->te_userInput->toPlainText().isEmpty())
    {
        return;
    }

    QString qsUserInput =  ui->te_userInput->toPlainText();

    if (true == qsUserInput.trimmed().isEmpty())
    {
        return;
    }

    QTextCursor qTextCursor = ui->te_chat->textCursor();
    qTextCursor.movePosition(QTextCursor::End);
    ui->te_chat->setTextCursor(qTextCursor);

    //insert the user's text
    ui->te_chat->setTextColor(Qt::black);
    ui->te_chat->insertPlainText("You> ");
    ui->te_chat->insertPlainText(qsUserInput + "\n");


    //insert the bot's text
    QString qsBotResponse = m_pAimlParser->getResponse(qsUserInput);
    ui->te_chat->setTextColor(Qt::blue);
    ui->te_chat->insertPlainText("Bot> ");
    ui->te_chat->setTextColor(Qt::black);
    QString qsHtml = Qt::convertFromPlainText(qsBotResponse);
    qsHtml.replace(*m_pQRegExp, "<\\1\\2>"),
    ui->te_chat->insertHtml(qsHtml + "<br>");
    ui->te_chat->ensureCursorVisible();

    ui->te_userInput->clear();

    QCryptographicHash md5Gen(QCryptographicHash::Md5);
    md5Gen.addData(qsUserInput.toLocal8Bit());

    QCryptographicHash md5Gen1(QCryptographicHash::Md5);
    md5Gen1.addData(qsBotResponse.toLocal8Bit());

    m_qChatHistoryMap.insert(QString(md5Gen.result().toHex()),qsUserInput);
    m_qChatHistoryMap.insert(QString(md5Gen1.result().toHex()),qsBotResponse);

    m_QTimer.start();
}
