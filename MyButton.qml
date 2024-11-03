import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.0
import QtLocation 5.12
import QtQuick.Controls.Material 2.15
import QtQuick 2.0
import QtMultimedia 5.15


MouseArea {
    id: control
    //    signal click()
    width: 130
    height: 70
    property string icon_btn
    property bool icon_visible: true
    property string text
    property bool text_visible: true
    property bool clicked: false
    property int radius: 5
    property int bottonMarg: 10
    property int fontsize: 14
    property int contentLeftMargin: 40
    property string fontFamily: "Roboto"
    property string buttonColor: "#28292A"
    property string btnColor: "#28292A"


    property string borderColor: "#FFFFFF"
    property string fontColor: "#FFFFFF"

    property bool animate : false
    property int animate_duration: 2000
    property int border_width: 0

    property int iconsize: 35

    cursorShape: Qt.PointingHandCursor

    Rectangle {
        radius: control.radius
        anchors.fill: parent
        color: control.buttonColor

        Label{
            id: font
        }

        Behavior on color {
            ColorAnimation {
                duration: 300 // Animation duration in milliseconds
            }
        }


        border.color: borderColor
        border.width: border_width

        RowLayout{
            anchors.fill: parent

            Item { Layout.fillWidth: true }
            Image{
                id: img
                visible: control.icon_visible
                source: control.icon_btn
                Layout.preferredWidth: control.iconsize
                Layout.preferredHeight: control.iconsize

                SequentialAnimation {
                    id: animation
                    running: animate
                    loops: Animation.Infinite
                    NumberAnimation { target: img; property: "rotation"; from: 360; to: 0; duration: 1000 }
                    onStarted: animTimer.start()
                }

                Timer {
                    id: animTimer
                    interval: animate_duration
                    running: false
                    repeat: false
                    onTriggered: animate=false
                }
                // onEnabledChanged: {
                //     addBtn.buttonColor = btn_color
                // }
                //Layout.leftMargin: control.contentLeftMargin
            }

            Text{
                id: txt
                text: control.text
                font.pixelSize: fontsize
                font.bold: true
                font.family: control.fontFamily
                color: fontColor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                visible: control.text_visible
                //Layout.leftMargin: control.contentLeftMargin
            }
            Item { Layout.fillWidth: true }
        }
    }
    Layout.bottomMargin: bottonMarg
}
