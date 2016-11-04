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
    font.pointSize: pointsMoving.width / 2.2
    font.bold: true
  }
  states: [
    // In state 'middleRight', move the image to middleRightRect
    State {
      name: "pointsWon"
      PropertyChanges {
        target: pointsMoving;
        x: pointsMoving.x // currentCellId.x + currentCellId.width / 2;
        y: pointsMoving.y // currentCellId.y + currentCellId.height / 2;
        visible: true; wonStatus: true
      }
    },

    // In state 'bottomLeft', move the image to bottomLeftRect
    State {
      name: "pointsLost"
      PropertyChanges {
        target: pointsMoving;
        x: pointsMoving.x // currentCellId.x + currentCellId.width / 2;
        y: pointsMoving.y // currentCellId.y + currentCellId.height / 2;
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

    // When transitioning to 'middleRight' move x,y over a duration of 1 second,
    // with OutBounce easing function.
    Transition {
      from: "*"; to: "pointsWon"
      PropertyAnimation {
        target: pointsMoving
        properties: "y"; to: maxY
        easing.type: Easing.InOutBounce
        easing.amplitude: -300
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
        easing.amplitude:  300
        duration: timerAnimation.interval

      }
      NumberAnimation {
        target: pointsMoving
        properties: "x"; to: maxX
        duration: timerAnimation.interval * 0.7

      }
    }//,

  ]
}
