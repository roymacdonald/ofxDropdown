#include "ofxDropdownOption.hpp"

void ofxDropdownOption::disableElement() {
    bGuiActive = false;
    value.setWithoutEventNotifications(false);
    setNeedsRedraw();
}

bool ofxDropdownOption::setValue(float mx, float my, bool bCheck){
    
    if( !isGuiDrawing() ){
        bGuiActive = false;
        return false;
    }
    if( bCheck ){
        if( b.inside(mx, my) ){
            bGuiActive = true;
        }else{
            bGuiActive = false;
            
        }
    }
    if( bGuiActive ){
        value =  !value;
        return true;
    }
    return false;
}
