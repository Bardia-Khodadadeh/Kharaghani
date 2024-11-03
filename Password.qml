import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.0
import QtLocation 5.12
import QtQuick.Controls.Material 2.15
import QtQuick 2.0
import QtMultimedia 5.15

Popup {
    width: 400
    height: 120
    anchors.centerIn: parent
    id: messageBox
    property string msgText : ""
    property bool timer: false

    background: Rectangle{
        color: "white"
        radius: 10
    }

    Rectangle {
        anchors.fill: parent
        color: "white"
        radius: 10
        border.color: "white"

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 5

            RowLayout{
                Text {
                    text: "Passwor"
                    Layout.alignment: Qt.AlignCenter
                    color: "black"
                    font.pixelSize: 18
                }

            }




        }



    }

}
