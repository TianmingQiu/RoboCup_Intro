/**
* Sends an annotation.
* @param annotation The annotation.
*/
option(Annotation, (const std::string&) annotation, (float) (1.56) value)
{
  initial_state(send)
  {
    transition
    {
      if(state_time)
        goto waitForNewAnnotation;
    }
    action
    {
        // convert value to a string
        std::ostringstream ss;  
        ss << value;
        std::string s(ss.str());
        // add it to the annotation
      ANNOTATION("Behavior:" , annotation + s);
      lastAnnotationSend = annotation;
    }
  }

  target_state(waitForNewAnnotation)
  {
    transition
    {
      if(annotation != lastAnnotationSend)
        goto send;
    }
  }
}

std::string lastAnnotationSend;
