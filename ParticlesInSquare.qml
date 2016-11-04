import QtQuick 2.0
import QtQuick.Particles 2.0

Item {
  id: particlesInSquare
  property bool showParticle: false
  property var starPath
  property int interval: 2000
  property int emitRate: 2000
  // Particles
  ParticleSystem {
    id: particleSystem
  }
  Emitter {
    Timer {
      id: timerEmitter
      interval: particlesInSquare.interval
      running: particlesInSquare.showParticle
      repeat: false
    }
    id: emitterClickedParticle
    enabled: timerEmitter.running
    anchors.centerIn: parent
    width: parent.width
    height: parent.height
    system: particleSystem
    emitRate: particlesInSquare.emitRate
    lifeSpan: 400
    lifeSpanVariation: 100
    size: 5
    endSize: 14
  }
  ImageParticle {
    id: clickedParticlesId
    system: particleSystem
    source: parent.starPath
    entryEffect: ImageParticle.None
  }
}

