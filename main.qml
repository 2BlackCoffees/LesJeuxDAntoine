import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 1.5
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
// our module
import main.model 1.0
import points.status 1.0


Window {
  id: mainWindow
  visible: true
  width: Screen.width
  height: Screen.height
  flags: Qt.SplashScreen //| Qt.WindowStaysOnTopHint
  color: "black"
  property var start: {
    splash.visible = true
    return false
  }

  DynamicEntryModel {
    id: dynamic
    property var maxWidthValue: dynamic.maxWidth()
    property var maxHeightValue: dynamic.maxHeight()
  }

  PointsStatus {
    id: pointsdynamic
  }

  function processWinning() {
    winningDialog.visible = true
    winningDialog.message = dynamic.getWinningText()
    winningDialog.showStars = false
    if(dynamic.isGoingToNextLevel()) {
      winningDialog.statusColor = "green"
      winningDialog.showStars = true
      curtainEffectRight.open = false
      curtainEffectLeft.open = false
    } else if(dynamic.hadErrors()) {
      winningDialog.statusColor = "yellow"
    } else {
      winningDialog.statusColor = "green"
    }

    dynamic.setData(0, 0, DynamicEntryModel.AllIsWonRole)
    mainPanelId.width = getMainPanelWidth()
    mainPanelId.height= getMainPanelHeight()
    grid.rows = dynamic.rowToDisplay()
    grid.columns = dynamic.columnCount()
    idFlickable.contentHeight = getRequiredHeight()
    idFlickable.contentWidth = getRequiredWidth()
    mainPanelId.width = mainWindow.getMainPanelWidth()
    mainPanelId.height = mainWindow.getMainPanelHeight()
    dynamic.setData(0, 0, DynamicEntryModel.ForceRepaintRole)

    return true
  }

  Rectangle {
    id: rectangleBkGndImage
    x: 3
    y: 3
    width: mainWindow.width - 6
    height: mainWindow.height - 6
    visible: true
    Image {
      id: bkgndImage
      sourceSize.width: parent.width
      sourceSize.height: parent.height
      source: dynamic.getBackgroundImagePath()
      anchors.centerIn: parent
    }
  }

  function getRequiredWidth() {
    return dynamic.columnCount() * (dynamic.maxWidthValue + 10);
  }

  function getRequiredHeight() {
    return dynamic.rowToDisplay() * (dynamic.maxHeightValue + 10);
  }

  function getMainPanelWidth() {
    return Math.min(getRequiredWidth(), mainWindow.width - 20);
  }

  function getMainPanelHeight() {
    return Math.min(getRequiredHeight(), 3 * mainWindow.height / 4)
  }


  LocalToolbar {
    width: mainWindow.width
    height: mainWindow.height / 8
    model: pointsdynamic
    buttonWidth: mainWindow.width / 5
    clickEnumValue: PointsStatus.Clicked
    x: 0
    y: 0
  }

  ScrollView {
    id: mainPanelId
    width: mainWindow.getMainPanelWidth()
    height: mainWindow.getMainPanelHeight()
    anchors.centerIn: parent
    contentItem: Flickable {
      id: idFlickable
      interactive: true
      boundsBehavior: Flickable.StopAtBounds
      contentHeight: getRequiredHeight()
      contentWidth: getRequiredWidth()


      Item {
        id: grid
        anchors.margins: 10
        property int rowSpacing: 10
        property int columnSpacing: 10
        property int rows: dynamic.rowToDisplay()
        property int columns: dynamic.columnCount()
        height: getRequiredHeight()
        width: getRequiredWidth()
        x: 0
        y: 0

        Repeater {
          model: dynamic
          Item {
            id: currentCellId
            width: dynamic.maxWidthValue
            height: dynamic.maxHeightValue
            property int row: index / grid.columns
            property int column: index % grid.columns
            x: grid.columnSpacing + (width + grid.columnSpacing) * column
            y: grid.rowSpacing + (height + grid.rowSpacing) * row

            Item {

              property var starPathRed: dynamic.startPathRed()
              property var starPathYellow: dynamic.startPathYellow()

              // Show Rectangle or image
              Rectangle {
                id: hidingRectangle
                visible: !model.wantImage
                width: dynamic.maxWidthValue
                height: dynamic.maxHeightValue
                gradient: Gradient {
                  GradientStop { position: 0.0; color: "red"}
                  GradientStop { position: 0.5; color: "green" }
                  GradientStop { position: 1.0; color: "blue" }
                }
                Text {
                  anchors.centerIn: parent
                  color: "white"
                  text: model.textOnRectangle
                  font.pointSize: 16

                }
                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    dynamic.setData(index, index, DynamicEntryModel.ClickOnRectangleRole)
                    if(model.allIsWon) {
                      mainWindow.processWinning()
                    }
                  }
                }
              }
            }

            Image {
              id: currentImage
              source: model.display
              visible: model.wantImage
              anchors.centerIn: parent
              opacity: model.opacity
              MouseArea {
                anchors.fill: parent
                onClicked: {
                  dynamic.setData(index, index, DynamicEntryModel.ClickOnImageRole)
                }
              }
            }

            ParticlesInSquare {
              anchors.centerIn: parent
              showParticle: model.wantImage
              starPath: dynamic.startPathRed()
              width: parent.width
              height: parent.height
            }

            // Points
            PointsMoving {
              id: pointsMoving
              x: dynamic.maxWidthValue / 4;
              y: dynamic.maxHeightValue / 4;
              width:(x < y ? x : y) * 2
              pointsLost: model.pointsLost
              pointsWon: model.pointsWon
              pointsChangeToShow: model.pointsChangeToShow
              state: {
                var returnValue = "none"
                if(model.wantImage) {
                  if(model.pointsWon)   returnValue = "pointsWon"
                  if(model.pointsLost)  returnValue = "pointsLost"
                }
                dynamic.setData(index, index, DynamicEntryModel.ResetPointsValueRole)

                return returnValue
              }
              maxX: mainWindow.width
              maxY: model.pointsWon ? 0 : mainWindow.height
            }
          }
        }
      }
    }


  }

  CurtainEffectRight {
    id: curtainEffectRight
    imagePath: "Misc/Curtain.jpg"
    open: false
    minWidth: 50
  }

  CurtainEffectLeft {
    id: curtainEffectLeft
    imagePath: "Misc/Curtain.jpg"
    open: false
    minWidth: 50
  }

  StatusDialog {
    id: winningDialog
    visible: false
    width: mainWindow.width / 2
    height: mainWindow.height / 3
    x: mainWindow.width/ 8
    y: mainWindow.height / 8
    title: "Bravo !!!"
    imagePath: dynamic.getLocalDialogImage()
    starPath: dynamic.startPathYellow()
  }

  Image {
    id: splash
    source: "Misc/Splash.png"
    visible: true
    anchors.centerIn: parent
    Timer {
      id: timerSplash
      interval: 4500
      running: splash.visible
      repeat: false
      onTriggered: {
        splash.visible = false
      }
    }
    ParticlesInSquare {
      anchors.fill: splash
      showParticle: true
      starPath: dynamic.startPathYellow()
      emitRate: 4000
      interval: 3500
    }

  }


}


