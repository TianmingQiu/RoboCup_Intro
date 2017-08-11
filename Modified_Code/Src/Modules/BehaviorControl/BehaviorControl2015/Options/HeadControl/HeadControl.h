option(HeadControl)
{
  common_transition
  {
    if(!theGroundContactState.contact && theGameInfo.state != STATE_INITIAL)
      goto lookForward;

    switch(theHeadControlMode)
    {
      case HeadControl::off: goto off;
      case HeadControl::lookForward: goto lookForward;
	  case HeadControl::lookLeftAndRight: goto lookLeftAndRight;
      default: goto none;
    }
  }

  initial_state(none) {}
  state(off) {action SetHeadPanTilt(JointAngles::off, JointAngles::off, 0.f);}
  state(lookForward) {action LookForward();}
  state(lookLeftAndRight) {
      action LookLeftAndRight(headControlTurnAngle);
      headControlTurnAngle = (float) 1.56; //reset to default after, or you will cause confusion if people forget to reset it
    }
}

struct HeadControl
{
  ENUM(Mode,
  {,
    none,
    off,
    lookForward,
	lookLeftAndRight,
  });
};

HeadControl::Mode theHeadControlMode = HeadControl::Mode::none; /**< The head control mode executed by the option HeadControl. */
float headControlTurnAngle = float(1.56); //maximum turn angle of the head, set to Pi/2

