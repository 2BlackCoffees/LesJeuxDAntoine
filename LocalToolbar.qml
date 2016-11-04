import QtQuick 2.0

Item {
  id: localToolbar
  width: mainWindow.width
  height: mainWindow.height / 8
  property int buttonWidth
  property var model
  property int clickEnumValue
  property var col: ["yellow", "green", "orange", "purple", "black"]
  Repeater {
    model: localToolbar.model

    Item {
      id: boxSelection
      width: localToolbar.buttonWidth
      height: localToolbar.height
      x: index * width
      y: 0
      Rectangle
      {
        id: selectionBkGnd
        color: localToolbar.col[index]
        anchors.fill: parent
        radius: canClick ? 30 : 0;
        opacity: 0.2
        property bool canClick: model.canClick

        gradient: Gradient {
          GradientStop { position: 0.0; color: "red"}
          GradientStop { position: 0.5; color: selectionBkGnd.color}
          GradientStop { position: 1.0; color: "blue" }

        }
      }

      Text {
        anchors.centerIn: parent
        text: model.display
        font.pointSize: 18
        horizontalAlignment: Text.AlignHCenter
      }
      MouseArea {
        anchors.fill: parent
        onClicked: {
          localToolbar.model.setData(index, index, localToolbar.clickEnumValue)
        }
      }

    }
  }
}
