option(StandRobot)
{
    initial_state(start)
    {
        transition
        {
            if(state_time > 1000)
                goto standRobot;
        }
        action
        { 
            theHeadControlMode = HeadControl::lookForward;
            Stand();
        }
    }

    state(standRobot)
    {
        action
        {
            theHeadControlMode = HeadControl::lookForward;
            Stand();
        }
    }
}