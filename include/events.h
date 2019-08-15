/*
* ARTD (Augmented Reality Tower Defense)
* Copyright (C) 2019 Jaeger,Stegmueller,Boche,Nandha 
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _EVENTS_H
#define _EVENTS_H

#include <string>

#include "image_processor.h"

/**
 * Base Event
 */
class Event {
 public:
  Event() { _canceled = false; };

  // DONT DELETE: virtual member is important for typeid operator behaviour
  virtual ~Event() {}

  bool GetCanceled() { return _canceled; };
  void SetCanceled(bool canceled) { _canceled = canceled; };

 private:
  bool _canceled;
};

/**
 * Game events
 */
class GameEvent : public Event {
 public:
    enum EventType {
        PlayerHit,
        PlayerScore
    };

  GameEvent(GameEvent::EventType t) : _t(t) {}
  GameEvent::EventType GetType() { return _t; }

  private:
    GameEvent::EventType _t;    
};

/**
 * Event containing marker information
 */
class MarkersDetectedEvent : public Event {
  Marker _m;

 public:
  MarkersDetectedEvent(Marker m) : _m(m) {}
  Marker GetMarker() { return _m; }
};

/**
 * Event to inform system to remove unit
 */
class UnitKilledEvent : public Event {
  std::string _name;

 public:
  UnitKilledEvent(std::string name) : _name(name) {}
  std::string GetName() { return _name; }
};

/**
 * Event used for projection / mapping
 */
class HomographyComputedEvent : public Event {
  cv::Mat _homography;
  cv::Mat _camMat;
  cv::Mat _distCoeffs;
  Marker _m0;
  Marker _m1;

 public:
  HomographyComputedEvent(cv::Mat homography, cv::Mat camMat, cv::Mat distCoeffs, Marker m0, Marker m1) :
      _camMat(camMat), _distCoeffs(distCoeffs), _m0(m0), _m1(m1), _homography{homography} {}
  cv::Mat GetHomography() { return _homography; }
  cv::Mat GetCamMatrix() { return _camMat; }
  cv::Mat GetDistCoeffs() { return _distCoeffs; }
  Marker GetMarker0() { return _m0; }
  Marker GetMarker1() { return _m1; }
};

#endif
