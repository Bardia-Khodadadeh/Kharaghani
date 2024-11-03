import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15 // Import the Material Design module

Window {
    id: mainWindow

    height: 1100
    width: 1800

    color: "#16FF16"

    Material.accent: "#00d76a"

    property string hover_color: "#009dc4"

    visible: true
    title: qsTr("Perticon")
    Material.theme: Material.Dark

    Home {
        id: homePage
        anchors.fill: parent
    }
}
