// CommonFunctions.qml

import QtQuick 2.15

QtObject {
    // JavaScript function to find the index of an item by text in a ComboBox
    function findIndexByText(comboBox, searchText) {
        for (var i = 0; i < comboBox.model.count; i++) {
            if (comboBox.model.get(i) === searchText) {
                return i;
            }
        }
        return -1; // Return -1 if not found
    }
}
