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

Rectangle {
  id: pointsMoving
  //property alias state: pointsMoving.state
  property bool pointsWon
  property bool pointsLost
  property int pointsChangeToShow
  height: width
  property int maxX
  property int maxY

  Timer {
    id: timerAnimation
    interval: 3000
    running: pointsWon | pointsLost
    repeat: false
    onTriggered: pointsMoving.visible = false
  }
  visible: false
  border.color: "black"
  border.width: 1
  radius: pointsMoving.width * 0.5
  property bool wonStatus: true
  gradient: Gradient {
    GradientStop { position: 0.0; color: pointsMoving.wonStatus ? "red" : "gray"}
    GradientStop { position: 0.5; color: "black" }
    GradientStop { position: 1.0; color: pointsMoving.wonStatus ? "white" : "lightgray" }
  }
  Text {
    anchors.centerIn: parent
    color: pointsMoving.wonStatus ? "yellow" : "darkgray"
    text: pointsMoving.wonStatus ? "+" + pointsMoving.pointsChangeToShow : pointsMoving.pointsChangeToShow
    font.pointSize: (pointsMoving.width / 2.2) + 1
    font.bold: true
  }
  states: [
    // In state 'middleRight', move the image to middleRightRect
    State {
      name: "pointsWon"
      PropertyChanges {
        target: pointsMoving;
        x: pointsMoving.x
        y: pointsMoving.y
        visible: true; wonStatus: true
      }
    },

    State {
      name: "pointsLost"
      PropertyChanges {
        target: pointsMoving;
        x: pointsMoving.x
        y: pointsMoving.y
        visible: true; wonStatus: false
      }
    },
    State {
      name: "none"
      PropertyChanges {
        target: pointsMoving; visible: false
      }
    }

  ]
  transitions: [

    Transition {
      from: "*"; to: "pointsWon"
      PropertyAnimation {
        target: pointsMoving
        properties: "y"; to: maxY
        easing.type: Easing.InOutBounce
        easing.amplitude: -10
        duration: timerAnimation.interval

      }
      NumberAnimation {
        target: pointsMoving
        properties: "x"; to: maxX
        duration: timerAnimation.interval * 0.7

      }
    },
    Transition {
      from: "*"; to: "pointsLost"
      PropertyAnimation {
        target: pointsMoving
        properties: "y"; to: maxY
        easing.type: Easing.OutBounce
        easing.amplitude:  10
        duration: timerAnimation.interval

      }
      NumberAnimation {
        target: pointsMoving
        properties: "x"; to: maxX
        duration: timerAnimation.interval * 0.7

      }
    }

  ]
}
