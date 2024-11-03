#include "databasecontroller.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>


DatabaseController::DatabaseController(QObject *parent) : QObject{parent}
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(dbPath); // Replace with the actual path to your SQLite database file

    // Try to open the database
    if (!db.open()) {
        qDebug() << "Error: Failed to open database";
        qDebug() << "Database connection error: " << db.lastError().text();
        // Handle the error appropriately
    } else {
        qDebug() << "Database opened successfully";
        // Perform database operations here
        // ...
        // Close the database connection when done
    }

    db.exec("PRAGMA foreign_keys = ON;");

    QVector<QString> tables = {"Generals", "General", "Outputs", "Line_A", "Line_B", "Last_Run", "Motors", "Cams",
                               "Color_Group", "Colors", "Fruits", "Confidences", "Report", "Users", "Ejector_A", "Ejector_B"};

    for(auto &table:tables)
        getColumnNames(table);
}

DatabaseController::~DatabaseController()
{
    db.close();
}

void DatabaseController::getColumnNames(QString tableName){
    if(tableName == "Generals") {
        QSqlRecord record = db.record(tableName);
        m_general.clear();
        _generalCols.clear();

        for (int i = 0; i < record.count(); ++i) {
            _generalCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _generalCols.size(); ++i) {
            m_general.push_back("");
        }
    }
    else if(tableName == "General") {
        QSqlRecord record = db.record(tableName);
        m_gen.clear();
        _genCols.clear();

        for (int i = 0; i < record.count(); ++i) {
            _genCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _genCols.size(); ++i) {
            m_gen.push_back("");
        }
    }
    else if(tableName == "Outputs") {
        QSqlRecord record = db.record(tableName);
        m_output.clear();
        _outputCols.clear();

        for (int i = 1; i < record.count(); ++i) {
            _outputCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _outputCols.size(); ++i) {
            m_output.push_back("");
        }
    }
    else if(tableName == "Line_A") {
        QSqlRecord record = db.record(tableName);
        m_lnaSetting.clear();
        lnaCols.clear();

        for (int i = 1; i < record.count(); ++i) {
            lnaCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < lnaCols.size(); ++i) {
            m_lnaSetting.push_back("");
        }
    }
    else if(tableName == "Line_B") {
        QSqlRecord record = db.record(tableName);
        m_lnbSetting.clear();
        lnbCols.clear();

        for (int i = 1; i < record.count(); ++i) {
            lnbCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < lnbCols.size(); ++i) {
            m_lnbSetting.push_back("");
        }
    }
    else if(tableName == "Last_Run") {
        QSqlRecord record = db.record(tableName);
        m_lastRun.clear();
        lastRunCols.clear();

        for (int i = 1; i < record.count(); ++i) {
            lastRunCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < lastRunCols.size(); ++i) {
            m_lastRun.push_back("");
        }
    }
    else if(tableName == "Report") {
        QSqlRecord record = db.record(tableName);
        m_report.clear();
        reportCols.clear();

        for (int i = 1; i < record.count(); ++i) {
            reportCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < lastRunCols.size(); ++i) {
            m_report.push_back("");
        }
    }
    else if(tableName == "Cams") {
        QSqlRecord record = db.record(tableName);
        m_camSetting.clear();
        _camsCols.clear();

        for (int i = 1; i < record.count(); ++i) {
            _camsCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _camsCols.size(); ++i) {
            m_camSetting.push_back("");
        }

    }
    else if(tableName == "Color_Group") {
        QSqlRecord record = db.record(tableName);
        m_colorGroup.clear();
        _colorGroupCols.clear();

        for (int i = 0; i < record.count(); ++i) {
            _colorGroupCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _colorGroupCols.size(); ++i) {
            m_colorGroup.push_back("");
        }
    }
    else if(tableName == "Colors") {
        QSqlRecord record = db.record(tableName);
        m_color.clear();
        _colorCols.clear();

        for (int i = 1; i < record.count(); ++i) {
            _colorCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _colorCols.size(); ++i) {
            m_color.push_back("");
        }
    }
    else if(tableName == "Fruits") {
        QSqlRecord record = db.record(tableName);
        m_fruit.clear();
        _fruitCols.clear();

        for (int i = 0; i < record.count(); ++i) {
            _fruitCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _fruitCols.size(); ++i) {
            m_fruit.push_back("");
        }
    }
    else if(tableName == "Confidences") {
        QSqlRecord record = db.record(tableName);
        m_confidence.clear();
        _confidenceCols.clear();

        for (int i = 1; i < record.count(); ++i) {
            _confidenceCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _confidenceCols.size(); ++i) {
            m_confidence.push_back("");
        }
    }
    else if(tableName == "Motors") {
        QSqlRecord record = db.record(tableName);
        m_motor.clear();
        _motorCols.clear();

        for (int i = 0; i < record.count(); ++i) {
            _motorCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _motorCols.size(); ++i) {
            m_motor.push_back("");
        }
    }
    else if(tableName == "Users") {
        QSqlRecord record = db.record(tableName);
        m_users.clear();
        _usersCols.clear();

        for (int i = 0; i < record.count(); ++i) {
            _usersCols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _usersCols.size(); ++i) {
            m_users.push_back("");
        }
    }
    else if(tableName == "Ejector_A") {
        QSqlRecord record = db.record(tableName);
        m_ejectorA.clear();
        _ejectorA_Cols.clear();

        for (int i = 0; i < record.count(); ++i) {
            _ejectorA_Cols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _ejectorA_Cols.size(); ++i) {
            m_ejectorA.push_back("");
        }
    }
    else if(tableName == "Ejector_B") {
        QSqlRecord record = db.record(tableName);
        m_ejectorB.clear();
        _ejectorB_Cols.clear();

        for (int i = 0; i < record.count(); ++i) {
            _ejectorB_Cols.push_back(record.fieldName(i));
        }

        for (int i = 0; i < _ejectorB_Cols.size(); ++i) {
            m_ejectorB.push_back("");
        }
    }

}

QVector<QString> DatabaseController::getGeneralExfs() {
    QVector<QString> exfList;
    QSqlQuery query;
    QString queryStr = "SELECT DISTINCT ExitFeature FROM Generals ORDER BY ROWID";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        while (query.next()) {
            exfList.push_back(query.value(0).toString());
        }
    }

    return exfList;
}

QVector<QString> &DatabaseController::getCols(QString tableName) {
    if(tableName == "Generals") {
        return _generalCols;
    }
    else if(tableName == "Outputs") {
        return _outputCols;
    }
    else if(tableName == "Line_A") {
        return lnaCols;
    }
    else if(tableName == "Line_B") {
        return lnbCols;
    }
    else if(tableName == "Last_Run") {
        return lastRunCols;
    }
    else if(tableName == "Report") {
        return reportCols;
    }
    else if(tableName == "Cams") {
        return _camsCols;
    }
    else if(tableName == "Colors") {
        return _colorCols;
    }
    else if(tableName == "Color_Group") {
        return _colorGroupCols;
    }
    else if(tableName == "Fruits") {
        return _fruitCols;
    }
    else if(tableName == "Confidences") {
        return _confidenceCols;
    }
    else if(tableName == "Report") {
        return reportCols;
    }
    else if(tableName == "Motors") {
        return _motorCols;
    }
    else if(tableName == "Users") {
        return _usersCols;
    }
    else if(tableName == "Ejector_A") {
        return _ejectorA_Cols;
    }
    else if(tableName == "Ejector_B") {
        return _ejectorB_Cols;
    }
}

QVector<QString> &DatabaseController::getVec(QString tableName) {
    if(tableName == "Generals") {
        return m_general;
    }
    else if(tableName == "General") {
        return m_gen;
    }
    else if(tableName == "Outputs") {
        return m_output;
    }
    else if(tableName == "Line_A") {
        return m_lnaSetting;
    }
    else if(tableName == "Line_B") {
        return m_lnbSetting;
    }
    else if(tableName == "Last_Run") {
        return m_lastRun;
    }
    else if(tableName == "Report") {
        return m_report;
    }
    else if(tableName == "Cams") {
        return m_camSetting;
    }
    else if(tableName == "Colors") {
        // qDebug() << "_________" << m_color;
        return m_color;
    }
    else if(tableName == "Color_Group") {
        return m_colorGroup;
    }
    else if(tableName == "Fruits") {
        return m_fruit;
    }
    else if(tableName == "Confidences") {
        return m_confidence;
    }
    else if(tableName == "Report") {
        return m_report;
    }
    else if(tableName == "Motors") {
        return m_motor;
    }
    else if(tableName == "Users") {
        return m_users;
    }
    else if(tableName == "Ejector_A") {
        return m_ejectorA;
    }
    else if(tableName == "Ejector_B") {
        return m_ejectorB;
    }
}

void DatabaseController::setIdx(QString tableName, QString setting, int idx) {
    QVector<QString> &vec = getVec(tableName);

    // if(tableName == "Colors")
    //     qDebug() << vec << "_________" << idx << "___________" << setting;
    vec[idx] = setting;


}

void DatabaseController::insert(QString tableName) {
    QSqlQuery query;
    QString columns = "";
    QString values = "";

    QVector<QString> &cols = getCols(tableName);
    QVector<QString> &setting = getVec(tableName);

    for (int i = 0; i < cols.size(); i++) {
        if(i != cols.size() - 1){
            columns += cols[i] + " ,";
            values += " '"+ setting[i] +"' ,";
        }
        else{
            columns += cols[i];
            values += " '"+ setting[i] +"'";
        }
    }

    QString queryStr = "INSERT INTO '" + tableName +"' (" + columns + ") VALUES (" + values + ")";
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to insert record to '" + tableName +"'";
    } else {
        qDebug() << "Record inserted successfully to '" + tableName +"'";
    }
}

void DatabaseController::insertCams()
{
    QSqlQuery query;

    QString queryStr = "INSERT INTO Cams (Serial, Ip, Gateway) VALUES ('"+m_camSetting[0]+"', '"+m_camSetting[1]+"', '"+m_camSetting[2]+"')";
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to insert record";
    } else {
        qDebug() << "Record inserted successfully";
    }
}

void DatabaseController::readGenerals(QString exf) {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Generals WHERE ExitFeature= '" + exf +"' ";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        if (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 0; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }


    if(!setting.isEmpty())
        setGeneral(setting); //Change
    else {
        for (int i = 0; i < _generalCols.size(); ++i) { //Change Cols
            setting.push_back("");
        }
        setGeneral(setting);// Change
    }
}

void DatabaseController::readGeneral() {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM General";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        if (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 0; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }


    if(!setting.isEmpty())
        setGen(setting); //Change
    else {
        for (int i = 0; i < _genCols.size(); ++i) { //Change Cols
            setting.push_back("");
        }
        setGen(setting);// Change
    }
}

void DatabaseController::readEjectorA() {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Ejector_A";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        if (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 0; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }

    if(!setting.isEmpty())
        setEjectorA(setting); //Change
    else {
        for (int i = 0; i < _ejectorA_Cols.size(); ++i) { //Change Cols
            setting.push_back("");
        }
        setEjectorA(setting);// Change
    }
}

void DatabaseController::readEjectorB() {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Ejector_B";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        if (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 0; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }

    if(!setting.isEmpty())
        setEjectorB(setting); //Change
    else {
        for (int i = 0; i < _ejectorB_Cols.size(); ++i) { //Change Cols
            setting.push_back("");
        }
        setEjectorB(setting);// Change
    }
}

void DatabaseController::updateEjectorA() {
    QSqlQuery query;

    QString update = "";

    for (int i = 0; i < _ejectorA_Cols.size(); ++i) {//Change
        if(i != _ejectorA_Cols.size()-1) {//Change
            update += _ejectorA_Cols[i] + "= " + "'"+ m_ejectorA[i] +"', ";//Change
        } else {
            update += _ejectorA_Cols[i] + "= " + "'"+ m_ejectorA[i] +"' ";//Change
        }
    }

    QString queryStr = "UPDATE Ejector_A SET " + update + "";//Change

    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        db.commit();
        qDebug() << "Record updated successfully";
    }
}

void DatabaseController::updateEjectorB() {
    QSqlQuery query;

    QString update = "";

    for (int i = 0; i < _ejectorB_Cols.size(); ++i) {//Change
        if(i != _ejectorB_Cols.size()-1) {//Change
            update += _ejectorB_Cols[i] + "= " + "'"+ m_ejectorB[i] +"', ";//Change
        } else {
            update += _ejectorB_Cols[i] + "= " + "'"+ m_ejectorB[i] +"' ";//Change
        }
    }

    QString queryStr = "UPDATE Ejector_B SET " + update + "";//Change

    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        db.commit();
        qDebug() << "Record updated successfully";
    }
}

void DatabaseController::updateGeneral() {
    QSqlQuery query;

    QString update = "";

    for (int i = 0; i < _genCols.size(); ++i) {//Change
        if(i != _genCols.size()-1) {//Change
            update += _genCols[i] + "= " + "'"+ m_gen[i] +"', ";//Change
        } else {
            update += _genCols[i] + "= " + "'"+ m_gen[i] +"' ";//Change
        }
    }

    QString queryStr = "UPDATE General SET " + update + "";//Change

    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        db.commit();
        qDebug() << "Record updated successfully";
    }
}

void DatabaseController::updateGenerals(QString exf) {
    QSqlQuery query(db);

    QString update = "";

    db.transaction();

    for (int i = 0; i < _generalCols.size(); ++i) {//Change
        if(i != _generalCols.size()-1) {//Change
            update += _generalCols[i] + "= " + "'"+ m_general[i] +"', ";//Change
        } else {
            update += _generalCols[i] + "= " + "'"+ m_general[i] +"' ";//Change
        }
    }

    QString queryStr = "UPDATE Generals SET " + update + " WHERE ExitFeature= '"+ exf +"'";//Change

    qDebug() << queryStr;
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        db.commit();
        qDebug() << "Record updated successfully";
    }
}

void DatabaseController::deleteGenerals(QString exf){
    QSqlQuery query;
    QString queryStr;

    queryStr = "DELETE FROM Generals WHERE ExitFeature= '"+ exf +"'";//Change
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        qDebug() << "Row(s) deleted successfully";
    }
}

void DatabaseController::deleteOutputs(QString exf, QString ruleID){
    QSqlQuery query;
    QString queryStr;

    queryStr = "DELETE FROM Outputs WHERE ExitFeature= '"+ exf +"' AND RuleID= '"+ ruleID +"'";//Change
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        qDebug() << "Row(s) deleted successfully";
    }
}

void DatabaseController::updateOutputs(QString exf, QString ruleID) {
    QSqlQuery query;

    QString update = "";

    for (int i = 1; i < _outputCols.size(); ++i) {
        if(i != _outputCols.size()-1) {
            update += _outputCols[i] + "= " + "'"+ m_output[i] +"', ";
        } else {
            update += _outputCols[i] + "= " + "'"+ m_output[i] +"' ";
        }
    }

    QString queryStr = "UPDATE Outputs SET " + update + " WHERE ExitFeature = '" + exf +"' AND RuleID = '" + ruleID +"'";

    query.prepare(queryStr);

    qDebug() << "Update outputs__________________________________________________";

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        qDebug() << "Record updated successfully";
    }
}

void DatabaseController::readOutput(QString exf, QString ruleID) {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Outputs WHERE ExitFeature = '" + exf +"' AND RuleID = '" + ruleID +"'";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        while (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 1; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }
    if(!setting.isEmpty())
        setOutput(setting);
    else {
        for (int i = 0; i < _outputCols.size(); ++i) {
            setting.push_back("");
        }
        setOutput(setting);
    }
}

QVector<QVector<QString>> DatabaseController::readOutputs(QString exf) {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Outputs WHERE ExitFeature = '" + exf +"'";
    query.prepare(queryStr);

    QVector<QVector<QString>> res;

    QVector<QString> output;
    QVector<QString> wLow;
    QVector<QString> wHigh;
    QVector<QString> sizes;
    QVector<QString> enables;
    QVector<QString> defectEnables;
    QVector<QString> colorRanges;
    QVector<QString> colorCondition;
    QVector<QString> outputsUsed;

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        while (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 2; i < numColumns; i++) {
                if(i == 2) output.push_back(query.value(i).toString());
                else if(i == 3) wLow.push_back(query.value(i).toString());
                else if(i == 4) wHigh.push_back(query.value(i).toString());
                else if(i == 5) sizes.push_back(query.value(i).toString());
                else if(i == 6) enables.push_back(query.value(i).toString());
                else if(i == 7) defectEnables.push_back(query.value(i).toString());
                else if(i == 8) colorRanges.push_back(query.value(i).toString());
                else if(i == 9) colorCondition.push_back(query.value(i).toString());
                else if(i == 10) outputsUsed.push_back(query.value(i).toString());
            }
        }
    }

    //qDebug() << "_______" << sizes;


    res.push_back(output);
    res.push_back(wLow);
    res.push_back(wHigh);
    res.push_back(sizes);
    res.push_back(enables);
    res.push_back(defectEnables);
    res.push_back(colorRanges);
    res.push_back(colorCondition);
    res.push_back(outputsUsed);

    return res;
}

void DatabaseController::readLna(QString speed) {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Line_A WHERE speed= '" + speed +"' ";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        if (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 1; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }

    if(!setting.isEmpty())
        setLnaSetting(setting);
    else {
        for (int i = 0; i < lnaCols.size(); ++i) {
            setting.push_back("");
        }
        setLnaSetting(setting);
    }
}

void DatabaseController::updateLna(QString speed) {
    QSqlQuery query;

    QString update = "";

    for (int i = 1; i < lnaCols.size(); ++i) {
        if(i != lnaCols.size()-1) {
            update += lnaCols[i] + "= " + "'"+ m_lnaSetting[i] +"', ";
        } else {
            update += lnaCols[i] + "= " + "'"+ m_lnaSetting[i] +"' ";
        }
    }

    QString queryStr = "UPDATE Line_A SET " + update + " WHERE speed= '"+ speed +"'";

    qDebug() << queryStr;
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        qDebug() << "Record updated successfully";
    }
}

void DatabaseController::readLnb(QString speed) {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Line_B WHERE speed= '" + speed +"' ";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        if (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 1; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }
    if(!setting.isEmpty())
        setLnbSetting(setting);
    else {
        for (int i = 0; i < lnbCols.size(); ++i) {
            setting.push_back("");
        }
        setLnbSetting(setting);
    }
}

void DatabaseController::updateLnb(QString speed) {
    QSqlQuery query;

    QString update = "";

    for (int i = 1; i < lnbCols.size(); ++i) {
        if(i != lnbCols.size()-1) {
            update += lnbCols[i] + "= " + "'"+ m_lnbSetting[i] +"', ";
        } else {
            update += lnbCols[i] + "= " + "'"+ m_lnbSetting[i] +"' ";
        }
    }

    QString queryStr = "UPDATE Line_B SET " + update + " WHERE speed= '"+ speed +"'";

    qDebug() << queryStr;
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        qDebug() << "Record updated successfully";
    }
}

void DatabaseController::readLastRun() {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Last_Run";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        if (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 1; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }
    if(!setting.isEmpty())
        setLastRun(setting);
    else {
        for (int i = 0; i < lnbCols.size(); ++i) {
            setting.push_back("");
        }
        setLastRun(setting);
    }
}

void DatabaseController::updateLastRun() {
    QSqlQuery query;

    QString update = "";

    for (int i = 0; i < lastRunCols.size(); ++i) {
        if(i != lastRunCols.size()-1) {
            update += lastRunCols[i] + "= " + "'"+ m_lastRun[i] +"', ";
        } else {
            update += lastRunCols[i] + "= " + "'"+ m_lastRun[i] +"' ";
        }
    }

    QString queryStr = "UPDATE Last_Run SET " + update + "";

    //qDebug() << queryStr;
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        qDebug() << "Record updated successfully";
    }
}

void DatabaseController::readCams(QString serial) {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Cams WHERE Serial= '" + serial +"' ";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        if (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 1; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }

    //qDebug() << "SSSS" << m_camSetting;

    if(!setting.isEmpty())
        setCamSetting(setting);

    //Q_EMIT camSettingChanged();
}

void DatabaseController::updateCams(QString serial)
{
    QSqlQuery query;

    QString update = "";

    for (int i = 1; i < _camsCols.size(); ++i) {
        if(i != _camsCols.size()-1) {
            update += _camsCols[i] + "= " + "'"+ m_camSetting[i] +"', ";
        } else {
            update += _camsCols[i] + "= " + "'"+ m_camSetting[i] +"' ";
        }
    }

    QString queryStr = "UPDATE Cams SET " + update + " WHERE Serial= '"+ serial +"'";
    query.prepare(queryStr);

    qDebug() << queryStr;

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        qDebug() << "Record updated successfully";
    }
}

QVector<QString> DatabaseController::getCameras() {
    QVector<QString> camList;
    QSqlQuery query;
    QString queryStr = "SELECT DISTINCT Serial FROM Cams ORDER BY ROWID";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        while (query.next()) {
            camList.push_back(query.value(0).toString());
        }
    }

    return camList;
}

QString DatabaseController::getIp(QString serial) {
    QString ip;
    QSqlQuery query;
    QString queryStr = "SELECT Ip FROM Cams Where Serial='"+serial+"'";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        while (query.next()) {
            ip = query.value(0).toString();
        }
    }

    return ip;
}

QString DatabaseController::getGw(QString serial) {
    QString gw;
    QSqlQuery query;
    QString queryStr = "SELECT Gateway FROM Cams Where Serial='"+serial+"'";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        while (query.next()) {
            gw = query.value(0).toString();
        }
    }

    return gw;
}

QVector<QString> DatabaseController::getColorGroups() {
    QVector<QString> colorList;
    QSqlQuery query;
    QString queryStr = "SELECT DISTINCT Grp FROM Color_Group ORDER BY ROWID";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        while (query.next()) {
            colorList.push_back(query.value(0).toString());
        }
    }

    return colorList;
}

QVector<QString> DatabaseController::getColors(QString Group) {
    QVector<QString> colorList;
    QSqlQuery query;
    QString queryStr = "SELECT DISTINCT Color FROM Colors Where Grp='"+Group+"' ORDER BY ROWID";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        while (query.next()) {
            colorList.push_back(query.value(0).toString());
        }
    }

    return colorList;
}


void DatabaseController::deleteColorGroup(QString Group)
{
    QSqlQuery query;
    QString queryStr;

    queryStr = "DELETE FROM Color_Group WHERE Grp= '"+ Group +"'";//Change
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        qDebug() << "Row(s) deleted successfully";
    }
}

void DatabaseController::readColor(QString Group, QString Color) {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Colors WHERE Grp= '" + Group +"' AND Color='"+Color+"'";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        if (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 1; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }
    if(!setting.isEmpty())
        setColor(setting);

}

void DatabaseController::updateColorGroup(QString Group) {
    QSqlQuery query;

    QString update = "";

    for (int i = 0; i < _colorGroupCols.size(); ++i) {
        if(i != _colorGroupCols.size()-1) {
            update += _colorGroupCols[i] + "= " + "'"+ m_colorGroup[i] +"', ";
        } else {
            update += _colorGroupCols[i] + "= " + "'"+ m_colorGroup[i] +"' ";
        }
    }

    QString queryStr = "UPDATE Color_Group SET " + update + " WHERE Grp= '"+ Group +"'";
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        qDebug() << "Record updated successfully";
    }
}

void DatabaseController::updateColors(QString Group, QString Color) {
    QSqlQuery query;

    QString update = "";

    for (int i = 1; i < _colorCols.size(); ++i) {
        if(i != _colorCols.size()-1) {
            update += _colorCols[i] + "= " + "'"+ m_color[i] +"', ";
        } else {
            update += _colorCols[i] + "= " + "'"+ m_color[i] +"' ";
        }
    }

    QString queryStr = "UPDATE Colors SET " + update + " WHERE Grp= '"+ Group +"' AND Color='"+Color+"'";
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        qDebug() << "Record updated successfully";
    }
}


void DatabaseController::updateFruit(QString exf) {
    QSqlQuery query;

    QString queryStr = "UPDATE Fruits SET ExitFeature='"+ m_fruit[0] +"' WHERE ExitFeature= '"+ exf +"'";

    //    qDebug() << queryStr;
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        qDebug() << "Record updated successfully";
    }
}

void DatabaseController::deleteFruit(QString exf){
    QSqlQuery query;
    QString queryStr;

    queryStr = "DELETE FROM Fruits WHERE ExitFeature= '"+ exf +"'";
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        qDebug() << "Row(s) deleted successfully";
    }
}

void DatabaseController::updateConfidences(QString exf, QString className) {
    QSqlQuery query;

    QString update = "";

    for (int i = 1; i < _confidenceCols.size(); ++i) {
        if(i != _confidenceCols.size()-1) {
            update += _confidenceCols[i] + "= " + "'"+ m_confidence[i] +"', ";
        } else {
            update += _confidenceCols[i] + "= " + "'"+ m_confidence[i] +"' ";
        }
    }

    QString queryStr = "UPDATE Confidences SET " + update + " WHERE ExitFeature= '"+ exf +"' AND ClassName='"+className+"'";

    qDebug() << queryStr;
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        qDebug() << "Record updated successfully";
    }
}

QVector<QString> DatabaseController::getFruitExfs() {
    QVector<QString> exfs;
    QSqlQuery query;
    QString queryStr = "SELECT DISTINCT ExitFeature FROM Fruits ORDER BY ROWID";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        while (query.next()) {
            exfs.push_back(query.value(0).toString());
        }
    }

    return exfs;
}

QVector<QVector<QString>> DatabaseController::readConfidences(QString exf) {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Confidences WHERE ExitFeature = '" + exf +"'";
    query.prepare(queryStr);

    QVector<QVector<QString>> res;

    QVector<QString> classesName;
    QVector<QString> classesConf;
    QVector<QString> quantityCount;
    QVector<QString> quantityConf;

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        while (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 2; i < numColumns; i++) {
                if(i == 2) classesName.push_back(query.value(i).toString());
                else if(i == 3) classesConf.push_back(query.value(i).toString());
                else if(i == 4) quantityCount.push_back(query.value(i).toString());
                else if(i == 5) quantityConf.push_back(query.value(i).toString());
            }
        }
    }

    res.push_back(classesName);
    res.push_back(classesConf);
    res.push_back(quantityCount);
    res.push_back(quantityConf);

    return res;
}

void DatabaseController::readReport(QString start, QString stop) {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Report WHERE start_time >= '" + start +"' AND stop_time < '" + stop +"'";
    query.prepare(queryStr);

    //qDebug() << queryStr;

    m_reportData.clear();

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        while (query.next()) {
            QVector<QString> data;
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 1; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
                data.push_back(query.value(i).toString());
            }
            m_reportData.push_back(data);
        }
    }
    if(!setting.isEmpty())
        setReport(setting);
    else {
        for (int i = 0; i < reportCols.size(); ++i) {
            setting.push_back("");
        }
        setReport(setting);
    }
}

void DatabaseController::updateReport(QString start_time) {
    QSqlQuery query;

    QString update = "";

    for (int i = 1; i < reportCols.size(); ++i) {
        if(i != reportCols.size()-1) {
            update += reportCols[i] + "= " + "'"+ m_report[i] +"', ";
        } else {
            update += reportCols[i] + "= " + "'"+ m_report[i] +"' ";
        }
    }

    QString queryStr = "UPDATE Report SET " + update + " WHERE start_time= '"+ start_time +"'";

    //    qDebug() << queryStr;
    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        //        qDebug() << "Record updated successfully";
    }
}

void DatabaseController::readMotors() {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Motors";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        if (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 0; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }


    if(!setting.isEmpty())
        setMotor(setting); //Change
    else {
        for (int i = 0; i < _motorCols.size(); ++i) { //Change Cols
            setting.push_back("");
        }
        setMotor(setting);// Change
    }
}

void DatabaseController::readUsers() {
    QSqlQuery query;
    QString queryStr = "SELECT * FROM Users";
    query.prepare(queryStr);

    QVector<QString> setting;
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
    } else {
        if (query.next()) {
            QSqlRecord record = query.record();
            int numColumns = record.count();
            for (int i = 0; i < numColumns; i++) {
                setting.push_back(query.value(i).toString());
            }
        }
    }

    if(!setting.isEmpty())
        setUsers(setting); //Change
    else {
        for (int i = 0; i < _usersCols.size(); ++i) { //Change Cols
            setting.push_back("");
        }
        setUsers(setting);// Change
    }
}

void DatabaseController::updateMotors() {
    QSqlQuery query;

    QString update = "";

    for (int i = 0; i < _motorCols.size(); ++i) {
        if(i != _motorCols.size()-1) {
            update += _motorCols[i] + "= " + "'"+ m_motor[i] +"', ";
        } else {
            update += _motorCols[i] + "= " + "'"+ m_motor[i] +"' ";
        }
    }

    QString queryStr = "UPDATE Motors SET " + update + "";

    query.prepare(queryStr);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update record";
    } else {
        qDebug() << "Record updated successfully";
    }
}

const QVector<QString> &DatabaseController::lnaSetting() const
{
    return m_lnaSetting;
}

void DatabaseController::setLnaSetting(const QVector<QString> &newLnaSetting)
{
    if (m_lnaSetting == newLnaSetting)
        return;
    m_lnaSetting = newLnaSetting;
    Q_EMIT lnaSettingChanged();
}

const QVector<QString> &DatabaseController::lnbSetting() const
{
    return m_lnbSetting;
}

void DatabaseController::setLnbSetting(const QVector<QString> &newLnbSetting)
{
    if (m_lnbSetting == newLnbSetting)
        return;
    m_lnbSetting = newLnbSetting;
    Q_EMIT lnbSettingChanged();
}


const QVector<QString> &DatabaseController::lastRun() const
{
    return m_lastRun;
}

void DatabaseController::setLastRun(const QVector<QString> &newLastRun)
{
    if (m_lastRun == newLastRun)
        return;
    m_lastRun = newLastRun;
    Q_EMIT lastRunChanged();
}

const QVector<QString> &DatabaseController::exitFeatureList() const
{
    return m_exitFeatureList;
}

void DatabaseController::setExitFeatureList(const QVector<QString> &newExitFeatureList)
{
    if (m_exitFeatureList == newExitFeatureList)
        return;
    m_exitFeatureList = newExitFeatureList;
    Q_EMIT exitFeatureListChanged();
}

const QVector<QString> &DatabaseController::report() const
{
    return m_report;
}

void DatabaseController::setReport(const QVector<QString> &newReport)
{
    if (m_report == newReport)
        return;
    m_report = newReport;
    Q_EMIT reportChanged();
}


QVector<QString> DatabaseController::camSetting() const
{
    return m_camSetting;
}

void DatabaseController::setCamSetting(const QVector<QString> &newCamSetting)
{
    if (m_camSetting == newCamSetting)
        return;
    m_camSetting = newCamSetting;
    Q_EMIT camSettingChanged();
}

QVector<QString> DatabaseController::color() const
{
    return m_color;
}

void DatabaseController::setColor(const QVector<QString> &newColor)
{
    if (m_color == newColor)
        return;
    m_color = newColor;
    Q_EMIT colorChanged();
}

QVector<QString> DatabaseController::fruit() const
{
    return m_fruit;
}

void DatabaseController::setFruit(const QVector<QString> &newFruit)
{
    if (m_fruit == newFruit)
        return;
    m_fruit = newFruit;
    Q_EMIT fruitChanged();
}

QVector<QString> DatabaseController::confidence() const
{
    return m_confidence;
}

void DatabaseController::setConfidence(const QVector<QString> &newConfidence)
{
    if (m_confidence == newConfidence)
        return;
    m_confidence = newConfidence;
    Q_EMIT confidenceChanged();
}

QVector<QString> DatabaseController::general() const
{
    return m_general;
}

void DatabaseController::setGeneral(QVector<QString> newGeneral)
{
    m_general = newGeneral;
    Q_EMIT generalChanged();
}

QVector<QString> DatabaseController::output() const
{
    return m_output;
}

void DatabaseController::setOutput(const QVector<QString> &newOutput)
{
    if (m_output == newOutput)
        return;
    m_output = newOutput;
    Q_EMIT outputChanged();
}

QVector<QString> DatabaseController::colorGroup() const
{
    return m_colorGroup;
}

void DatabaseController::setColorGroup(const QVector<QString> &newColorGroup)
{
    if (m_colorGroup == newColorGroup)
        return;
    m_colorGroup = newColorGroup;
    Q_EMIT colorGroupChanged();
}

QVector<QString> DatabaseController::gen() const
{
    return m_gen;
}

void DatabaseController::setGen(const QVector<QString> &newGen)
{
    if (m_gen == newGen)
        return;
    m_gen = newGen;
    Q_EMIT genChanged();
}

QVector<QVector<QString> > DatabaseController::reportData() const
{
    return m_reportData;
}

void DatabaseController::setReportData(const QVector<QVector<QString> > &newReportData)
{
    if (m_reportData == newReportData)
        return;
    m_reportData = newReportData;
    Q_EMIT reportDataChanged();
}

QVector<QString> DatabaseController::motor() const
{
    return m_motor;
}

void DatabaseController::setMotor(const QVector<QString> &newMotor)
{
    if (m_motor == newMotor)
        return;
    m_motor = newMotor;
    Q_EMIT motorChanged();
}

QVector<QString> DatabaseController::users() const
{
    return m_users;
}

void DatabaseController::setUsers(const QVector<QString> &newUsers)
{
    if (m_users == newUsers)
        return;
    m_users = newUsers;
    Q_EMIT usersChanged();
}

QVector<QString> DatabaseController::ejectorA() const
{
    return m_ejectorA;
}

void DatabaseController::setEjectorA(const QVector<QString> &newEjectorA)
{
    if (m_ejectorA == newEjectorA)
        return;
    m_ejectorA = newEjectorA;
    Q_EMIT ejectorAChanged();
}

QVector<QString> DatabaseController::ejectorB() const
{
    return m_ejectorB;
}

void DatabaseController::setEjectorB(const QVector<QString> &newEjectorB)
{
    if (m_ejectorB == newEjectorB)
        return;
    m_ejectorB = newEjectorB;
    Q_EMIT ejectorBChanged();
}
