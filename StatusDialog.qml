import QtQuick 2.0
import QtQuick.Layouts 1.2

Item  {
  id: statusDialog
  property string title
  property string message
  property string imagePath
  opacity: 0.95
  property int imageWidth: width / 5
  property bool showStars
  property color statusColor
  property string starPath
  Timer {
    id: timerShowDialog
    interval: 3000
    running: statusDialog.visible
    repeat: true
    onTriggered: {
      statusDialog.visible = false
    }
  }

  Rectangle {
    id: titleDialog
    height: 25
    x: statusDialog.x
    y: statusDialog.y
    width: statusDialog.width
    gradient: Gradient {
      GradientStop { position: 0.0; color: statusColor}
      GradientStop { position: 1.0; color: "gray" }
    }

    Text {
      anchors.centerIn: parent
      text: statusDialog.title
      color: "black"
      font.pointSize: 18
      horizontalAlignment: Text.AlignHCenter
    }
  }

  Rectangle {
    id: bodyDialog
    color: "lightskyblue"
    anchors.top: titleDialog.bottom
    anchors.left: titleDialog.left
    width: statusDialog.width
    height: statusDialog.height - titleDialog.height
    gradient: Gradient {
      GradientStop { position: 0.0; color: "gray"}
      GradientStop { position: 1.0; color: statusColor }
    }
    Text {
      text: statusDialog.message
      color: "navy"
      anchors.centerIn: parent
      font.pointSize: 18
      horizontalAlignment: Text.AlignHCenter
    }
  }

  Image {
    id: dialogImage
    x: statusDialog.x + statusDialog.width - 4 * statusDialog.imageWidth / 5
    y: statusDialog.y - statusDialog.imageWidth / 5
    sourceSize.width: statusDialog.imageWidth
    source: imagePath
  }

  ParticlesInSquare {
    x: statusDialog.x
    y: statusDialog.y
    showParticle: statusDialog.showStars
    starPath: statusDialog.starPath
    width: parent.width
    height: parent.height
    emitRate: 1000
    //interval: 200000
  }

}
