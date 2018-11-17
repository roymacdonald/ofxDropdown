#pragma once

#include "ofxToggle.h"

class ofxDropdownOption : public ofxToggle {
  
public:
    void disableElement();
    
protected:
    bool setValue(float mx, float my, bool bCheck);
    
};
