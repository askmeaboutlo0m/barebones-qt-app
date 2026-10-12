// SPDX-License-Identifier: MIT
import QtQuick as QQ
import QtQuick.Controls as C
import QtQuick.Layouts as L

L.ColumnLayout {
    anchors.fill: parent

    C.Label {
        id: label
        property int presses: 0
        text: titleEdit.text + " " + presses
        L.Layout.fillWidth: true
    }

    C.Button {
        text: "Button"
        onClicked: ++label.presses
        L.Layout.fillWidth: true
    }

    C.TextField {
        id: titleEdit
        text: "Presses:"
        L.Layout.fillWidth: true
    }

    QQ.Item {
        L.Layout.fillHeight: true
    }
}
