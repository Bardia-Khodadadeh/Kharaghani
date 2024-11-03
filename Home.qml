import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15
import Qt.labs.qmlmodels 1.0
import app.WeightController 1.0
import app.DatabaseController 1.0
import app.EthernetController 1.0
import app.CameraController 1.0
import app.CaptureViewPort 1.0
import QtQuick.Dialogs 1.0



Pane {
    id: mainItm

    property var defectChbValues: [false, false, false, false, false, false]
    property var colorRanges: []
    property var colorOperations: []
    property var densities: []
    property var sizes: []

    property bool colorPopupOpened: false
    property bool defectPopupOpened: false
    property bool densityPopupOpened: false
    property bool sizePopupOpened: false
    property bool programStarted: false

    property bool flagGrader: false
    property bool flagFeeding: false

    property var cmbsOpened: [false, false, false, false]

    property var cmbCompleted: [false, false, false, false]
    property var updates: [false, false, false]


    property string iconsPath: "file:////home/perticon/Application/Images/Icons/"
    property int  cmbIdx : 0

    ListModel { id: classesModel }
    ListModel { id: outputsModel }
    ListModel { id: defectsModel }

    DatabaseController{
        id: dbController

        onColorsChanged: {
            // color[1] = colorComboBox.currentValue


            if(color[1] === "Crop")
            {
                if(camController.cropHsv.length > 0)
                {
                    camController.cropHsv[0] = color[2]
                    camController.cropHsv[1] = color[3]
                    camController.cropHsv[2] = color[4]
                    camController.cropHsv[3] = color[5]
                    camController.cropHsv[4] = color[6]
                    camController.cropHsv[5] = color[7]
                    camController.cropHsv[6] = color[8]
                }
            }
            if(color[1] === "Red")
            {
                if(camController.rdHsv.length > 0)
                {
                    camController.rdHsv[0] = color[2]
                    camController.rdHsv[1] = color[3]
                    camController.rdHsv[2] = color[4]
                    camController.rdHsv[3] = color[5]
                    camController.rdHsv[4] = color[6]
                    camController.rdHsv[5] = color[7]
                    camController.rdHsv[6] = color[8]
                }
            }
            if(color[1] === "Orange")
            {
                if(camController.orHsv.length > 0)
                {
                    camController.orHsv[0] = color[2]
                    camController.orHsv[1] = color[3]
                    camController.orHsv[2] = color[4]
                    camController.orHsv[3] = color[5]
                    camController.orHsv[4] = color[6]
                    camController.orHsv[5] = color[7]
                    camController.orHsv[6] = color[8]
                }
            }
            if(color[1] === "Yellow")
            {
                if(camController.ylHsv.length > 0)
                {
                    camController.ylHsv[0] = color[2]
                    camController.ylHsv[1] = color[3]
                    camController.ylHsv[2] = color[4]
                    camController.ylHsv[3] = color[5]
                    camController.ylHsv[4] = color[6]
                    camController.ylHsv[5] = color[7]
                    camController.ylHsv[6] = color[8]
                }
            }
            if(color[1] === "Green")
            {
                if(camController.gnHsv.length > 0)
                {
                    camController.gnHsv[0] = color[2]
                    camController.gnHsv[1] = color[3]
                    camController.gnHsv[2] = color[4]
                    camController.gnHsv[3] = color[5]
                    camController.gnHsv[4] = color[6]
                    camController.gnHsv[5] = color[7]
                    camController.gnHsv[6] = color[8]
                }
            }
        }

    }

    EthernetController{
        id: ethController

        onEncoderChanged: {
            // camController.setEncoder(encoder)
            // weightController.setEncoder(encoder)
        }

        onConnectionStatusChanged: {
            0 + ethController.connectionStatus === 0 ? ethStatus.source = "qrc:/Icons/eth.png" : ethStatus.source = "qrc:/Icons/eth_dis.png"
        }

        onGraderStatusChanged: {
            switch(0 + graderStatus)
            {
            case 0:
                calibStatus.text = "   Grading"
                calibStatus.color = "green"
                break
            case 1:
                calibStatus.text = "   Calibrating..."
                calibStatus.color = "#f56702"
                break
            case 2:
                calibStatus.text = "   Waiting For Start"
                calibStatus.color = "red"
                break
            default:
                break
            }
        }

        onStopChartA: {
            tableA.checked = false
        }

        onStopChartB: {
            tableB.checked = false
        }

        Component.onCompleted: {

        }
    }

    CameraController{
        id: camController

        property int _total: 1
        onClassesCountChanged: {

            //            if(classesCount[0] || classesCount[1]){ // for kiwi
            if(classesCount[0]){
                //                camController.total = camController.total + 1
                camController.setTotal(camController.total + 1)
            }
        }
        onReadyToProcessChanged: {
            //            if(readyToProcess){
            //                startBtnRec.color = "green"
            //                stopBtnRec.color = "gray"
            //            }
            //            else {
            //                startBtnRec.color = "gray";
            //                stopBtnRec.color = "red";
            //            }
        }

        onCameraIsOpenedChanged: {
            camController.cameraIsOpened[0] ? camLna.source = "qrc:/Icons/Camera.png" : camLna.source = "qrc:/Icons/Camera_dis.png"
            camController.cameraIsOpened[1] ? camLnb.source = "qrc:/Icons/Camera.png" : camLnb.source = "qrc:/Icons/Camera_dis.png"
        }
    }

    WeightController {
        id: weightController
    }

    function showMessageBox(text)
    {
        messageBox.msgText = text
        messageBox.timer = true
        messageBox.open()
    }

    function updateOutputs()
    {
        ethController.readDb(exfSettingCmb.currentValue)

        outputsModel.clear()

        //console.log(ethController.outputs)

        for (var i = 0; i < ethController.wLow.length; ++i){
            outputsModel.append({
                                    ID: i,
                                    ruleID: ethController.ruleID[i],
                                    wLow: ethController.wLow[i],
                                    wHigh: ethController.wHigh[i],
                                    sizes: ethController.sizeDensity[i],
                                    defectEnabled: ethController.enables[i].split(',')[0],
                                    colorEnabled: ethController.enables[i].split(',')[1],
                                    outputsUsed : ethController.outputsUsed[i]
                                })
        }
    }

    function updateClassesModel()
    {
        classesModel.clear()
        for (var i = 0; i < camController.classNamesList.length; ++i){
            // if(!camController.classEnableList[i])
            //     continue

            if(i !== 0 && i !==  3 && i !== 4)
                classesModel.append({
                                        ID: i,
                                        // Enabled: camController.classEnableList[i],
                                        Name: camController.classNamesList[i],
                                        Conf: camController.classConfList[i],
                                        //                                        Quantity: camController.classQuantityList[i],
                                        Quantity: camController.quantityBasedClassCountList[i],
                                        QuantityEnabled: camController.classQuantityEnabledList[i],
                                        QuantityConf: camController.quantityBasedClassConfList[i]
                                    })
        }
    }

    function updateExfs()
    {

        var exitFeatureList = []

        for (var i=0; i < dbController.getGeneralExfs().length; ++i){
            exitFeatureList.push(dbController.getGeneralExfs()[i])
        }

        exfMainCmb.model = exitFeatureList
        exfSettingCmb.model = exitFeatureList

        updates[0] = true
    }

    function updateColors()
    {
        let colorGroupList= []
        for (var i=0; i < dbController.getColorGroups().length; ++i){
            colorGroupList.push(dbController.getColorGroups()[i])
        }

        colorGroupComboBox.model = colorGroupList
        colorMainCmb.model = colorGroupList

        let colorList= []
        for (i=0; i < dbController.getColors(colorGroupComboBox.currentValue).length; ++i){
            colorList.push(dbController.getColors(colorGroupComboBox.currentValue)[i])
        }

        colorComboBox.model = colorList

        updates[1] = true
    }

    function updateDefects()
    {
        let defectExfList= []
        for (var i=0; i < dbController.getFruitExfs().length; ++i){
            defectExfList.push(dbController.getFruitExfs()[i])
        }

        defectComboBox.model = defectExfList
        defectMainCmb.model = defectExfList

        updates[2] = true
    }

    property int k: 1
    property int b: 1
    property int c: 0

    property string startColor: "#04c758"
    property string stopColor: "#c40202"
    property string applyColor: "#026627"
    property string editColor: "#044794"
    property string rectColor: "#181818"
    property string mainRectColor: "white"
    property string testColor: "#850419"
    property string circleButtonColor: "#273475"
    property string password: ""

    property string disabledColor: "#545b5c"

    property string excelDate: ""

    function enter() {
        Layout.preferredHeight += 5
        Layout.preferredWidth += 5
    }

    function exit() {
        Layout.preferredHeight -= 5
        Layout.preferredWidth -= 5
    }


    function setTextFieldsEnables(e)
    {
        shift.enabled = e
        distanceFromLC.enabled = e

        aLow.enabled = e
        aHigh.enabled = e
        aEnergize.enabled = e
        aKFactor.enabled = e
        aMin.enabled = e
        aMax.enabled = e
        aAverage.enabled = e
        aError.enabled = e

        aaCup.enabled = e
        abCup.enabled = e
        acCup.enabled = e
        adCup.enabled = e
        aeCup.enabled = e
        afCup.enabled = e
        agCup.enabled = e
        ahCup.enabled = e

        bLow.enabled = e
        bHigh.enabled = e
        bEnergize.enabled = e
        bKFactor.enabled = e
        bMin.enabled = e
        bMax.enabled = e
        bAverage.enabled = e
        bError.enabled = e

        baCup.enabled = e
        bbCup.enabled = e
        bcCup.enabled = e
        bdCup.enabled = e
        beCup.enabled = e
        bfCup.enabled = e
        bgCup.enabled = e
        bhCup.enabled = e

        camIp.enabled = e
        camGateway.enabled = e

        exposure.enabled = e
        gain.enabled = e
        blackLevel.enabled = e
        whiteBalanceRed.enabled = e
        whiteBalanceBlue.enabled = e
        whiteBalanceGreen.enabled = e
        gamma.enabled = e
        autoWhiteBalanceComboBox.enabled = e
        whiteBalanceModeComboBox.enabled = e
        whiteBalanceAutoPresetComboBox.enabled = e
        whiteBalanceTempraturePresetComboBox.enabled = e
        strobeComboBox.enabled = e

        setBtn1.enabled = e
        setBtn2.enabled = e
        setBtn3.enabled = e
        setBtn4.enabled = e
        setBtn5.enabled = e
        setBtn6.enabled = e
        setBtn7.enabled = e

        hue.enabled = e
        saturation.enabled = e
        value.enabled = e
        rHue.enabled = e
    }

    MyMessageBox{
        id: messageBox
    }

    Popup {
        id: passPopup
        width: 400
        height: 120
        anchors.centerIn: parent
        property bool timer: false

        background: Rectangle{
            color: "black"

            radius: 10
        }

        onClosed: {
            password = passText.text

            dbController.readUsers()

            if(password === dbController.users[0])
            {
                settingPopup.open()

                setTextFieldsEnables(false)

                exfSettingCmb.currentIndex = exfSettingCmb.find(exfMainCmb.currentValue)
                dbController.readGenerals(exfMainCmb.currentValue)
            }
            else if(password === dbController.users[1])
            {
                settingPopup.open()

                setTextFieldsEnables(true)

                exfSettingCmb.currentIndex = exfSettingCmb.find(exfMainCmb.currentValue)
                dbController.readGenerals(exfMainCmb.currentValue)
            }
            else
            {
                showMessageBox("Password Is Incorrect")
            }

        }

        Rectangle {
            anchors.fill: parent
            color: "black"
            radius: 10
            border.color: "white"
            border.width: 0

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 5

                RowLayout
                {
                    spacing: 20
                    Label {text: "Password"; font.pixelSize: 15; color: "white"}

                    TextField{
                        id: passText
                        font.bold: true
                        color: "white"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        echoMode: TextInput.Password
                        //Layout.leftMargin: control.contentLeftMargin
                    }
                }

                Button {
                    Layout.alignment: Qt.AlignHCenter
                    height: 30
                    Layout.preferredWidth: 200
                    text: "OK"

                    onClicked: {
                        passPopup.close()
                    }
                }
            }

        }
    }

    Popup {
        id: defectPopup
        width: 200
        height: 300
        visible: false

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        background: Rectangle {
            color: "#212121"
            radius: 10
        }

        onVisibleChanged: {
            //defectChbValues = []
            defectsModel.clear()
            var classes = ["Mis_shape","Nubbin","Blotch","Scab","Bruising","Cut"]
            var enables = [dbController.output[6].split(',')[0], dbController.output[6].split(',')[1], dbController.output[6].split(',')[2],
                           dbController.output[6].split(',')[3], dbController.output[6].split(',')[4], dbController.output[6].split(',')[5]]

            //defectChbValues = enables

            // console.log(enables)

            for (var i = 0; i < classes.length; ++i){
                defectsModel.append({
                                        ID: i,
                                        Name: classes[i],
                                        Enabled: enables[i] === "false" ? false : true
                                    })
                // if(enables[i] === "false") defectChbValues.push(false)
                // else defectChbValues.push(true)
            }


        }

        onClosed: {
            //console.log("tttt " + defectChbValues)

        }



        RowLayout{
            anchors.fill: parent

            ListView {
                Layout.preferredHeight: 300
                Layout.preferredWidth: 180

                model: defectsModel


                delegate: RowLayout {
                    Item{
                        Layout.preferredWidth: 40
                    }
                    CheckBox {
                        id: classEnabled
                        Layout.preferredWidth: 120
                        text: model.Name
                        checked: model.Enabled;
                        onCheckedChanged: {
                            defectChbValues[model.ID] = checked
                            //console.log("kkkk " + defectChbValues)
                            //camController.setClassesEnabled(model.ID, classEnabled.checked)
                        }

                        Component.onCompleted: {
                            defectChbValues[model.ID] = checked
                        }

                        Layout.alignment: Qt.AlignHCenter
                        //                        visible: defectChb.checked

                    }
                }



            }
        }
    }

    Popup{
        id: colorPopup
        width: 300
        height: 350
        visible: false

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        background: Rectangle {
            color: "#212121"
            radius: 10
        }


        ColumnLayout{
            anchors.fill: parent

            RowLayout{
                Layout.alignment: Qt.AlignHCenter
                Image {
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 20
                    source: "qrc:/colors/red_color.jpg"
                }
                Item{Layout.preferredWidth: 5}
                Label {text: "Red"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Item{Layout.preferredWidth: 30}
                TextField {
                    id: rdLow
                    text: dbController.output[7].split(',')[0]
                    color: "white"
                    Layout.preferredWidth: 50
                    font.pixelSize: 16
                    selectByMouse: true
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Item{Layout.preferredWidth: 10}
                TextField {
                    id: rdHigh
                    text: dbController.output[7].split(',')[1] ? dbController.output[7].split(',')[1] : ""
                    color: "white"
                    Layout.preferredWidth: 50
                    font.pixelSize: 16
                    selectByMouse: true
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignHCenter
                Image {
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 20
                    source: "qrc:/colors/orange_color.jpg"
                }
                Item{Layout.preferredWidth: 5}
                Label {text: "Orange"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Item{Layout.preferredWidth: 5}
                TextField {
                    id: orLow
                    text: dbController.output[7].split(',')[2] ? dbController.output[7].split(',')[2] : ""
                    color: "white"
                    Layout.preferredWidth: 50
                    font.pixelSize: 16
                    selectByMouse: true
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Item{Layout.preferredWidth: 10}
                TextField {
                    id: orHigh
                    text: dbController.output[7].split(',')[3] ? dbController.output[7].split(',')[3] : ""
                    color: "white"
                    Layout.preferredWidth: 50
                    font.pixelSize: 16
                    selectByMouse: true
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignHCenter
                Image {
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 20
                    source: "qrc:/colors/yellow_color.jpg"
                }
                Item{Layout.preferredWidth: 5}
                Label {text: "Yellow"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Item{Layout.preferredWidth: 10}
                TextField {
                    id: ylLow
                    text: dbController.output[7].split(',')[4] ? dbController.output[7].split(',')[4] : ""
                    color: "white"
                    Layout.preferredWidth: 50
                    font.pixelSize: 16
                    selectByMouse: true
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Item{Layout.preferredWidth: 10}
                TextField {
                    id: ylHigh
                    text: dbController.output[7].split(',')[5] ? dbController.output[7].split(',')[5] : ""
                    color: "white"
                    Layout.preferredWidth: 50
                    font.pixelSize: 16
                    selectByMouse: true
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            RowLayout{
                Layout.alignment: Qt.AlignHCenter
                Image {
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 20
                    source: "qrc:/colors/green_color.jpg"
                }
                Item{Layout.preferredWidth: 5}
                Label {text: "Green"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Item{Layout.preferredWidth: 15}
                TextField {
                    id: gnLow
                    text: dbController.output[7].split(',')[6] ? dbController.output[7].split(',')[6] : ""
                    color: "white"
                    Layout.preferredWidth: 50
                    font.pixelSize: 16
                    selectByMouse: true
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Item{Layout.preferredWidth: 10}
                TextField {
                    id: gnHigh
                    text: dbController.output[7].split(',')[7] ? dbController.output[7].split(',')[7] : ""
                    color: "white"
                    Layout.preferredWidth: 50
                    font.pixelSize: 16
                    selectByMouse: true
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        onClosed: {
            colorRanges = []

            rdLow.text === "" ? colorRanges.push("0") : colorRanges.push(rdLow.text)
            rdHigh.text === "" ? colorRanges.push("100") : colorRanges.push(rdHigh.text)
            orLow.text === "" ? colorRanges.push("0") : colorRanges.push(orLow.text)
            orHigh.text === "" ? colorRanges.push("100") : colorRanges.push(orHigh.text)
            ylLow.text === "" ? colorRanges.push("0") : colorRanges.push(ylLow.text)
            ylHigh.text === "" ? colorRanges.push("100") : colorRanges.push(ylHigh.text)
            gnLow.text === "" ? colorRanges.push("0") : colorRanges.push(gnLow.text)
            gnHigh.text === "" ? colorRanges.push("100") : colorRanges.push(gnHigh.text)

        }
    }

    Popup {
        id: settingPopup
        width: 1357
        height: 1010
        visible: false
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        background: Rectangle {
            color: "#121212"
            radius: 10
        }

        onClosed: {
            password = ""
            passText.text = ""
        }

        Component.onCompleted: {
            dbController.readMotors()

            camController.getCameraSerials()


            var camList = []

            for (var i=0; i < camController.serials.length; ++i){
                camList.push(camController.serials[i])
            }

            camComboBox.model = camList
            camSettingComboBox.model = camList

            updateColors()

            updateDefects()

            dbController.readLastRun()
            exfMainCmb.currentIndex = exfMainCmb.find(dbController.lastRun[0])
            speedMainComboBox.currentIndex = speedMainComboBox.find(dbController.lastRun[1])
            colorMainCmb.currentIndex = colorMainCmb.find(dbController.lastRun[2])
            defectMainCmb.currentIndex = defectMainCmb.find(dbController.lastRun[3])

            lnaSpeedComboBox.currentIndex = speedMainComboBox.find(dbController.lastRun[1])
            lnbSpeedComboBox.currentIndex = speedMainComboBox.find(dbController.lastRun[1])
            exfSettingCmb.currentIndex =  exfMainCmb.find(dbController.lastRun[0])
            colorComboBox.currentIndex = colorMainCmb.find(dbController.lastRun[2])
            defectComboBox.currentIndex = defectMainCmb.find(dbController.lastRun[3])

            cmbCompleted[0] = true
            cmbCompleted[1] = true
            cmbCompleted[2] = true
            cmbCompleted[3] = true
        }

        closePolicy: "NoAutoClose"

        onVisibleChanged: {

        }


        ColumnLayout{
            anchors.fill: parent
            spacing: 6
            RowLayout{
                Item{
                    Layout.fillWidth: true
                }

                MyButton{
                    text: "X"
                    fontsize: 15
                    buttonColor: "red"
                    icon_visible: false
                    width: 25
                    height: 25
                    radius: 20
                    bottonMarg: 0

                    onClicked: {
                        settingPopup.close()
                        ethController.readDb(exfMainCmb.currentValue)

                        chartA.checked = false
                        chartB.checked = false

                        tableA.checked = false
                        tableB.checked = false

                        settingTabChb.checked = false
                        colorTabChb.checked = false
                        defectTabChb.checked = false

                        // exitA.text = "Exit A (" + ethController.wLow[0] + "-" + ethController.wHigh[0] + ")"
                        // exitB.text = "Exit B (" + ethController.wLow[1] + "-" + ethController.wHigh[1] + ")"
                        // exitC.text = "Exit C (" + ethController.wLow[2] + "-" + ethController.wHigh[2] + ")"
                        // exitD.text = "Exit D (" + ethController.wLow[3] + "-" + ethController.wHigh[3] + ")"
                        // exitE.text = "Exit E (" + ethController.wLow[4] + "-" + ethController.wHigh[4] + ")"
                        // exitF.text = "Exit F (" + ethController.wLow[5] + "-" + ethController.wHigh[5] + ")"
                        // exitG.text = "Exit G (" + ethController.wLow[6] + "-" + ethController.wHigh[6] + ")"
                        // exitH.text = "Exit H (" + ethController.wLow[7] + "-" + ethController.wHigh[7] + ")"
                    }
                }
            }

            TabBar {
                id: settingBar
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 50
                TabButton {
                    text: qsTr("General")
                    Layout.preferredHeight: 50
                    onClicked: stackLayout.currentIndex = 0

                }
                TabButton {
                    text: qsTr("Ejectors")
                    Layout.preferredHeight: 50
                    onClicked: stackLayout.currentIndex = 1

                }
                TabButton {
                    text: qsTr("Line A")
                    Layout.preferredHeight: 50
                    onClicked: stackLayout.currentIndex = 2

                }
                TabButton {
                    text: qsTr("Line B")
                    Layout.preferredHeight: 50
                    onClicked: stackLayout.currentIndex = 3
                }
                TabButton {
                    text: qsTr("Cameras")
                    Layout.preferredHeight: 50
                    onClicked: stackLayout.currentIndex = 4
                }
                TabButton {
                    text: qsTr("Camera Setting")
                    Layout.preferredHeight: 50
                    onClicked: stackLayout.currentIndex = 5
                }
                TabButton {
                    text: qsTr("Color")
                    Layout.preferredHeight: 50
                    onClicked: stackLayout.currentIndex = 6
                }
                TabButton {
                    text: qsTr("Confidence")
                    Layout.preferredHeight: 50
                    onClicked: stackLayout.currentIndex = 7
                }
            }

            StackLayout {
                id: stackLayout
                Layout.preferredWidth: parent.width
                currentIndex: settingBar.currentIndex
                visible: true

                Item {
                    id: generalTab

                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height

                    ColumnLayout{
                        anchors.fill: parent
                        spacing: 0

                        Rectangle {
                            Layout.preferredWidth: parent.width - 10
                            Layout.preferredHeight: 800
                            Layout.alignment: Qt.AlignHCenter

                            color: rectColor

                            border.color: "white"
                            radius: 10
                            border.width: 0

                            ColumnLayout{
                                spacing: 0
                                anchors.fill: parent


                                ColumnLayout{
                                    Layout.alignment: Qt.AlignCenter

                                    Item{
                                        Layout.preferredHeight: 10
                                    }

                                    RowLayout{
                                        Layout.alignment: Qt.AlignHCenter

                                        Label {text: "Calibration Rounds"; font.pixelSize: 15; color: "white"}
                                        TextField {
                                            id: calRounds
                                            text: dbController.gen[1]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter
                                        }

                                        Item {
                                            Layout.preferredWidth: 10
                                        }

                                        Label {text: "Shift"; font.pixelSize: 15; color: "white"}
                                        TextField {
                                            id: shift
                                            text: dbController.gen[0]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter
                                        }

                                        Item {
                                            Layout.preferredWidth: 10
                                        }

                                        Label {text: "Distance From LC"; font.pixelSize: 15; color: "white"}
                                        TextField {
                                            id: distanceFromLC
                                            text: dbController.gen[2]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter
                                        }

                                        Item {
                                            Layout.preferredWidth: 10
                                        }

                                        Label {text: "Energize"; font.pixelSize: 15; color: "white"; visible: false}
                                        TextField {
                                            id: energize
                                            text: dbController.gen[3]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter
                                            visible: false
                                        }

                                        Item {
                                            Layout.preferredWidth: 10
                                        }

                                        Label {text: "Size Ratio"; font.pixelSize: 15; color: "white";visible: false}
                                        TextField {
                                            id: sizeRatio
                                            text: dbController.gen[4]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter
                                            visible: false
                                        }
                                        Item {
                                            Layout.preferredWidth: 10
                                            visible: false

                                        }

                                        MyButton {
                                            text: "Apply"
                                            icon_btn: "qrc:/Icons/apply.png"
                                            Layout.preferredHeight: 50
                                            Layout.preferredWidth: 120
                                            Layout.alignment: Qt.AlignHCenter

                                            buttonColor: applyColor

                                            onClicked: {
                                                let setting = []
                                                setting.push(shift.text)
                                                setting.push(distanceFromLC.text)
                                                setting.push(energize.text)
                                                setting.push(calRounds.text)

                                                ethController.applyGeneral(setting)
                                                showMessageBox("General Setting Applied")
                                            }
                                        }
                                        Item {
                                            Layout.preferredWidth: 10
                                        }
                                        MyButton {
                                            text: "Edit"
                                            icon_btn: "qrc:/Icons/draw.png"
                                            Layout.preferredHeight: 50
                                            Layout.preferredWidth: 120
                                            Layout.alignment: Qt.AlignHCenter

                                            buttonColor: editColor

                                            onClicked: {
                                                dbController.setIdx("General", shift.text, 0)
                                                dbController.setIdx("General", calRounds.text, 1)
                                                dbController.setIdx("General", distanceFromLC.text, 2)
                                                dbController.setIdx("General", energize.text, 3)
                                                dbController.setIdx("General", sizeRatio.text, 4)

                                                showMessageBox("General Setting Updated")

                                                //dbController.readLastRun()

                                                dbController.updateGeneral()
                                            }
                                        }


                                    }

                                    Item{
                                        Layout.preferredHeight: 10
                                    }

                                    RowLayout{
                                        Layout.alignment: Qt.AlignHCenter

                                        Label {text: "Group : "; font.pixelSize: 15; color: "white"}
                                        ComboBox {
                                            id: exfSettingCmb
                                            Layout.alignment: Qt.AlignHCenter

                                            Layout.preferredWidth: 150

                                            Component.onCompleted: {
                                                currentIndex = exfSettingCmb.find(exfMainCmb.currentValue)
                                                dbController.readGenerals(currentValue)
                                                dbController.readGeneral()

                                                dbController.readEjectorA()
                                                dbController.readEjectorB()

                                                updateOutputs()
                                            }

                                            onCurrentValueChanged: {
                                                ethController.assignControllers(weightController, camController)
                                                ethController.assignDb(dbController)
                                                weightController.assignDb(dbController)
                                                camController.assignDb(dbController)
                                                dbController.readGenerals(currentValue)
                                                updateOutputs()
                                                cmbIdx = 0
                                            }
                                        }

                                        MyButton {
                                            //Layout.fillWidth: true
                                            //addjjjjjjj
                                            Layout.alignment: Qt.AlignHCenter

                                            icon_btn: "qrc:/Icons/add.png"
                                            buttonColor: "#181818"
                                            iconsize: 40
                                            Layout.preferredHeight: 40

                                            hoverEnabled: true

                                            onEntered: {
                                                buttonColor = "#00d76a"
                                            }
                                            onExited: {
                                                buttonColor = "#181818"
                                            }


                                            border_width: 0

                                            onClicked: {
                                                dbController.setIdx("Outputs", exfText.text, 0)
                                                dbController.setIdx("Outputs", parseInt(ethController.ruleID[ethController.ruleID.length - 1]) + 1, 1)
                                                dbController.setIdx("Outputs", "0", 2)
                                                dbController.setIdx("Outputs", "100", 3)
                                                dbController.setIdx("Outputs", "0,100,0,100", 4)
                                                dbController.setIdx("Outputs", "false,false", 5)
                                                dbController.setIdx("Outputs", "true,true,true,true,true,true", 6)
                                                dbController.setIdx("Outputs", "0,100,0,100,0,100,0,100", 7)
                                                dbController.setIdx("Outputs", "", 8)
                                                dbController.setIdx("Outputs", "false,false,false,false,false,false,false,false", 9)

                                                dbController.insert("Outputs")

                                                updateOutputs()
                                            }

                                        }




                                    }
                                }

                                Item{
                                    Layout.preferredHeight: 30
                                }

                                RowLayout{
                                    Item {Layout.preferredWidth: 230 }
                                    Label {text: "Weight (gr)"; font.pixelSize: 15; color: "white"}
                                    Item {Layout.preferredWidth: 80 }
                                    Label {text: "Density"; font.pixelSize: 15; color: "white"; visible: false}
                                    Item {Layout.preferredWidth: 90 ; visible: false}
                                    Label {text: "Size"; font.pixelSize: 15; color: "white"; visible: false}
                                    Item {Layout.preferredWidth: 110; visible: false}
                                    Label {text: "Defect"; font.pixelSize: 15; color: "white"}
                                    Item {Layout.preferredWidth: 100}
                                    Label {text: "Color"; font.pixelSize: 15; color: "white"}
                                    Item {Layout.preferredWidth: 85}
                                    Label {text: "O1"; font.pixelSize: 15; color: "white"}
                                    Item {Layout.preferredWidth: 13}
                                    Label {text: "O2"; font.pixelSize: 15; color: "white"}
                                    Item {Layout.preferredWidth: 15}
                                    Label {text: "O3"; font.pixelSize: 15; color: "white"}
                                    Item {Layout.preferredWidth: 17}
                                    Label {text: "O4"; font.pixelSize: 15; color: "white"}
                                    Item {Layout.preferredWidth: 15}
                                    Label {text: "O5"; font.pixelSize: 15; color: "white"}
                                    Item {Layout.preferredWidth: 17}
                                    Label {text: "O6"; font.pixelSize: 15; color: "white"}
                                    Item {Layout.preferredWidth: 15}
                                    Label {text: "O7"; font.pixelSize: 15; color: "white"}
                                    Item {Layout.preferredWidth: 15}
                                    Label {text: "O8"; font.pixelSize: 15; color: "white"}
                                }

                                Item
                                {
                                    Layout.preferredHeight: 20
                                }

                                RowLayout {
                                    Layout.alignment: Qt.AlignHCenter
                                    Layout.preferredWidth: 1300
                                    Layout.preferredHeight: 900
                                    spacing: 10

                                    Item {Layout.preferredWidth: 150}

                                    ColumnLayout{
                                        spacing: 0
                                        Layout.preferredWidth: 1300
                                        Layout.preferredHeight: 700
                                        // Layout.fillHeight: true
                                        // Layout.fillWidth: true
                                        ListView {
                                            //spacing: 0
                                            id: outputList
                                            Layout.fillHeight: true
                                            Layout.fillWidth: true
                                            ScrollBar.vertical: ScrollBar {
                                                policy: ScrollBar.AlwaysOn
                                            }

                                            //clip: true

                                            Layout.alignment: Qt.AlignHCenter

                                            model: outputsModel

                                            delegate: Item{
                                                width: outputList.width
                                                height: 50

                                                RowLayout {
                                                    Layout.alignment: Qt.AlignHCenter

                                                    Label {text: model.ID; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter; Layout.preferredWidth: 20}

                                                    ToolSeparator { Layout.fillHeight: true}

                                                    TextField {
                                                        id: wLow
                                                        text: model.wLow
                                                        color: "white"
                                                        Layout.preferredWidth: 50
                                                        font.pixelSize: 16
                                                        selectByMouse: true
                                                        Layout.alignment: Qt.AlignHCenter
                                                        horizontalAlignment: Text.AlignHCenter
                                                    }

                                                    Item{ Layout.preferredWidth: 5}

                                                    TextField {
                                                        id: wHigh
                                                        text: model.wHigh
                                                        color: "white"
                                                        Layout.preferredWidth: 50
                                                        font.pixelSize: 16
                                                        selectByMouse: true
                                                        Layout.alignment: Qt.AlignHCenter
                                                        horizontalAlignment: Text.AlignHCenter
                                                    }

                                                    ToolSeparator { Layout.fillHeight: true}

                                                    // ComboBox {
                                                    //     id: weightDensityCmb
                                                    //     Layout.alignment: Qt.AlignHCenter

                                                    //     model: ["AND", "OR"]

                                                    //     Layout.preferredWidth: 90


                                                    //     Component.onCompleted: {
                                                    //         currentIndex = find(ethController.operations[cmbIdx - 1].split(',')[0])

                                                    //     }
                                                    // }


                                                    TextField {
                                                        id: densLow
                                                        text: model.sizes.split(',')[0]
                                                        color: "white"
                                                        Layout.preferredWidth: 50
                                                        font.pixelSize: 16
                                                        selectByMouse: true
                                                        Layout.alignment: Qt.AlignHCenter
                                                        horizontalAlignment: Text.AlignHCenter
                                                        visible: false
                                                    }

                                                    Item{ Layout.preferredWidth: 5; visible: false}

                                                    TextField {
                                                        id: densHigh
                                                        text: model.sizes.split(',')[1]
                                                        color: "white"
                                                        Layout.preferredWidth: 50
                                                        font.pixelSize: 16
                                                        selectByMouse: true
                                                        Layout.alignment: Qt.AlignHCenter
                                                        horizontalAlignment: Text.AlignHCenter
                                                        visible: false
                                                    }


                                                    // ComboBox {
                                                    //     id: densitySizeCmb
                                                    //     Layout.alignment: Qt.AlignHCenter

                                                    //     model: ["AND", "OR"]

                                                    //     Layout.preferredWidth: 90


                                                    //     Component.onCompleted: {
                                                    //         currentIndex = find(ethController.operations[cmbIdx - 1].split(',')[1])
                                                    //     }
                                                    // }

                                                    ToolSeparator { Layout.fillHeight: true; visible: false}

                                                    TextField {
                                                        id: sizeLow
                                                        text: model.sizes.split(',')[2]
                                                        color: "white"
                                                        Layout.preferredWidth: 50
                                                        font.pixelSize: 16
                                                        selectByMouse: true
                                                        Layout.alignment: Qt.AlignHCenter
                                                        horizontalAlignment: Text.AlignHCenter
                                                        visible: false
                                                    }

                                                    Item{ Layout.preferredWidth: 5; visible: false}

                                                    TextField {
                                                        id: sizeHigh
                                                        text: model.sizes.split(',')[3]
                                                        color: "white"
                                                        Layout.preferredWidth: 50
                                                        font.pixelSize: 16
                                                        selectByMouse: true
                                                        Layout.alignment: Qt.AlignHCenter
                                                        horizontalAlignment: Text.AlignHCenter
                                                        visible: false
                                                    }

                                                    ToolSeparator { Layout.fillHeight: true; visible: false}

                                                    // ComboBox {
                                                    //     id: sizeDefectCmb
                                                    //     Layout.alignment: Qt.AlignHCenter

                                                    //     model: ["AND", "OR"]

                                                    //     Layout.preferredWidth: 90


                                                    //     Component.onCompleted: {
                                                    //         currentIndex = find(ethController.operations[cmbIdx - 1].split(',')[2])
                                                    //     }
                                                    // }


                                                    CheckBox {
                                                        id: defectChb
                                                        checked: model.defectEnabled === "true" ? true : false
                                                    }

                                                    Item{ Layout.preferredWidth: 5}

                                                    Button{
                                                        text: "Open"
                                                        onClicked: {
                                                            dbController.readOutput(exfSettingCmb.currentValue, model.ruleID)
                                                            defectPopupOpened = true
                                                            defectPopup.open()
                                                        }
                                                    }

                                                    ToolSeparator { Layout.fillHeight: true}

                                                    // ComboBox {
                                                    //     id: defectColorCmb
                                                    //     Layout.alignment: Qt.AlignHCenter

                                                    //     model: ["AND", "OR"]

                                                    //     Layout.preferredWidth: 90


                                                    //     Component.onCompleted: {
                                                    //         currentIndex = find(ethController.operations[cmbIdx].split(',')[3])
                                                    //         cmbIdx = cmbIdx + 1
                                                    //     }
                                                    // }


                                                    CheckBox {
                                                        id: colorChb
                                                        checked: model.colorEnabled === "true" ? true : false
                                                    }

                                                    Item{ Layout.preferredWidth: 5}

                                                    Button{
                                                        text: "Open"
                                                        onClicked: {
                                                            dbController.readOutput(exfSettingCmb.currentValue, model.ruleID)
                                                            colorPopupOpened = true
                                                            colorPopup.open()
                                                        }
                                                    }

                                                    ToolSeparator { Layout.fillHeight: true}

                                                    CheckBox {
                                                        id: output0Chb
                                                        checked: model.outputsUsed.split(',')[0] === "true" ? true : false
                                                        onCheckedChanged: {
                                                            if(checked)
                                                            {
                                                                output1Chb.checked = false
                                                                output2Chb.checked = false
                                                                output3Chb.checked = false
                                                                output4Chb.checked = false
                                                                output5Chb.checked = false
                                                                output6Chb.checked = false
                                                                output7Chb.checked = false
                                                            }
                                                        }
                                                    }

                                                    CheckBox {
                                                        id: output1Chb
                                                        checked: model.outputsUsed.split(',')[1] === "true" ? true : false
                                                        onCheckedChanged: {
                                                            if(checked)
                                                            {
                                                                output0Chb.checked = false
                                                                output2Chb.checked = false
                                                                output3Chb.checked = false
                                                                output4Chb.checked = false
                                                                output5Chb.checked = false
                                                                output6Chb.checked = false
                                                                output7Chb.checked = false
                                                            }
                                                        }
                                                    }

                                                    CheckBox {
                                                        id: output2Chb
                                                        checked: model.outputsUsed.split(',')[2] === "true" ? true : false
                                                        onCheckedChanged: {
                                                            if(checked)
                                                            {
                                                                output0Chb.checked = false
                                                                output1Chb.checked = false
                                                                output3Chb.checked = false
                                                                output4Chb.checked = false
                                                                output5Chb.checked = false
                                                                output6Chb.checked = false
                                                                output7Chb.checked = false
                                                            }
                                                        }
                                                    }

                                                    CheckBox {
                                                        id: output3Chb
                                                        checked: model.outputsUsed.split(',')[3] === "true" ? true : false
                                                        onCheckedChanged: {
                                                            if(checked)
                                                            {
                                                                output0Chb.checked = false
                                                                output1Chb.checked = false
                                                                output2Chb.checked = false
                                                                output4Chb.checked = false
                                                                output5Chb.checked = false
                                                                output6Chb.checked = false
                                                                output7Chb.checked = false
                                                            }
                                                        }
                                                    }

                                                    CheckBox {
                                                        id: output4Chb
                                                        checked: model.outputsUsed.split(',')[4] === "true" ? true : false
                                                        onCheckedChanged: {
                                                            if(checked)
                                                            {
                                                                output0Chb.checked = false
                                                                output1Chb.checked = false
                                                                output2Chb.checked = false
                                                                output3Chb.checked = false
                                                                output5Chb.checked = false
                                                                output6Chb.checked = false
                                                                output7Chb.checked = false
                                                            }
                                                        }
                                                    }

                                                    CheckBox {
                                                        id: output5Chb
                                                        checked: model.outputsUsed.split(',')[5] === "true" ? true : false
                                                        onCheckedChanged: {
                                                            if(checked)
                                                            {
                                                                output0Chb.checked = false
                                                                output1Chb.checked = false
                                                                output2Chb.checked = false
                                                                output3Chb.checked = false
                                                                output4Chb.checked = false
                                                                output6Chb.checked = false
                                                                output7Chb.checked = false
                                                            }
                                                        }
                                                    }

                                                    CheckBox {
                                                        id: output6Chb
                                                        checked: model.outputsUsed.split(',')[6] === "true" ? true : false
                                                        onCheckedChanged: {
                                                            if(checked)
                                                            {
                                                                output0Chb.checked = false
                                                                output1Chb.checked = false
                                                                output2Chb.checked = false
                                                                output3Chb.checked = false
                                                                output4Chb.checked = false
                                                                output5Chb.checked = false
                                                                output7Chb.checked = false
                                                            }
                                                        }
                                                    }

                                                    CheckBox {
                                                        id: output7Chb
                                                        checked: model.outputsUsed.split(',')[7] === "true" ? true : false
                                                        onCheckedChanged: {
                                                            if(checked)
                                                            {
                                                                output0Chb.checked = false
                                                                output1Chb.checked = false
                                                                output2Chb.checked = false
                                                                output3Chb.checked = false
                                                                output4Chb.checked = false
                                                                output5Chb.checked = false
                                                                output6Chb.checked = false
                                                            }
                                                        }
                                                    }

                                                    ToolSeparator { Layout.fillHeight: true}

                                                    Rectangle {
                                                        id: editOutputBtn
                                                        width: 30
                                                        height: 30
                                                        color: "#181818" // Default background color
                                                        radius: 5

                                                        MouseArea {
                                                            anchors.fill: parent
                                                            onClicked: {
                                                                dbController.readOutput(exfSettingCmb.currentValue, model.ruleID)

                                                                let condition = ""
                                                                let outputEnables = ""
                                                                let defects = ""
                                                                let c1 = ""
                                                                let c2 = ""
                                                                let c3 = ""
                                                                let c4 = ""

                                                                let operation = ""
                                                                // dbController.setIdxOutputs(exfSettingComboBox.currentValue, 0)
                                                                // dbController.setIdxOutputs(model.output, 1)
                                                                let sizeAndDensity = densLow.text + "," + densHigh.text + "," + sizeLow.text + "," + sizeHigh.text
                                                                dbController.setIdx("Outputs", wLow.text, 2)
                                                                dbController.setIdx("Outputs", wHigh.text, 3)
                                                                dbController.setIdx("Outputs", sizeAndDensity, 4)
                                                                dbController.setIdx("Outputs", defectChb.checked + "," + colorChb.checked, 5)

                                                                if(defectPopupOpened)
                                                                {
                                                                    console.log("Hello" + defectChbValues)
                                                                    defects = defectChbValues[0] + "," + defectChbValues[1] + "," + defectChbValues[2] + "," + defectChbValues[3] + "," +
                                                                            defectChbValues[4] + "," + defectChbValues[5]
                                                                    dbController.setIdx("Outputs", defects, 6)
                                                                    defectPopupOpened = false
                                                                }

                                                                if(colorPopupOpened)
                                                                {
                                                                    let ranges = colorRanges[0] + "," + colorRanges[1] + "," + colorRanges[2] + "," + colorRanges[3] + "," +
                                                                        colorRanges[4] + "," + colorRanges[5] + "," + colorRanges[6] + "," + colorRanges[7]

                                                                    dbController.setIdx("Outputs", ranges, 7)

                                                                    c1 = `(${colorRanges[0]} <= rd && rd <= ${colorRanges[1]}) &&`
                                                                    c2 = `(${colorRanges[2]} <= or && or <= ${colorRanges[3]}) &&`
                                                                    c3 = `(${colorRanges[4]} <= yl && yl <= ${colorRanges[5]}) &&`
                                                                    c4 = `(${colorRanges[6]} <= gn && gn <= ${colorRanges[7]})`

                                                                    condition = c1 + c2 + c3 + c4

                                                                    dbController.setIdx("Outputs", condition, 8)

                                                                    colorPopupOpened = false
                                                                }

                                                                outputEnables = output0Chb.checked + "," + output1Chb.checked + "," + output2Chb.checked + "," +
                                                                        output3Chb.checked + "," + output4Chb.checked + "," + output5Chb.checked + "," +
                                                                        output6Chb.checked + "," + output7Chb.checked
                                                                dbController.setIdx("Outputs", outputEnables, 9)

                                                                dbController.updateOutputs(exfSettingCmb.currentValue, model.ruleID)

                                                                ethController.readDb(exfMainCmb.currentValue)

                                                                showMessageBox("Rule Updated");
                                                            }
                                                            hoverEnabled: true
                                                            onEntered: {
                                                                editOutputBtn.color = "#00d76a"
                                                            }
                                                            onExited: {
                                                                editOutputBtn.color = "#181818"
                                                            }
                                                        }

                                                        // Image to display the icon
                                                        Image {
                                                            source: "qrc:/Icons/edit.png"
                                                            anchors.centerIn: parent
                                                            width: 24
                                                            height: 24
                                                        }
                                                    }

                                                    Rectangle {
                                                        id: deleteOutputBtn
                                                        width: 30
                                                        height: 30
                                                        color: "#181818" // Default background color
                                                        radius: 5

                                                        MouseArea {
                                                            anchors.fill: parent
                                                            onClicked: {
                                                                if(ethController.wLow.length === 1)
                                                                {
                                                                    showMessageBox("Can Not Delete All Rules")
                                                                    return
                                                                }

                                                                dbController.deleteOutputs(exfSettingCmb.currentValue, model.ruleID)
                                                                updateOutputs()
                                                            }

                                                            hoverEnabled: true
                                                            onEntered: {
                                                                deleteOutputBtn.color = "#00d76a"
                                                            }
                                                            onExited: {
                                                                deleteOutputBtn.color = "#181818"
                                                            }

                                                        }

                                                        // Image to display the icon
                                                        Image {
                                                            source: "qrc:/Icons/delete2.png"
                                                            anchors.centerIn: parent
                                                            width: 24
                                                            height: 24
                                                        }
                                                    }

                                                    //     Button{
                                                    //         text: "Edit"
                                                    //         onClicked: {
                                                    //             dbController.readOutput(exfSettingCmb.currentValue, model.output)

                                                    //             let condition = ""
                                                    //             let c1 = ""
                                                    //             let c2 = ""
                                                    //             let c3 = ""
                                                    //             let c4 = ""

                                                    //             let operation = ""
                                                    //             // dbController.setIdxOutputs(exfSettingComboBox.currentValue, 0)
                                                    //             // dbController.setIdxOutputs(model.output, 1)
                                                    //             dbController.setIdx("Outputs", wLow.text, 2)
                                                    //             dbController.setIdx("Outputs", wHigh.text, 3)
                                                    //             condition = `${wLow.text} <= w && w < ${wHigh.text}`
                                                    //             dbController.setIdx("Outputs", condition, 4)

                                                    //             if(densityPopupOpened)
                                                    //             {
                                                    //                 dbController.output[5].split(',')[0] = densities[0]
                                                    //                 dbController.output[5].split(',')[1] = densities[1]
                                                    //                 let str = densities[0] + "," + densities[1] + "," + dbController.output[5].split(',')[2] + "," + dbController.output[5].split(',')[3]
                                                    //                 dbController.setIdx("Outputs", str, 5)
                                                    //                 str = `${densities[0]} <= dens && dens < ${densities[1]}`
                                                    //                 dbController.setIdx("Outputs", str, 6)
                                                    //             }

                                                    //             if(sizePopupOpened)
                                                    //             {
                                                    //                 dbController.output[5].split(',')[2] = sizes[0]
                                                    //                 dbController.output[5].split(',')[3] = sizes[1]
                                                    //                 let str = dbController.output[5].split(',')[0] + "," + dbController.output[5].split(',')[1] + "," + sizes[0] + "," + sizes[1]
                                                    //                 dbController.setIdx("Outputs", str, 5)
                                                    //                 str = `${sizes[0]} <= size && size < ${sizes[1]}`
                                                    //                 dbController.setIdx("Outputs", str, 7)
                                                    //             }

                                                    //             let ops = weightDensityCmb.currentValue + "," + densitySizeCmb.currentValue + "," + sizeDefectCmb.currentValue + "," + defectColorCmb.currentValue
                                                    //             dbController.setIdx("Outputs", ops, 8)

                                                    //             dbController.setIdx("Outputs", defectChb.checked + "," + colorChb.checked, 9)

                                                    //             if(defectPopupOpened)
                                                    //             {
                                                    //                 ops = defectChbValues[0] + "," + defectChbValues[1] + "," + defectChbValues[2] + "," + defectChbValues[3] + "," +
                                                    //                         defectChbValues[4] + "," + defectChbValues[5]
                                                    //                 dbController.setIdx("Outputs", ops, 10)

                                                    //             }

                                                    //             if(colorPopupOpened)
                                                    //             {
                                                    //                 let ranges = colorRanges[0] + "," + colorRanges[1] + "," + colorRanges[2] + "," + colorRanges[3] + "," +
                                                    //                     colorRanges[4] + "," + colorRanges[5] + "," + colorRanges[6] + "," + colorRanges[7]

                                                    //                 dbController.setIdx("Outputs", ranges, 11)

                                                    //                 ops = colorOperations[0] + "," + colorOperations[1] + "," + colorOperations[2]

                                                    //                 dbController.setIdx("Outputs", ops, 12)

                                                    //                 let colorOps = ["&&", "&&",  "&&"]

                                                    //                 if(colorOperations[0] === "AND") colorOps[0] = "&&"
                                                    //                 else colorOps[0] = "||"

                                                    //                 if(colorOperations[1] === "AND") colorOps[1] = "&&"
                                                    //                 else colorOps[1] = "||"

                                                    //                 if(colorOperations[2] === "AND") colorOps[2] = "&&"
                                                    //                 else colorOps[2] = "||"



                                                    //                 c1 = `(${colorRanges[0]} <= rd && rd <= ${colorRanges[1]}) ${colorOps[0]} `
                                                    //                 c2 = `(${colorRanges[2]} <= or && or <= ${colorRanges[3]}) ${colorOps[1]} `
                                                    //                 c3 = `(${colorRanges[4]} <= yl && yl <= ${colorRanges[5]}) ${colorOps[2]} `
                                                    //                 c4 = `(${colorRanges[6]} <= gn && gn <= ${colorRanges[7]})`

                                                    //                 condition = c1 + c2 + c3 + c4

                                                    //                 dbController.setIdx("Outputs", condition, 13)

                                                    //                 colorPopupOpened = false

                                                    //             }

                                                    //             dbController.updateOutputs(exfSettingCmb.currentValue, model.output)

                                                    //             showMessageBox("Output " + model.output + " Setting Updated");
                                                    //         }
                                                    //     }

                                                }

                                                // ToolSeparator{
                                                //     Layout.fillWidth: true
                                                //     orientation: Qt.Horizontal
                                                // }
                                            }



                                        }
                                    }

                                    Item {Layout.fillWidth: true}

                                }

                                Item{
                                    Layout.preferredHeight: 50
                                }

                                RowLayout{
                                    spacing: 20
                                    Layout.alignment: Qt.AlignCenter

                                    // MyButton {
                                    //     text: "Apply"
                                    //     icon_btn: "qrc:/Icons/apply.png"
                                    //     Layout.preferredHeight: 50
                                    //     Layout.preferredWidth: 120
                                    //     Layout.alignment: Qt.AlignHCenter

                                    //     buttonColor: applyColor


                                    //     onClicked: {
                                    //         let setting = []
                                    //         setting.push(shift.text)
                                    //         setting.push(distanceFromLC.text)
                                    //         ethController.applyGeneral(setting)
                                    //         showMessageBox("General Setting Applied")
                                    //     }
                                    // }
                                    MyButton {
                                        id: editGeneral
                                        text: "Edit"
                                        icon_btn: "qrc:/Icons/draw.png"
                                        Layout.preferredHeight: 50
                                        Layout.preferredWidth: 120
                                        Layout.alignment: Qt.AlignHCenter

                                        buttonColor: editColor

                                        onClicked: {
                                            dbController.setIdx("Generals", exfText.text, 0)

                                            showMessageBox("Exit Feature Updated")


                                            dbController.updateGenerals(exfSettingCmb.currentValue)

                                            updateExfs()
                                        }
                                    }
                                    MyButton {
                                        id: deleteGeneral
                                        text: "Delete"
                                        icon_btn: "qrc:/Icons/delete.png"
                                        Layout.preferredHeight: 50
                                        Layout.preferredWidth: 120
                                        Layout.alignment: Qt.AlignHCenter

                                        buttonColor: stopColor

                                        onClicked: {
                                            if(dbController.getGeneralExfs().length === 1)
                                            {
                                                showMessageBox("Can Not Delete All Exit Features")

                                                return
                                            }

                                            dbController.deleteGenerals(exfSettingCmb.currentValue)

                                            showMessageBox("Exit Feature Deleted")

                                            //dbController.readLastRun()

                                            updateExfs()

                                            exfSettingCmb.currentIndex = 0

                                            updateOutputs()

                                            dbController.readGenerals(exfSettingCmb.currentValue)
                                        }

                                    }
                                    MyButton {
                                        id: addGeneral
                                        text: "Add"
                                        icon_btn: "qrc:/Icons/add.png"
                                        Layout.preferredHeight: 50
                                        Layout.preferredWidth: 120
                                        Layout.alignment: Qt.AlignHCenter

                                        buttonColor: startColor

                                        onClicked: {
                                            for(var j=0;j<dbController.getGeneralExfs().length;j++){
                                                if(exfText.text === dbController.getGeneralExfs()[j]){
                                                    showMessageBox("Exit Feature Already Exists")

                                                    return
                                                }
                                            }



                                            dbController.setIdx("Generals", exfText.text, 0)
                                            dbController.insert("Generals")

                                            dbController.setIdx("Outputs", exfText.text, 0)
                                            dbController.setIdx("Outputs", 0, 1)
                                            dbController.setIdx("Outputs", "0", 2)
                                            dbController.setIdx("Outputs", "100", 3)
                                            dbController.setIdx("Outputs", "0,100,0,100", 4)
                                            dbController.setIdx("Outputs", "false,false", 5)
                                            dbController.setIdx("Outputs", "true,true,true,true,true,true", 6)
                                            dbController.setIdx("Outputs", "0,100,0,100,0,100,0,100", 7)
                                            dbController.setIdx("Outputs", "", 8)
                                            dbController.setIdx("Outputs", "false,false,false,false,false,false,false,false", 9)

                                            dbController.insert("Outputs")

                                            updateOutputs()

                                            //addooooo

                                            // for(var i=0;i<ethController.outputs.length;i++)
                                            // {
                                            //     dbController.setIdx("Outputs", exfText.text, 0)
                                            //     dbController.setIdx("Outputs", "0", 1)
                                            //     dbController.setIdx("Outputs", "100", 2)
                                            //     dbController.setIdx("Outputs", "0,100,0,100", 3)
                                            //     dbController.setIdx("Outputs", "false,false", 4)
                                            //     dbController.setIdx("Outputs", "true,true,true,true,true,true", 5)
                                            //     dbController.setIdx("Outputs", "0,100,0,100,0,100,0,100", 6)
                                            //     dbController.setIdx("Outputs", "", 7)
                                            //     dbController.setIdx("Outputs", "false,false,false,false,false,false,false,false", 8)

                                            //     dbController.insert("Outputs")
                                            // }

                                            // updateOutputs()

                                            showMessageBox("Exit Feature Added")

                                            //dbController.readLastRun()

                                            updateExfs()

                                            exfSettingCmb.currentIndex = exfSettingCmb.find(exfSettingCmb.currentValue)
                                            dbController.readGenerals(exfSettingCmb.currentValue)
                                        }
                                    }
                                    TextField {
                                        id: exfText
                                        text: dbController.general[0]
                                        color: "white"
                                        Layout.preferredWidth: 100
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter

                                    }

                                }

                            }


                        }
                        //                        RowLayout{
                        //                            //                            Layout.preferredWidth: parent.width
                        //                            spacing: 15
                        //                            Layout.alignment: Qt.AlignHCenter

                        //                            MyButton {
                        //                                id: startBtnSetting
                        //                                text: "Start"
                        //                                icon_btn: "qrc:/Icons/start.png"
                        //                                Layout.preferredHeight: 50
                        //                                Layout.alignment: Qt.AlignHCenter

                        //                                onClicked: {
                        //                                    //                                    weightController.flagSetting = true
                        //                                    //                                    startBtnSetting.enabled = false;
                        //                                    //                                    stopBtnSetting.enabled = true;
                        //                                    weightController.start(exfMainComboBox.currentValue, speedMainComboBox.currentValue)
                        //                                }
                        //                            }
                        //                            MyButton {
                        //                                id: stopBtnSetting
                        //                                //Layout: true
                        //                                text: "Stop"
                        //                                icon_btn: "qrc:/Icons/stop.png"
                        //                                Layout.preferredHeight: 50
                        //                                Layout.alignment: Qt.AlignHCenter


                        //                                onClicked: {
                        //                                    //                                    startBtnSetting.enabled = false;
                        //                                    //                                    stopBtnSetting.enabled = true;
                        //                                    weightController.stop()
                        //                                }
                        //                            }
                        //                        }

                        //                        Rectangle{
                        //                            Layout.preferredWidth: parent.width - 10
                        //                            Layout.preferredHeight: 330

                        //                            Layout.alignment: Qt.AlignHCenter

                        //                            color: rectColor

                        //                            border.color: "white"
                        //                            radius: 10
                        //                            border.width: 0

                        //                            ColumnLayout{
                        //                                anchors.fill: parent

                        //                                RowLayout {
                        //                                    Layout.preferredWidth: parent.width
                        //                                    spacing: 30

                        //                                    ColumnLayout{
                        //                                        spacing: -10
                        //                                        Layout.alignment: Qt.AlignHCenter

                        //                                        Label {text: "Loading Conveyor"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        //                                        Item{
                        //                                            Layout.preferredHeight: 20
                        //                                        }
                        //                                        Switch {
                        //                                            text: "Auto"
                        //                                            checked: true

                        //                                            onToggled: {
                        //                                                text = checked ? "Auto" : "Manual"
                        //                                                engine0.checked = weightController.mbBinary[0]
                        //                                                weightController.firstAuto[0] = checked
                        //                                                weightController.setIdxFirstMb(0, engine0.checked)
                        //                                            }
                        //                                        }
                        //                                        Switch {
                        //                                            id: engine0
                        //                                            text: dbController.engine[0]
                        //                                            checked: dbController.engine[0] === "On" ? true : false

                        //                                            onToggled: {
                        //                                                text = checked ? "ON" : "OFF"

                        //                                                dbController.engine[0] = checked ? "On" : "Off"

                        //                                                weightController.setIdxFirstMb(0, checked)
                        //                                            }
                        //                                            onCheckedChanged: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                            }
                        //                                        }
                        //                                    }

                        //                                    ColumnLayout{
                        //                                        spacing: -10
                        //                                        Layout.alignment: Qt.AlignHCenter
                        //                                        Label {text: "Lift Conveyor"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        //                                        Item{
                        //                                            Layout.preferredHeight: 20
                        //                                        }
                        //                                        Switch {
                        //                                            text: "Auto"
                        //                                            checked: true

                        //                                            onToggled: {
                        //                                                text = checked ? "Auto" : "Manual"
                        //                                                engine1.checked = weightController.mbBinary[1]
                        //                                                weightController.firstAuto[1] = checked
                        //                                                weightController.setIdxFirstMb(1, engine1.checked)

                        //                                            }
                        //                                        }
                        //                                        Switch {
                        //                                            id: engine1
                        //                                            text: dbController.engine[1]
                        //                                            checked: dbController.engine[1] === "On" ? true : false

                        //                                            onToggled: {
                        //                                                text = checked ? "ON" : "OFF"

                        //                                                dbController.engine[1] = checked ? "On" : "Off"

                        //                                                weightController.setIdxFirstMb(1, checked)
                        //                                            }
                        //                                            onCheckedChanged: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                            }
                        //                                        }
                        //                                    }
                        //                                    ColumnLayout{
                        //                                        spacing: -10
                        //                                        Layout.alignment: Qt.AlignHCenter
                        //                                        Label {text: "Check Conveyor"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        //                                        Item{
                        //                                            Layout.preferredHeight: 20
                        //                                        }
                        //                                        Switch {
                        //                                            text: "Auto"
                        //                                            checked: true

                        //                                            onToggled: {
                        //                                                text = checked ? "Auto" : "Manual"
                        //                                                engine2.checked = weightController.mbBinary[2]
                        //                                                weightController.firstAuto[2] = checked
                        //                                                weightController.setIdxFirstMb(2, engine2.checked)

                        //                                            }
                        //                                        }
                        //                                        Switch {
                        //                                            id: engine2
                        //                                            text: dbController.engine[2]
                        //                                            checked: dbController.engine[2] === "On" ? true : false

                        //                                            onToggled: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                                dbController.engine[2] = checked ? "On" : "Off"

                        //                                                weightController.setIdxFirstMb(2, checked)
                        //                                            }
                        //                                            onCheckedChanged: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                            }
                        //                                        }
                        //                                    }
                        //                                    ColumnLayout{
                        //                                        spacing: -10
                        //                                        Layout.alignment: Qt.AlignHCenter


                        //                                        Label {text: "Brusher"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        //                                        Item{

                        //                                            Layout.preferredHeight: 20
                        //                                        }
                        //                                        Switch {
                        //                                            text: "Auto"
                        //                                            checked: true

                        //                                            onToggled: {
                        //                                                text = checked ? "Auto" : "Manual"
                        //                                                engine3.checked = weightController.mbBinary[3]
                        //                                                weightController.firstAuto[3] = checked
                        //                                                weightController.setIdxFirstMb(3, engine3.checked)
                        //                                            }
                        //                                        }
                        //                                        Switch {
                        //                                            id: engine3
                        //                                            text: dbController.engine[3]
                        //                                            checked: dbController.engine[3] === "On" ? true : false

                        //                                            onToggled: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                                dbController.engine[3] = checked ? "On" : "Off"

                        //                                                weightController.setIdxFirstMb(3, checked)
                        //                                            }
                        //                                            onCheckedChanged: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                            }
                        //                                        }
                        //                                    }

                        //                                }
                        //                                RowLayout {
                        //                                    Layout.preferredWidth: parent.width
                        //                                    spacing: 30

                        //                                    ColumnLayout{
                        //                                        spacing: -10
                        //                                        Layout.alignment: Qt.AlignHCenter

                        //                                        Label {text: "Pre Aligner"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        //                                        Item{
                        //                                            Layout.preferredHeight: 20
                        //                                        }
                        //                                        Switch {
                        //                                            text: "Auto"
                        //                                            checked: true

                        //                                            onToggled: {
                        //                                                text = checked ? "Auto" : "Manual"
                        //                                                engine4.checked = weightController.mbBinary[4]

                        //                                                weightController.firstAuto[4] = checked
                        //                                                weightController.setIdxFirstMb(4, engine4.checked)
                        //                                            }
                        //                                        }
                        //                                        Switch {
                        //                                            id: engine4
                        //                                            text: dbController.engine[4]
                        //                                            checked: dbController.engine[4] === "On" ? true : false

                        //                                            onToggled: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                                dbController.engine[4] = checked ? "On" : "Off"

                        //                                                weightController.setIdxFirstMb(4, checked)
                        //                                            }
                        //                                            onCheckedChanged: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                            }
                        //                                        }
                        //                                    }

                        //                                    ColumnLayout{
                        //                                        spacing: -10
                        //                                        Layout.alignment: Qt.AlignHCenter

                        //                                        Label {text: "Aligner"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        //                                        Item{
                        //                                            Layout.preferredHeight: 20
                        //                                        }
                        //                                        Switch {
                        //                                            text: "Auto"
                        //                                            checked: true

                        //                                            onToggled: {
                        //                                                text = checked ? "Auto" : "Manual"
                        //                                                engine5.checked = weightController.mbBinary[5]

                        //                                                weightController.firstAuto[5] = checked
                        //                                                weightController.setIdxFirstMb(5, engine5.checked)
                        //                                            }
                        //                                        }
                        //                                        Switch {
                        //                                            id: engine5
                        //                                            text: dbController.engine[5]
                        //                                            checked: dbController.engine[5] === "On" ? true : false

                        //                                            onToggled: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                                dbController.engine[5] = checked ? "On" : "Off"

                        //                                                weightController.setIdxFirstMb(5, checked)
                        //                                            }
                        //                                            onCheckedChanged: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                            }
                        //                                        }
                        //                                    }
                        //                                    ColumnLayout{
                        //                                        spacing: -10
                        //                                        Layout.alignment: Qt.AlignHCenter

                        //                                        Label {text: "Grader"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        //                                        Item{
                        //                                            Layout.preferredHeight: 20
                        //                                        }
                        //                                        Switch {
                        //                                            text: "Auto"
                        //                                            checked: true

                        //                                            onToggled: {
                        //                                                text = checked ? "Auto" : "Manual"
                        //                                                engine6.checked = weightController.mbBinary[6]
                        //                                                weightController.firstAuto[6] = checked
                        //                                                weightController.setIdxFirstMb(6, engine6.checked)
                        //                                            }
                        //                                        }
                        //                                        Switch {
                        //                                            id: engine6
                        //                                            text: dbController.engine[6]
                        //                                            checked: dbController.engine[6] === "On" ? true : false


                        //                                            onToggled: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                                dbController.engine[6] = checked ? "On" : "Off"

                        //                                                weightController.setIdxFirstMb(6, checked)
                        //                                            }
                        //                                            onCheckedChanged: {
                        //                                                text = checked ? "ON" : "OFF"
                        //                                            }
                        //                                        }
                        //                                    }

                        //                                }

                        //                                MyButton {
                        //                                    text: "Edit"
                        //                                    icon_btn: "qrc:/Icons/draw.png"
                        //                                    Layout.preferredHeight: 50
                        //                                    Layout.preferredWidth: 120
                        //                                    Layout.alignment: Qt.AlignHCenter

                        //                                    buttonColor: editColor


                        //                                    onClicked: {
                        //                                        dbController.updateEngine()

                        //                                        messageBox.msgText = "Engines Updated"
                        //                                        messageBox.timer = true
                        //                                        messageBox.open()
                        //                                    }
                        //                                }
                        //                            }


                        //                        }


                        //                        Item {
                        //                            Layout.preferredWidth: 500
                        //                        }
                    }

                }

                Item {
                    id: ejectorsTab

                    Rectangle{
                        anchors.centerIn: parent
                        width: 1200
                        height: 600
                        radius: 10
                        color: rectColor

                        ColumnLayout{
                            anchors.centerIn: parent
                            spacing: 50
//eoooooooooooooooooooooooooooo
                            Text {
                                text: "Line A"
                                color: "white"
                                Layout.preferredWidth: 110
                                font.pixelSize: 16
                                Layout.alignment: Qt.AlignHCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            RowLayout{
                                Layout.alignment: Qt.AlignHCenter
                                spacing: 40
                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "A"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: aaIp
                                        text: dbController.ejectorA[0].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: aaOutput
                                        text: dbController.ejectorA[0].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "B"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: abIp
                                        text: dbController.ejectorA[1].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: abOutput
                                        text: dbController.ejectorA[1].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "C"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: acIp
                                        text: dbController.ejectorA[2].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: acOutput
                                        text: dbController.ejectorA[2].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "D"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: adIp
                                        text: dbController.ejectorA[3].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }

                                    TextField {
                                        id: adOutput
                                        text: dbController.ejectorA[3].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "E"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: aeIp
                                        text: dbController.ejectorA[4].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: aeOutput
                                        text: dbController.ejectorA[4].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "F"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: afIp
                                        text: dbController.ejectorA[5].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }

                                    TextField {
                                        id: afOutput
                                        text: dbController.ejectorA[5].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "G"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: agIp
                                        text: dbController.ejectorA[6].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 100
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: agOutput
                                        text: dbController.ejectorA[6].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 50
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "H"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: ahIp
                                        text: dbController.ejectorA[7].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: ahOutput
                                        text: dbController.ejectorA[7].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                            }


                            Text {
                                text: "Line B"
                                color: "white"
                                Layout.preferredWidth: 110
                                font.pixelSize: 16
                                Layout.alignment: Qt.AlignHCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            RowLayout{
                                Layout.alignment: Qt.AlignHCenter
                                spacing: 40
                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "A"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: baIp
                                        text: dbController.ejectorB[0].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: baOutput
                                        text: dbController.ejectorB[0].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "B"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: bbIp
                                        text: dbController.ejectorB[1].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: bbOutput
                                        text: dbController.ejectorB[1].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "C"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: bcIp
                                        text: dbController.ejectorB[2].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: bcOutput
                                        text: dbController.ejectorB[2].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "D"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: bdIp
                                        text: dbController.ejectorB[3].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }

                                    TextField {
                                        id: bdOutput
                                        text: dbController.ejectorB[3].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "E"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: beIp
                                        text: dbController.ejectorB[4].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: beOutput
                                        text: dbController.ejectorB[4].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "F"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: bfIp
                                        text: dbController.ejectorB[5].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }

                                    TextField {
                                        id: bfOutput
                                        text: dbController.ejectorB[5].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "G"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: bgIp
                                        text: dbController.ejectorB[6].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 100
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: bgOutput
                                        text: dbController.ejectorB[6].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 50
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                ColumnLayout{
                                    spacing: 15
                                    Label {text: "H"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    TextField {
                                        id: bhIp
                                        text: dbController.ejectorB[7].split(',')[0]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    TextField {
                                        id: bhOutput
                                        text: dbController.ejectorB[7].split(',')[1]
                                        color: "white"
                                        Layout.preferredWidth: 110
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                            }

                            MyButton {
                                text: "Edit"
                                icon_btn: "qrc:/Icons/draw.png"
                                Layout.preferredHeight: 50
                                Layout.preferredWidth: 120
                                Layout.alignment: Qt.AlignHCenter

                                buttonColor: editColor

                                onClicked: {
                                    dbController.setIdx("Ejector_A", aaIp.text + "," + aaOutput.text, 0)
                                    dbController.setIdx("Ejector_A", abIp.text + "," + abOutput.text, 1)
                                    dbController.setIdx("Ejector_A", acIp.text + "," + acOutput.text, 2)
                                    dbController.setIdx("Ejector_A", adIp.text + "," + adOutput.text, 3)
                                    dbController.setIdx("Ejector_A", aeIp.text + "," + aeOutput.text, 4)
                                    dbController.setIdx("Ejector_A", afIp.text + "," + afOutput.text, 5)
                                    dbController.setIdx("Ejector_A", agIp.text + "," + agOutput.text, 6)
                                    dbController.setIdx("Ejector_A", ahIp.text + "," + ahOutput.text, 7)
                                    dbController.updateEjectorA()

                                    dbController.setIdx("Ejector_B", baIp.text + "," + baOutput.text, 0)
                                    dbController.setIdx("Ejector_B", bbIp.text + "," + bbOutput.text, 1)
                                    dbController.setIdx("Ejector_B", bcIp.text + "," + bcOutput.text, 2)
                                    dbController.setIdx("Ejector_B", bdIp.text + "," + bdOutput.text, 3)
                                    dbController.setIdx("Ejector_B", beIp.text + "," + beOutput.text, 4)
                                    dbController.setIdx("Ejector_B", bfIp.text + "," + bfOutput.text, 5)
                                    dbController.setIdx("Ejector_B", bgIp.text + "," + bgOutput.text, 6)
                                    dbController.setIdx("Ejector_B", bhIp.text + "," + bhOutput.text, 7)
                                    dbController.updateEjectorA()

                                    showMessageBox("Ejectors Setting Updated")
                                }
                            }

                        }


                    }


                }


                Item {
                    id: lnaTab

                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height

                    RowLayout{
                        anchors.fill: parent
                        spacing: 5

                        ColumnLayout{
                            Layout.preferredHeight: parent.height
                            Rectangle {
                                Layout.preferredWidth: 800
                                Layout.preferredHeight: 500

                                color: rectColor

                                border.color: "white"
                                radius: 10
                                border.width: 0

                                ColumnLayout {
                                    spacing: 0
                                    anchors.fill: parent
                                    Layout.alignment: Qt.AlignCenter
                                    RowLayout{
                                        spacing: 10
                                        Layout.leftMargin: 300
                                        Label {text: "Speed : "; font.pixelSize: 15; color: "white"}
                                        ComboBox {
                                            id: lnaSpeedComboBox
                                            Layout.alignment: Qt.AlignHCenter

                                            Layout.preferredWidth: 150

                                            model: ListModel {
                                                ListElement { text: "2" }
                                                ListElement { text: "6" }
                                                ListElement { text: "8" }
                                                // Add more ListElements as needed
                                            }
                                            // Initial selected index
                                            currentIndex: 0

                                            Component.onCompleted: {
                                                dbController.readLna(currentValue)
                                            }

                                            onCurrentValueChanged: {
                                                dbController.readLna(currentValue)
                                            }
                                        }
                                    }
                                    RowLayout {
                                        spacing: 0

                                        Item {Layout.fillWidth: true}

                                        ColumnLayout{
                                            Layout.topMargin: 20

                                            Label {text: ""; font.pixelSize: 15; color: "white"}
                                            Label {text: "Cup"; font.pixelSize: 15; color: "white"}
                                            Item{
                                                Layout.preferredHeight: 15
                                            }
                                            Label {text: "Test"; font.pixelSize: 15; color: "white"}
                                            Item{
                                                Layout.preferredHeight: 2
                                            }
                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "A"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item {
                                                Layout.preferredHeight: 15
                                            }

                                            TextField {
                                                id: aaCup
                                                text: dbController.lnaSetting[7]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestLeft(0, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60

                                                iconsize: 0

                                                radius: 2

                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(0, 0) : showMessageBox("Program Is Not Started")
                                                }
                                            }
                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "B"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item{
                                                Layout.preferredHeight: 15
                                            }

                                            TextField {
                                                id: abCup
                                                text: dbController.lnaSetting[8]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestLeft(1, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60

                                                iconsize: 0

                                                radius: 2

                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(0, 1) : showMessageBox("Program Is Not Started")
                                                }
                                            }

                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "C"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item{
                                                Layout.preferredHeight: 15
                                            }

                                            TextField {
                                                id: acCup
                                                text: dbController.lnaSetting[9]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }

                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestLeft(2, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(0, 2) : showMessageBox("Program Is Not Started")
                                                }
                                            }

                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "D"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item{
                                                Layout.preferredHeight: 15
                                            }

                                            TextField {
                                                id: adCup
                                                text: dbController.lnaSetting[10]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestLeft(3, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(0, 3) : showMessageBox("Program Is Not Started")
                                                }
                                            }

                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "E"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item{
                                                Layout.preferredHeight: 15
                                            }

                                            TextField {
                                                id: aeCup
                                                text: dbController.lnaSetting[11]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestLeft(4, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(0, 4) : showMessageBox("Program Is Not Started")
                                                }
                                            }

                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "F"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item{
                                                Layout.preferredHeight: 15
                                            }

                                            TextField {
                                                id: afCup
                                                text: dbController.lnaSetting[12]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestLeft(5, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(0, 5) : showMessageBox("Program Is Not Started")
                                                }
                                            }

                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "G"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item{
                                                Layout.preferredHeight: 15
                                            }

                                            TextField {
                                                id: agCup
                                                text: dbController.lnaSetting[13]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestLeft(6, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(0, 6) : showMessageBox("Program Is Not Started")
                                                }
                                            }
                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "H"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item{
                                                Layout.preferredHeight: 15
                                            }

                                            TextField {
                                                id: ahCup
                                                text: dbController.lnaSetting[14]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestLeft(7, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(0, 7) : showMessageBox("Program Is Not Started")
                                                }
                                            }
                                        }

                                        Item {Layout.fillWidth: true}

                                    }

                                    ToolSeparator{ Layout.fillWidth: true; orientation: Qt.Horizontal}

                                    RowLayout{
                                        spacing: 15
                                        Item{
                                            Layout.fillWidth: true
                                        }
                                        Label {text: "Min_Pos"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: aLow
                                            text: dbController.lnaSetting[5]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Label {text: "Max_Pos"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: aHigh
                                            text: dbController.lnaSetting[6]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Label {text: "KFactor"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: aKFactor
                                            text: dbController.lnaSetting[1]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Label {text: "Energize"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: aEnergize
                                            text: dbController.lnaSetting[16]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Item{
                                            Layout.fillWidth: true
                                        }
                                    }

                                    ToolSeparator{ Layout.fillWidth: true; orientation: Qt.Horizontal}

                                    RowLayout{
                                        spacing:15
                                        Layout.preferredWidth: parent.width
                                        Layout.alignment: Qt.AlignHCenter
                                        Item{
                                            Layout.fillWidth: true
                                        }
                                        Label {text: "Min"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: aMin
                                            text: dbController.lnaSetting[2]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Label {text: "Max"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: aMax
                                            text: dbController.lnaSetting[3]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Label {text: "Average"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: aAverage
                                            text: dbController.lnaSetting[4]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Label {text: "Error Range"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: aError
                                            text: dbController.lnaSetting[15]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Item{
                                            Layout.fillWidth: true
                                        }
                                    }
                                    ToolSeparator{ Layout.fillWidth: true; orientation: Qt.Horizontal}

                                    RowLayout{
                                        spacing: 20
                                        Layout.alignment: Qt.AlignCenter

                                        Label {text: "Chart"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        CheckBox{
                                            id: chartA
                                            onCheckedChanged: {
                                                weightController.flagChartA = checked
                                            }
                                        }

                                        Label {text: "Table"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        CheckBox{
                                            id: tableA
                                            onCheckedChanged: {
                                                weightController.flagTableA = checked
                                            }
                                        }

                                        MyButton {
                                            text: "Apply"
                                            icon_btn: "qrc:/Icons/apply.png"
                                            Layout.preferredHeight: 50
                                            Layout.preferredWidth: 120
                                            Layout.alignment: Qt.AlignHCenter

                                            buttonColor: applyColor

                                            onClicked: {
                                                let setting = []
                                                setting.push(aKFactor.text)
                                                setting.push(aMin.text)
                                                setting.push(aMax.text)
                                                setting.push(aAverage.text)
                                                setting.push(aLow.text)
                                                setting.push(aHigh.text)
                                                setting.push(aaCup.text)
                                                setting.push(abCup.text)
                                                setting.push(acCup.text)
                                                setting.push(adCup.text)
                                                setting.push(aeCup.text)
                                                setting.push(afCup.text)
                                                setting.push(agCup.text)
                                                setting.push(ahCup.text)
                                                setting.push(aError.text)
                                                setting.push(aEnergize.text)
                                                weightController.applySetting(0, setting)
                                                ethController.applyLineA(setting)

                                                showMessageBox("Line A Setting Applied")
                                            }
                                        }
                                        MyButton {
                                            text: "Edit"
                                            icon_btn: "qrc:/Icons/draw.png"
                                            Layout.preferredHeight: 50
                                            Layout.preferredWidth: 120
                                            Layout.alignment: Qt.AlignHCenter

                                            buttonColor: editColor

                                            onClicked: {
                                                dbController.setIdx("Line_A", aKFactor.text, 1)
                                                dbController.setIdx("Line_A", aMin.text, 2)
                                                dbController.setIdx("Line_A", aMax.text, 3)
                                                dbController.setIdx("Line_A", aAverage.text, 4)
                                                dbController.setIdx("Line_A", aLow.text, 5)
                                                dbController.setIdx("Line_A", aHigh.text, 6)
                                                dbController.setIdx("Line_A", aaCup.text, 7)
                                                dbController.setIdx("Line_A", abCup.text, 8)
                                                dbController.setIdx("Line_A", acCup.text, 9)
                                                dbController.setIdx("Line_A", adCup.text, 10)
                                                dbController.setIdx("Line_A", aeCup.text, 11)
                                                dbController.setIdx("Line_A", afCup.text, 12)
                                                dbController.setIdx("Line_A", agCup.text, 13)
                                                dbController.setIdx("Line_A", ahCup.text, 14)
                                                dbController.setIdx("Line_A", aError.text, 15)
                                                dbController.setIdx("Line_A", aEnergize.text, 16)
                                                dbController.updateLna(lnaSpeedComboBox.currentValue)

                                                dbController.setIdx("Ejector_A", aaIp.text + "," + aaOutput.text, 0)
                                                dbController.setIdx("Ejector_A", abIp.text + "," + abOutput.text, 1)
                                                dbController.setIdx("Ejector_A", acIp.text + "," + acOutput.text, 2)
                                                dbController.setIdx("Ejector_A", adIp.text + "," + adOutput.text, 3)
                                                dbController.setIdx("Ejector_A", aeIp.text + "," + aeOutput.text, 4)
                                                dbController.setIdx("Ejector_A", afIp.text + "," + afOutput.text, 5)
                                                dbController.setIdx("Ejector_A", agIp.text + "," + agOutput.text, 6)
                                                dbController.setIdx("Ejector_A", ahIp.text + "," + ahOutput.text, 7)
                                                dbController.updateEjectorA()

                                                showMessageBox("Line A Setting Updated")
                                            }
                                        }

                                        CheckBox{
                                            onCheckedChanged: {
                                                ethController.errorCupA = checked
                                            }
                                        }
                                        TextField {
                                            text: "10"
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                            Component.onCompleted: {
                                                ethController.errorCupValueA = parseInt(text)
                                            }

                                            onTextChanged: {
                                                ethController.errorCupValueA = parseInt(text)
                                                console.log(ethController.errorCupValueA)
                                            }
                                        }
                                    }

                                }

                            }

                            ChartView {
                                Layout.preferredHeight: 400
                                Layout.preferredWidth: 800

                                // Main data LineSeries
                                LineSeries {
                                    name: "Line A"
                                    color: "blue"
                                    id: lnaPoints
                                    axisX: axisXa
                                    axisY: axisYa
                                    width: 3

                                    Repeater {
                                        model: Math.min(weightController.posLui.length, weightController.weightLui.length)
                                    }
                                }

                                // First vertical line
                                LineSeries {
                                    id: verticalLine1
                                    color: "red"
                                    axisX: axisXa
                                    axisY: axisYa
                                    width: 3

                                    // Adjust x and y values to place the line at the desired position
                                    XYPoint { x: 30; y: weightController.minAxisA } // Start point
                                    XYPoint { x: 30; y: weightController.maxAxisA } // End point
                                }

                                // Second vertical line
                                LineSeries {
                                    id: verticalLine2
                                    color: "red"
                                    axisX: axisXa
                                    axisY: axisYa
                                    width: 3

                                    // Adjust x and y values to place the line at the desired position
                                    XYPoint { x: 70; y: weightController.minAxisA } // Start point
                                    XYPoint { x: 70; y: weightController.maxAxisA } // End point
                                }

                                ValueAxis {
                                    id: axisXa
                                    titleText: "Position"
                                    min: 0
                                    max: 100
                                    tickCount: 11
                                }

                                ValueAxis {
                                    id: axisYa
                                    titleText: "Weight"
                                    min: weightController.minAxisA
                                    max: weightController.maxAxisA
                                    tickCount: 10
                                }

                                Connections {
                                    target: weightController

                                    function onWeightLeftChanged() {
                                        lnaPoints.clear()
                                        verticalLine1.clear()
                                        verticalLine2.clear()
                                        //console.log(weightController.posLui, weightController.weightLui)

                                        for (var i = 0; i < weightController.posLui.length; i++) {
                                            lnaPoints.append(weightController.posLui[i], weightController.weightLui[i])
                                        }

                                        verticalLine1.append(dbController.lnaSetting[5], weightController.minAxisA)
                                        verticalLine1.append(dbController.lnaSetting[5], weightController.maxAxisA)

                                        verticalLine2.append(dbController.lnaSetting[6], weightController.minAxisA)
                                        verticalLine2.append(dbController.lnaSetting[6], weightController.maxAxisA)
                                    }
                                }
                            }
                            Item{
                                Layout.fillHeight: true
                            }
                        }


                        ColumnLayout{
                            Layout.preferredHeight: parent.width

                            Item{
                                Layout.fillHeight: true
                            }

                            TableView {
                                Layout.preferredWidth: 400
                                Layout.preferredHeight: 35
                                Layout.alignment: Qt.AlignCenter
                                rowSpacing: 1
                                clip: true

                                model: TableModel {
                                    TableModelColumn { display: "1" }
                                    TableModelColumn { display: "2" }
                                    TableModelColumn { display: "3" }
                                    TableModelColumn { display: "4" }
                                    TableModelColumn { display: "5" }
                                    TableModelColumn { display: "6" }

                                    rows: [
                                        {
                                            "1": "Cup",
                                            "2": "LC",
                                            "3": "Gross",
                                            "4": "Tare",
                                            "5": "Net",
                                            "6": "Tare*"
                                        }
                                    ]
                                }

                                delegate: Rectangle {
                                    implicitWidth: 63
                                    implicitHeight: 35
                                    border.width: 1

                                    Text {
                                        text: model.display
                                        anchors.centerIn: parent
                                    }
                                }
                            }
                            TableView {
                                Layout.preferredWidth: 400
                                Layout.preferredHeight: 670
                                Layout.alignment: Qt.AlignCenter
                                rowSpacing: 1
                                clip: true

                                model: TableModel {
                                    id: tableModelA
                                    TableModelColumn { display: "1" }
                                    TableModelColumn { display: "2" }
                                    TableModelColumn { display: "3" }
                                    TableModelColumn { display: "4" }
                                    TableModelColumn { display: "5" }
                                    TableModelColumn { display: "6" }


                                }

                                delegate: Rectangle {
                                    implicitWidth: 63
                                    implicitHeight: 35
                                    border.width: 1

                                    Text {
                                        text: model.display
                                        anchors.centerIn: parent
                                    }
                                }

                                Connections {
                                    target: weightController

                                    function onDataTableAChanged() {
                                        var data = []

                                        for (var i=0; i < weightController.dataTableA.length; ++i){
                                            data.push(weightController.dataTableA[i])
                                        }

                                        tableModelA.setRow((parseInt(data[0]) % 283) % 18, { "1": data[0], "2": data[1], "3": data[2], "4": data[3], "5": data[4], "6": data[5] })
                                    }
                                }
                            }

                            Item{
                                Layout.fillHeight: true
                            }
                        }

                    }

                }

                Item {
                    id: lnbTab

                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height

                    RowLayout{
                        anchors.fill: parent
                        spacing: 5

                        ColumnLayout{
                            Layout.preferredHeight: parent.height

                            Rectangle {
                                Layout.preferredWidth: 800
                                Layout.preferredHeight: 500

                                color: rectColor

                                border.color: "white"
                                radius: 10
                                border.width: 0

                                ColumnLayout {
                                    spacing:0
                                    anchors.fill: parent
                                    Layout.alignment: Qt.AlignCenter
                                    RowLayout{
                                        Layout.leftMargin: 300
                                        spacing: 10
                                        Label {text: "Speed : "; font.pixelSize: 15; color: "white"}
                                        ComboBox {
                                            id: lnbSpeedComboBox
                                            Layout.alignment: Qt.AlignHCenter

                                            Layout.preferredWidth: 150


                                            model: ListModel {
                                                ListElement { text: "2" }
                                                ListElement { text: "6" }
                                                ListElement { text: "8" }
                                                // Add more ListElements as needed
                                            }
                                            // Initial selected index
                                            currentIndex: 0

                                            Component.onCompleted: {
                                                dbController.readLnb(currentValue)
                                            }

                                            onCurrentValueChanged: {
                                                dbController.readLnb(currentValue)
                                            }
                                        }
                                    }
                                    RowLayout {
                                        spacing: 0

                                        Item {Layout.fillWidth: true}

                                        ColumnLayout{
                                            ColumnLayout{
                                                Layout.topMargin: 20
                                                Label {text: ""; font.pixelSize: 15; color: "white"}
                                                Label {text: "Cup"; font.pixelSize: 15; color: "white"}
                                                Item{
                                                    Layout.preferredHeight: 15
                                                }
                                                Label {text: "Test"; font.pixelSize: 15; color: "white"}
                                                Item{
                                                    Layout.preferredHeight: 2
                                                }
                                            }
                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "A"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item {
                                                Layout.preferredHeight: 15
                                            }
                                            TextField {
                                                id: baCup
                                                text: dbController.lnbSetting[7]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestRight(0, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(1, 0) : showMessageBox("Program Is Not Started")
                                                }
                                            }
                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "B"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item {
                                                Layout.preferredHeight: 15
                                            }
                                            TextField {
                                                id: bbCup
                                                text: dbController.lnbSetting[8]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestRight(1, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(1, 1) : showMessageBox("Program Is Not Started")
                                                }
                                            }

                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "C"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item {
                                                Layout.preferredHeight: 15
                                            }
                                            TextField {
                                                id: bcCup
                                                text: dbController.lnbSetting[9]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestRight(2, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(1, 2) : showMessageBox("Program Is Not Started")
                                                }
                                            }
                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "D"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item {
                                                Layout.preferredHeight: 15
                                            }
                                            TextField {
                                                id: bdCup
                                                text: dbController.lnbSetting[10]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestRight(3, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(1, 3) : showMessageBox("Program Is Not Started")
                                                }
                                            }
                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "E"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item {
                                                Layout.preferredHeight: 15
                                            }
                                            TextField {
                                                id: beCup
                                                text: dbController.lnbSetting[11]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestRight(4, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(1, 4) : showMessageBox("Program Is Not Started")
                                                }
                                            }

                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "F"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item {
                                                Layout.preferredHeight: 15
                                            }
                                            TextField {
                                                id: bfCup
                                                text: dbController.lnbSetting[12]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestRight(5, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(1, 5) : showMessageBox("Program Is Not Started")
                                                }
                                            }

                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "G"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item {
                                                Layout.preferredHeight: 15
                                            }
                                            TextField {
                                                id: bgCup
                                                text: dbController.lnbSetting[13]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestRight(6, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(1, 6) : showMessageBox("Program Is Not Started")
                                                }
                                            }
                                        }

                                        Item {Layout.preferredWidth: 30}

                                        ColumnLayout{
                                            Layout.topMargin: 40
                                            spacing: 5
                                            Label {text: "H"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                            Item {
                                                Layout.preferredHeight: 15
                                            }
                                            TextField {
                                                id: bhCup
                                                text: dbController.lnbSetting[14]
                                                color: "white"
                                                Layout.preferredWidth: 50
                                                font.pixelSize: 16
                                                selectByMouse: true
                                                Layout.alignment: Qt.AlignHCenter
                                                horizontalAlignment: Text.AlignHCenter
                                            }
                                            CheckBox {
                                                Layout.alignment: Qt.AlignHCenter
                                                onCheckedChanged: {
                                                    ethController.setIdxTestRight(7, checked)
                                                }
                                            }
                                            MyButton {
                                                Layout.preferredHeight: 40
                                                Layout.preferredWidth: 60
                                                Layout.alignment: Qt.AlignHCenter

                                                iconsize: 0
                                                radius: 2
                                                buttonColor: testColor

                                                text: "Test"
                                                onClicked: {
                                                    programStarted === true ? ethController.testEachEjector(1, 7) : showMessageBox("Program Is Not Started")
                                                }
                                            }
                                        }

                                        Item {Layout.fillWidth: true}
                                    }
                                    ToolSeparator{ Layout.fillWidth: true; orientation: Qt.Horizontal}

                                    RowLayout{
                                        spacing: 15
                                        Item{
                                            Layout.fillWidth: true
                                        }
                                        Label {text: "Min_Pos"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: bLow
                                            text: dbController.lnbSetting[5]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Label {text: "Max_Pos"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: bHigh
                                            text: dbController.lnbSetting[6]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Label {text: "KFactor"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: bKFactor
                                            text: dbController.lnbSetting[1]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Label {text: "Energize"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: bEnergize
                                            text: dbController.lnbSetting[16]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Item{
                                            Layout.fillWidth: true
                                        }
                                    }
                                    ToolSeparator{ Layout.fillWidth: true; orientation: Qt.Horizontal}

                                    RowLayout{
                                        spacing:15
                                        Layout.preferredWidth: parent.width
                                        Layout.alignment: Qt.AlignHCenter
                                        Item{
                                            Layout.fillWidth: true
                                        }

                                        Label {text: "Min"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: bMin
                                            text: dbController.lnbSetting[2]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Label {text: "Max"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: bMax
                                            text: dbController.lnbSetting[3]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Label {text: "Average"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: bAverage
                                            text: dbController.lnbSetting[4]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }
                                        Label {text: "Error Range"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        TextField {
                                            id: bError
                                            text: dbController.lnbSetting[15]
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Item{
                                            Layout.fillWidth: true
                                        }
                                    }

                                    ToolSeparator{ Layout.fillWidth: true; orientation: Qt.Horizontal}

                                    RowLayout{
                                        spacing: 20
                                        Layout.alignment: Qt.AlignCenter

                                        Label {text: "Chart"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        CheckBox{
                                            id: chartB
                                            onCheckedChanged: {
                                                weightController.flagChartB = checked
                                            }
                                        }

                                        Label {text: "Table"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        CheckBox{
                                            id: tableB
                                            onCheckedChanged: {
                                                weightController.flagTableB = checked
                                            }
                                        }

                                        MyButton {
                                            text: "Apply"
                                            icon_btn: "qrc:/Icons/apply.png"
                                            Layout.preferredHeight: 50
                                            Layout.preferredWidth: 120
                                            Layout.alignment: Qt.AlignHCenter

                                            buttonColor: applyColor

                                            onClicked: {
                                                let setting = []
                                                setting.push(bKFactor.text)
                                                setting.push(bMin.text)
                                                setting.push(bMax.text)
                                                setting.push(bAverage.text)
                                                setting.push(bLow.text)
                                                setting.push(bHigh.text)
                                                setting.push(baCup.text)
                                                setting.push(bbCup.text)
                                                setting.push(bcCup.text)
                                                setting.push(bdCup.text)
                                                setting.push(beCup.text)
                                                setting.push(bfCup.text)
                                                setting.push(bgCup.text)
                                                setting.push(bhCup.text)
                                                setting.push(bError.text)
                                                setting.push(bEnergize.text)
                                                weightController.applySetting(1, setting)
                                                ethController.applyLineB(setting)

                                                showMessageBox("Line B Setting Applied")
                                            }
                                        }
                                        MyButton {
                                            text: "Edit"
                                            icon_btn: "qrc:/Icons/draw.png"
                                            Layout.preferredHeight: 50
                                            Layout.preferredWidth: 120
                                            Layout.alignment: Qt.AlignHCenter
                                            buttonColor: editColor

                                            onClicked: {
                                                dbController.setIdx("Line_B", bKFactor.text, 1)
                                                dbController.setIdx("Line_B", bMin.text, 2)
                                                dbController.setIdx("Line_B", bMax.text, 3)
                                                dbController.setIdx("Line_B", bAverage.text, 4)
                                                dbController.setIdx("Line_B", bLow.text, 5)
                                                dbController.setIdx("Line_B", bHigh.text, 6)
                                                dbController.setIdx("Line_B", baCup.text, 7)
                                                dbController.setIdx("Line_B", bbCup.text, 8)
                                                dbController.setIdx("Line_B", bcCup.text, 9)
                                                dbController.setIdx("Line_B", bdCup.text, 10)
                                                dbController.setIdx("Line_B", beCup.text, 11)
                                                dbController.setIdx("Line_B", bfCup.text, 12)
                                                dbController.setIdx("Line_B", bgCup.text, 13)
                                                dbController.setIdx("Line_B", bhCup.text, 14)
                                                dbController.setIdx("Line_B", bError.text, 15)
                                                dbController.setIdx("Line_B", bEnergize.text, 16)
                                                dbController.updateLnb(lnbSpeedComboBox.currentValue)

                                                dbController.setIdx("Ejector_B", baIp.text + "," + baOutput.text, 0)
                                                dbController.setIdx("Ejector_B", bbIp.text + "," + bbOutput.text, 1)
                                                dbController.setIdx("Ejector_B", bcIp.text + "," + bcOutput.text, 2)
                                                dbController.setIdx("Ejector_B", bdIp.text + "," + bdOutput.text, 3)
                                                dbController.setIdx("Ejector_B", beIp.text + "," + beOutput.text, 4)
                                                dbController.setIdx("Ejector_B", bfIp.text + "," + bfOutput.text, 5)
                                                dbController.setIdx("Ejector_B", bgIp.text + "," + bgOutput.text, 6)
                                                dbController.setIdx("Ejector_B", bhIp.text + "," + bhOutput.text, 7)
                                                dbController.updateEjectorB()

                                                showMessageBox("Line B Setting Updated")
                                            }
                                        }

                                        CheckBox{
                                            onCheckedChanged: {
                                                ethController.errorCupB = checked
                                            }
                                        }
                                        TextField {
                                            text: "10"
                                            color: "white"
                                            Layout.preferredWidth: 50
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                            Component.onCompleted: {
                                                ethController.errorCupValueB = parseInt(text)
                                            }

                                            onTextChanged: {
                                                ethController.errorCupValueB = parseInt(text)
                                            }
                                        }
                                    }

                                }

                            }

                            ChartView {
                                Layout.preferredHeight: 400
                                Layout.preferredWidth: 800


                                LineSeries {
                                    name: "Line B"
                                    id: lnbPoints
                                    axisX: axisXb
                                    axisY: axisYb
                                    color: "purple"
                                    width: 3

                                    Repeater {
                                        model: Math.min(weightController.posRui.length, weightController.weightRui.length)
                                    }
                                }

                                LineSeries {
                                    id: verticalLineb1
                                    color: "red"
                                    axisX: axisXb
                                    axisY: axisYb
                                    width: 3

                                }

                                // Second vertical line
                                LineSeries {
                                    id: verticalLineb2
                                    color: "red"
                                    axisX: axisXb
                                    axisY: axisYb
                                    width: 3

                                }
                                ValueAxis {
                                    id: axisXb
                                    titleText: "Position"
                                    min: 0
                                    max: 100
                                    tickCount: 11
                                }

                                ValueAxis {
                                    id: axisYb
                                    titleText: "Weight"
                                    min: weightController.minAxisB
                                    max: weightController.maxAxisB
                                    tickCount: 10
                                }

                                Connections {
                                    target: weightController

                                    function onWeightRightChanged() {
                                        lnbPoints.clear()
                                        verticalLineb1.clear()
                                        verticalLineb2.clear()

                                        for(var i=0;i<weightController.posRui.length;i++) {
                                            lnbPoints.append(weightController.posRui[i], weightController.weightRui[i])
                                        }

                                        verticalLineb1.append(dbController.lnbSetting[5], weightController.minAxisB)
                                        verticalLineb1.append(dbController.lnbSetting[5], weightController.maxAxisB)

                                        verticalLineb2.append(dbController.lnbSetting[6], weightController.minAxisB)
                                        verticalLineb2.append(dbController.lnbSetting[6], weightController.maxAxisB)
                                    }
                                }

                            }

                            Item{
                                Layout.fillHeight: true
                            }

                        }

                        ColumnLayout{
                            Layout.preferredHeight: parent.width

                            Item{
                                Layout.fillHeight: true
                            }

                            TableView {
                                Layout.preferredWidth: 400
                                Layout.preferredHeight: 35
                                Layout.alignment: Qt.AlignCenter
                                rowSpacing: 1
                                clip: true

                                model: TableModel {
                                    TableModelColumn { display: "1" }
                                    TableModelColumn { display: "2" }
                                    TableModelColumn { display: "3" }
                                    TableModelColumn { display: "4" }
                                    TableModelColumn { display: "5" }
                                    TableModelColumn { display: "6" }

                                    rows: [
                                        {
                                            "1": "Cup",
                                            "2": "LC",
                                            "3": "Gross",
                                            "4": "Tare",
                                            "5": "Net",
                                            "6": "Tare*"
                                        }
                                    ]
                                }

                                delegate: Rectangle {
                                    implicitWidth: 63
                                    implicitHeight: 35
                                    border.width: 1



                                    Text {
                                        text: model.display
                                        anchors.centerIn: parent
                                    }
                                }
                            }
                            TableView {
                                Layout.preferredWidth: 400
                                Layout.preferredHeight: 670
                                Layout.alignment: Qt.AlignCenter
                                rowSpacing: 1
                                clip: true

                                model: TableModel {
                                    id: tableModelB
                                    TableModelColumn { display: "1" }
                                    TableModelColumn { display: "2" }
                                    TableModelColumn { display: "3" }
                                    TableModelColumn { display: "4" }
                                    TableModelColumn { display: "5" }
                                    TableModelColumn { display: "6" }
                                }

                                delegate: Rectangle {
                                    implicitWidth: 63
                                    implicitHeight: 35
                                    border.width: 1



                                    Text {
                                        text: model.display
                                        anchors.centerIn: parent
                                    }
                                }

                                Connections {
                                    target: weightController

                                    function onDataTableBChanged() {
                                        var data = []

                                        for (var i=0; i < weightController.dataTableB.length; ++i){
                                            data.push(weightController.dataTableB[i])
                                        }

                                        tableModelB.setRow((parseInt(data[0]) % 283) % 18, { "1": data[0], "2": data[1], "3": data[2], "4": data[3], "5": data[4], "6": data[5] });
                                    }
                                }
                            }

                            Item{
                                Layout.fillHeight: true
                            }
                        }
                    }



                }

                Item {
                    id: camerasTab

                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height
                    Rectangle{
                        anchors.centerIn: parent
                        width: 300
                        height: 400
                        radius: 10
                        color: rectColor
                        ColumnLayout{
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            anchors.centerIn: parent

                            ComboBox {
                                id: camComboBox
                                Layout.preferredWidth: 200
                                Layout.preferredHeight: 45
                                onCurrentValueChanged: {
                                    dbController.readCams(camComboBox.currentValue)

                                }
                                Layout.alignment: Qt.AlignHCenter

                            }

                            Label{
                                text: "Ip"
                                font.pixelSize: 16
                                color: "white"
                                Layout.alignment: Qt.AlignHCenter
                            }
                            TextField {
                                id: camIp
                                text: dbController.camSetting[1]
                                Layout.preferredWidth: 230
                                Layout.preferredHeight: 40
                                font.pixelSize: 16
                                selectByMouse: true
                                Layout.alignment: Qt.AlignHCenter

                            }
                            Label{
                                text: "Gateway"
                                font.pixelSize: 16
                                color: "white"
                                Layout.alignment: Qt.AlignHCenter
                            }
                            TextField {
                                id: camGateway
                                text: dbController.camSetting[2]
                                Layout.preferredWidth: 230
                                Layout.preferredHeight: 40
                                font.pixelSize: 16
                                selectByMouse: true
                                Layout.alignment: Qt.AlignHCenter

                            }
                            Item {
                                Layout.preferredHeight: 30
                            }

                            MyButton {
                                //Layout.fillWidth: true
                                text: "Add"
                                icon_btn: "qrc:/Icons/add.png"
                                Layout.preferredWidth: 180
                                Layout.preferredHeight: 50
                                Layout.alignment: Qt.AlignHCenter
                                buttonColor: applyColor

                                onClicked: {
                                    for(var i=0;i<dbController.getCameras().length;i++)
                                        if(camComboBox.currentValue === dbController.getCameras()[i])
                                        {
                                            showMessageBox("Camera " + camComboBox.currentValue + " Already Exists")
                                            return
                                        }


                                    dbController.setIdx("Cams", camComboBox.currentValue, 0)
                                    dbController.setIdx("Cams", camIp.text, 1)
                                    dbController.setIdx("Cams", camGateway.text, 2)

                                    dbController.insertCams()
                                    dbController.readCams(camComboBox.currentValue)

                                    showMessageBox("Camera " + camComboBox.currentValue + " Added")
                                }
                            }
                            MyButton {
                                //Layout.fillWidth: true
                                text: "Update"
                                icon_btn: "qrc:/Icons/undo.png"
                                Layout.preferredWidth: 180
                                Layout.preferredHeight: 50
                                Layout.alignment: Qt.AlignHCenter
                                buttonColor: editColor

                                onClicked: {
                                    dbController.setIdx("Cams", camIp.text, 1)
                                    dbController.setIdx("Cams", camGateway.text, 2)
                                    dbController.updateCams(camComboBox.currentValue)

                                    showMessageBox("Camera " + camComboBox.currentValue + " Setting Updated")
                                }
                            }

                        }

                    }

                }

                Item {
                    id: camSettingTab

                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height

                    property int lblWidth1: 60
                    property int txtWidth: 60
                    property int sliderWidth: 100
                    property int textFontSize : 14
                    property int textSize : 60
                    property int sliderSize : 70
                    property int labelSize : 170
                    property int itemHeight: 40

                    ColumnLayout{
                        anchors.fill: parent

                        RowLayout{
                            Layout.preferredWidth: parent.width
                            Layout.preferredHeight: parent.height

                            Item {
                                Layout.fillWidth: true
                            }

                            Rectangle{
                                Layout.preferredWidth: 700
                                Layout.preferredHeight: 690

                                color: rectColor

                                border.color: "white"
                                radius: 10
                                border.width: 0
                                ColumnLayout{
                                    anchors.fill: parent
                                    spacing: 2
                                    Item {
                                        width: 200
                                        height: 45
                                        Layout.alignment: Qt.AlignHCenter

                                        ComboBox {
                                            id: camSettingComboBox
                                            width: 200
                                            height: 45
                                            // You can also use a ListModel or other models as the 'model'\
                                            // Initial selected index

                                            onCurrentValueChanged: {
                                                camComboBox.currentIndex = camComboBox.find(currentValue)
                                                dbController.readCams(this.currentValue)

                                                exposure.value = dbController.camSetting[3]
                                                gain.value = dbController.camSetting[4]
                                                blackLevel.value = dbController.camSetting[5]
                                                whiteBalanceRed.value = dbController.camSetting[6]
                                                whiteBalanceGreen.value = dbController.camSetting[7]
                                                whiteBalanceBlue.value = dbController.camSetting[8]
                                                gamma.value = dbController.camSetting[9]

                                                // autoWhiteBalanceComboBox.currentIndex = autoWhiteBalanceComboBox.find(dbController.camSetting[10])
                                                // whiteBalanceModeComboBox.currentIndex = whiteBalanceModeComboBox.find(dbController.camSetting[11])
                                                // whiteBalanceAutoPresetComboBox.currentIndex = whiteBalanceAutoPresetComboBox.find(dbController.camSetting[12])
                                                // whiteBalanceTempraturePresetComboBox.currentIndex = whiteBalanceTempraturePresetComboBox.find(dbController.camSetting[13])
                                                // strobeComboBox.currentIndex = strobeComboBox.ind(dbController.camSetting[4])

                                                // camController.setNewSetting(autoWhiteBalanceComboBox.currentValue, 9)
                                                // camController.setNewSetting(whiteBalanceModeComboBox.currentValue, 10)
                                                // camController.setNewSetting(whiteBalanceAutoPresetComboBox.currentValue, 11)
                                                // camController.setNewSetting(whiteBalanceTempraturePresetComboBox.currentValue, 12)

                                                // dbController.setIdx("Cams", this.currentValue, 14)
                                                // camController.setNewSetting(camSettingComboBox.currentValue, 14)

                                                dbController.setIdx("Cams", autoWhiteBalanceComboBox.currentValue, 10)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 10)

                                                dbController.setIdx("Cams", whiteBalanceModeComboBox.currentValue, 11)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 11)

                                                dbController.setIdx("Cams", whiteBalanceAutoPresetComboBox.currentValue, 12)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 12)

                                                dbController.setIdx("Cams", whiteBalanceTempraturePresetComboBox.currentValue, 13)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 13)

                                                dbController.setIdx("Cams", strobeComboBox.currentValue, 14)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 14)

                                            }
                                            Component.onCompleted: {
                                                dbController.readCams(this.currentValue)
                                            }

                                            Layout.alignment: Qt.AlignHCenter
                                        }
                                    }
                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "Exposure:"
                                            Layout.preferredWidth: camSettingTab.labelSize

                                            font.pixelSize: camSettingTab.textFontSize
                                        }
                                        Slider {
                                            id: exposure
                                            width: camSettingTab.sliderSize
                                            Layout.preferredHeight: 30
                                            value: dbController.camSetting[3] !== "" ? dbController.camSetting[3] : ""
                                            from: 1.00
                                            to: 20000.00
                                            stepSize: 1.00
                                            onValueChanged: {
                                                dbController.setIdx("Cams", this.value.toString(), 3)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 3)
                                            }

                                        }
                                        TextField {
                                            id: exposureText
                                            selectByMouse: true
                                            Layout.preferredWidth: camSettingTab.textSize
                                            font.pixelSize: camSettingTab.textFontSize
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            text: exposure.value.toFixed(2)
                                            color: "white"
                                        }
                                        Button {
                                            id: setBtn1
                                            text: "Set"
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            onClicked: {
                                                exposure.value = parseFloat(exposureText.text)
                                            }
                                        }
                                    }
                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "Gain:"
                                            Layout.preferredWidth: camSettingTab.labelSize
                                            font.pixelSize: camSettingTab.textFontSize
                                        }
                                        Slider {
                                            id: gain
                                            width: camSettingTab.sliderSize
                                            value: dbController.camSetting[4] !== "" ? dbController.camSetting[4] : ""
                                            from: 0.00
                                            to: 48.00
                                            stepSize: 1.00
                                            onValueChanged: {
                                                dbController.setIdx("Cams", this.value.toString(), 4)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 4)
                                            }
                                        }
                                        TextField {
                                            id: gainText
                                            selectByMouse: true
                                            Layout.preferredWidth: camSettingTab.textSize
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            font.pixelSize: camSettingTab.textFontSize
                                            text: gain.value.toFixed(2)
                                            color: "white"
                                        }
                                        Button {
                                            id: setBtn2
                                            text: "Set"
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            onClicked: {
                                                gain.value = parseFloat(gainText.text)
                                            }
                                        }

                                    }
                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "Black Level:"
                                            Layout.preferredWidth: camSettingTab.labelSize
                                            font.pixelSize: camSettingTab.textFontSize
                                        }
                                        Slider {
                                            id: blackLevel
                                            x: 90
                                            width: camSettingTab.sliderSize
                                            value: dbController.camSetting[5] !== "" ? dbController.camSetting[5] : ""
                                            from: 0.00
                                            to: 4095.00
                                            stepSize: 1.00
                                            onValueChanged: {
                                                dbController.setIdx("Cams", this.value.toString(), 5)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 5)
                                            }
                                        }
                                        TextField {
                                            id: blackLevelText
                                            selectByMouse: true
                                            Layout.preferredWidth: camSettingTab.textSize
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            font.pixelSize: camSettingTab.textFontSize
                                            text:blackLevel.value.toFixed(2)
                                            color: "white"
                                        }
                                        Button {
                                            id: setBtn3
                                            text: "Set"
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            onClicked: {
                                                blackLevel.value = parseFloat(blackLevelText.text)
                                            }
                                        }
                                    }

                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "White Balance Red:"
                                            enabled: autoWhiteBalanceComboBox.currentValue === "Off"
                                            Layout.preferredWidth: camSettingTab.labelSize
                                            font.pixelSize: camSettingTab.textFontSize
                                        }
                                        Slider {
                                            id: whiteBalanceRed
                                            width: camSettingTab.sliderSize
                                            value: dbController.camSetting[6] !== "" ? dbController.camSetting[6] : ""
                                            from: 0.00
                                            to: 3.98
                                            stepSize: 0.01
                                            onValueChanged: {
                                                dbController.setIdx("Cams", this.value.toString(), 6)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 6)
                                            }
                                        }
                                        TextField {
                                            id: whiteBalanceRedText
                                            selectByMouse: true
                                            Layout.preferredWidth: camSettingTab.textSize
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            font.pixelSize: camSettingTab.textFontSize
                                            text:whiteBalanceRed.value.toFixed(2)
                                            color: "white"
                                        }
                                        Button {
                                            id: setBtn4
                                            text: "Set"
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            onClicked: {
                                                whiteBalanceRed.value = parseFloat(whiteBalanceRedText.text)
                                            }
                                        }
                                    }

                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            id: whiteGreen
                                            text: "White Balance Green:"
                                            enabled: autoWhiteBalanceComboBox.currentValue === "Off"
                                            Layout.preferredWidth: camSettingTab.labelSize

                                            font.pixelSize: camSettingTab.textFontSize
                                        }
                                        Slider {
                                            id: whiteBalanceGreen
                                            width: camSettingTab.sliderSize
                                            value: dbController.camSetting[7] !== "" ? dbController.camSetting[7] : ""
                                            from: 0.00
                                            to: 3.98
                                            stepSize: 0.01
                                            onValueChanged: {
                                                dbController.setIdx("Cams", this.value.toString(), 7)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 7)
                                            }
                                        }
                                        TextField
                                        {
                                            id: whiteBalanceGreenText
                                            selectByMouse: true
                                            Layout.preferredWidth: camSettingTab.textSize
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            font.pixelSize: camSettingTab.textFontSize
                                            text:whiteBalanceGreen.value.toFixed(2)
                                            color: "white"
                                        }
                                        Button {
                                            id: setBtn5
                                            text: "Set"
                                            Layout.preferredHeight: 40
                                            onClicked: {
                                                whiteBalanceGreen.value = parseFloat(whiteBalanceGreenText.text)
                                            }
                                        }
                                    }

                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "White Balance Blue:"
                                            enabled: autoWhiteBalanceComboBox.currentValue === "Off"
                                            Layout.preferredWidth: camSettingTab.labelSize
                                            font.pixelSize: camSettingTab.textFontSize
                                        }
                                        Slider {
                                            id: whiteBalanceBlue
                                            width: camSettingTab.sliderSize
                                            value: dbController.camSetting[8] !== "" ? dbController.camSetting[8] : ""
                                            from: 0.00
                                            to: 3.98
                                            stepSize: 0.01
                                            onValueChanged: {
                                                dbController.setIdx("Cams", this.value.toString(), 8)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 8)
                                            }
                                        }
                                        TextField {
                                            id: whiteBalanceBlueText
                                            selectByMouse: true
                                            Layout.preferredWidth: camSettingTab.textSize
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            font.pixelSize: camSettingTab.textFontSize
                                            text:whiteBalanceBlue.value.toFixed(2)
                                            color: "white"
                                        }
                                        Button {
                                            id: setBtn6
                                            text: "Set"
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            onClicked: {
                                                whiteBalanceBlue.value = parseFloat(whiteBalanceBlueText.text)
                                            }
                                        }
                                    }

                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "Gamma:"
                                            Layout.preferredWidth: camSettingTab.labelSize
                                            font.pixelSize: camSettingTab.textFontSize
                                        }
                                        Slider {
                                            id: gamma
                                            width: camSettingTab.sliderSize
                                            value: dbController.camSetting[9] !== "" ? dbController.camSetting[9] : ""
                                            from: 0.01
                                            to: 5.00
                                            stepSize: 0.01
                                            onValueChanged: {
                                                dbController.setIdx("Cams", this.value.toString(), 9)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 9)
                                            }
                                        }
                                        TextField {
                                            id: gammaText
                                            selectByMouse: true
                                            Layout.preferredWidth: camSettingTab.textSize
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            font.pixelSize: camSettingTab.textFontSize
                                            text: gamma.value.toFixed(2)
                                            color: "white"
                                        }
                                        Button {
                                            id: setBtn7
                                            text: "Set"
                                            Layout.preferredHeight: camSettingTab.itemHeight
                                            onClicked: {
                                                gamma.value = parseFloat(gammaText.text)
                                            }
                                        }
                                    }

                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "Auto White Balance:"
                                            Layout.preferredWidth: camSettingTab.labelSize
                                            font.pixelSize: camSettingTab.textFontSize
                                        }

                                        ComboBox {
                                            id: autoWhiteBalanceComboBox
                                            Layout.preferredWidth: exposure.width + exposureText.width + setBtn1.width + 10
                                            Layout.preferredHeight: 40
                                            font.pixelSize: camSettingTab.textFontSize

                                            model: ["Continuous", "Off"]
                                            currentIndex: this.find(dbController.camSetting[10])

                                            Layout.alignment: Qt.AlignHCenter
                                            onCurrentValueChanged: {
                                                dbController.setIdx("Cams", this.currentValue, 10)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 10)
                                            }

                                        }

                                    }

                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "White Balance Mode:"
                                            Layout.preferredWidth: camSettingTab.labelSize
                                            font.pixelSize: camSettingTab.textFontSize
                                        }
                                        ComboBox {
                                            id: whiteBalanceModeComboBox
                                            Layout.preferredWidth: exposure.width + exposureText.width + setBtn1.width + 10
                                            Layout.preferredHeight: 40
                                            font.pixelSize: camSettingTab.textFontSize
                                            model: ["Temperature", "Gray World"]
                                            currentIndex: this.find(dbController.camSetting[11])
                                            Layout.alignment: Qt.AlignHCenter
                                            onCurrentValueChanged: {
                                                dbController.setIdx("Cams", this.currentValue, 11)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 11)
                                            }

                                        }
                                    }

                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "White Balance Auto:"
                                            Layout.preferredWidth: camSettingTab.labelSize
                                            font.pixelSize: camSettingTab.textFontSize
                                        }

                                        ComboBox {
                                            id: whiteBalanceAutoPresetComboBox
                                            Layout.preferredWidth: exposure.width + exposureText.width + setBtn1.width + 10
                                            Layout.preferredHeight: 40
                                            font.pixelSize: camSettingTab.textFontSize
                                            enabled: whiteBalanceModeComboBox.currentValue === "Temperature"
                                            model: ["Auto Deep Shade", "Auto Daylight", "Auto Warm White", "Auto Cool White", "Any"]
                                            currentIndex: this.find(dbController.camSetting[12])
                                            Layout.alignment: Qt.AlignHCenter
                                            onCurrentValueChanged: {
                                                dbController.setIdx("Cams", this.currentValue, 12)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 12)
                                            }
                                        }
                                    }

                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "White Balance Temperature:"
                                            Layout.preferredWidth: camSettingTab.labelSize
                                            font.pixelSize: 13
                                        }

                                        ComboBox {
                                            id: whiteBalanceTempraturePresetComboBox
                                            Layout.preferredWidth: exposure.width + exposureText.width + setBtn1.width + 10
                                            Layout.preferredHeight: 40
                                            font.pixelSize: camSettingTab.textFontSize
                                            enabled: whiteBalanceModeComboBox.currentValue === "Temperature" && autoWhiteBalanceComboBox.currentValue === "Off"
                                            model: ["Blue Sky", "Cool White LED", "Daylight", "Cool White Fluorescent Light", "Neutral White", "Halogen Light", "Warm Light", "Sodium-Vapor Lamp"]
                                            currentIndex: this.find(dbController.camSetting[13])
                                            Layout.alignment: Qt.AlignHCenter
                                            onCurrentValueChanged: {
                                                dbController.setIdx("Cams", this.currentValue, 13)
                                                switch(whiteBalanceTempraturePresetComboBox.currentValue) {
                                                case "Blue Sky":
                                                    dbController.setIdx("Cams", 1.98, 6)
                                                    dbController.setIdx("Cams", 1.00, 7)
                                                    dbController.setIdx("Cams", 2.00, 8)
                                                    break
                                                case "Coll White LED":
                                                    dbController.setIdx("Cams", 1.86, 6)
                                                    dbController.setIdx("Cams", 1.00, 7)
                                                    dbController.setIdx("Cams", 2.14, 8)
                                                    break
                                                case "Daylight":
                                                    dbController.setIdx("Cams", 1.75, 6)
                                                    dbController.setIdx("Cams", 1.00, 7)
                                                    dbController.setIdx("Cams", 2.14, 8)
                                                    break
                                                case "Cool White Fluorescent Light":
                                                    dbController.setIdx("Cams", 1.47, 6)
                                                    dbController.setIdx("Cams", 1.00, 7)
                                                    dbController.setIdx("Cams", 2.88, 8)
                                                    break
                                                case "Neutral White":
                                                    dbController.setIdx("Cams", 1.38, 6)
                                                    dbController.setIdx("Cams", 1.00, 7)
                                                    dbController.setIdx("Cams", 3.20, 8)
                                                    break
                                                case "Halogen Light":
                                                    dbController.setIdx("Cams", 1.27, 6)
                                                    dbController.setIdx("Cams", 1.00, 7)
                                                    dbController.setIdx("Cams", 3.80, 8)
                                                    break
                                                case "Warm Light":
                                                    dbController.setIdx("Cams", 1.19, 6)
                                                    dbController.setIdx("Cams", 1.00, 7)
                                                    dbController.setIdx("Cams", 3.98, 8)
                                                    break
                                                case "Sodium-Vapor Lamp":
                                                    dbController.setIdx("Cams", 1.14, 6)
                                                    dbController.setIdx("Cams", 1.00, 7)
                                                    dbController.setIdx("Cams", 3.98, 8)
                                                    break
                                                default:
                                                    dbController.setIdx("Cams", 1.86, 6)
                                                    dbController.setIdx("Cams", 1.00, 7)
                                                    dbController.setIdx("Cams", 2.14, 8)
                                                }

                                                camController.setNewSetting(camSettingComboBox.currentValue, 6)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 7)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 8)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 13)
                                            }
                                        }
                                    }

                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "Strobe:"
                                            Layout.preferredWidth: camSettingTab.labelSize
                                            font.pixelSize: camSettingTab.textFontSize
                                        }

                                        ComboBox {
                                            id: strobeComboBox
                                            Layout.preferredWidth: exposure.width + exposureText.width + setBtn1.width + 10
                                            Layout.preferredHeight: 40
                                            font.pixelSize: camSettingTab.textFontSize
                                            model: ["On", "Off"]
                                            currentIndex: this.find(dbController.camSetting[14])
                                            Layout.alignment: Qt.AlignHCenter
                                            onCurrentValueChanged: {
                                                dbController.setIdx("Cams", this.currentValue, 14)
                                                camController.setNewSetting(camSettingComboBox.currentValue, 14)
                                            }
                                        }
                                    }

                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Item{Layout.fillWidth: true}

                                        //                                        MyButton {
                                        //                                            //Layout.fillWidth: true
                                        //                                            text: "Show"
                                        //                                            icon_btn: "qrc:/Icons/camera.png"
                                        //                                            Layout.preferredHeight: 50
                                        //                                            Layout.preferredWidth: 170
                                        //                                            buttonColor: applyColor
                                        //                                            onClicked: {
                                        //                                                camController.showCamera()
                                        //                                                viewPortCamSetting.width = 400
                                        //                                                viewPortCamSetting.height = 680
                                        //                                                viewPortCamSetting.id_viewPort = 4
                                        //                                                camController.appendViewPort(viewPortCamSetting)
                                        //                                            }
                                        //                                        }
                                        Item{Layout.preferredWidth: 10}
                                        MyButton {
                                            //Layout.fillWidth: true
                                            text: "Update"
                                            icon_btn: "qrc:/Icons/undo.png"
                                            Layout.preferredHeight: 50
                                            Layout.preferredWidth: 170
                                            buttonColor: editColor
                                            onClicked: {
                                                dbController.updateCams(camSettingComboBox.currentValue)
                                                dbController.readCams(camSettingComboBox.currentValue)
                                                showMessageBox("Camera " + camSettingComboBox.currentValue + " Setting Updated")
                                            }
                                        }
                                        Item{Layout.fillWidth: true}
                                    }
                                }

                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            ToolSeparator { Layout.fillHeight: true; orientation: Qt.Vertical }

                            ColumnLayout{
                                Layout.preferredWidth: 400

                                RowLayout
                                {
                                    Layout.alignment: Qt.AlignHCenter
                                    Label{
                                        text: "Select Frame"
                                        font.pixelSize: 16
                                        color: "white"
                                    }

                                    Item{
                                        Layout.preferredWidth: 5
                                    }

                                    ComboBox
                                    {
                                        id: settingFrameComboBox
                                        Layout.preferredHeight: 45
                                        Layout.preferredWidth: 70

                                        Component.onCompleted: {
                                            model = ["1", "2", "3", "4"]
                                            // dbController.readLastRun()

                                            // currentIndex = speedMainComboBox.find(dbController.lastRun[1])
                                            // cmbsOpened[1] = true

                                            // weightController.speed = currentValue
                                        }
                                        onCurrentValueChanged: {
                                            camController.setIdxFrameIdx(0, currentValue)
                                            defectFrameComboBox.currentIndex = find(currentValue)
                                            colorFrameComboBox.currentIndex = find(currentValue)
                                        }
                                    }
                                }
                                Rectangle {
                                    color: "white"
                                    Layout.alignment: Qt.AlignHCenter
                                    radius: 10
                                    //anchors.centerIn: parent
                                    //property real _orgImgRatio: camController.inputResulation.width/camController.inputResulation.height
                                    //property real _parentRatio: frame_box.width/frame_box.height

                                    //width: 380
                                    //height: 380
                                    Layout.preferredWidth: 360
                                    Layout.preferredHeight: 360

                                    CaptureViewPort{
                                        id: viewPortCamSetting1
                                        //anchors.fill: parent
                                        anchors.centerIn: parent
                                    }
                                }
                                Rectangle {
                                    color: "white"
                                    Layout.alignment: Qt.AlignHCenter
                                    radius: 10
                                    //anchors.centerIn: parent
                                    //property real _orgImgRatio: camController.inputResulation.width/camController.inputResulation.height
                                    //property real _parentRatio: frame_box.width/frame_box.height

                                    //width: 380
                                    //height: 380
                                    Layout.preferredWidth: 360
                                    Layout.preferredHeight: 360

                                    CaptureViewPort{
                                        id: viewPortCamSetting2
                                        //anchors.fill: parent
                                        anchors.centerIn: parent
                                    }
                                }
                                CheckBox{
                                    id: settingTabChb
                                    Layout.alignment: Qt.AlignHCenter
                                    text: "Show"

                                    onCheckedChanged: {
                                        if(camController.cameraIsStarted[0] === false && camController.cameraIsStarted[0] === false)
                                        {
                                            showMessageBox("Camera(s) Not Started")
                                            checked = false
                                            return
                                        }

                                        camController.setIdxShowChecks(0, checked)
                                        camController.showChanged()
                                        if(checked)
                                        {
                                            viewPortCamSetting1.width = 360
                                            viewPortCamSetting1.height = 360
                                            viewPortCamSetting1.id_viewPort = 1
                                            camController.appendViewPort(viewPortCamSetting1)

                                            viewPortCamSetting2.width = 360
                                            viewPortCamSetting2.height = 360
                                            viewPortCamSetting2.id_viewPort = 2
                                            camController.appendViewPort(viewPortCamSetting2)

                                        }
                                        else
                                        {
                                        }

                                    }
                                }
                            }
                        }

                    }

                }

                Item {
                    id: colorTab

                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height

                    RowLayout{
                        anchors.fill: parent

                        Item{
                            Layout.fillWidth: true
                        }

                        ColumnLayout{
                            spacing: 10
                            Rectangle{
                                Layout.preferredWidth: 580
                                Layout.preferredHeight: 350
                                radius: 10
                                color: rectColor
                                ColumnLayout{
                                    spacing: 0
                                    anchors.fill: parent
                                    RowLayout{
                                        Layout.alignment: Qt.AlignHCenter
                                        Label {text: "Group : "; font.pixelSize: 15; color: "white"}
                                        ComboBox {
                                            id: colorGroupComboBox
                                            Layout.alignment: Qt.AlignHCenter

                                            Layout.preferredWidth: 150

                                            Component.onCompleted: {

                                            }

                                            onCurrentValueChanged: {
                                                dbController.readColor(colorGroupComboBox.currentValue, colorComboBox.currentValue)
                                                hue.first.value = dbController.color[2]
                                                hue.second.value = dbController.color[3]

                                                saturation.first.value = dbController.color[5]
                                                saturation.second.value = dbController.color[6]

                                                value.first.value = dbController.color[7]
                                                value.second.value = dbController.color[8]
                                            }
                                            // Initial selected index
                                        }
                                        Label {text: "Color : "; font.pixelSize: 15; color: "white";}
                                        ComboBox {
                                            id: colorComboBox

                                            Layout.preferredWidth: 150

                                            Component.onCompleted: {

                                            }

                                            onCurrentValueChanged: {
                                                dbController.readColor(colorGroupComboBox.currentValue, colorComboBox.currentValue)
                                                hue.first.value = dbController.color[2]
                                                hue.second.value = dbController.color[3]

                                                saturation.first.value = dbController.color[5]
                                                saturation.second.value = dbController.color[6]

                                                value.first.value = dbController.color[7]
                                                value.second.value = dbController.color[8]
                                            }

                                            // Initial selected index
                                        }
                                    }

                                    CheckBox{
                                        id: rHue
                                        Layout.alignment: Qt.AlignHCenter
                                        text: "Reverse Hue"
                                        Layout.preferredHeight: 40
                                        checked: dbController.color[4] === "true" ? true : false
                                        onCheckedChanged: {
                                            dbController.setIdx("Colors", checked, 4)
                                            dbController.colorsChanged()
                                        }
                                    }

                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "Hue"
                                            Layout.preferredWidth: 60
                                        }
                                        Item{
                                            Layout.preferredWidth: 20
                                        }
                                        Label {
                                            id: hueMin
                                            text: Math.round(hue.first.value)
                                            color: "white"
                                        }
                                        RangeSlider {
                                            id: hue
                                            width: camSettingTab.sliderSize
                                            Layout.preferredHeight: 30
                                            from: 0
                                            to: 179
                                            stepSize: 1
                                            first.value: dbController.color[2]
                                            second.value: dbController.color[3]

                                            first.onValueChanged: {
                                                dbController.setIdx("Colors", Math.round(first.value), 2)
                                                dbController.colorsChanged()
                                            }
                                            second.onValueChanged: {
                                                dbController.setIdx("Colors", Math.round(second.value), 3)
                                                dbController.colorsChanged()
                                            }
                                        }
                                        Label {
                                            id: hueMax
                                            text: Math.round(hue.second.value)
                                            color: "white"
                                        }
                                    }
                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "Saturation"
                                            Layout.preferredWidth: 60
                                        }
                                        Item{
                                            Layout.preferredWidth: 20
                                        }
                                        Label {
                                            id: saturationMin
                                            text: Math.round(saturation.first.value)
                                            color: "white"
                                        }
                                        RangeSlider {
                                            id: saturation
                                            width: camSettingTab.sliderSize
                                            Layout.preferredHeight: 30
                                            from: 0
                                            to: 255
                                            stepSize: 1
                                            first.value: dbController.color[5]
                                            second.value: dbController.color[6]

                                            first.onValueChanged: {
                                                dbController.setIdx("Colors", Math.round(first.value), 5)
                                                dbController.colorsChanged()
                                            }
                                            second.onValueChanged: {
                                                dbController.setIdx("Colors", Math.round(second.value), 6)
                                                dbController.colorsChanged()
                                            }
                                        }
                                        Label {
                                            id: saturationMax
                                            text: Math.round(saturation.second.value)
                                            color: "white"
                                        }
                                        CheckBox{
                                            Layout.alignment: Qt.AlignHCenter
                                            text: "Not"
                                            checked: dbController.color[4] === "true" ? true : false
                                            onCheckedChanged: {
                                                dbController.setIdx("Colors", checked, 4);
                                            }
                                            visible: false
                                        }
                                    }
                                    RowLayout {
                                        Layout.alignment: Qt.AlignHCenter
                                        Label{
                                            text: "Value"
                                            Layout.preferredWidth: 60
                                        }
                                        Item{
                                            Layout.preferredWidth: 20
                                        }
                                        Label {
                                            id: valueMin
                                            text: Math.round(value.first.value)
                                            color: "white"
                                        }
                                        RangeSlider {
                                            id: value
                                            width: camSettingTab.sliderSize
                                            Layout.preferredHeight: 30
                                            from: 0
                                            to: 255
                                            stepSize: 1
                                            first.value: dbController.color[7]
                                            second.value: dbController.color[8]

                                            first.onValueChanged: {
                                                dbController.setIdx("Colors", Math.round(first.value), 7)
                                                dbController.colorsChanged()
                                            }
                                            second.onValueChanged: {
                                                dbController.setIdx("Colors", Math.round(second.value), 8)
                                                dbController.colorsChanged()
                                            }
                                        }
                                        Label {
                                            id: valueMax
                                            text: Math.round(value.second.value)
                                            color: "white"
                                        }
                                        CheckBox{
                                            Layout.alignment: Qt.AlignHCenter
                                            text: "Not"
                                            checked: dbController.color[4] === "true" ? true : false
                                            onCheckedChanged: {
                                                dbController.setIdx("Colors", checked, 4);
                                            }
                                            visible: false
                                        }
                                    }
                                    RowLayout{
                                        spacing: 20
                                        Layout.alignment: Qt.AlignCenter

                                        MyButton {
                                            id: editColors
                                            text: "Edit"
                                            icon_btn: "qrc:/Icons/draw.png"
                                            Layout.preferredHeight: 50
                                            Layout.preferredWidth: 120
                                            Layout.alignment: Qt.AlignHCenter

                                            buttonColor: editColor

                                            onClicked: {
                                                dbController.setIdx("Color_Group", colorText.text, 0)
                                                dbController.updateColorGroup(colorGroupComboBox.currentValue)

                                                dbController.setIdx("Colors", colorText.text, 0)
                                                dbController.updateColors(colorGroupComboBox.currentValue, colorComboBox.currentValue)

                                                updateColors()

                                                dbController.readColor(colorGroupComboBox.currentValue, colorComboBox.currentValue)

                                                showMessageBox("Color Group Updated")
                                            }
                                        }
                                        MyButton {
                                            id: deleteColor
                                            text: "Delete"
                                            icon_btn: "qrc:/Icons/delete.png"
                                            Layout.preferredHeight: 50
                                            Layout.preferredWidth: 120
                                            Layout.alignment: Qt.AlignHCenter

                                            buttonColor: stopColor

                                            onClicked:
                                            {

                                                if(dbController.getColorGroups().length === 1)
                                                {
                                                    showMessageBox("Can Not Delete All Color Groups")

                                                    return
                                                }

                                                dbController.deleteColorGroup(colorGroupComboBox.currentValue)

                                                updateColors()

                                                colorGroupComboBox.currentIndex = 0

                                                dbController.readColor(colorGroupComboBox.currentValue, colorComboBox.currentValue)

                                            }
                                        }
                                        MyButton {
                                            id: addColor
                                            text: "Add"
                                            icon_btn: "qrc:/Icons/add.png"
                                            Layout.preferredHeight: 50
                                            Layout.preferredWidth: 120
                                            Layout.alignment: Qt.AlignHCenter

                                            buttonColor: startColor

                                            onClicked: {
                                                for (var i=0; i < dbController.getColorGroups().length; ++i){
                                                    if(colorText.text === dbController.getColorGroups()[i])
                                                    {
                                                        showMessageBox("Color Group Already Exists")

                                                        return
                                                    }
                                                }

                                                dbController.setIdx("Color_Group", colorText.text, 0)
                                                dbController.insert("Color_Group")

                                                let colors = ["Crop", "Red", "Orange", "Yellow", "Green"]

                                                dbController.setIdx("Colors", colorText.text, 0)
                                                dbController.setIdx("Colors", "0", 2)
                                                dbController.setIdx("Colors", "179", 3)
                                                dbController.setIdx("Colors", "false", 4)
                                                dbController.setIdx("Colors", "0", 5)
                                                dbController.setIdx("Colors", "255", 6)
                                                dbController.setIdx("Colors", "0", 7)
                                                dbController.setIdx("Colors", "255", 8)

                                                for(i=0;i<colors.length;i++) {
                                                    dbController.setIdx("Colors", colors[i], 1)
                                                    dbController.insert("Colors")
                                                }

                                                showMessageBox("Color Group Added")

                                                updateColors()

                                                colorGroupComboBox.currentIndex = colorGroupComboBox.find(colorText.text)

                                                dbController.readColor(colorGroupComboBox.currentValue, colorComboBox.currentValue)
                                            }


                                        }
                                        TextField {
                                            id: colorText
                                            text: dbController.color[0]
                                            color: "white"
                                            Layout.preferredWidth: 100
                                            font.pixelSize: 16
                                            selectByMouse: true
                                            Layout.alignment: Qt.AlignHCenter
                                            horizontalAlignment: Text.AlignHCenter

                                        }

                                    }

                                }


                                //                            ColumnLayout{
                                //                                Layout.fillHeight: true
                                //                                Layout.fillWidth: true
                                //                                anchors.centerIn: parent
                                //                                ColorDialog {
                                //                                    id: colorDialog
                                //                                    title: "Please choose a color"
                                //                                    onAccepted: {
                                //                                        console.log("You chose: " + colorDialog.color)
                                //                                    }
                                //                                    onRejected: {
                                //                                        console.log("Canceled")
                                //                                    }
                                //                                    Component.onCompleted: visible = true
                                //                                }
                                //                            }

                            }
                            Rectangle{
                                Layout.preferredWidth: 580
                                Layout.preferredHeight: 100
                                radius: 10
                                color: rectColor

                                ColumnLayout{
                                    anchors.fill: parent
                                    spacing: 0

                                    Item {
                                        Layout.fillHeight: true
                                    }
                                    RowLayout{
                                        Layout.alignment: Qt.AlignHCenter
                                        spacing: 5
                                        Image {
                                            Layout.preferredWidth: 20
                                            Layout.preferredHeight: 20
                                            source: "qrc:/colors/red_color.jpg"
                                        }
                                        Label {text: "Red: "; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        Label {text: camController.colorPercents[0] + "%"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        Image {
                                            Layout.preferredWidth: 20
                                            Layout.preferredHeight: 20
                                            source: "qrc:/colors/orange_color.jpg"
                                        }
                                        Label {text: "Orange: "; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        Label {text: camController.colorPercents[1] + "%"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        Image {
                                            Layout.preferredWidth: 20
                                            Layout.preferredHeight: 20
                                            source: "qrc:/colors/yellow_color.jpg"
                                        }
                                        Label {text: "Yellow: "; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        Label {text: camController.colorPercents[2] + "%"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        Image {
                                            Layout.preferredWidth: 20
                                            Layout.preferredHeight: 20
                                            source: "qrc:/colors/green_color.jpg"
                                        }
                                        Label {text: "Green: "; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        Label {text: camController.colorPercents[3] + "%"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        Image {
                                            Layout.preferredWidth: 20
                                            Layout.preferredHeight: 20
                                            source: "qrc:/colors/blue_color.jpg"
                                        }
                                        Label {text: "Other: "; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        Label {text: camController.colorPercents[4] + "%"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                    }
                                    ToolSeparator{
                                        Layout.fillWidth: true
                                        orientation: Qt.Horizontal
                                    }
                                    RowLayout{
                                        Layout.alignment: Qt.AlignHCenter
                                        spacing: 5
                                        Image {
                                            Layout.preferredWidth: 20
                                            Layout.preferredHeight: 20
                                            source: "qrc:/colors/red_color.jpg"
                                        }
                                        Label {text: "Red: "; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        Label {text: camController.colorPercents[5] + "%"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        Image {
                                            Layout.preferredWidth: 20
                                            Layout.preferredHeight: 20
                                            source: "qrc:/colors/orange_color.jpg"
                                        }
                                        Label {text: "Orange: "; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        Label {text: camController.colorPercents[6] + "%"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        Image {
                                            Layout.preferredWidth: 20
                                            Layout.preferredHeight: 20
                                            source: "qrc:/colors/yellow_color.jpg"
                                        }
                                        Label {text: "Yellow: "; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        Label {text: camController.colorPercents[7] + "%"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        Image {
                                            Layout.preferredWidth: 20
                                            Layout.preferredHeight: 20
                                            source: "qrc:/colors/green_color.jpg"
                                        }
                                        Label {text: "Green: "; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        Label {text: camController.colorPercents[8] + "%"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                        Image {
                                            Layout.preferredWidth: 20
                                            Layout.preferredHeight: 20
                                            source: "qrc:/colors/blue_color.jpg"
                                        }
                                        Label {text: "Other: "; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                        Label {text: camController.colorPercents[9] + "%"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    }
                                    Item {
                                        Layout.fillHeight: true
                                    }

                                }


                            }


                        }

                        Item{
                            Layout.preferredWidth: 20
                        }

                        ColumnLayout{
                            Layout.preferredWidth: 400

                            RowLayout
                            {
                                Layout.alignment: Qt.AlignHCenter
                                Label{
                                    text: "Select Frame"
                                    font.pixelSize: 16
                                    color: "white"
                                }

                                Item{
                                    Layout.preferredWidth: 5
                                }

                                ComboBox
                                {
                                    id: colorFrameComboBox
                                    Layout.preferredHeight: 45
                                    Layout.preferredWidth: 70

                                    Component.onCompleted: {
                                        model = ["1", "2", "3", "4"]
                                        // dbController.readLastRun()

                                        // currentIndex = speedMainComboBox.find(dbController.lastRun[1])
                                        // cmbsOpened[1] = true

                                        // weightController.speed = currentValue
                                    }
                                    onCurrentValueChanged: {
                                        camController.setIdxFrameIdx(0, currentValue)
                                        defectFrameComboBox.currentIndex = find(currentValue)
                                        settingFrameComboBox.currentIndex = find(currentValue)
                                    }
                                }
                            }

                            RowLayout{
                                Layout.alignment: Qt.AlignHCenter

                                ColumnLayout{

                                    Label {text: "Original"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    Rectangle {
                                        color: "white"
                                        Layout.alignment: Qt.AlignHCenter
                                        radius: 10
                                        //anchors.centerIn: parent
                                        //property real _orgImgRatio: camController.inputResulation.width/camController.inputResulation.height
                                        //property real _parentRatio: frame_box.width/frame_box.height

                                        //width: 380
                                        //height: 380
                                        Layout.preferredWidth: 360
                                        Layout.preferredHeight: 360

                                        CaptureViewPort{
                                            id: viewPortOrgColor1
                                            //anchors.fill: parent
                                            anchors.centerIn: parent
                                        }
                                    }
                                }

                                ColumnLayout{
                                    Label {text: "Color"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    Rectangle {
                                        color: "white"
                                        Layout.alignment: Qt.AlignHCenter
                                        radius: 10
                                        //anchors.centerIn: parent
                                        //property real _orgImgRatio: camController.inputResulation.width/camController.inputResulation.height
                                        //property real _parentRatio: frame_box.width/frame_box.height

                                        //width: 380
                                        //height: 380
                                        Layout.preferredWidth: 360
                                        Layout.preferredHeight: 360

                                        CaptureViewPort{
                                            id: viewPortColor1
                                            //anchors.fill: parent
                                            anchors.centerIn: parent
                                        }
                                    }
                                }

                            }

                            RowLayout{
                                Layout.alignment: Qt.AlignHCenter

                                ColumnLayout{
                                    Rectangle {
                                        color: "white"
                                        Layout.alignment: Qt.AlignHCenter
                                        radius: 10
                                        //anchors.centerIn: parent
                                        //property real _orgImgRatio: camController.inputResulation.width/camController.inputResulation.height
                                        //property real _parentRatio: frame_box.width/frame_box.height

                                        //width: 380
                                        //height: 380
                                        Layout.preferredWidth: 360
                                        Layout.preferredHeight: 360

                                        CaptureViewPort{
                                            id: viewPortOrgColor2
                                            //anchors.fill: parent
                                            anchors.centerIn: parent
                                        }
                                    }
                                }

                                ColumnLayout{
                                    Rectangle {
                                        color: "white"
                                        Layout.alignment: Qt.AlignHCenter
                                        radius: 10
                                        //anchors.centerIn: parent
                                        //property real _orgImgRatio: camController.inputResulation.width/camController.inputResulation.height
                                        //property real _parentRatio: frame_box.width/frame_box.height

                                        //width: 380
                                        //height: 380
                                        Layout.preferredWidth: 360
                                        Layout.preferredHeight: 360

                                        CaptureViewPort{
                                            id: viewPortColor2
                                            //anchors.fill: parent
                                            anchors.centerIn: parent
                                        }
                                    }
                                }

                            }


                            CheckBox{
                                Layout.alignment: Qt.AlignHCenter
                                text: "Show"
                                id: colorTabChb

                                onCheckedChanged: {
                                    if(camController.cameraIsStarted[0] === false && camController.cameraIsStarted[0] === false)
                                    {
                                        showMessageBox("Camera(s) Not Started")
                                        checked = false
                                        return
                                    }

                                    camController.setIdxShowChecks(1, checked)
                                    camController.showChanged()

                                    if(checked)
                                    {
                                        viewPortOrgColor1.width = 360
                                        viewPortOrgColor1.height = 360
                                        viewPortOrgColor1.id_viewPort = 3
                                        camController.appendViewPort(viewPortOrgColor1)

                                        viewPortColor1.width = 360
                                        viewPortColor1.height = 360
                                        viewPortColor1.id_viewPort = 4
                                        camController.appendViewPort(viewPortColor1)

                                        viewPortOrgColor2.width = 360
                                        viewPortOrgColor2.height = 360
                                        viewPortOrgColor2.id_viewPort = 5
                                        camController.appendViewPort(viewPortOrgColor2)

                                        viewPortColor2.width = 360
                                        viewPortColor2.height = 360
                                        viewPortColor2.id_viewPort = 6
                                        camController.appendViewPort(viewPortColor2)

                                    }
                                    else
                                    {
                                    }
                                }
                            }
                        }
                        Item{
                            Layout.fillWidth: true
                        }
                    }
                }

                Item {
                    id: confidenceTab

                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height

                    RowLayout{
                        anchors.fill: parent

                        Item{
                            Layout.fillWidth: true
                        }

                        Rectangle{
                            Layout.preferredWidth: 580
                            Layout.preferredHeight: 550
                            radius: 10
                            color: rectColor

                            ColumnLayout{
                                anchors.fill: parent
                                spacing: 0

                                ComboBox {
                                    id: defectComboBox

                                    Layout.alignment: Qt.AlignHCenter


                                    Component.onCompleted: {
                                        camController.readDb(currentValue)
                                        updateClassesModel()
                                    }

                                    onCurrentValueChanged: {
                                        camController.readDb(currentValue)
                                        updateClassesModel()
                                    }
                                }

                                RowLayout {
                                    Layout.alignment: Qt.AlignHCenter
                                    Item {Layout.preferredWidth: 5}

                                    Component.onCompleted: {
                                        classesModel.clear()
                                        for (var i = 0; i < camController.classNamesList.length; ++i){
                                            // if(!camController.classEnableList[i])
                                            //     continue

                                            if(i !== 0 && i !==  3 && i !== 4)
                                                classesModel.append({
                                                                        ID: i,
                                                                        // Enabled: camController.classEnableList[i],
                                                                        Name: camController.classNamesList[i],
                                                                        Conf: camController.classConfList[i],
                                                                        //                                        Quantity: camController.classQuantityList[i],
                                                                        Quantity: camController.quantityBasedClassCountList[i],
                                                                        QuantityEnabled: camController.classQuantityEnabledList[i],
                                                                        QuantityConf: camController.quantityBasedClassConfList[i]
                                                                    })
                                        }
                                    }
                                }

                                Item {Layout.preferredHeight: 10}
                                RowLayout {
                                    spacing: 0
                                    Item{ Layout.preferredWidth: 60}
                                    Label {text: "Class Name"; color:"#00d76a";font.pixelSize: 15}
                                    Item{ Layout.preferredWidth: 25}
                                    Label {text: "Defect Num."; color:"#00d76a";font.pixelSize: 15}
                                    Item{ Layout.preferredWidth: 10}
                                    Label {text: "Quantity Conf"; color:"#00d76a";font.pixelSize: 15;}
                                    Item{ Layout.preferredWidth: 10}
                                    Label {text: "Confidence"; color:"#00d76a";font.pixelSize: 15}
                                }
                                ListView {
                                    Layout.preferredHeight: 300
                                    Layout.preferredWidth: 550
                                    Layout.alignment: Qt.AlignHCenter

                                    model: classesModel

                                    delegate: RowLayout {
                                        // Layout.preferredHeight: parent.height
                                        // Layout.preferredWidth: parent.width
                                        Layout.alignment: Qt.AlignHCenter

                                        Item{
                                            Layout.preferredWidth: 40
                                        }

                                        Label {
                                            id: className
                                            Layout.preferredWidth: 75
                                            text: model.Name
                                        }
                                        ToolSeparator { Layout.fillHeight: true; orientation: Qt.Vertical }
                                        TextField {
                                            id: defectNumberTxt
                                            Layout.preferredWidth: 65
                                            selectByMouse: true
                                            text: model.Quantity
                                            horizontalAlignment: Text.AlignHCenter
                                        }

                                        ToolSeparator { Layout.fillHeight: true; orientation: Qt.Vertical }
                                        TextField {
                                            id: quantityConfTxt
                                            Layout.preferredWidth: 65
                                            selectByMouse: true
                                            text: model.QuantityConf
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        ToolSeparator { Layout.fillHeight: true; orientation: Qt.Vertical }
                                        TextField {
                                            id: confTxt
                                            Layout.preferredWidth: 65
                                            selectByMouse: true
                                            text: model.Conf
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        ToolSeparator { Layout.fillHeight: true; orientation: Qt.Vertical }
                                        Button {
                                            text: "Save"

                                            onClicked: {
                                                dbController.setIdx("Confidences", defectComboBox.currentValue, 0)
                                                dbController.setIdx("Confidences", className.text, 1)
                                                dbController.setIdx("Confidences", confTxt.text, 2)
                                                dbController.setIdx("Confidences", defectNumberTxt.text, 3)
                                                dbController.setIdx("Confidences", quantityConfTxt.text, 4)

                                                dbController.updateConfidences(defectComboBox.currentValue, className.text)

                                                camController.setClassesConf(model.ID, confTxt.text);
                                                camController.setclassQuantity(model.ID, defectNumberTxt.text)

                                                showMessageBox("Defect " + className.text + " Setting Updated")
                                            }
                                        }
                                    }

                                }

                                RowLayout{
                                    spacing: 20
                                    Layout.alignment: Qt.AlignCenter


                                    MyButton {
                                        id: editDefect
                                        text: "Edit"
                                        icon_btn: "qrc:/Icons/draw.png"
                                        Layout.preferredHeight: 50
                                        Layout.preferredWidth: 120
                                        Layout.alignment: Qt.AlignHCenter

                                        buttonColor: editColor

                                        onClicked: {
                                            showMessageBox("Exit Feature Updated")
                                            dbController.fruit[0] = defectText.text
                                            dbController.updateFruit(defectComboBox.currentValue)
                                            camController.readDb(defectComboBox.currentValue)
                                            updateClassesModel()
                                            updateDefects()
                                        }
                                    }
                                    MyButton {
                                        id: deleteDefect
                                        text: "Delete"
                                        icon_btn: "qrc:/Icons/delete.png"
                                        Layout.preferredHeight: 50
                                        Layout.preferredWidth: 120
                                        Layout.alignment: Qt.AlignHCenter

                                        buttonColor: stopColor

                                        onClicked: {
                                            if(dbController.getFruitExfs().length === 1)
                                            {
                                                showMessageBox("Can Not Delete All Exit Features")

                                                return
                                            }

                                            showMessageBox("Exit Feature Deleted")
                                            dbController.deleteFruit(defectComboBox.currentValue)

                                            updateDefects()

                                            defectComboBox.currentIndex = 0

                                            camController.readDb(defectComboBox.currentValue)
                                            updateClassesModel()
                                        }

                                    }
                                    MyButton {
                                        id: addDefect
                                        text: "Add"
                                        icon_btn: "qrc:/Icons/add.png"
                                        Layout.preferredHeight: 50
                                        Layout.preferredWidth: 120
                                        Layout.alignment: Qt.AlignHCenter

                                        buttonColor: startColor

                                        onClicked: {
                                            var fruits = ["Apple","Mis_shape","Nubbin","Top","Bottom","Blotch","Scab","Bruising","Cut"]

                                            for(var j=0;j<dbController.getFruitExfs().length;j++) {
                                                if(defectText.text === dbController.getFruitExfs()[j])
                                                {
                                                    showMessageBox("Exit Feature Already Exists")

                                                    return
                                                }
                                            }

                                            dbController.setIdx("Fruits", defectText.text, 0)
                                            dbController.insert("Fruits")

                                            for(var i=0;i<fruits.length;i++)
                                            {
                                                dbController.setIdx("Confidences", defectText.text, 0)
                                                dbController.setIdx("Confidences", fruits[i], 1)
                                                dbController.setIdx("Confidences", "50", 2)
                                                dbController.setIdx("Confidences", "10", 3)
                                                dbController.setIdx("Confidences", "50", 4)

                                                dbController.insert("Confidences")
                                            }

                                            showMessageBox("Exit Feature Added")

                                            camController.readDb(defectText.text)
                                            updateClassesModel()

                                            updateDefects()

                                            console.log(defectText.text)

                                            defectComboBox.currentIndex = defectComboBox.find(defectText.text)
                                        }


                                    }
                                    TextField {
                                        id: defectText
                                        text: defectComboBox.currentValue
                                        color: "white"
                                        Layout.preferredWidth: 100
                                        font.pixelSize: 16
                                        selectByMouse: true
                                        Layout.alignment: Qt.AlignHCenter
                                        horizontalAlignment: Text.AlignHCenter

                                    }

                                }

                            }


                        }

                        Item{
                            Layout.preferredWidth: 20
                        }

                        ColumnLayout{
                            Layout.preferredWidth: 400

                            RowLayout
                            {
                                Layout.alignment: Qt.AlignHCenter
                                Label{
                                    text: "Select Frame"
                                    font.pixelSize: 16
                                    color: "white"
                                }

                                Item{
                                    Layout.preferredWidth: 5
                                }

                                ComboBox
                                {
                                    id: defectFrameComboBox
                                    Layout.preferredHeight: 45
                                    Layout.preferredWidth: 70

                                    Component.onCompleted: {
                                        model = ["1", "2", "3", "4"]
                                        // dbController.readLastRun()

                                        // currentIndex = speedMainComboBox.find(dbController.lastRun[1])
                                        // cmbsOpened[1] = true

                                        // weightController.speed = currentValue
                                    }
                                    onCurrentValueChanged: {
                                        camController.setIdxFrameIdx(0, currentValue)
                                        settingFrameComboBox.currentIndex = find(currentValue)
                                        colorFrameComboBox.currentIndex = find(currentValue)
                                    }
                                }
                            }
                            RowLayout{
                                Layout.alignment: Qt.AlignHCenter

                                ColumnLayout{
                                    Label {text: "Original"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    Rectangle {
                                        color: "white"
                                        Layout.alignment: Qt.AlignHCenter
                                        radius: 10
                                        //anchors.centerIn: parent
                                        //property real _orgImgRatio: camController.inputResulation.width/camController.inputResulation.height
                                        //property real _parentRatio: frame_box.width/frame_box.height

                                        //width: 380
                                        //height: 380
                                        Layout.preferredWidth: 360
                                        Layout.preferredHeight: 360

                                        CaptureViewPort{
                                            id: viewPortOrgDefect1
                                            //anchors.fill: parent
                                            anchors.centerIn: parent
                                        }
                                    }
                                }

                                ColumnLayout{
                                    Label {text: "Defect"; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                                    Rectangle {
                                        color: "white"
                                        Layout.alignment: Qt.AlignHCenter
                                        radius: 10
                                        //anchors.centerIn: parent
                                        //property real _orgImgRatio: camController.inputResulation.width/camController.inputResulation.height
                                        //property real _parentRatio: frame_box.width/frame_box.height

                                        //width: 380
                                        //height: 380
                                        Layout.preferredWidth: 360
                                        Layout.preferredHeight: 360

                                        CaptureViewPort{
                                            id: viewPortDefect1
                                            //anchors.fill: parent
                                            anchors.centerIn: parent
                                        }
                                    }
                                }

                            }

                            RowLayout{
                                Layout.alignment: Qt.AlignHCenter

                                ColumnLayout{


                                    Rectangle {
                                        color: "white"
                                        Layout.alignment: Qt.AlignHCenter
                                        radius: 10
                                        //anchors.centerIn: parent
                                        //property real _orgImgRatio: camController.inputResulation.width/camController.inputResulation.height
                                        //property real _parentRatio: frame_box.width/frame_box.height

                                        //width: 380
                                        //height: 380
                                        Layout.preferredWidth: 360
                                        Layout.preferredHeight: 360

                                        CaptureViewPort{
                                            id: viewPortOrgDefect2
                                            //anchors.fill: parent
                                            anchors.centerIn: parent
                                        }
                                    }
                                }

                                ColumnLayout{
                                    Rectangle {
                                        color: "white"
                                        Layout.alignment: Qt.AlignHCenter
                                        radius: 10
                                        //anchors.centerIn: parent
                                        //property real _orgImgRatio: camController.inputResulation.width/camController.inputResulation.height
                                        //property real _parentRatio: frame_box.width/frame_box.height

                                        //width: 380
                                        //height: 380
                                        Layout.preferredWidth: 360
                                        Layout.preferredHeight: 360

                                        CaptureViewPort{
                                            id: viewPortDefect2
                                            //anchors.fill: parent
                                            anchors.centerIn: parent
                                        }
                                    }
                                }

                            }

                            CheckBox{
                                Layout.alignment: Qt.AlignHCenter
                                text: "Show"
                                id: defectTabChb

                                onCheckedChanged: {
                                    if(camController.cameraIsStarted[0] === false && camController.cameraIsStarted[0] === false)
                                    {
                                        showMessageBox("Camera(s) Not Started")
                                        checked = false
                                        return
                                    }

                                    camController.setIdxShowChecks(2, checked)
                                    camController.showChanged()

                                    if(checked)
                                    {
                                        viewPortOrgDefect1.width = 360
                                        viewPortOrgDefect1.height = 360
                                        viewPortOrgDefect1.id_viewPort = 7
                                        camController.appendViewPort(viewPortOrgDefect1)

                                        viewPortDefect1.width = 360
                                        viewPortDefect1.height = 360
                                        viewPortDefect1.id_viewPort = 8
                                        camController.appendViewPort(viewPortDefect1)

                                        viewPortOrgDefect2.width = 360
                                        viewPortOrgDefect2.height = 360
                                        viewPortOrgDefect2.id_viewPort = 9
                                        camController.appendViewPort(viewPortOrgDefect2)

                                        viewPortDefect2.width = 360
                                        viewPortDefect2.height = 360
                                        viewPortDefect2.id_viewPort = 10
                                        camController.appendViewPort(viewPortDefect2)
                                    }
                                    else
                                    {
                                    }
                                }
                            }
                        }

                        Item{
                            Layout.fillWidth: true
                        }

                    }
                }

            }

        }
    }

    Popup {
        id: calendarPopup
        width: 500
        height: 450
        visible: false
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        background: Rectangle {
            color: "#181818"
        }

        closePolicy: "NoAutoClose"

        onClosed: {
            var currentDay = ""
            currentDay = Qt.formatDate(calendar.selectedDate, "yyyy-MM-dd")

            // Calculate the next day
            var nextDay = new Date(calendar.selectedDate)
            nextDay.setDate(calendar.selectedDate.getDate() + 1)

            // Format the next day as yyyy-MM-dd
            var year = nextDay.getFullYear()
            var month = ("0" + (nextDay.getMonth() + 1)).slice(-2)
            var day = ("0" + nextDay.getDate()).slice(-2)

            currentDay += " 00:00:00"
            nextDay = year + "-" + month + "-" + day + " 00:00:00"

            dbController.readReport(currentDay, nextDay)
            dbController.showReport()
        }

        ColumnLayout{
            anchors.fill: parent

            spacing: 0

            RowLayout{
                Item{
                    Layout.fillWidth: true
                }

                MyButton{
                    text: "X"
                    fontsize: 15
                    buttonColor: "red"
                    icon_visible: false
                    width: 25
                    height: 25
                    radius: 20
                    bottonMarg: 0

                    onClicked: {
                        calendarPopup.close()
                    }
                }
            }

            CustomCalendar
            {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 350
                Layout.preferredWidth: 350

                id: calendar
            }
        }




    }

    Popup {
        id: reportPopup
        width: 1300
        height: 580
        visible: false
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        background: Rectangle {
            color: "#121212"
            radius: 10
        }

        Component.onCompleted: {


        }

        closePolicy: "NoAutoClose"

        onVisibleChanged: {

        }

        ColumnLayout{
            anchors.fill: parent

            spacing: 0

            RowLayout{
                Item{
                    Layout.fillWidth: true
                }

                MyButton{
                    text: "X"
                    fontsize: 15
                    buttonColor: "red"
                    icon_visible: false
                    width: 25
                    height: 25
                    radius: 20
                    bottonMarg: 0

                    onClicked: {
                        reportPopup.close()
                    }
                }
            }

            Item{
                Layout.preferredHeight: 20
            }

            TableView {
                id: tt
                Layout.preferredWidth: 1250
                Layout.preferredHeight: 35
                Layout.alignment: Qt.AlignHCenter
                rowSpacing: 1
                clip: true

                model: TableModel {
                    TableModelColumn { display: "1" }
                    TableModelColumn { display: "2" }
                    TableModelColumn { display: "3" }
                    TableModelColumn { display: "4" }
                    TableModelColumn { display: "5" }
                    TableModelColumn { display: "6" }
                    TableModelColumn { display: "7" }
                    TableModelColumn { display: "8" }
                    TableModelColumn { display: "9" }
                    TableModelColumn { display: "10" }
                    TableModelColumn { display: "11" }
                    TableModelColumn { display: "12" }
                    TableModelColumn { display: "13" }


                    rows: [
                        {
                            "1": "Start",
                            "2": "Stop",
                            "3": "Exf",
                            "4": "Exit A",
                            "5": "Exit B",
                            "6": "Exit C",
                            "7": "Exit D",
                            "8": "Exit E",
                            "9": "Exit F",
                            "10": "Exit G",
                            "11": "Exit H",
                            "12": "Runtime",
                            "13": "Weight"
                        }
                    ]
                }

                delegate: Rectangle {
                    implicitWidth: 95
                    implicitHeight: 35
                    border.width: 0
                    color: rectColor


                    Text {
                        text: model.display
                        anchors.centerIn: parent
                        color: "white"
                    }
                }
            }

            Item{
                Layout.preferredHeight: 5
            }

            TableView {
                Layout.preferredWidth: 1250
                Layout.preferredHeight: 300
                Layout.alignment: Qt.AlignHCenter
                rowSpacing: 1
                clip: true

                model: TableModel {
                    id: reportTable
                    TableModelColumn { display: "0" }
                    TableModelColumn { display: "1" }
                    TableModelColumn { display: "2" }
                    TableModelColumn { display: "3" }
                    TableModelColumn { display: "4" }
                    TableModelColumn { display: "5" }
                    TableModelColumn { display: "6" }
                    TableModelColumn { display: "7" }
                    TableModelColumn { display: "8" }
                    TableModelColumn { display: "9" }
                    TableModelColumn { display: "10" }
                    TableModelColumn { display: "11" }
                    TableModelColumn { display: "12" }

                }

                delegate: Rectangle {
                    implicitWidth: 95
                    implicitHeight: 35
                    border.width: 0
                    color: rectColor

                    Text {
                        text: model.display
                        anchors.centerIn: parent
                        color: "white"
                    }
                }

                Connections {
                    target: dbController

                    function onShowReport() {
                        var data = []

                        reportTable.clear()


                        for (var i=0; i < dbController.reportData.length; ++i){
                            reportTable.appendRow({"0": "", "1": "", "2": "", "3": "", "4": "", "5": "", "6": "", "7": "", "8": "", "9": "", "10": "", "11": "", "12": ""});
                        }


                        for(i=0; i < dbController.reportData.length; ++i)
                        {
                            var rdata = []
                            var rowData = {}
                            for(var j=0; j < dbController.reportData[i].length; ++j)
                            {
                                if(dbController.reportData[i][j].includes(" "))
                                    rowData[j.toString()] = dbController.reportData[i][j].split(' ')[1].toString()
                                else
                                    rowData[j.toString()] = dbController.reportData[i][j].toString()
                            }
                            reportTable.setRow(i, rowData)
                        }
                    }
                }
            }

            Item{
                Layout.preferredHeight: 80
            }

            RowLayout{
                spacing: 25
                Layout.alignment: Qt.AlignHCenter


                Rectangle {
                    id: calendarBtn
                    width: 48
                    height: 48
                    color: "#121212" // Default background color
                    radius: 5

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            calendarPopup.open()

                        }


                        hoverEnabled: true
                        onEntered: {
                            calendarBtn.color = "#00d76a"
                        }
                        onExited: {
                            calendarBtn.color = "#121212"
                        }
                    }

                    // Image to display the icon
                    Image {
                        source: "qrc:/Icons/calendar.png"
                        anchors.centerIn: parent
                        width: 48
                        height: 48
                    }
                }

                Rectangle {
                    id: excelBtn
                    width: 48
                    height: 48
                    color: "#121212" // Default background color
                    radius: 5

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if(dbController.reportData.length !== 0) {
                                ethController.writeToExcel(excelDate)
                                showMessageBox("Saved To Excel")
                            } else {
                                showMessageBox("No Data Available In The Selected Day")
                            }

                        }
                        hoverEnabled: true
                        onEntered: {
                            excelBtn.color = "#00d76a"
                        }
                        onExited: {
                            excelBtn.color = "#121212"
                        }
                    }

                    // Image to display the icon
                    Image {
                        source: "qrc:/Icons/excel.png"
                        anchors.centerIn: parent
                        width: 48
                        height: 48
                    }
                }

            }

            Item{
                Layout.fillHeight: true
            }


        }
    }

    Popup{
        id: poolPopup
        width: 1170
        height: 300
        visible: false
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        background: Rectangle {
            color: "#121212"
            radius: 10
        }

        RowLayout{
            anchors.fill: parent
            anchors.centerIn: parent
            Item{
                Layout.fillWidth: true
            }

            ColumnLayout{
                Label {text: "Leaf Pool"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Rectangle{
                    color: rectColor
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 150
                    Layout.alignment: Qt.AlignHCenter

                    radius: 10
                    border.color: "white"
                    border.width: 3

                    ColumnLayout{
                        anchors.centerIn: parent

                        Switch {
                            id: cup2Auto
                            text: "AUTO " + dbController.motor[13]
                            checked: dbController.motor[13] === "ON" ? true : false
                            font.pixelSize: 18
                            onToggled: {
                                text = checked ? "AUTO ON" : "AUTO OFF"
                                dbController.setIdx("Motors", text.split(' ')[1], 13)
                                dbController.updateMotors()
                            }
                        }
                        Switch {
                            id: cup2On
                            text: "MANUAL OFF"
                            checked: false
                            onToggled: {
                                text = checked ? "MANUAL ON" : "MANUAL OFF"
                                ethController.startManual(0, 3, checked)
                            }
                            font.pixelSize: 18
                        }

                    }


                }

            }

            Item{
                Layout.preferredWidth: 20
            }

            ColumnLayout{
                Label {text: "Water Pump Nozzle"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Rectangle{
                    color: rectColor
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 150
                    Layout.alignment: Qt.AlignHCenter

                    radius: 10
                    border.color: "white"
                    border.width: 3

                    ColumnLayout{
                        anchors.centerIn: parent

                        Switch {
                            id: nozzleAuto
                            text: "AUTO " + dbController.motor[0]
                            checked: dbController.motor[0] === "ON" ? true : false
                            font.pixelSize: 18
                            onToggled: {
                                text = checked ? "AUTO ON" : "AUTO OFF"
                                dbController.setIdx("Motors", text.split(' ')[1], 0)
                                dbController.updateMotors()
                            }
                        }
                        Switch {
                            id: nozzleOn
                            text: "MANUAL OFF"
                            checked: false
                            onToggled: {
                                text = checked ? "MANUAL ON" : "MANUAL OFF"
                                ethController.startManual(0, 4, checked)
                            }
                            font.pixelSize: 18
                        }

                    }


                }

            }

            Item{
                Layout.preferredWidth: 20
            }

            ColumnLayout{
                Label {text: "Water Pump"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Rectangle{
                    color: rectColor
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 150
                    Layout.alignment: Qt.AlignHCenter

                    radius: 10
                    border.color: "white"
                    border.width: 3

                    ColumnLayout{
                        anchors.centerIn: parent

                        Switch {
                            id: waterPumpAuto
                            text: "AUTO " + dbController.motor[1]
                            checked: dbController.motor[1] === "ON" ? true : false
                            font.pixelSize: 18
                            onToggled: {
                                text = checked ? "AUTO ON" : "AUTO OFF"
                                dbController.setIdx("Motors", text.split(' ')[1], 1)
                                dbController.updateMotors()
                            }
                        }
                        Switch {
                            id: waterPumpOn
                            text: "MANUAL OFF"
                            checked: false
                            onToggled: {
                                text = checked ? "MANUAL ON" : "MANUAL OFF"
                                ethController.startManual(0, 0, checked)
                            }
                            font.pixelSize: 18
                        }

                    }


                }

            }

            Item{
                Layout.preferredWidth: 20
            }

            ColumnLayout{
                Label {text: "Rollic Conveyor"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Rectangle{
                    color: rectColor
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 150
                    Layout.alignment: Qt.AlignHCenter

                    radius: 10
                    border.color: "white"
                    border.width: 3

                    ColumnLayout{
                        anchors.centerIn: parent

                        Switch {
                            id: rollicAuto
                            text: "AUTO " + dbController.motor[2]
                            checked: dbController.motor[2] === "ON" ? true : false
                            font.pixelSize: 18
                            onToggled: {
                                text = checked ? "AUTO ON" : "AUTO OFF"
                                dbController.setIdx("Motors", text.split(' ')[1], 2)
                                dbController.updateMotors()
                            }
                        }
                        Switch {
                            id: rollicOn
                            text: "MANUAL OFF"
                            checked: false
                            onToggled: {
                                text = checked ? "MANUAL ON" : "MANUAL OFF"
                                ethController.setManualStatus(0, checked)
                            }
                            font.pixelSize: 18
                        }

                    }


                }

            }

            Item{
                Layout.preferredWidth: 20
            }

            ColumnLayout{
                Label {text: "Observation"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Rectangle{
                    color: rectColor
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 150
                    Layout.alignment: Qt.AlignHCenter

                    radius: 10
                    border.color: "white"
                    border.width: 3

                    ColumnLayout{
                        anchors.centerIn: parent

                        Switch {
                            id: observationAuto
                            text: "AUTO " + dbController.motor[3]
                            checked: dbController.motor[3] === "ON" ? true : false
                            font.pixelSize: 18
                            onToggled: {
                                text = checked ? "AUTO ON" : "AUTO OFF"
                                dbController.setIdx("Motors", text.split(' ')[1], 3)
                                dbController.updateMotors()
                            }
                        }
                        Switch {
                            id: observationOn
                            text: "MANUAL OFF"
                            checked: false
                            onToggled: {
                                text = checked ? "MANUAL ON" : "MANUAL OFF"
                                ethController.setManualStatus(1, checked)
                            }
                            font.pixelSize: 18
                        }

                    }


                }

            }


            Item{
                Layout.fillWidth: true
            }

        }

    }

    Popup{
        id: brushingPopup
        width: 750
        height: 450
        visible: false
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        background: Rectangle {
            color: "#121212"
            radius: 10
        }

        ColumnLayout{
            anchors.fill: parent

            RowLayout{
                Item{
                    Layout.fillWidth: true
                }

                ColumnLayout{
                    Label {text: "Brushing Machine"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                    Rectangle{
                        color: rectColor
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        radius: 10
                        border.color: "white"
                        border.width: 3

                        ColumnLayout{
                            anchors.centerIn: parent

                            Switch {
                                id: brushAuto
                                text: "AUTO " + dbController.motor[4]
                                checked: dbController.motor[4] === "ON" ? true : false
                                font.pixelSize: 18
                                onToggled: {
                                    text = checked ? "AUTO ON" : "AUTO OFF"
                                    dbController.setIdx("Motors", text.split(' ')[1], 4)
                                    dbController.updateMotors()
                                }
                            }
                            Switch {
                                id: brushOn
                                text: "MANUAL OFF"
                                checked: false
                                onToggled: {
                                    text = checked ? "MANUAL ON" : "MANUAL OFF"
                                    ethController.startManual(0, 1, checked)
                                }
                                font.pixelSize: 18
                            }

                        }
                    }
                }

                Item{
                    Layout.preferredWidth: 20
                }

                ColumnLayout{
                    Label {text: "Fan"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                    Rectangle{
                        color: rectColor
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        radius: 10
                        border.color: "white"
                        border.width: 3

                        ColumnLayout{
                            anchors.centerIn: parent

                            Switch {
                                id: fanAuto
                                text: "AUTO " + dbController.motor[5]
                                checked: dbController.motor[5] === "ON" ? true : false
                                font.pixelSize: 18
                                onToggled: {
                                    text = checked ? "AUTO ON" : "AUTO OFF"
                                    dbController.setIdx("Motors", text.split(' ')[1], 5)
                                    dbController.updateMotors()
                                }
                            }
                            Switch {
                                id: fanOn
                                text: "MANUAL OFF"
                                checked: false
                                onToggled: {
                                    text = checked ? "MANUAL ON" : "MANUAL OFF"
                                    ethController.startManual(0, 10, checked)
                                }
                                font.pixelSize: 18
                            }
                        }

                    }

                }

                Item{
                    Layout.preferredWidth: 20
                }

                ColumnLayout{
                    Label {text: "Pre Aligner"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                    Rectangle{
                        color: rectColor
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        radius: 10
                        border.color: "white"
                        border.width: 3

                        ColumnLayout{
                            anchors.centerIn: parent

                            Switch {
                                id: prealignerAuto
                                text: "AUTO " + dbController.motor[6]
                                checked: dbController.motor[6] === "ON" ? true : false
                                font.pixelSize: 18
                                onToggled: {
                                    text = checked ? "AUTO ON" : "AUTO OFF"
                                    dbController.setIdx("Motors", text.split(' ')[1], 6)
                                    dbController.updateMotors()
                                }
                            }
                            Switch {
                                id: prealignerOn
                                text: "MANUAL OFF"
                                checked: false
                                onToggled: {
                                    text = checked ? "MANUAL ON" : "MANUAL OFF"
                                    ethController.setManualStatus(2, checked)
                                }
                                font.pixelSize: 18
                            }

                        }


                    }

                }

                Item{
                    Layout.fillWidth: true
                }
            }

            RowLayout{
                Item{
                    Layout.fillWidth: true
                }

                ColumnLayout{
                    Label {text: "Heater 33%"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                    Rectangle{
                        color: rectColor
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        radius: 10
                        border.color: "white"
                        border.width: 3

                        ColumnLayout{
                            anchors.centerIn: parent

                            Switch {
                                id: heater1Auto
                                text: "AUTO " + dbController.motor[7]
                                checked: dbController.motor[7] === "ON" ? true : false
                                font.pixelSize: 18
                                onToggled: {
                                    text = checked ? "AUTO ON" : "AUTO OFF"
                                    dbController.setIdx("Motors", text.split(' ')[1], 7)
                                    dbController.updateMotors()
                                }
                            }
                            Switch {
                                id: heater1On
                                text: "MANUAL OFF"
                                checked: false
                                onToggled: {
                                    text = checked ? "MANUAL ON" : "MANUAL OFF"
                                    ethController.setManualStatus(6, checked)
                                }
                                font.pixelSize: 18
                            }

                        }


                    }

                }

                Item{
                    Layout.preferredWidth: 20
                }

                ColumnLayout{
                    Label {text: "Heater 66%"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                    Rectangle{
                        color: rectColor
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        radius: 10
                        border.color: "white"
                        border.width: 3

                        ColumnLayout{
                            anchors.centerIn: parent

                            Switch {
                                id: heater2Auto
                                text: "AUTO " + dbController.motor[8]
                                checked: dbController.motor[8] === "ON" ? true : false
                                font.pixelSize: 18
                                onToggled: {
                                    text = checked ? "AUTO ON" : "AUTO OFF"
                                    dbController.setIdx("Motors", text.split(' ')[1], 8)
                                    dbController.updateMotors()
                                }
                            }
                            Switch {
                                id: heater2On
                                text: "MANUAL OFF"
                                checked: false
                                onToggled: {
                                    text = checked ? "MANUAL ON" : "MANUAL OFF"
                                    ethController.setManualStatus(7, checked)
                                }
                                font.pixelSize: 18
                            }

                        }


                    }

                }

                Item{
                    Layout.preferredWidth: 20
                }

                ColumnLayout{
                    Label {text: "Heater 99%"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                    Rectangle{
                        color: rectColor
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        radius: 10
                        border.color: "white"
                        border.width: 3

                        ColumnLayout{
                            anchors.centerIn: parent

                            Switch {
                                id: heater3Auto
                                text: "AUTO " + dbController.motor[9]
                                checked: dbController.motor[9] === "ON" ? true : false
                                font.pixelSize: 18
                                onToggled: {
                                    text = checked ? "AUTO ON" : "AUTO OFF"
                                    dbController.setIdx("Motors", text.split(' ')[1], 9)
                                    dbController.updateMotors()
                                }
                            }
                            Switch {
                                id: heater3On
                                text: "MANUAL OFF"
                                checked: false
                                onToggled: {
                                    text = checked ? "MANUAL ON" : "MANUAL OFF"
                                    ethController.setManualStatus(8, checked)
                                }
                                font.pixelSize: 18
                            }

                        }


                    }

                }

                Item{
                    Layout.fillWidth: true
                }
            }
        }
    }

    Popup{
        id: sorterPopup
        width: 950
        height: 300
        visible: false
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        background: Rectangle {
            color: "#121212"
            radius: 10
        }

        RowLayout{
            anchors.fill: parent
            anchors.centerIn: parent
            Item{
                Layout.fillWidth: true
            }

            ColumnLayout{
                Label {text: "Aligner"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Rectangle{
                    color: rectColor
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 150
                    Layout.alignment: Qt.AlignHCenter

                    radius: 10
                    border.color: "white"
                    border.width: 3

                    ColumnLayout{
                        anchors.centerIn: parent

                        Switch {
                            id: alignerAuto
                            text: "AUTO " + dbController.motor[10]
                            checked: dbController.motor[10] === "ON" ? true : false
                            font.pixelSize: 18
                            onToggled: {
                                text = checked ? "AUTO ON" : "AUTO OFF"
                                dbController.setIdx("Motors", text.split(' ')[1], 10)
                                dbController.updateMotors()
                            }
                        }
                        Switch {
                            id: alignerOn
                            text: "MANUAL OFF"
                            checked: false
                            onToggled: {
                                text = checked ? "MANUAL ON" : "MANUAL OFF"
                                ethController.setManualStatus(3, checked)
                            }
                            font.pixelSize: 18
                        }

                    }


                }

            }

            Item{
                Layout.preferredWidth: 20
            }

            ColumnLayout{
                Label {text: "Grader"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Rectangle{
                    color: rectColor
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 150
                    Layout.alignment: Qt.AlignHCenter

                    radius: 10
                    border.color: "white"
                    border.width: 3

                    ColumnLayout{
                        anchors.centerIn: parent

                        Switch {
                            id: sorterAuto
                            text: "AUTO " + dbController.motor[11]
                            checked: dbController.motor[11] === "ON" ? true : false
                            font.pixelSize: 18
                            onToggled: {
                                text = checked ? "AUTO ON" : "AUTO OFF"
                                dbController.setIdx("Motors", text.split(' ')[1], 11)
                                dbController.updateMotors()
                            }
                        }
                        Switch {
                            id: sorterOn
                            text: "MANUAL OFF"
                            checked: false
                            onToggled: {
                                text = checked ? "MANUAL ON" : "MANUAL OFF"
                                ethController.setManualStatus(4, checked)
                            }
                            font.pixelSize: 18
                        }

                    }


                }

            }

            Item{
                Layout.preferredWidth: 20
            }

            ColumnLayout{
                Label {text: "Resort"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                Rectangle{
                    color: rectColor
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 150
                    Layout.alignment: Qt.AlignHCenter

                    radius: 10
                    border.color: "white"
                    border.width: 3

                    ColumnLayout{
                        anchors.centerIn: parent

                        Switch {
                            id: cup1Auto
                            text: "AUTO " + dbController.motor[12]
                            checked: dbController.motor[12] === "ON" ? true : false
                            font.pixelSize: 18
                            onToggled: {
                                text = checked ? "AUTO ON" : "AUTO OFF"
                                dbController.setIdx("Motors", text.split(' ')[1], 12)
                                dbController.updateMotors()
                            }
                        }
                        Switch {
                            id: cup1On
                            text: "MANUAL OFF"
                            checked: false
                            onToggled: {
                                text = checked ? "MANUAL ON" : "MANUAL OFF"
                                ethController.startManual(0, 2, checked)
                            }
                            font.pixelSize: 18
                        }

                    }


                }

            }



            Item{
                Layout.fillWidth: true
            }
        }

    }

    Popup{
        id: tablesPopup
        width: 750
        height: 500
        visible: false
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        background: Rectangle {
            color: "#121212"
            radius: 10
        }

        ColumnLayout{
            anchors.fill: parent
            spacing: 0

            RowLayout{
                Item{
                    Layout.fillWidth: true
                }

                ColumnLayout{
                    Label {text: "Exit 1,Exit 2"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                    Rectangle{
                        color: rectColor
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        radius: 10
                        border.color: "white"
                        border.width: 3

                        ColumnLayout{
                            anchors.centerIn: parent

                            Switch {
                                id: exitabAuto
                                text: "AUTO " + dbController.motor[14]
                                checked: dbController.motor[14] === "ON" ? true : false
                                font.pixelSize: 18
                                onToggled: {
                                    text = checked ? "AUTO ON" : "AUTO OFF"
                                    dbController.setIdx("Motors", text.split(' ')[1], 14)
                                    dbController.updateMotors()
                                }
                            }
                            Switch {
                                id: exitabOn
                                text: "MANUAL OFF"
                                checked: false
                                onToggled: {
                                    text = checked ? "MANUAL ON" : "MANUAL OFF"
                                    ethController.startManual(0, 5, checked)
                                }
                                font.pixelSize: 18
                            }

                        }


                    }

                }

                Item{
                    Layout.preferredWidth: 20
                }

                ColumnLayout{
                    Label {text: "Exit 3,Exit 4"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                    Rectangle{
                        color: rectColor
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        radius: 10
                        border.color: "white"
                        border.width: 3

                        ColumnLayout{
                            anchors.centerIn: parent

                            Switch {
                                id: exitcdAuto
                                text: "AUTO " + dbController.motor[15]
                                checked: dbController.motor[15] === "ON" ? true : false
                                font.pixelSize: 18
                                onToggled: {
                                    text = checked ? "AUTO ON" : "AUTO OFF"
                                    dbController.setIdx("Motors", text.split(' ')[1], 15)
                                    dbController.updateMotors()
                                }
                            }
                            Switch {
                                id: exitcdOn
                                text: "MANUAL OFF"
                                checked: false
                                onToggled: {
                                    text = checked ? "MANUAL ON" : "MANUAL OFF"
                                    ethController.startManual(0, 6, checked)
                                }
                                font.pixelSize: 18
                            }

                        }


                    }

                }

                Item{
                    Layout.preferredWidth: 20
                }

                ColumnLayout{
                    Label {text: "Exit 5,Exit 6"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                    Rectangle{
                        color: rectColor
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        radius: 10
                        border.color: "white"
                        border.width: 3

                        ColumnLayout{
                            anchors.centerIn: parent

                            Switch {
                                id: exitefAuto
                                text: "AUTO " + dbController.motor[16]
                                checked: dbController.motor[16] === "ON" ? true : false
                                font.pixelSize: 18
                                onToggled: {
                                    text = checked ? "AUTO ON" : "AUTO OFF"
                                    dbController.setIdx("Motors", text.split(' ')[1], 16)
                                    dbController.updateMotors()
                                }
                            }
                            Switch {
                                id: exitefOn
                                text: "MANUAL OFF"
                                checked: false
                                onToggled: {
                                    text = checked ? "MANUAL ON" : "MANUAL OFF"
                                    ethController.startManual(0, 7, checked)
                                }
                                font.pixelSize: 18
                            }

                        }


                    }

                }
                Item{
                    Layout.fillWidth: true
                }
            }


            RowLayout{
                Item{
                    Layout.fillWidth: true
                }

                ColumnLayout{
                    Label {text: "Exit 7,Exit 8"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                    Rectangle{
                        color: rectColor
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        radius: 10
                        border.color: "white"
                        border.width: 3

                        ColumnLayout{
                            anchors.centerIn: parent

                            Switch {
                                id: exitghAuto
                                text: "AUTO " + dbController.motor[17]
                                checked: dbController.motor[17] === "ON" ? true : false
                                font.pixelSize: 18
                                onToggled: {
                                    text = checked ? "AUTO ON" : "AUTO OFF"
                                    dbController.setIdx("Motors", text.split(' ')[1], 17)
                                    dbController.updateMotors()
                                }
                            }
                            Switch {
                                id: exitghOn
                                text: "MANUAL OFF"
                                checked: false
                                onToggled: {
                                    text = checked ? "MANUAL ON" : "MANUAL OFF"
                                    ethController.startManual(0,  8, checked)
                                }
                                font.pixelSize: 18
                            }

                        }


                    }

                }

                Item{
                    Layout.preferredWidth: 20
                }

                ColumnLayout{
                    Label {text: "Mechanical Exit"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}
                    Rectangle{
                        color: rectColor
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        radius: 10
                        border.color: "white"
                        border.width: 3

                        ColumnLayout{
                            anchors.centerIn: parent

                            Switch {
                                id: exitAuto
                                text: "AUTO " + dbController.motor[18]
                                checked: dbController.motor[18] === "ON" ? true : false
                                font.pixelSize: 18
                                onToggled: {
                                    text = checked ? "AUTO ON" : "AUTO OFF"
                                    dbController.setIdx("Motors", text.split(' ')[1], 18)
                                    dbController.updateMotors()
                                }
                            }
                            Switch {
                                id: exitOn
                                text: "MANUAL OFF"
                                checked: false
                                onToggled: {
                                    text = checked ? "MANUAL ON" : "MANUAL OFF"
                                    ethController.startManual(0, 9, checked)
                                }
                                font.pixelSize: 18
                            }

                        }


                    }

                }
                Item{
                    Layout.fillWidth: true
                }
            }

        }


    }


    background: Rectangle {
        color: "#121212"

    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout{
            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignHCenter

            MyButton {
                id: usersBtn
                //Layout.fillWidth: true
                icon_btn: "qrc:/Icons/report.png"
                buttonColor: "#121212"
                iconsize: 60

                hoverEnabled: true

                onEntered: {
                    buttonColor = "#00d76a"
                }
                onExited: {
                    buttonColor = "#121212"
                }

                border_width: 0

                onClicked: {
                    var currentDate = new Date();

                    // Get the current year
                    var currentYear = currentDate.getFullYear();

                    // Get the current month (0-indexed, so add 1 to get the actual month)
                    var currentMonth = currentDate.getMonth() + 1;
                    currentMonth = (currentMonth < 10 ? '0' : '') + currentMonth

                    // Get the current day of the month
                    var currentDay = currentDate.getDate();
                    currentDay = (currentDay < 10 ? '0' : '') + currentDay;

                    var tomorrow = new Date(currentDate);
                    tomorrow.setDate(currentDate.getDate() + 1);

                    // Get components of tomorrow's date
                    var tomorrowYear = tomorrow.getFullYear();

                    var tomorrowMonth = tomorrow.getMonth() + 1;
                    tomorrowMonth = (tomorrowMonth < 10 ? '0' : '') + tomorrowMonth

                    var tomorrowDayOfMonth = tomorrow.getDate();
                    tomorrowDayOfMonth = (tomorrowDayOfMonth < 10 ? '0' : '') + tomorrowDayOfMonth


                    var start = currentYear + "-" + currentMonth + "-" + currentDay + " 00:00:00"
                    var stop = tomorrowYear + "-" + tomorrowMonth + "-" + tomorrowDayOfMonth + " 00:00:00"

                    reportTable.clear()

                    excelDate = currentYear + "-" + currentMonth + "-" + currentDay

                    reportPopup.open()
                    dbController.readReport(start, stop)
                    dbController.showReport()
                    // if(dbController.report[0] !== "") {
                    //     weightController.writeToExcel(excelDate)
                    // } else {
                    //     showMessageBox("No Data Available In The Selected Day")
                    // }
                }

            }

            Item {
                Layout.fillWidth: true
            }

            Image {
                Layout.preferredWidth: 400
                Layout.preferredHeight: 120
                //Layout.alignment: Qt.AlignCenter
                source: "qrc:/Icons/Logo.png"
                Layout.alignment: Qt.AlignCenter
                Layout.leftMargin: 15

                //fillMode: .PreserveAspectFit
            }


            Item {
                Layout.fillWidth: true
            }

            MyButton {
                id: settingBtn
                //Layout.fillWidth: true
                icon_btn: "qrc:/Icons/setting.png"

                buttonColor: "#121212"
                iconsize: 60

                border_width: 0

                hoverEnabled: true

                onEntered: {
                    buttonColor = "#00d76a"
                }
                onExited: {
                    buttonColor = "#121212"
                }

                onClicked: {
                    settingPopup.open()





                }
            }
        }

        ColumnLayout{
            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignHCenter

            RowLayout{
                Layout.preferredWidth: parent.width
                spacing: 10

                Item{
                    Layout.fillWidth: true
                }

                Rectangle {
                    Layout.preferredWidth: 1314
                    Layout.preferredHeight: 190
                    Layout.alignment: Qt.AlignHCenter

                    color: rectColor

                    border.color: "white"
                    radius: 5
                    border.width: 0


                    RowLayout {
                        anchors.fill: parent

                        Item{
                            Layout.fillWidth: true
                        }

                        ColumnLayout{
                            spacing: 0

                            RowLayout {
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignHCenter
                                Item {Layout.fillWidth: true}
                                Label {text: "Speed : "; font.pixelSize: 15; color: "white"}
                                ComboBox {
                                    id: speedMainComboBox
                                    Layout.alignment: Qt.AlignHCenter

                                    Layout.preferredWidth: 95

                                    Component.onCompleted: {
                                        speedMainComboBox.model = ["2", "6", "8"]
                                    }

                                    onCurrentValueChanged: {
                                        if(cmbCompleted[0] && cmbCompleted[1] && cmbCompleted[2] && cmbCompleted[3])
                                        {
                                            dbController.readLastRun()
                                            dbController.setIdx("Last_Run", dbController.lastRun[0], 0)
                                            dbController.setIdx("Last_Run", currentValue, 1)
                                            dbController.setIdx("Last_Run", dbController.lastRun[2], 2)
                                            dbController.setIdx("Last_Run", dbController.lastRun[3], 3)

                                            dbController.updateLastRun()


                                        }
                                        if(currentValue !== "")
                                        {
                                            ethController.speed = currentValue

                                            lnaSpeedComboBox.currentIndex = speedMainComboBox.find(currentValue)
                                            lnbSpeedComboBox.currentIndex = speedMainComboBox.find(currentValue)

                                            ethController.setTikaSpeed(currentValue)
                                        }
                                    }

                                    // Initial selected index
                                }
                                Item {Layout.preferredWidth: 10}

                                Label {text: "Group : "; font.pixelSize: 15; color: "white"}
                                ComboBox {
                                    id: exfMainCmb

                                    Layout.alignment: Qt.AlignHCenter

                                    Layout.preferredWidth: 95


                                    Component.onCompleted: {
                                        var exitFeatureList = []

                                        for (var i=0; i < dbController.getGeneralExfs().length; ++i){
                                            exitFeatureList.push(dbController.getGeneralExfs()[i])
                                        }

                                        exfMainCmb.model = exitFeatureList
                                        exfSettingCmb.model = exitFeatureList

                                        // dbController.readLastRun()

                                        // currentIndex = exfMainCmb.find(dbController.lastRun[0])
                                        // cmbCompleted[1] = true


                                        // cmbsOpened[0] = true
                                        // dbController.readGeneral(dbController.lastRun[0])
                                    }

                                    onCurrentValueChanged: {
                                        ethController.readDb(currentValue)

                                        exitA.text = "Exit A"
                                        exitB.text = "Exit B"
                                        exitC.text = "Exit C"
                                        exitD.text = "Exit D"
                                        exitE.text = "Exit E"
                                        exitF.text = "Exit F"
                                        exitG.text = "Exit G"
                                        exitH.text = "Exit H"

                                        // if(k > 1) {

                                        // console.log("value exf changed")

                                        if(cmbCompleted[0] && cmbCompleted[1] && cmbCompleted[2] && cmbCompleted[3])
                                        {
                                            dbController.readLastRun()
                                            dbController.setIdx("Last_Run", currentValue, 0)
                                            dbController.setIdx("Last_Run", dbController.lastRun[1], 1)
                                            dbController.setIdx("Last_Run", dbController.lastRun[2], 2)
                                            dbController.setIdx("Last_Run", dbController.lastRun[3], 3)
                                            dbController.updateLastRun()

                                            exfSettingCmb.currentIndex = find(currentValue)
                                        }

                                        // }
                                        // k = 2
                                    }
                                }

                                Item {Layout.preferredWidth: 10}

                                Label {text: "Color : "; font.pixelSize: 15; color: "white"}
                                ComboBox {
                                    id: colorMainCmb
                                    Layout.alignment: Qt.AlignHCenter

                                    Layout.preferredWidth: 95


                                    Component.onCompleted: {
                                        // dbController.readLastRun()
                                        // console.log(dbController.lastRun[2])
                                        // currentIndex = colorMainCmb.find(dbController.lastRun[2])
                                        // // cmbsOpened[2] = true
                                        // cmbCompleted[2] = true
                                        // console.log("color comp")


                                    }

                                    onCurrentValueChanged: {
                                        camController.readColors(colorMainCmb.currentValue)
                                        if(cmbCompleted[0] && cmbCompleted[1] && cmbCompleted[2] && cmbCompleted[3])
                                        {
                                            dbController.readLastRun()
                                            dbController.setIdx("Last_Run", dbController.lastRun[0], 0)
                                            dbController.setIdx("Last_Run", dbController.lastRun[1], 1)
                                            dbController.setIdx("Last_Run", currentValue, 2)
                                            dbController.setIdx("Last_Run", dbController.lastRun[3], 3)
                                            dbController.updateLastRun()

                                            colorComboBox.currentIndex = colorMainCmb.find(dbController.lastRun[2])

                                        }
                                    }
                                }

                                Item {Layout.preferredWidth: 10}

                                Label {text: "Defect : ";    font.pixelSize: 15; color: "white"}
                                ComboBox {
                                    id: defectMainCmb
                                    Layout.alignment: Qt.AlignHCenter

                                    Layout.preferredWidth: 105

                                    Component.onCompleted: {
                                        // dbController.readLastRun()
                                        // currentIndex = defectMainCmb.find(dbController.lastRun[3])
                                        // cmbsOpened[3] = true
                                        cmbCompleted[3] = true


                                    }

                                    onCurrentValueChanged: {
                                        camController.readDb(defectMainCmb.currentValue)

                                        // console.log("value defect changed")

                                        if(cmbCompleted[0] && cmbCompleted[1] && cmbCompleted[2] && cmbCompleted[3])
                                        {
                                            dbController.readLastRun()
                                            dbController.setIdx("Last_Run", dbController.lastRun[0], 0)
                                            dbController.setIdx("Last_Run", dbController.lastRun[1], 1)
                                            dbController.setIdx("Last_Run", dbController.lastRun[2], 2)
                                            dbController.setIdx("Last_Run", currentValue, 3)
                                            dbController.updateLastRun()

                                            defectComboBox.currentIndex = defectMainCmb.find(dbController.lastRun[3])
                                        }
                                    }
                                }


                                Item {Layout.fillWidth: true}

                            }





                            RowLayout{
                                spacing: 0
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignHCenter

                                Item{
                                    Layout.preferredWidth: 20
                                }

                                ColumnLayout{
                                    Label {text: "Feeding"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                    RowLayout{
                                        spacing: 20
                                        MyButton {
                                            id: startFeeding
                                            text: "Start"
                                            iconsize: 0
                                            Layout.preferredHeight: 100
                                            Layout.preferredWidth: 100
                                            Layout.alignment: Qt.AlignHCenter
                                            buttonColor: enabled ? startColor : disabledColor
                                            radius: 50

                                            onClicked: {
                                                if(flagGrader){
                                                    flagFeeding = true
                                                    enabled = false
                                                    stopFeeding.enabled = true
                                                    ethController.autoFeeding = true
                                                    ethController.startFeeding()
                                                }
                                                else{
                                                    messageBox.msgText = "Grader Should Get Started First"
                                                    messageBox.timer = true
                                                    messageBox.open()
                                                }
                                            }
                                        }

                                        MyButton {
                                            id: stopFeeding
                                            text: "Stop"
                                            iconsize: 0
                                            Layout.preferredHeight: 100
                                            Layout.preferredWidth: 100
                                            Layout.alignment: Qt.AlignHCenter
                                            buttonColor: enabled ? stopColor : disabledColor
                                            enabled: false
                                            radius: 50

                                            onClicked: {
                                                flagFeeding = false
                                                enabled = false
                                                startFeeding.enabled = true
                                                ethController.autoFeeding = false
                                                ethController.stopFeeding()
                                            }
                                        }
                                    }
                                }

                                Item{
                                    Layout.preferredWidth: 20
                                }

                                ColumnLayout{
                                    spacing: 10

                                    RowLayout{
                                        spacing: 10
                                        Layout.preferredWidth: 200
                                        Layout.alignment: Qt.AlignHCenter

                                        Item{
                                            Layout.fillWidth: true
                                        }


                                        CheckBox{
                                            text: "Start Cameras"
                                            checked: dbController.lastRun[4] === "true" ? true : false  // means the default checkbox status
                                            onCheckedChanged: {
                                                camController.startCameras = checked
                                                dbController.setIdx("Last_Run", checked, 4)
                                                dbController.updateLastRun()
                                            }
                                            Component.onCompleted: {
                                                camController.startCameras = checked
                                            }
                                        }

                                        CheckBox{
                                            text: "Quantity Based Detection "

                                            checked: camController.quantityEnable   // means the default checkbox status
                                            onCheckedChanged: camController.quantityEnable = checked

                                        }

                                        CheckBox {
                                            text: "Save Result Images"
                                            onCheckedChanged: camController.saveResult = checked

                                        }



                                        Item{
                                            Layout.fillWidth: true
                                        }
                                    }

                                    RowLayout{
                                        Layout.alignment: Qt.AlignHCenter

                                        MyButton {
                                            id: startBtn
                                            text: "Start"
                                            icon_btn: "qrc:/Icons/start.png"
                                            Layout.preferredHeight: 45
                                            Layout.preferredWidth: 250
                                            Layout.alignment: Qt.AlignHCenter
                                            buttonColor: enabled ? startColor : disabledColor


                                            hoverEnabled: true

                                            onClicked: {
                                                this.enabled = false
                                                speedMainComboBox.enabled = false
                                                stopBtn.enabled = true

                                                programStarted = true

                                                for(var i=0;i<18;i++) {
                                                    tableModelA.appendRow({ "1": "", "2": "", "3": "", "4": "", "5": "", "6": "" });
                                                    tableModelB.appendRow({ "1": "", "2": "", "3": "", "4": "", "5": "", "6": "" });
                                                }


                                                weightController.readDb(exfMainCmb.currentValue, speedMainComboBox.currentValue)
                                                ethController.readDb(exfMainCmb.currentValue)
                                                camController.readColors(colorMainCmb.currentValue)
                                                camController.readDb(defectMainCmb.currentValue)

                                                var startCameras = false

                                                // ethController.test()

                                                for(i=0;i<ethController.enables.length;i++)
                                                {
                                                    if(ethController.enables[i].split(",")[0] === "true" || ethController.enables[i].split(",")[1] === "true")
                                                    {
                                                        startCameras = true
                                                        break
                                                    }
                                                }


                                                ethController.start(speedMainComboBox.currentValue)

                                                weightController.start()

                                                exfSettingCmb.enabled = false
                                                colorGroupComboBox.enabled = false
                                                defectComboBox.enabled = false
                                                lnaSpeedComboBox.enabled = false
                                                lnbSpeedComboBox.enabled = false

                                                editGeneral.enabled = false
                                                deleteGeneral.enabled = false
                                                addGeneral.enabled = false

                                                editColors.enabled = false
                                                deleteColor.enabled = false
                                                addColor.enabled = false

                                                editDefect.enabled = false
                                                deleteDefect.enabled = false
                                                addDefect.enabled = false


                                                // if(camController.serials.length !== 0)
                                                //     startCameras ? camController.start() : showMessageBox("Camera Is Not Needed By Any Output")

                                            }
                                        }
                                        MyButton {
                                            id: stopBtn
                                            //Layout: true
                                            text: "Stop"
                                            icon_btn: "qrc:/Icons/stop.png"
                                            Layout.preferredHeight: 45
                                            Layout.preferredWidth: 250
                                            Layout.alignment: Qt.AlignHCenter
                                            buttonColor: enabled ? stopColor : disabledColor
                                            enabled: false

                                            onClicked: {
                                                programStarted = false
                                                speedMainComboBox.enabled = true

                                                // Layout.preferredHeight = 50
                                                // Layout.preferredWidth = 230
                                                startBtn.enabled = true
                                                stopBtn.enabled = false

                                                tableModelA.clear()
                                                tableModelB.clear()
                                                camController.stopAnalise()
                                                //                                settingBtn.enabled = true;
                                                weightController.stop()
                                                ethController.stop()

                                                exfSettingCmb.enabled = true
                                                colorGroupComboBox.enabled = true
                                                defectComboBox.enabled = true
                                                lnaSpeedComboBox.enabled = true
                                                lnbSpeedComboBox.enabled = true

                                                editGeneral.enabled = true
                                                deleteGeneral.enabled = true
                                                addGeneral.enabled = true

                                                editColors.enabled = true
                                                deleteColor.enabled = true
                                                addColor.enabled = true

                                                editDefect.enabled = true
                                                deleteDefect.enabled = true
                                                addDefect.enabled = true
                                            }
                                        }
                                    }
                                }


                                Item{
                                    Layout.preferredWidth: 20
                                }

                                ColumnLayout{
                                    Label {text: "Grader"; font.pixelSize: 18; color: "white"; Layout.alignment: Qt.AlignHCenter}

                                    RowLayout{
                                        spacing: 20

                                        MyButton {
                                            id: startGrader
                                            text: "Start"
                                            iconsize: 0
                                            Layout.preferredHeight: 100
                                            Layout.preferredWidth: 100
                                            Layout.alignment: Qt.AlignHCenter
                                            buttonColor: enabled ? startColor : disabledColor
                                            radius: 50

                                            onClicked: {
                                                if(programStarted)
                                                {
                                                    flagGrader = true
                                                    enabled = false
                                                    stopGrader.enabled = true
                                                    ethController.startGrader(speedMainComboBox.currentValue)
                                                    ethController.autoGrading = true

                                                    poolBtn.enabled = false
                                                    brushBtn.enabled = false
                                                    graderBtn.enabled = false
                                                    tablesBtn.enabled = false
                                                }
                                                else
                                                {
                                                    messageBox.msgText = "Program Is Not Started"
                                                    messageBox.timer = true
                                                    messageBox.open()
                                                }


                                            }
                                        }

                                        MyButton {
                                            id: stopGrader
                                            text: "Stop"
                                            iconsize: 0
                                            Layout.preferredHeight: 100
                                            Layout.preferredWidth: 100
                                            Layout.alignment: Qt.AlignHCenter
                                            buttonColor: enabled ? stopColor : disabledColor
                                            radius: 50

                                            enabled: false

                                            onClicked: {
                                                if(!flagFeeding) {
                                                    flagGrader = false
                                                    enabled = false
                                                    startGrader.enabled = true
                                                    ethController.autoGrading = false
                                                    ethController.stopGrader()

                                                    poolBtn.enabled = true
                                                    brushBtn.enabled = true
                                                    graderBtn.enabled = true
                                                    tablesBtn.enabled = true
                                                }
                                                else {
                                                    messageBox.msgText = "Feeding Should Be Stopped First"
                                                    messageBox.timer = true
                                                    messageBox.open()
                                                }

                                            }
                                        }
                                    }



                                }

                                Item{
                                    Layout.preferredWidth: 20
                                }
                            }

                        }

                        Item{
                            Layout.fillWidth: true
                        }
                    }
                }

                Rectangle{
                    Layout.preferredWidth: 280
                    Layout.preferredHeight: 190
                    color: rectColor

                    border.color: "white"
                    radius: 5
                    border.width: 0
                    RowLayout {
                        anchors.fill: parent
                        Item {Layout.fillWidth: true}

                        ColumnLayout{
                            spacing: 30
                            RowLayout{
                                Label {text: "Cup Number : "; font.pixelSize: 15; color: "white"}
                                Label {text: ethController.cup; font.pixelSize: 15; color: "white"}
                            }
                            RowLayout{
                                Label {text: "Calibration : "; font.pixelSize: 15; color: "white"}
                                Label {id: calibStatus; text: "   Waiting For Start"; font.pixelSize: 15;  font.bold: true;
                                    color: "red"
                                }
                            }
                            RowLayout{
                                Label {text: "Elapsed Time : "; font.pixelSize: 15; color: "white"}
                                Label {text: ethController.elapsedTime; font.pixelSize: 15; color: "white"}
                            }
                            RowLayout{
                                Label {text: "Total Weight : "; font.pixelSize: 15; color: "white"}
                                Label {text: " " + ethController.totalWeight + " Kg"; font.pixelSize: 15; color: "white"}
                            }
                        }

                        Item {Layout.fillWidth: true}
                    }

                }


                // Rectangle {
                //     Layout.preferredWidth: 300
                //     Layout.preferredHeight: 190
                //     Layout.alignment: Qt.AlignHCenter

                //     color: rectColor

                //     border.color: "white"
                //     radius: 5
                //     border.width: 0

                //     ColumnLayout{
                //         anchors.fill: parent
                //         spacing: 5
                //         ColumnLayout{
                //             spacing: 10
                //             Layout.alignment: Qt.AlignHCenter
                //             Label {text: "Connection"; font.pixelSize: 15; color: "white";Layout.alignment: Qt.AlignHCenter}
                //             Image {
                //                 Layout.preferredWidth: 80
                //                 Layout.preferredHeight: 80
                //                 //Layout.alignment: Qt.AlignCenter
                //                 source: ethController.connectionStatus === 0 ? "qrc:/Icons/eth.png" : "qrc:/Icons/eth_dis.png"
                //                 Layout.alignment: Qt.AlignHCenter
                //                 //fillMode: .PreserveAspectFit
                //             }
                //         }


                //         ColumnLayout{
                //             spacing: 10
                //             Layout.alignment: Qt.AlignHCenter
                //             Label {text: "Connection"; font.pixelSize: 15; color: "white";Layout.alignment: Qt.AlignHCenter}
                //             Image {
                //                 Layout.preferredWidth: 80
                //                 Layout.preferredHeight: 80
                //                 //Layout.alignment: Qt.AlignCenter
                //                 source: ethController.connectionStatus === 0 ? "qrc:/Icons/eth.png" : "qrc:/Icons/eth_dis.png"
                //                 Layout.alignment: Qt.AlignHCenter
                //                 //fillMode: .PreserveAspectFit
                //             }
                //         }


                //         ColumnLayout{
                //             spacing: 10
                //             Layout.alignment: Qt.AlignHCenter
                //             Label {text: "Connection"; font.pixelSize: 15; color: "white";Layout.alignment: Qt.AlignHCenter}
                //             Image {
                //                 Layout.preferredWidth: 80
                //                 Layout.preferredHeight: 80
                //                 //Layout.alignment: Qt.AlignCenter
                //                 source: ethController.connectionStatus === 0 ? "qrc:/Icons/eth.png" : "qrc:/Icons/eth_dis.png"
                //                 Layout.alignment: Qt.AlignHCenter
                //                 //fillMode: .PreserveAspectFit
                //             }
                //         }
                //     }
                // }





                Item{
                    Layout.fillWidth: true
                }
            }

            Item {
                Layout.preferredHeight: 2
            }

            RowLayout
            {
                Item {
                    Layout.fillWidth: true
                }
                Rectangle {
                    Layout.preferredWidth: 845
                    Layout.preferredHeight: 190
                    Layout.alignment: Qt.AlignHCenter

                    color: rectColor

                    border.color: "white"
                    radius: 5
                    border.width: 0

                    RowLayout {
                        anchors.fill: parent

                        spacing: 10

                        Item {Layout.fillWidth: true}

                        ColumnLayout{
                            spacing: 15
                            Label {text: ""; font.pixelSize: 15; color: "white"}
                            Item{ Layout.preferredHeight: 2}
                            Label {text: "Number"; font.pixelSize: 15; color: "white"}
                            Item{ Layout.preferredHeight: 2}
                            Label {text: "Weight (Kg)"; font.pixelSize: 15; color: "white";
                            }
                        }

                        Item{
                            Layout.preferredWidth: 10
                        }

                        ColumnLayout{
                            spacing: 15
                            Label {text: `Exit A`; font.pixelSize: 15; color: "white"; id: exitA ;}
                            Item{ Layout.preferredHeight: 2}
                            Label {text: ethController.totalCountLeft[0] + ethController.totalCountRight[0]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                            Item{ Layout.preferredHeight: 2}
                            Label {id:totalExitA ;text: ethController.totalWeightOutput[0]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}

                        }

                        Item{
                            Layout.preferredWidth: 10
                        }

                        ColumnLayout{
                            spacing: 15
                            Label {text: "Exit B"; font.pixelSize: 15; color: "white"; id: exitB}
                            Item{ Layout.preferredHeight: 2}
                            Label {text: ethController.totalCountLeft[1] + ethController.totalCountRight[1]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                            Item{ Layout.preferredHeight: 2}
                            Label {id:totalExitB ;text: ethController.totalWeightOutput[1]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        }


                        Item{
                            Layout.preferredWidth: 10
                        }

                        ColumnLayout{
                            spacing: 15
                            Label {text: "Exit C"; font.pixelSize: 15; color: "white"; id: exitC}
                            Item{ Layout.preferredHeight: 2}
                            Label {text: ethController.totalCountLeft[2] + ethController.totalCountRight[2]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                            Item{ Layout.preferredHeight: 2}
                            Label {id:totalExitC ;text: ethController.totalWeightOutput[2]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        }


                        Item{
                            Layout.preferredWidth: 10
                        }
                        ColumnLayout{
                            spacing: 15
                            Label {text: "Exit D"; font.pixelSize: 15; color: "white"; id: exitD}
                            Item{ Layout.preferredHeight: 2}
                            Label {text: ethController.totalCountLeft[3] + ethController.totalCountRight[3]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                            Item{ Layout.preferredHeight: 2}
                            Label {id:totalExitD ;text: ethController.totalWeightOutput[3]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        }

                        Item{
                            Layout.preferredWidth: 10
                        }
                        ColumnLayout{
                            spacing: 15
                            Label {text: "Exit E"; font.pixelSize: 15; color: "white"; id: exitE}
                            Item{ Layout.preferredHeight: 2}
                            Label {text: ethController.totalCountLeft[4] + ethController.totalCountRight[4]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                            Item{ Layout.preferredHeight: 2}
                            Label {id:totalExitE ;text: ethController.totalWeightOutput[4]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        }

                        Item{
                            Layout.preferredWidth: 10
                        }

                        ColumnLayout{
                            spacing: 15
                            Label {text: "Exit F"; font.pixelSize: 15; color: "white"; id: exitF}
                            Item{ Layout.preferredHeight: 2}
                            Label {text: ethController.totalCountLeft[5] + ethController.totalCountRight[5]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                            Item{ Layout.preferredHeight: 2}
                            Label {id:totalExitF ;text: ethController.totalWeightOutput[5]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        }


                        Item{
                            Layout.preferredWidth: 10
                        }
                        ColumnLayout{
                            spacing: 15
                            Label {text: "Exit G"; font.pixelSize: 15; color: "white"; id: exitG}
                            Item{ Layout.preferredHeight: 2}
                            Label {text: ethController.totalCountLeft[6] + ethController.totalCountRight[6]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                            Item{ Layout.preferredHeight: 2}
                            Label {id:totalExitG ;text: ethController.totalWeightOutput[6]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        }

                        Item{
                            Layout.preferredWidth: 10
                        }

                        ColumnLayout{
                            spacing: 15
                            Label {text: "Exit H"; font.pixelSize: 15; color: "white"; id: exitH}
                            Item{ Layout.preferredHeight: 2}
                            Label {text: ethController.totalCountLeft[7] + ethController.totalCountRight[7]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                            Item{ Layout.preferredHeight: 2}
                            Label {id:totalExitH ;text: ethController.totalWeightOutput[7]; font.pixelSize: 15; color: "white"; Layout.alignment: Qt.AlignHCenter}
                        }

                        Item {Layout.fillWidth: true}

                    }
                }


                Item{
                    Layout.preferredWidth: 2
                }

                Rectangle{
                    Layout.preferredWidth: 749
                    Layout.preferredHeight: 190
                    color: rectColor

                    border.color: "white"
                    radius: 5
                    border.width: 0
                    RowLayout{
                        anchors.fill: parent
                        spacing: 40
                        Item{
                            Layout.fillWidth: true
                        }

                        ColumnLayout{
                            spacing: 10
                            Layout.alignment: Qt.AlignHCenter
                            Label {text: "Line A"; font.pixelSize: 15; color: "white";Layout.alignment: Qt.AlignHCenter}
                            Image {
                                id: camLna
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 80
                                //Layout.alignment: Qt.AlignCenter
                                source: camController.cameraIsOpened[0] ? "qrc:/Icons/Camera.png" : "qrc:/Icons/Camera_dis.png"
                                Layout.alignment: Qt.AlignHCenter
                                //fillMode: .PreserveAspectFit
                            }
                        }
                        ColumnLayout{
                            spacing: 10
                            Layout.alignment: Qt.AlignHCenter
                            Label {text: "Line B"; font.pixelSize: 15; color: "white";Layout.alignment: Qt.AlignHCenter}
                            Image {
                                id: camLnb
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 80
                                //Layout.alignment: Qt.AlignCenter
                                source: camController.cameraIsOpened[1] ? "qrc:/Icons/Camera.png" : "qrc:/Icons/Camera_dis.png"
                                Layout.alignment: Qt.AlignHCenter
                                //fillMode: .PreserveAspectFit
                            }
                        }

                        ColumnLayout{
                            spacing: 10
                            Layout.alignment: Qt.AlignHCenter
                            Label {text: "Encoder"; font.pixelSize: 15; color: "white";Layout.alignment: Qt.AlignHCenter}
                            Image {
                                id: ethStatus
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 80
                                //Layout.alignment: Qt.AlignCenter
                                source: ethController.connectionStatus === 0 ? "qrc:/Icons/eth.png" : "qrc:/Icons/eth_dis.png"
                                Layout.alignment: Qt.AlignHCenter
                                //fillMode: .PreserveAspectFit
                            }
                        }

                        ColumnLayout{
                            spacing: 10
                            Layout.alignment: Qt.AlignHCenter
                            Label {text: "Ejector A"; font.pixelSize: 15; color: "white";Layout.alignment: Qt.AlignHCenter}
                            Image {
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 80
                                //Layout.alignment: Qt.AlignCenter
                                source: ethController.boardStatus[0] ? "qrc:/Icons/eth.png" : "qrc:/Icons/eth_dis.png"
                                Layout.alignment: Qt.AlignHCenter
                                //fillMode: .PreserveAspectFit
                            }
                        }

                        ColumnLayout{
                            spacing: 10
                            Layout.alignment: Qt.AlignHCenter
                            Label {text: "Ejector B"; font.pixelSize: 15; color: "white";Layout.alignment: Qt.AlignHCenter}
                            Image {
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 80
                                //Layout.alignment: Qt.AlignCenter
                                source: ethController.boardStatus[1] ? "qrc:/Icons/eth.png" : "qrc:/Icons/eth_dis.png"
                                Layout.alignment: Qt.AlignHCenter
                                //fillMode: .PreserveAspectFit
                            }
                        }

                        ColumnLayout{
                            spacing: 10
                            Layout.alignment: Qt.AlignHCenter
                            Label {text: "Engines"; font.pixelSize: 15; color: "white";Layout.alignment: Qt.AlignHCenter}
                            Image {
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 80
                                //Layout.alignment: Qt.AlignCenter
                                source: ethController.boardStatus[2] ? "qrc:/Icons/eth.png" : "qrc:/Icons/eth_dis.png"
                                Layout.alignment: Qt.AlignHCenter
                                //fillMode: .PreserveAspectFit
                            }
                        }

                        Item{
                            Layout.fillWidth: true
                        }
                    }

                }






                Item {
                    Layout.fillWidth: true
                }
            }


            Rectangle
            {
                Layout.preferredWidth: 1070
                Layout.preferredHeight: 518
                color: "#121212"
                Layout.alignment: Qt.AlignHCenter

                border.color: "white"
                radius: 5
                border.width: 0

                MyButton {
                    id: poolBtn
                    text: "Pool"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 80   // X offset from the left edge
                    anchors.bottomMargin: 170    // Y offset from the top edge
                    height: 70
                    width: 70
                    Layout.alignment: Qt.AlignHCenter
                    iconsize: 0
                    radius: 35

                    buttonColor: circleButtonColor
                    onClicked: {
                        ethController.speed = speedMainComboBox.currentValue

                        poolPopup.open()
                    }
                }

                MyButton {
                    id: brushBtn
                    text: "Brush"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 300   // X offset from the left edge
                    anchors.bottomMargin: 250    // Y offset from the top edge
                    height: 70
                    width: 70
                    Layout.alignment: Qt.AlignHCenter
                    iconsize: 0
                    radius: 35

                    buttonColor: circleButtonColor

                    onClicked: {
                        ethController.speed = speedMainComboBox.currentValue

                        brushingPopup.open()
                    }
                }

                MyButton {
                    id: graderBtn
                    text: "Grader"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 800   // X offset from the left edge
                    anchors.bottomMargin: 300    // Y offset from the top edge
                    height: 70
                    width: 70
                    Layout.alignment: Qt.AlignHCenter
                    iconsize: 0
                    radius: 35

                    buttonColor: circleButtonColor

                    onClicked: {
                        ethController.speed = speedMainComboBox.currentValue

                        sorterPopup.open()
                    }
                }

                MyButton {
                    id: tablesBtn
                    text: "Tables"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 520   // X offset from the left edge
                    anchors.bottomMargin: 100    // Y offset from the top edge
                    height: 70
                    width: 70
                    Layout.alignment: Qt.AlignHCenter
                    iconsize: 0
                    radius: 35

                    buttonColor: circleButtonColor

                    onClicked: {
                        ethController.speed = speedMainComboBox.currentValue

                        tablesPopup.open()
                    }
                }

                Image {
                    //Layout.alignment: Qt.AlignCenter
                    source: "qrc:/Icons/grader_4.png"
                    Layout.alignment: Qt.AlignHCenter
                    width: 1000
                    height: 460
                    z: 1
                    //fillMode: .PreserveAspectFit
                }
            }


            RowLayout{
                Layout.preferredWidth: parent.width
                spacing: 5
                Item{
                    Layout.fillWidth: true
                }

                // Label {text: "RC : "; font.pixelSize: 10; color: "black"; Layout.alignment: Qt.AlignHCenter}
                // Label {text: weightController.reconnect; font.pixelSize: 10; color: "black"; Layout.alignment: Qt.AlignHCenter}

                Item{
                    Layout.fillWidth: true
                }
            }

            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: 400
            }

        }
    }

}
