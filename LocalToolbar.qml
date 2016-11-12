/*
 *  This file is part of LesJeuxDAntoine.
 *
 *  LesJeuxDAntoine is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  EasyTimeTracker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with EasyTimeTracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  LesJeuxDAntoine: First creation by 2BlackCoffees: http://www.twoblackcoffees.com/
 *
 **/
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
