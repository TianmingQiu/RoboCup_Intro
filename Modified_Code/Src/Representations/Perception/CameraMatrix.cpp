/**
 * @file CameraMatrix.cpp
 *
 * Implementation of struct CameraMatrix.
 */

#include "CameraMatrix.h"
#include "Tools/Boundary.h"
#include "Tools/Debugging/DebugDrawings.h"
#include "Tools/Math/Geometry.h"
#include "Tools/Math/Transformation.h"
#include "Representations/Infrastructure/CameraInfo.h"
#include "Tools/Module/Blackboard.h"
#include "Modules/Infrastructure/NaoProvider.h"
#include "libbhuman/bhuman.h"

CameraMatrix::CameraMatrix(const Pose3f& pose) :
  Pose3f(pose), isValid(true)
{}

void CameraMatrix::draw() const
{
  DECLARE_DEBUG_DRAWING("representation:CameraMatrix:Image", "drawingOnImage"); // Shows the robot coordinate system
  DECLARE_DEBUG_DRAWING("representation:CameraMatrix:Field", "drawingOnField"); // Shows the robot coordinate system

  COMPLEX_DRAWING("representation:CameraMatrix:Field")
  {
    CameraInfo cameraInfo;
    if(Blackboard::getInstance().exists("CameraInfo"))
      cameraInfo = static_cast<const CameraInfo&>(Blackboard::getInstance()["CameraInfo"]);
    Vector2i pointOnField[6];
    bool isValid[6];
    // calculate the projection of the four image corners to the ground
    isValid[0] = Transformation::imageToRobot(0, 0, *this, cameraInfo, pointOnField[0]);
    isValid[1] = Transformation::imageToRobot(cameraInfo.width, 0, *this, cameraInfo, pointOnField[1]);
    isValid[2] = Transformation::imageToRobot(cameraInfo.width, cameraInfo.height, *this, cameraInfo, pointOnField[2]);
    isValid[3] = Transformation::imageToRobot(0, cameraInfo.height, *this, cameraInfo, pointOnField[3]);

    // calculate a line 15 pixels below the horizon in the image
    const Geometry::Line horizon = Geometry::calculateHorizon(*this, cameraInfo);
    Geometry::Line lineBelowHorizon;
    const Vector2f vertLineDirection(-horizon.direction.y(), horizon.direction.x());
    lineBelowHorizon.direction = horizon.direction;
    lineBelowHorizon.base = horizon.base;
    lineBelowHorizon.base += vertLineDirection * 15.0f;

    // calculate the projection to the ground of the intersection points of the line parallel to the horizon and the image borders
    Vector2i beginPoint;
    Vector2i endPoint;
    if(Geometry::getIntersectionPointsOfLineAndRectangle(Vector2i::Zero(), Vector2i(cameraInfo.width - 1, cameraInfo.height - 1), lineBelowHorizon, beginPoint, endPoint))
    {
      isValid[4] = Transformation::imageToRobot(beginPoint.x(), beginPoint.y(), *this, cameraInfo, pointOnField[4]);
      isValid[5] = Transformation::imageToRobot(endPoint.x(), endPoint.y(), *this, cameraInfo, pointOnField[5]);
      if(isValid[4] && isValid[5])
        LINE("representation:CameraMatrix:Field", pointOnField[4].x(), pointOnField[4].y(), pointOnField[5].x(), pointOnField[5].y(), 30, Drawings::solidPen, ColorRGBA::yellow);
    }
    else
      isValid[4] = isValid[5] = false;

    // determine the boundary of all the points that were projected to the ground
    Boundaryi boundary(-10000, +10000);
    for(int i = 0; i < 6; ++i)
      if(isValid[i])
      {
        boundary.add(pointOnField[i]);
        const ColorRGBA& color = i < 4 ? ColorRGBA::white : ColorRGBA::yellow;
        CIRCLE("representation:CameraMatrix:Field", pointOnField[i].x(), pointOnField[i].y(), 100, 50, Drawings::solidPen, color, Drawings::noBrush, color);
      }

    LINE("representation:CameraMatrix:Field", boundary.x.min, boundary.y.min, boundary.x.max, boundary.y.min, 30, Drawings::solidPen, ColorRGBA::red);
    LINE("representation:CameraMatrix:Field", boundary.x.max, boundary.y.min, boundary.x.max, boundary.y.max, 30, Drawings::solidPen, ColorRGBA::yellow);
    LINE("representation:CameraMatrix:Field", boundary.x.max, boundary.y.max, boundary.x.min, boundary.y.max, 30, Drawings::solidPen, ColorRGBA::blue);
    LINE("representation:CameraMatrix:Field", boundary.x.min, boundary.y.max, boundary.x.min, boundary.y.min, 30, Drawings::solidPen, ColorRGBA::white);

    // fill the bounding rectangle with coordinate system lines (and reproject it to the image)
    const int spacing = 100;
    for(int xx = boundary.x.min - boundary.x.min % spacing + spacing; xx <= boundary.x.max; xx += spacing)
    {
      LINE("representation:CameraMatrix:Field", xx, boundary.y.min, xx, boundary.y.max, 5, Drawings::solidPen, ColorRGBA::white);
    }
    for(int yy = boundary.y.min - boundary.y.min % spacing + spacing; yy <= boundary.y.max; yy += spacing)
    {
      LINE("representation:CameraMatrix:Field", boundary.x.min, yy, boundary.x.max, yy, 5, Drawings::solidPen, ColorRGBA::white);
    }
  } // end complex drawing

  COMPLEX_DRAWING("representation:CameraMatrix:Image")
  {
    if(Blackboard::getInstance().exists("CameraInfo"))
    {
      const CameraInfo cameraInfo = static_cast<const CameraInfo&>(Blackboard::getInstance()["CameraInfo"]);
      Vector2i pointOnField[6];
      bool isValid[6];
      // calculate the projection of the four image corners to the ground
      isValid[0] = Transformation::imageToRobot(0, 0, *this, cameraInfo, pointOnField[0]);
      isValid[1] = Transformation::imageToRobot(cameraInfo.width, 0, *this, cameraInfo, pointOnField[1]);
      isValid[2] = Transformation::imageToRobot(cameraInfo.width, cameraInfo.height, *this, cameraInfo, pointOnField[2]);
      isValid[3] = Transformation::imageToRobot(0, cameraInfo.height, *this, cameraInfo, pointOnField[3]);

      // calculate a line 15 pixels below the horizon in the image
      const Geometry::Line horizon = Geometry::calculateHorizon(*this, cameraInfo);
      Geometry::Line lineBelowHorizon;
      const Vector2f vertLineDirection(-horizon.direction.y(), horizon.direction.x());
      lineBelowHorizon.direction = horizon.direction;
      lineBelowHorizon.base = horizon.base;
      lineBelowHorizon.base += vertLineDirection * 15.0f;

      // calculate the projection to the ground of the intersection points of the line parallel to the horizon and the image borders
      Vector2f beginPoint = Vector2f::Zero();
      Vector2f endPoint = Vector2f::Zero();
      const Vector2f topRight = Vector2f(cameraInfo.width - 1.f, cameraInfo.height - 1.f);
      if(Geometry::getIntersectionPointsOfLineAndRectangle(Vector2f::Zero(), topRight, lineBelowHorizon, beginPoint, endPoint))
      {
        LINE("representation:CameraMatrix:Image", beginPoint.x(), beginPoint.y(), endPoint.x(), endPoint.y(), 3, Drawings::dashedPen, ColorRGBA::white);
        Vector2f pointOnFieldFloat = Vector2f::Zero();
        isValid[4] = Transformation::imageToRobot(beginPoint, *this, cameraInfo, pointOnFieldFloat);
        pointOnField[4].x() = static_cast<int>(std::floor(pointOnFieldFloat.x() + 0.5));
        pointOnField[4].y() = static_cast<int>(std::floor(pointOnFieldFloat.y() + 0.5));
        isValid[5] = Transformation::imageToRobot(endPoint, *this, cameraInfo, pointOnFieldFloat);
        pointOnField[5].x() = static_cast<int>(std::floor(pointOnFieldFloat.x() + 0.5));
        pointOnField[5].y() = static_cast<int>(std::floor(pointOnFieldFloat.y() + 0.5));
      }
      else
        isValid[4] = isValid[5] = false;

      // determine the boundary of all the points that were projected to the ground
      Boundaryi boundary(-10000, +10000);
      for(int i = 0; i < 6; ++i)
        if(isValid[i])
          boundary.add(pointOnField[i]);

      // fill the bounding rectangle with coordinate system lines (and reproject it to the image)
      int spacing = 100;
      for(int xx = boundary.x.min - boundary.x.min % spacing + spacing; xx <= boundary.x.max; xx += spacing)
        if(Transformation::robotToImage(Vector2f(static_cast<float>(xx), static_cast<float>(boundary.y.min)), *this, cameraInfo, beginPoint) &&
           Transformation::robotToImage(Vector2f(static_cast<float>(xx), static_cast<float>(boundary.y.max)), *this, cameraInfo, endPoint))
          LINE("representation:CameraMatrix:Image",
               beginPoint.x(), beginPoint.y(), endPoint.x(), endPoint.y(),
               xx == 0 ? 3 : 0, Drawings::solidPen, ColorRGBA::white);
      for(int yy = boundary.y.min - boundary.y.min % spacing + spacing; yy <= boundary.y.max; yy += spacing)
        if(Transformation::robotToImage(Vector2f(static_cast<float>(boundary.x.min), static_cast<float>(yy)), *this, cameraInfo, beginPoint) &&
           Transformation::robotToImage(Vector2f(static_cast<float>(boundary.x.max), static_cast<float>(yy)), *this, cameraInfo, endPoint))
          LINE("representation:CameraMatrix:Image",
               beginPoint.x(), beginPoint.y(), endPoint.x(), endPoint.y(),
               yy == 0 ? 3 : 0, Drawings::solidPen, ColorRGBA::white);
    }
  } // end complex drawing
}

void RobotCameraMatrix::draw() const
{
  DECLARE_DEBUG_DRAWING("representation:RobotCameraMatrix:Image", "drawingOnImage"); // Shows the robot coordinate system
  DECLARE_DEBUG_DRAWING("representation:RobotCameraMatrix:Field", "drawingOnField"); // Shows the robot coordinate system

  COMPLEX_DRAWING("representation:RobotCameraMatrix:Field")
  {
    CameraInfo cameraInfo;
    if(Blackboard::getInstance().exists("CameraInfo"))
      cameraInfo = static_cast<const CameraInfo&>(Blackboard::getInstance()["CameraInfo"]);
    Vector2i pointOnField[6];
    bool isValid[6];
    // calculate the projection of the four image corners to the ground
    isValid[0] = Transformation::imageToRobot(0, 0, *this, cameraInfo, pointOnField[0]);
    isValid[1] = Transformation::imageToRobot(cameraInfo.width, 0, *this, cameraInfo, pointOnField[1]);
    isValid[2] = Transformation::imageToRobot(cameraInfo.width, cameraInfo.height, *this, cameraInfo, pointOnField[2]);
    isValid[3] = Transformation::imageToRobot(0, cameraInfo.height, *this, cameraInfo, pointOnField[3]);

    // calculate a line 15 pixels below the horizon in the image
    const Geometry::Line horizon = Geometry::calculateHorizon(*this, cameraInfo);
    Geometry::Line lineBelowHorizon;
    const Vector2f vertLineDirection(-horizon.direction.y(), horizon.direction.x());
    lineBelowHorizon.direction = horizon.direction;
    lineBelowHorizon.base = horizon.base;
    lineBelowHorizon.base += vertLineDirection * 15.0;

    // calculate the projection to the ground of the intersection points of the line parallel to the horizon and the image borders
    Vector2i beginPoint;
    Vector2i endPoint;
    if(Geometry::getIntersectionPointsOfLineAndRectangle(Vector2i::Zero(), Vector2i(cameraInfo.width - 1, cameraInfo.height - 1), lineBelowHorizon, beginPoint, endPoint))
    {
      isValid[4] = Transformation::imageToRobot(beginPoint.x(), beginPoint.y(), *this, cameraInfo, pointOnField[4]);
      isValid[5] = Transformation::imageToRobot(endPoint.x(), endPoint.y(), *this, cameraInfo, pointOnField[5]);
      if(isValid[4] && isValid[5])
        LINE("representation:CameraMatrix:Field", pointOnField[4].x(), pointOnField[4].y(), pointOnField[5].x(), pointOnField[5].y(), 30, Drawings::solidPen, ColorRGBA::yellow);
    }

    // determine the boundary of all the points that were projected to the ground
    Boundaryi boundary(-10000, +10000);
    for(int i = 0; i < 6; ++i)
      if(isValid[i])
      {
        boundary.add(pointOnField[i]);
        const ColorRGBA& color = i < 4 ? ColorRGBA::white : ColorRGBA::yellow;
        CIRCLE("representation:RobotCameraMatrix:Field", pointOnField[i].x(), pointOnField[i].y(), 100, 50, Drawings::solidPen, color, Drawings::noBrush, color);
      }

    LINE("representation:RobotCameraMatrix:Field", boundary.x.min, boundary.y.min, boundary.x.max, boundary.y.min, 30, Drawings::solidPen, ColorRGBA::red);
    LINE("representation:RobotCameraMatrix:Field", boundary.x.max, boundary.y.min, boundary.x.max, boundary.y.max, 30, Drawings::solidPen, ColorRGBA::yellow);
    LINE("representation:RobotCameraMatrix:Field", boundary.x.max, boundary.y.max, boundary.x.min, boundary.y.max, 30, Drawings::solidPen, ColorRGBA::blue);
    LINE("representation:RobotCameraMatrix:Field", boundary.x.min, boundary.y.max, boundary.x.min, boundary.y.min, 30, Drawings::solidPen, ColorRGBA::white);

    // fill the bounding rectangle with coordinate system lines (and reproject it to the image)
    const int spacing = 100;
    for(int xx = boundary.x.min - boundary.x.min % spacing + spacing; xx <= boundary.x.max; xx += spacing)
    {
      LINE("representation:RobotCameraMatrix:Field", xx, boundary.y.min, xx, boundary.y.max, 5, Drawings::solidPen, ColorRGBA::white);
    }
    for(int yy = boundary.y.min - boundary.y.min % spacing + spacing; yy <= boundary.y.max; yy += spacing)
    {
      LINE("representation:RobotCameraMatrix:Field", boundary.x.min, yy, boundary.x.max, yy, 5, Drawings::solidPen, ColorRGBA::white);
    }
  } // end complex drawing

  COMPLEX_DRAWING("representation:RobotCameraMatrix:Image")
  {
    if(Blackboard::getInstance().exists("CameraInfo"))
    {
      const CameraInfo cameraInfo = static_cast<const CameraInfo&>(Blackboard::getInstance()["CameraInfo"]);
      Vector2i pointOnField[6];
      bool isValid[6];
      // calculate the projection of the four image corners to the ground
      isValid[0] = Transformation::imageToRobot(0, 0, *this, cameraInfo, pointOnField[0]);
      isValid[1] = Transformation::imageToRobot(cameraInfo.width, 0, *this, cameraInfo, pointOnField[1]);
      isValid[2] = Transformation::imageToRobot(cameraInfo.width, cameraInfo.height, *this, cameraInfo, pointOnField[2]);
      isValid[3] = Transformation::imageToRobot(0, cameraInfo.height, *this, cameraInfo, pointOnField[3]);

      // calculate a line 15 pixels below the horizon in the image
      const Geometry::Line horizon = Geometry::calculateHorizon(*this, cameraInfo);
      Geometry::Line lineBelowHorizon;
      const Vector2f vertLineDirection(-horizon.direction.y(), horizon.direction.x());
      lineBelowHorizon.direction = horizon.direction;
      lineBelowHorizon.base = horizon.base;
      lineBelowHorizon.base += vertLineDirection * 15.0;

      // calculate the projection to the ground of the intersection points of the line parallel to the horizon and the image borders
      Vector2f beginPoint;
      Vector2f endPoint;
      if(Geometry::getIntersectionPointsOfLineAndRectangle(Vector2f::Zero(),
        Vector2f(static_cast<float>(cameraInfo.width - 1), static_cast<float>(cameraInfo.height - 1)),
        lineBelowHorizon, beginPoint, endPoint))
      {
        LINE("representation:RobotCameraMatrix:Image", beginPoint.x(), beginPoint.y(), endPoint.x(), endPoint.y(), 3, Drawings::dashedPen, ColorRGBA::white);
        Vector2f pointOnFieldFloat;
        isValid[4] = Transformation::imageToRobot(beginPoint, *this, cameraInfo, pointOnFieldFloat);
        pointOnField[4].x() = static_cast<int>(std::floor(pointOnFieldFloat.x() + 0.5));
        pointOnField[4].y() = static_cast<int>(std::floor(pointOnFieldFloat.y() + 0.5));
        isValid[5] = Transformation::imageToRobot(endPoint, *this, cameraInfo, pointOnFieldFloat);
        pointOnField[5].x() = static_cast<int>(std::floor(pointOnFieldFloat.x() + 0.5));
        pointOnField[5].y() = static_cast<int>(std::floor(pointOnFieldFloat.y() + 0.5));
      }
      else
        isValid[4] = isValid[5] = false;

      // determine the boundary of all the points that were projected to the ground
      Boundaryi boundary(-10000, +10000);
      for(int i = 0; i < 6; ++i)
        if(isValid[i])
          boundary.add(pointOnField[i]);

      // fill the bounding rectangle with coordinate system lines (and reproject it to the image)
      const int spacing = 100;
      for(int xx = boundary.x.min - boundary.x.min % spacing + spacing; xx <= boundary.x.max; xx += spacing)
        if(Transformation::robotToImage(Vector2f(static_cast<float>(xx), static_cast<float>(boundary.y.min)), *this, CameraInfo(), beginPoint) &&
           Transformation::robotToImage(Vector2f(static_cast<float>(xx), static_cast<float>(boundary.y.max)), *this, CameraInfo(), endPoint))
          LINE("representation:RobotCameraMatrix:Image",
               beginPoint.x(), beginPoint.y(), endPoint.x(), endPoint.y(),
               xx == 0 ? 3 : 0, Drawings::solidPen, ColorRGBA::yellow);
      for(int yy = boundary.y.min - boundary.y.min % spacing + spacing; yy <= boundary.y.max; yy += spacing)
        if(Transformation::robotToImage(Vector2f(static_cast<float>(boundary.x.min), static_cast<float>(yy)), *this, CameraInfo(), beginPoint) &&
           Transformation::robotToImage(Vector2f(static_cast<float>(boundary.x.max), static_cast<float>(yy)), *this, CameraInfo(), endPoint))
          LINE("representation:RobotCameraMatrix:Image",
               beginPoint.x(), beginPoint.y(), endPoint.x(), endPoint.y(),
               yy == 0 ? 3 : 0, Drawings::solidPen, ColorRGBA::yellow);
    }
  } // end complex drawing
}

RobotCameraMatrix::RobotCameraMatrix(const RobotDimensions& robotDimensions, float headYaw, float headPitch, const CameraCalibration& cameraCalibration, bool upperCamera)
{
  computeRobotCameraMatrix(robotDimensions, headYaw, headPitch, cameraCalibration, upperCamera);
}

void RobotCameraMatrix::computeRobotCameraMatrix(const RobotDimensions& robotDimensions, float headYaw, float headPitch, const CameraCalibration& cameraCalibration, bool upperCamera)
{
  *this = RobotCameraMatrix();
  NaoBody naoBody;
  float* sensors = naoBody.getSensors();


  translate(0., 0., robotDimensions.hipToNeckLength * cos(sensors[gyroXSensor]));
  //translate(0., 0., robotDimensions.hipToNeckLength);
  rotateZ(headYaw);
  rotateY(headPitch);
  if(upperCamera)
  {
    translate(robotDimensions.xOffsetNeckToUpperCamera, 0.f, robotDimensions.zOffsetNeckToUpperCamera);
    rotateY(robotDimensions.tiltNeckToUpperCamera + cameraCalibration.upperCameraRotationCorrection.y());
    rotateX(cameraCalibration.upperCameraRotationCorrection.x());
    rotateZ(cameraCalibration.upperCameraRotationCorrection.z());
  }
  else
  {
    translate(robotDimensions.xOffsetNeckToLowerCamera, 0.f, robotDimensions.zOffsetNeckToLowerCamera);
    rotateY(robotDimensions.tiltNeckToLowerCamera + cameraCalibration.lowerCameraRotationCorrection.y());
    rotateX(cameraCalibration.lowerCameraRotationCorrection.x());
    rotateZ(cameraCalibration.lowerCameraRotationCorrection.z());
  }
}

CameraMatrix::CameraMatrix(const Pose3f& torsoMatrix, const Pose3f& robotCameraMatrix, const CameraCalibration& cameraCalibration)
{
  computeCameraMatrix(torsoMatrix, robotCameraMatrix, cameraCalibration);
}

void CameraMatrix::computeCameraMatrix(const Pose3f& torsoMatrix, const Pose3f& robotCameraMatrix, const CameraCalibration& cameraCalibration)
{
  static_cast<Pose3f&>(*this) = torsoMatrix;
  rotateY(cameraCalibration.bodyRotationCorrection.y());
  rotateX(cameraCalibration.bodyRotationCorrection.x());
  conc(robotCameraMatrix);
}