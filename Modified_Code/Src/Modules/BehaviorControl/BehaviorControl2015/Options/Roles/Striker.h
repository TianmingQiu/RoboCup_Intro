/** A Striker.
 */
option(Striker)
{
    // Debug
    Annotation("robotPose deviation: ", theRobotPose.deviation);
    Annotation("robotPose validity: ", theRobotPose.validity);

    common_transition{
        if (state_time > 5000 && (theRobotPose.deviation > libCodeRelease.lostDeviation || theRobotPose.validity < libCodeRelease.minValidity))
            goto localizeSelf;
    }
    initial_state(start)
    {
        transition
        {
            if(state_time > libCodeRelease.initialDelay)
                goto walkToStartPosition;
        }
        action
        { 
            theHeadControlMode = HeadControl::lookLeftAndRight;
        }
    }
    
    state(walkToStartPosition)
	  transition
        { // go to searchForBall if you've reached the goal
            // currentPos - goalPos
            Vector2f ownPosFieldCoordinates = libCodeRelease.robotToField(Vector2f(0.f, 0.f));
            float distance = libCodeRelease.distanceBetween(ownPosFieldCoordinates,
                                                            libCodeRelease.strikerStartPosition);
            if(distance < 100.f) // after reaching the center of the penalty area, face toward the opponent side
                goto faceOpponentDirection;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight;
            headControlTurnAngle = (float)1.0;
            Vector2f centerDefenseAreaRobotCoordinates = libCodeRelease.fieldToRobot(
                libCodeRelease.strikerStartPosition); // convert to relative coordinates
            Pose2f target = Pose2f(centerDefenseAreaRobotCoordinates.angle(),
                                   centerDefenseAreaRobotCoordinates); // turn to face in the right direction
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), target);
        }
        
    state(faceOpponentDirection)
    {
        transition
        {
            // if (action_done)
            //	goto defendGoal;
            if(std::abs(libCodeRelease.fieldToRobot(libCodeRelease.centerOpponentGroundLine).angle()) < 5_deg) {
                goto turnToBall;
            }
        }
        action
        { // face toward the center of the field
            Annotation("Turning towards opponent...");
            theHeadControlMode = HeadControl::lookLeftAndRight;
            headControlTurnAngle = (float)0.5; // in radians
            Vector2f relativeOpponentGroundLine =
                libCodeRelease.fieldToRobot(libCodeRelease.centerOpponentGroundLine); // transform to robot coordinates
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(relativeOpponentGroundLine.angle(), 0.f, 0.f));
        }
    }

    state(turnToBall)
    {
        transition
        {
            if(libCodeRelease.timeSinceBallWasSeen() > 7000)
                goto searchForBall;
            else if(std::abs(theBallModel.estimate.position.angle()) < 5_deg)
                goto walkToBall;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight; headControlTurnAngle = float(0.5);
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
        }
    }

    state(walkToBall)
    {
        transition
        {
            if(libCodeRelease.timeSinceBallWasSeen() > 7000)
                goto searchForBall;
            else if(theBallModel.estimate.position.norm() < 700.f)
                goto alignToGoal;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight; headControlTurnAngle = float(0.5);
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(),theBallModel.estimate.position));
        }
    }

    state(alignToGoal)
    {
        transition
        {
            if(libCodeRelease.timeSinceBallWasSeen() > 7000)
                goto searchForBall;
            else if(std::abs(libCodeRelease.angleToGoal) < 8_deg && std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto alignBehindBall;
            else if(theBallModel.estimate.position.norm() > 600.f)
                goto turnToBall;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight; headControlTurnAngle = float(0.5);
            WalkToTarget(Pose2f(100.f, 100.f, 100.f),
                         Pose2f(libCodeRelease.angleToGoal,
                                theBallModel.estimate.position.x() - 200.f,
                                theBallModel.estimate.position.y()), 1, 500);  //set collision avoidance to lower value
        }
    }

    state(alignBehindBall)
    {
        transition
        {
            if(libCodeRelease.timeSinceBallWasSeen() > 7000)
                goto searchForBall;
            else if(libCodeRelease.between(theBallModel.estimate.position.y(), 20.f, 50.f) &&
               libCodeRelease.between(theBallModel.estimate.position.x(), 140.f, 170.f) &&
               std::abs(libCodeRelease.angleToGoal) < 2_deg)
                goto kick;
            else if(theBallModel.estimate.position.norm() > 400.f)
                goto alignToGoal;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight; headControlTurnAngle = float(0.5);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f),
                         Pose2f(libCodeRelease.angleToGoal,
                                theBallModel.estimate.position.x() - 150.f,
                                theBallModel.estimate.position.y() - 30.f), 1, 200); //set collision avoidance to lower value
        }
    }

    state(kick)
    {
        transition
        {
            if(state_time > 3000 || (state_time > 10 && action_done))
                goto turnToBall;
        }
        action
        {
            Annotation("Alive and Kickin'");
            theHeadControlMode = HeadControl::lookForward;
            InWalkKick(WalkRequest::left,
                       Pose2f(libCodeRelease.angleToGoal,
                              theBallModel.estimate.position.x() - 160.f,
                              theBallModel.estimate.position.y() - 55.f));
        }
    }

    state(searchForBall)
    {
        transition
        {
            if(libCodeRelease.timeSinceBallWasSeen() < 300)
                // goto headturn;
                goto turnToBall;
            else if(state_time > 10000)
				goto walkToStartPosition; //go back to radius, then face opponent direction
        }
        action
        { //first, turn towards ball, then turn in circles to look for it.
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
            WalkAtSpeedPercentage(Pose2f(0.6f, 0.f, 0.f));
            theHeadControlMode = HeadControl::lookLeftAndRight;headControlTurnAngle = float(0.5f);
        }
    }
    
    state(localizeSelf)
    {
        transition
        {
            if(theRobotPose.deviation < libCodeRelease.localizedDeviation && theRobotPose.validity > libCodeRelease.minValidity)
                goto searchForBall;
        }
        action
        {
            //first, face towards where you think the enemy is
            Vector2f relativeOpponentGroundLine =
				libCodeRelease.fieldToRobot(libCodeRelease.centerOpponentGroundLine); // transform to robot coordinates
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(relativeOpponentGroundLine.angle(), 0.f, 0.f));
            if (state_time < 2000){
                theHeadControlMode = HeadControl::lookLeftAndRight;
            }
            else {
                float turnSpeed = 0.5f; //clockwise
				if (libCodeRelease.robotToField(Vector2f(0.f, 0.f)).y() > 0)
					turnSpeed = -0.5f; //counterclockwise, to turn towards center of the field
                WalkAtSpeedPercentage(Pose2f(turnSpeed, 0.05f, 0.05f)); // add some x and y to prevent getting stuck in local minima
                theHeadControlMode = HeadControl::lookLeftAndRight;
            }
        }
    }
}
