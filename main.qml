import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 1.5
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
// our module
import QMLInterface 1.0
import PointsStatus 1.0
import ModelCards 1.0


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

  QMLInterface {
    id: qmlInterfaceId
    property var modelCards: qmlInterfaceId.getDynamicEntryModel()
    property var modelPoints: qmlInterfaceId.getPointsStatus()

  }


  function processWinning() {
    winningDialog.visible = true
    winningDialog.message = qmlInterfaceId.getWinningText()
    winningDialog.showStars = false
    if(qmlInterfaceId.isGoingToNextLevel()) {
      winningDialog.statusColor = "green"
      winningDialog.showStars = true
      curtainEffectRight.open = false
      curtainEffectLeft.open = false
    } else if(qmlInterfaceId.hadErrors()) {
      winningDialog.statusColor = "yellow"
    } else {
      winningDialog.statusColor = "green"
    }

    qmlInterfaceId.modelCards.setData(0, 0, DynamicEntryModel.AllIsWonRole)
    mainPanelId.width = getMainPanelWidth()
    mainPanelId.height= getMainPanelHeight()
    grid.rows = qmlInterfaceId.rowToDisplay()
    grid.columns = qmlInterfaceId.columnCount()
    idFlickable.contentHeight = getRequiredHeight()
    idFlickable.contentWidth = getRequiredWidth()
    mainPanelId.width = mainWindow.getMainPanelWidth()
    mainPanelId.height = mainWindow.getMainPanelHeight()
    qmlInterfaceId.modelCards.setData(0, 0, DynamicEntryModel.ForceRepaintRole)

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
      source: qmlInterfaceId.getBackgroundImagePath()
      anchors.centerIn: parent
    }
  }

  function getRequiredWidth() {
    return qmlInterfaceId.columnCount() * (qmlInterfaceId.maxWidth() + 10);
  }

  function getRequiredHeight() {
    return qmlInterfaceId.rowToDisplay() * (qmlInterfaceId.maxHeight() + 10);
  }

  function getMainPanelWidth() {
    return Math.min(getRequiredWidth(), mainWindow.width - 20);
  }

  function getMainPanelHeight() {
    return Math.min(getRequiredHeight(), 3 * mainWindow.height / 4)
  }


  LocalToolbar {
    x: 0
    y: 0
    width: mainWindow.width
    height: mainWindow.height / 8
    model: qmlInterfaceId.modelPoints
    buttonWidth: mainWindow.width / 5
    clickEnumValue: PointsStatus.Clicked
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
        property int rows: qmlInterfaceId.rowToDisplay()
        property int columns: qmlInterfaceId.columnCount()
        height: getRequiredHeight()
        width: getRequiredWidth()
        x: 0
        y: 0

        Repeater {
          model: qmlInterfaceId.modelCards
          Item {
            id: currentCellId
            width: qmlInterfaceId.maxWidth()
            height: qmlInterfaceId.maxHeight()
            property int row: index / grid.columns
            property int column: index % grid.columns
            x: grid.columnSpacing + (width + grid.columnSpacing) * column
            y: grid.rowSpacing + (height + grid.rowSpacing) * row

            Item {

              property var starPathRed: qmlInterfaceId.startPathRed()
              property var starPathYellow: qmlInterfaceId.startPathYellow()

              // Show Rectangle or image
              Rectangle {
                id: hidingRectangle
                visible: !model.wantImage
                width: qmlInterfaceId.maxWidth()
                height: qmlInterfaceId.maxHeight()
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
                    qmlInterfaceId.modelCards.setData(index, index, DynamicEntryModel.ClickOnRectangleRole)
                    // Manage points moving
                    pointsMoving.x = mainPanelId.x + currentCellId.x + hidingRectangle.width / 2
                    pointsMoving.y = mainPanelId.y + currentCellId.y + hidingRectangle.height / 2
                    pointsMoving.maxY = model.pointsWon ? 0 : mainWindow.height
                    pointsMoving.pointsLost = model.pointsLost
                    pointsMoving.pointsWon = model.pointsWon
                    pointsMoving.pointsChangeToShow = model.pointsChangeToShow
                    pointsMoving.averageWidth = Math.min(hidingRectangle.width, hidingRectangle.height)
                    var stateValue = "none"
                    if(model.wantImage) {
                      if(model.pointsWon)   stateValue = "pointsWon"
                      if(model.pointsLost)  stateValue = "pointsLost"
                    }
                    pointsMoving.state = stateValue
                    qmlInterfaceId.modelCards.setData(index, index, DynamicEntryModel.ResetPointsValueRole)
                    // Process all is won
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
                  qmlInterfaceId.modelCards.setData(index, index, DynamicEntryModel.ClickOnImageRole)
                }
              }
            }

            ParticlesInSquare {
              anchors.centerIn: parent
              showParticle: model.wantImage
              starPath: qmlInterfaceId.startPathRed()
              width: parent.width
              height: parent.height
            }
          }
        }
      }
    }


  }

  // Points
  PointsMoving {
    id: pointsMoving
    x: 0
    y: 0
    property int averageWidth: 10
    width: averageWidth - (x > y ? y/(x + 1) : x/(y + 1)) * averageWidth
    pointsLost: false
    pointsWon: false
    pointsChangeToShow: 0
    state: "none"
    maxY: 0
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
    imagePath: qmlInterfaceId.getLocalDialogImage()
    starPath: qmlInterfaceId.startPathYellow()
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
      starPath: qmlInterfaceId.startPathYellow()
      emitRate: 4000
      interval: 3500
    }

  }


}


