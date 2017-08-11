#pragma once

#include <ctime>
#include <list>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <Windows.h>

#include "KinectSensor.h"
#include "keyHandler.h"

extern keyHandler *Gmykeyhandler;



float *KinectSensor::handCoords{new float[3]}; 
int KinectSensor::gestureType = NO_GESTURE;
int KinectSensor::timer = 0;
bool KinectSensor::rightHand_closed = false;
bool KinectSensor::leftHand_closed = false;
float KinectSensor::hand_pos_x = 0;
float KinectSensor::hand_pos_y = 0;
float KinectSensor::hand_distance = 0;
bool KinectSensor::KeepUpdatingHandPos = true;

float KinectSensor::panning_delta_x = 0;
float KinectSensor::panning_delta_y = 0;

float KinectSensor::zoom_delta = 0;
std::chrono::milliseconds ms;

std::list<float> KinectSensor::handPosXBuf = std::list<float>(), KinectSensor::handPosYBuf = std::list<float>(), KinectSensor::handPosZBuf = std::list<float>();
std::list<float> KinectSensor::spinePosXBuf = std::list<float>(), KinectSensor::spinePosYBuf = std::list<float>(), KinectSensor::spinePosZBuf = std::list<float>();

void KinectSensor::addHandJoint(Joint handJoint) {
  if (handPosXBuf.size() < BUFFER_SIZE) {
    handPosXBuf.push_back(handJoint.Position.X);
    handPosYBuf.push_back(handJoint.Position.Y);
    handPosZBuf.push_back(handJoint.Position.Z);
  } else {
    handPosXBuf.pop_front();
    handPosYBuf.pop_front();
    handPosZBuf.pop_front();
    handPosXBuf.push_back(handJoint.Position.X);
    handPosYBuf.push_back(handJoint.Position.Y);
    handPosZBuf.push_back(handJoint.Position.Z);
  }
}

void KinectSensor::addSpineJoint(Joint spineJoint) {
  if (spinePosXBuf.size() < BUFFER_SIZE) {
    spinePosXBuf.push_back(spineJoint.Position.X);
    spinePosYBuf.push_back(spineJoint.Position.Y);
    spinePosZBuf.push_back(spineJoint.Position.Z);
  } else {
    spinePosXBuf.pop_front();
    spinePosYBuf.pop_front();
    spinePosZBuf.pop_front();
    spinePosXBuf.push_back(spineJoint.Position.X);
    spinePosYBuf.push_back(spineJoint.Position.Y);
    spinePosZBuf.push_back(spineJoint.Position.Z);
  }
}

float KinectSensor::getHandJointPosX() {
  float avg = 0.0f;
  for (std::list<float>::iterator i = handPosXBuf.begin();
       i != handPosXBuf.end(); i++)
    avg += (*i);
  return avg / handPosXBuf.size();
}

float KinectSensor::getHandJointPosY() {
  float avg = 0.0f;
  for (std::list<float>::iterator i = handPosYBuf.begin();
       i != handPosYBuf.end(); i++)
    avg += (*i);
  return avg / handPosYBuf.size();
}

float KinectSensor::getHandJointPosZ() {
  float avg = 0.0f;
  for (std::list<float>::iterator i = handPosZBuf.begin();
       i != handPosZBuf.end(); i++)
    avg += (*i);
  return avg / handPosZBuf.size();
}

float KinectSensor::getSpineJointPosX() {
  float avg = 0.0f;
  for (std::list<float>::iterator i = spinePosXBuf.begin();
       i != spinePosXBuf.end(); i++)
    avg += (*i);
  return avg / spinePosXBuf.size();
}

float KinectSensor::getSpineJointPosY() {
  float avg = 0.0f;
  for (std::list<float>::iterator i = spinePosYBuf.begin();
       i != spinePosYBuf.end(); i++)
    avg += (*i);
  return avg / spinePosYBuf.size();
}

float KinectSensor::getSpineJointPosZ() {
  float avg = 0.0f;
  for (std::list<float>::iterator i = spinePosZBuf.begin();
       i != spinePosZBuf.end(); i++)
    avg += (*i);
  return avg / spinePosZBuf.size();
}

KinectSensor::KinectSensor() {
  HRESULT hr;

  ms = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch());

  hr = GetDefaultKinectSensor(&pKinectSensor); 
  if (FAILED(hr))
    error("ERROR : getDefaultKinectSensor() : ", hr);

  hr = pKinectSensor->Open();
  if (FAILED(hr))
    error("ERROR : pKinectSensor->open() : ", hr);

  // GET THE DEPTH READER //
  IDepthFrameSource *pDepthFrameSource;
  hr = pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
  if (FAILED(hr))
    error("ERROR : pKinectSensor->getDepthFrameSource() : ", hr);
  hr = pDepthFrameSource->OpenReader(&pDepthFrameReader);
  if (FAILED(hr))
    error("ERROR : pDepthFrameSource->OpenReader() : ", hr);
  SafeRelease<IDepthFrameSource>(pDepthFrameSource);

  // GET THE COLOR READER //
  IColorFrameSource *pColorFrameSource;
  hr = pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
  if (FAILED(hr))
    error("ERROR : pKinectSensor->getColorFrameSource() : ", hr);
  hr = pColorFrameSource->OpenReader(&pColorFrameReader);
  if (FAILED(hr))
    error("ERROR : pColorFrameSource->OpenReader() : ", hr);
  SafeRelease<IColorFrameSource>(pColorFrameSource);

  // GET THE INFRARED READER //
  IInfraredFrameSource *pInfraredFrameSource;
  hr = pKinectSensor->get_InfraredFrameSource(&pInfraredFrameSource);
  if (FAILED(hr))
    error("ERROR : pKinectSensor->getInfraredFrameSource() : ", hr);
  hr = pInfraredFrameSource->OpenReader(&pInfraredFrameReader);
  if (FAILED(hr))
    error("ERROR : pInfraredFrameSource->OpenReader() : ", hr);
  SafeRelease<IInfraredFrameSource>(pInfraredFrameSource);

  // GET THE BODY READER //
  IBodyFrameSource *pBodyFrameSource;
  hr = pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
  if (FAILED(hr))
    error("ERROR : pKinectSensor->getBodyFrameSource() : ", hr);
  hr = pBodyFrameSource->OpenReader(&pBodyFrameReader);
  if (FAILED(hr))
    error("ERROR : pBodyFrameSource->OpenReader() : ", hr);
  SafeRelease<IBodyFrameSource>(pBodyFrameSource);

  hr = pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
  if (FAILED(hr))
    error("ERROR : pKinectSensor->getCoordinateMapper() : ", hr);
}

KinectSensor::~KinectSensor() {}

IDepthFrame *KinectSensor::getNextDepthFrame() {
  IDepthFrame *depthFrame = NULL;
  HRESULT hr = pDepthFrameReader->AcquireLatestFrame(&depthFrame);
  if (FAILED(hr))
    error("ERROR : pDepthFrameReader->AcquireLatestFrame() : ", hr);
  return depthFrame;
}

IColorFrame *KinectSensor::getNextColorFrame() {
  IColorFrame *colorFrame = NULL;
  HRESULT hr = pColorFrameReader->AcquireLatestFrame(&colorFrame);
  if (FAILED(hr))
    error("ERROR : pColorFramReader->AcquireLatestFrame() : ", hr);
  return colorFrame;
}

IInfraredFrame *KinectSensor::getNextInfraredFrame() {
  IInfraredFrame *infraredFrame = NULL;
  HRESULT hr = pInfraredFrameReader->AcquireLatestFrame(&infraredFrame);
  if (FAILED(hr))
    error("ERROR : pInfraredFrameReader->AcquireLatestFrame() : ", hr);
  return infraredFrame;
}

IBodyFrame *KinectSensor::getNextBodyFrame() {
  IBodyFrame *bodyFrame = NULL;

  HRESULT hr = pBodyFrameReader->AcquireLatestFrame(&bodyFrame);
  if (FAILED(hr))
    error("ERROR : pBodyFrameReader->AcquireLatestFrame() : ", hr);
  return bodyFrame;
}

IDepthFrameReader *KinectSensor::getDepthFrameReader() {
  return pDepthFrameReader;
}

IColorFrameReader *KinectSensor::getColorFrameReader() {
  return pColorFrameReader;
}

IInfraredFrameReader *KinectSensor::getInfraredFrameReader() {
  return pInfraredFrameReader;
}

IBodyFrameReader *KinectSensor::getBodyFrameReader() {
  return pBodyFrameReader;
}

ICoordinateMapper *KinectSensor::getCoordinateMapper() {
  return m_pCoordinateMapper;
}

IKinectSensor *KinectSensor::getKinectSensor() {
	return pKinectSensor;
}

template <class Interface>
void KinectSensor::SafeRelease(Interface *&pInterface) {
  if (pInterface != NULL) {
    pInterface->Release();
    pInterface = NULL;
  }
}

void KinectSensor::error(std::string e, HRESULT hr) {
  // std::cout << e;
  std::cout << hr;
  Sleep(5000);
  exit(-1);
}

bool KinectSensor::is_closest_body(Joint joint, IBody ** ppbodies) {
	float z = joint.Position.X;
	Joint joints[JointType_Count];
	for (int i = 0; i < BODY_COUNT; i++) {
		IBody *pBody = ppbodies[i];
		if (pBody) {
			BOOLEAN bTracked = false;
			HRESULT hr = pBody->get_IsTracked(&bTracked);

			hr = pBody->GetJoints(_countof(joints), joints);
			if (SUCCEEDED(hr) && bTracked) {
				if (joints[JointType_SpineBase].Position.X > z) {
					Gmykeyhandler->handleGesture(3, z, joints[JointType_SpineBase].Position.X,true);
					return false;
				}
			}
		}
	}
	return true;
}

void KinectSensor::updateHandPosition(KinectSensor kinectSensor) {
  // Initialize the Kinect sensor and the readers
  //KinectSensor kinectSensor = KinectSensor();
  IBodyFrame *bodyFrame;
  IBodyFrameReader *bodyreader = kinectSensor.getBodyFrameReader();
  
  while (KeepUpdatingHandPos) {
	  //Sleep(100);
	 
    IBody *ppBodies[BODY_COUNT] = {0};

    // Get the frames //
    HRESULT hr = bodyreader->AcquireLatestFrame(&bodyFrame);

    if (!SUCCEEDED(hr) || bodyFrame == NULL)
      continue;
	
    hr = bodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
    bodyFrame->Release();
    bodyFrame = NULL;

    if (!SUCCEEDED(hr))
      continue;
	
    // Process bodies //
    Joint joints[JointType_Count];
    for (int i = 0; i < _countof(ppBodies); i++) {

      IBody *pBody = ppBodies[i];
      if (pBody) {
        BOOLEAN bTracked = false;
        hr = pBody->get_IsTracked(&bTracked);

        if (SUCCEEDED(hr) && bTracked) {
			
          hr = pBody->GetJoints(_countof(joints), joints);
		  if (is_closest_body(joints[JointType_SpineBase], ppBodies)) {
			  addHandJoint(joints[JointType_HandRight]);
			  addSpineJoint(joints[JointType_SpineBase]);
			  handCoords[0] = getHandJointPosX() - getSpineJointPosX();
			  handCoords[1] = getHandJointPosY() - getSpineJointPosY();
			  handCoords[2] = getHandJointPosZ() - getSpineJointPosZ();

			  if (Gmykeyhandler->kinectControl == true)
				updateGesture(pBody);
		  }
        }
        pBody->Release();
		ppBodies[i] = NULL;
      }
    }
  }

  IDepthFrameReader *pDepthFrameReader = kinectSensor.getDepthFrameReader();
  IColorFrameReader *pColorFrameReader = kinectSensor.getColorFrameReader();
  IInfraredFrameReader *pInfraredFrameReader = kinectSensor.getInfraredFrameReader();
  IBodyFrameReader *pBodyFrameReader = kinectSensor.getBodyFrameReader();

  ICoordinateMapper *m_pCoordinateMapper = kinectSensor.getCoordinateMapper();

  IKinectSensor *pKinectSensor = kinectSensor.getKinectSensor();

  if (pDepthFrameReader != NULL) {
	  pDepthFrameReader->Release();
  }
  if (pColorFrameReader != NULL) {
	  pColorFrameReader->Release();
  }
  if (pInfraredFrameReader != NULL) {
	  pInfraredFrameReader->Release();
  }
  if (pBodyFrameReader != NULL) {
	  pBodyFrameReader->Release();
  }
  if (m_pCoordinateMapper != NULL) {
	  m_pCoordinateMapper->Release();
  }

  if (pKinectSensor != NULL) {
	  pKinectSensor->Release();
  }
  return;
}

void KinectSensor::mapHandToCursor(float *handPosition, int screenWidth,
                                   int screenHeight, int *cursor) {
  float x = handPosition[0] - VIRTUAL_RECTANGLE_CORNER_L_X;
  cursor[0] = (x * screenWidth) /
              (VIRTUAL_RECTANGLE_CORNER_R_X - VIRTUAL_RECTANGLE_CORNER_L_X);
  cursor[0] = cursor[0] < 0 ? 0 : cursor[0];
  cursor[0] = cursor[0] > screenWidth ? screenWidth : cursor[0];

  float y = handPosition[1] - VIRTUAL_RECTANGLE_CORNER_L_Y;
  cursor[1] = (y * screenWidth) /
              (VIRTUAL_RECTANGLE_CORNER_R_Y - VIRTUAL_RECTANGLE_CORNER_L_Y);
  cursor[1] = cursor[1] < 0 ? 0 : cursor[1];
  cursor[1] = cursor[1] > screenHeight ? screenHeight : cursor[1];
}

void KinectSensor::updateGesture(IBody *pBody) {
  int hr;
  HandState rhandState, lhandState;
  pBody->get_HandRightState(&rhandState);
  pBody->get_HandLeftState(&lhandState);
  if (rhandState == HandState_Closed) {
	  ms = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch());
	  if (rightHand_closed) { // Right hand previously closed
        if (std::abs(hand_pos_x - handCoords[0]) >
            THRESHOLD_DISTANCE_SWAP_CANDIDATES && lhandState == HandState_Closed) {
			timer = std::time(nullptr);
			gestureType = SWAP;
			panning_delta_x = hand_pos_x - handCoords[0];
			panning_delta_y = hand_pos_y - handCoords[1];
			Gmykeyhandler->handleGesture(SWAP, panning_delta_x, panning_delta_y, leftHand_closed);
			leftHand_closed = true;
      } else {
			if (lhandState != HandState_Closed) {
				gestureType = PANNING;
				panning_delta_x = hand_pos_x - handCoords[0];
				panning_delta_y = hand_pos_y - handCoords[1];
				panning_delta_x = std::abs(panning_delta_x) > 0.02 ? panning_delta_x : 0;
				panning_delta_y = std::abs(panning_delta_y) > 0.02 ? panning_delta_y : 0;
				//std::cerr << panning_delta_x << ", " << panning_delta_y << std::endl;
				Gmykeyhandler->handleGesture(PANNING, panning_delta_x, panning_delta_y, true);
			}
			else
				leftHand_closed = false;
      }
	  
    } else {
		
			gestureType = PANNING;
			//panning_delta_x = hand_pos_x - handCoords[0];
			//panning_delta_y = hand_pos_y - handCoords[1];
			//panning_delta_x = panning_delta_x > 0.03 ? panning_delta_x : 0;
			//panning_delta_y = panning_delta_y > 0.03 ? panning_delta_y : 0;
			//std::cerr << panning_delta_x << ", " << panning_delta_y << std::endl;
			timer = std::time(nullptr);
			// Store hand position x
			hand_pos_x = handCoords[0];
			hand_pos_y = handCoords[1];
			//Gmykeyhandler->handleGesture(PANNING, panning_delta_x, panning_delta_y);
		
    }
	

    // Set rightHand closed state true
    rightHand_closed = true;
  } else {
	  std::chrono::milliseconds t1 = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch());
	  if ( t1.count() - ms.count() > 100) {
		  ms = t1;
		  // Detect no gesture
		  gestureType = NO_GESTURE;
		  hand_pos_x = 0;
		  hand_pos_y = 0;
		  rightHand_closed = false;
		  leftHand_closed = false;
	  }
  }
}

float KinectSensor::getHandsDistance(IBody *pBody) {
  Joint joints[JointType_Count];
  int hr = pBody->GetJoints(_countof(joints), joints);
  Joint rhj = joints[JointType_HandRight];
  Joint lhj = joints[JointType_HandLeft];
  float d = sqrt(
      (rhj.Position.X - lhj.Position.X) * (rhj.Position.X - lhj.Position.X) +
      (rhj.Position.Y - lhj.Position.Y) * (rhj.Position.Y - lhj.Position.Y));
  return d;
}

std::string KinectSensor::getGestureType() {
  switch (gestureType) {
  case NO_GESTURE:
    return "NO_GESTURE";
  case PANNING:
    return "PANNING";
  case SWAP:
    return "SWAP";
  default:
    return "";
  }
}

bool KinectSensor::getKeepUpdating() {
	return KeepUpdatingHandPos;
}

void KinectSensor::setKeepUpdating(bool value) {
	KeepUpdatingHandPos = value;
}


void KinectSensor::release_sensor(KinectSensor ks) {
	IDepthFrameReader *pDepthFrameReader = ks.getDepthFrameReader();
	IColorFrameReader *pColorFrameReader = ks.getColorFrameReader();
	IInfraredFrameReader *pInfraredFrameReader = ks.getInfraredFrameReader();
	IBodyFrameReader *pBodyFrameReader = ks.getBodyFrameReader();

	ICoordinateMapper *m_pCoordinateMapper = ks.getCoordinateMapper();

	IKinectSensor *pKinectSensor = ks.getKinectSensor();

	if (pDepthFrameReader != NULL) {
		pDepthFrameReader->Release();
	}
	if (pColorFrameReader != NULL) {
		pColorFrameReader->Release();
	}
	if (pInfraredFrameReader != NULL) {
		pInfraredFrameReader->Release();
	}
	if (pBodyFrameReader != NULL) {
		pBodyFrameReader->Release();
	}
	if (m_pCoordinateMapper != NULL) {
		m_pCoordinateMapper->Release();
	}

	if (pKinectSensor != NULL) {
		pKinectSensor->Release();
	}

}