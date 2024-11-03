#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H
#include <QtConcurrent/QtConcurrent>
#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <vector>
#include <QMutex>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>


class DatabaseController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<QString> gen READ gen WRITE setGen NOTIFY genChanged)
    Q_PROPERTY(QVector<QString> general READ general WRITE setGeneral NOTIFY generalChanged)
    Q_PROPERTY(QVector<QString> output READ output WRITE setOutput NOTIFY outputChanged)
    Q_PROPERTY(QVector<QString> lnaSetting READ lnaSetting WRITE setLnaSetting NOTIFY lnaSettingChanged)
    Q_PROPERTY(QVector<QString> lnbSetting READ lnbSetting WRITE setLnbSetting NOTIFY lnbSettingChanged)
    Q_PROPERTY(QVector<QString> lastRun READ lastRun WRITE setLastRun NOTIFY lastRunChanged)
    Q_PROPERTY(QVector<QString> report READ report WRITE setReport NOTIFY reportChanged)
    Q_PROPERTY(QVector<QVector<QString>> reportData READ reportData WRITE setReportData NOTIFY reportDataChanged)
    Q_PROPERTY(QVector<QString> exitFeatureList READ exitFeatureList WRITE setExitFeatureList NOTIFY exitFeatureListChanged)
    Q_PROPERTY(QVector<QString> camSetting READ camSetting WRITE setCamSetting NOTIFY camSettingChanged)
    Q_PROPERTY(QVector<QString> color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QVector<QString> colorGroup READ colorGroup WRITE setColorGroup NOTIFY colorGroupChanged)
    Q_PROPERTY(QVector<QString> fruit READ fruit WRITE setFruit NOTIFY fruitChanged)
    Q_PROPERTY(QVector<QString> confidence READ confidence WRITE setConfidence NOTIFY confidenceChanged)
    Q_PROPERTY(QVector<QString> motor READ motor WRITE setMotor NOTIFY motorChanged)
    Q_PROPERTY(QVector<QString> users READ users WRITE setUsers NOTIFY usersChanged)
    Q_PROPERTY(QVector<QString> ejectorA READ ejectorA WRITE setEjectorA NOTIFY ejectorAChanged)
    Q_PROPERTY(QVector<QString> ejectorB READ ejectorB WRITE setEjectorB NOTIFY ejectorBChanged)
public:
    explicit DatabaseController(QObject *parent = nullptr);
    ~DatabaseController();

    QVector<QString> m_report;
    QVector<QVector<QString> > m_reportData;


    void insertReport();
    void updateReport(QString start_time);

    QVector<QString> &getCols(QString tableName);
    QVector<QString> &getVec(QString tableName);

    const QVector<QString> &generalSetting() const;
    void setGeneralSetting(const QVector<QString> &newGeneralSetting);

    const QVector<QString> &lnaSetting() const;
    void setLnaSetting(const QVector<QString> &newLnaSetting);

    const QVector<QString> &lnbSetting() const;
    void setLnbSetting(const QVector<QString> &newLnbSetting);

    void getColumnNames(QString tableName);
    const QVector<QString> &lastRun() const;
    void setLastRun(const QVector<QString> &newLastRun);

    const QVector<QString> &exitFeatureList() const;
    void setExitFeatureList(const QVector<QString> &newExitFeatureList);

    const QVector<QString> &report() const;
    void setReport(const QVector<QString> &newReport);

    QVector<QString> camSetting() const;
    void setCamSetting(const QVector<QString> &newCamSetting);

    QVector<QString> color() const;
    void setColor(const QVector<QString> &newColor);

    QVector<QString> fruit() const;
    void setFruit(const QVector<QString> &newFruit);

    QVector<QString> confidence() const;
    void setConfidence(const QVector<QString> &newConfidence);

    QVector<QString> general() const;
    void setGeneral(QVector<QString> newGeneral);

    QVector<QString> output() const;
    void setOutput(const QVector<QString> &newOutput);

    QVector<QString> colorGroup() const;
    void setColorGroup(const QVector<QString> &newColorGroup);

    QVector<QString> gen() const;
    void setGen(const QVector<QString> &newGen);

    QVector<QVector<QString> > reportData() const;
    void setReportData(const QVector<QVector<QString> > &newReportData);

    QVector<QString> motor() const;
    void setMotor(const QVector<QString> &newMotor);

    QVector<QString> users() const;
    void setUsers(const QVector<QString> &newUsers);

    QVector<QString> ejectorA() const;
    void setEjectorA(const QVector<QString> &newEjectorA);

    QVector<QString> ejectorB() const;
    void setEjectorB(const QVector<QString> &newEjectorB);

public Q_SLOTS:
    void updateColors(QString Group, QString Color);
    void updateLna(QString speed);
    void readLna(QString speed);
    void readLnb(QString speed);
    void updateLnb(QString speed);
    void readLastRun();
    void updateLastRun();
    void readReport(QString start, QString stop);
    void readCams(QString serial);
    void updateCams(QString serial);
    QVector<QString> getColorGroups();
    QVector<QString> getColors(QString Group);
    void readColor(QString Group, QString Color);
    QVector<QVector<QString> > readConfidences(QString exf);
    QVector<QString> getFruitExfs();
    void deleteFruit(QString exf);
    void updateFruit(QString exf);
    void updateConfidences(QString exf, QString className);
    QVector<QString> getGeneralExfs();
    void readGenerals(QString exf);
    void updateGenerals(QString exf);
    void deleteGenerals(QString exf);
    QVector<QVector<QString> > readOutputs(QString exf);
    void updateOutputs(QString exf, QString ruleID);
    void readOutput(QString exf, QString ruleID);
    void deleteColorGroup(QString Group);
    void insert(QString tableName);
    void setIdx(QString tableName, QString setting, int idx);
    void insertCams();
    QVector<QString> getCameras();
    void updateColorGroup(QString Group);
    QString getIp(QString serial);
    QString getGw(QString serial);
    void deleteOutputs(QString exf, QString ruleID);
    void updateGeneral();
    void readGeneral();
    void updateMotors();
    void readMotors();
    void readUsers();
    void readEjectorA();
    void readEjectorB();
    void updateEjectorA();
    void updateEjectorB();
private:
    QSqlDatabase db;
    QVector<QString> _genCols;
    QVector<QString> _generalCols;
    QVector<QString> _outputCols;
    QVector<QString> lnaCols;
    QVector<QString> lnbCols;
    QVector<QString> lastRunCols;
    QVector<QString> reportCols;
    QVector<QString> _camsCols;
    QVector<QString> _colorCols;
    QVector<QString> _colorGroupCols;
    QVector<QString> _fruitCols;
    QVector<QString> _confidenceCols;
    QVector<QString> _motorCols;
    QVector<QString> _usersCols;
    QVector<QString> _ejectorA_Cols;
    QVector<QString> _ejectorB_Cols;

    QString dbPath = "/home/perticon/Application/Database/kiwi_new.db";

    QVector<QString> m_lnaSetting;

    QVector<QString> m_lnbSetting;

    QVector<QString> m_lastRun;

    QVector<QString> m_exitFeatureList;

    QVector<QString> m_camSetting;

    QVector<QString> m_color;

    QVector<QString> m_fruit;

    QVector<QString> m_confidence;

    QVector<QString> m_general;

    QVector<QString> m_output;

    QVector<QString> m_colorGroup;

    QVector<QString> m_gen;

    QVector<QString> m_motor;

    QVector<QString> m_users;

    QVector<QString> m_ejectorA;

    QVector<QString> m_ejectorB;

Q_SIGNALS:
    void lnaSettingChanged();
    void lnbSettingChanged();
    void lastRunChanged();
    void exitFeatureListChanged();
    void reportChanged();
    void showReport();
    void camSettingChanged();
    void colorChanged();
    void fruitChanged();
    void confidenceChanged();
    void generalChanged();
    void outputChanged();
    void colorGroupChanged();
    void colorsChanged();
    void genChanged();
    void reportDataChanged();
    void motorChanged();
    void usersChanged();
    void ejectorAChanged();
    void ejectorBChanged();
};

#endif // DATABASECONTROLLER_H
