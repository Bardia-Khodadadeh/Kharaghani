import QtQuick.Controls 1.4

Calendar {
    id: calendar
    property string date : ""

    minimumDate: new Date(2024, 4, 1)
    maximumDate: new Date(2098, 12, 1)

    onSelectedDateChanged: {
        date = selectedDate
    }


}
