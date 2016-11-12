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

