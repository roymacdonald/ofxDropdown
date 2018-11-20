//
//  ofxDropdown.cpp
//  dropdown
//
//  Created by Roy Macdonald on 10/14/18.
//
//


#include "ofxDropdown.h"
#include "ofGraphics.h"
#include "ofxGui.h"

template<class T>
ofParameter<bool> ofxDropdown_<T>::bCollapseOnSelection = ofParameter<bool>("Collapse On Selection", false);

//--------------------------------------------------------------
template<class T>
ofxDropdown_<T>::ofxDropdown_(std::string name, float width, float height){
	return setup(name, width, height);
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::setup(std::string name, float width , float height ){

	ofxToggle::setup(name, false , width, height);
	buttonListener = value.newListener(this,&ofxDropdown_::buttonClicked);

	selectedValue.setName(name);
    selectedValues.setName(name);
	group.setup();
	group.disableHeader();
	group.setParent(this);
	group.unregisterMouseEvents();

	return this;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T>::ofxDropdown_(ofParameter<T> param, float width , float height){
	setup(param,width,height);
}
template<class T>
ofxDropdown_<T>::ofxDropdown_(ofParameter<T> param, const std::map<T,std::string>& dropDownOptions, float width , float height){
    setup(param,width,height);
    add(dropDownOptions);
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T>::ofxDropdown_(ofParameter<std::vector<T>> param, float width , float height){
    setup(param,width,height);
}
template<class T>
ofxDropdown_<T>::ofxDropdown_(ofParameter<std::vector<T>> param, const std::map<T,std::string>& dropDownOptions, float width , float height){
    setup(param,width,height);
    add(dropDownOptions);
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::setup(ofParameter<T> param, float width, float height){
	selectedValue.makeReferenceTo(param);
    bMultiselection = false;
	return setup(param.getName(), width, height);
}
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::setup(ofParameter<std::vector<T>> param, float width, float height){
    selectedValues.makeReferenceTo(param);
    bMultiselection = true;
    return setup(param.getName(), width, height);
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::groupChanged(const void * sender,bool& b){
    int index = -1;
    auto& g = group.getParameter().castGroup();
    if(sender){
        if(!bGroupEnabled){
            std::cout << "ofxDropdown_::groupChanged(...) bGroupEnabled == false" << std::endl;
        }else{
            for(int i = 0; i <g.size(); i++){
                //            std::cout <<  i  << "  -  ";
                if(g.getBool(i).getInternalObject() == ((ofParameter<bool> *)(sender))->getInternalObject()){
                    index = i;
                    break;
                }
            }
        }
    }else{
        std::cout << "ofxDropdown_::groupChanged(...) sender = null" << std::endl;
    }
    if(index >= 0){
        if(b){
            if (bMultiselection) {
                std::vector<T> newValues = selectedValues.get();
                newValues.push_back(values[index]);
                selectedValues.set(newValues);
            }
            else {
                disableSiblings(&group, group.getControl(index));
                selectedValue = values[index];
            }
            //std::cout << "ofxDropdown_::groupChanged(...) sender " << selectedValue << std::endl;
            if(bCollapseOnSelection){
                hideDropdown("groupChanged");
            }
            
        } else if (bMultiselection) {
            std::vector<T> newValues = selectedValues.get();
            newValues.erase(std::remove(newValues.begin(), newValues.end(), values[index]), newValues.end());
            selectedValues.set(newValues);
        }
        ofNotifyEvent(change_E, options[index], this);
    }

}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const T& value) {
    return add(value, ofToString(value));
}
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const T& value, const std::string& option) {
    options.push_back(option);
    values.push_back(value);
    
    auto o = new ofxDropdownOption();
    if (bMultiselection) {
        const std::vector<T> &selValues = selectedValues.get();
        o->setup(option, std::find(selValues.begin(), selValues.end(), value) != selValues.end());
    }
    else {
        o->setup(option, value == selectedValue.get());
    }
    groupListeners.push(o->getParameter().cast<bool>().newListener(this, &ofxDropdown_::groupChanged));
    
    group.add(o);
    return this;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const std::vector<T> & options){
	for(auto& option: options){
		add(option);
	}
	return this;
}
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const std::map<T,std::string> & options){
    for(auto& option: options){
        add(option.first, option.second);
    }
    return this;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::addDropdown(ofxDropdown_* dd){
	if(dd){
		group.add(dd);
		childDropdowns.push_back(dd);
		childDropdownListeners.push(dd->dropdownHidden_E.newListener(this, &ofxDropdown_::childDropdownHidden));
	}else{
		ofLogWarning("ofxDropdown_::addDropdown", "cant add nullptr dropdown");
	}
	return dd;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::newDropdown(std::string name){
	return addDropdown(new ofxDropdown_(name, getWidth(), getHeight()));
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::newDropdown(ofParameter<T> param){
	return addDropdown(new ofxDropdown_(param, getWidth(), getHeight()));
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::childDropdownHidden(const void * sender, std::string& s){
	std::cout << "childDropdownHidden callback: "<< getName() << ". eventParam: " << s << std::endl;
//	auto& g = group.getParameter().castGroup(); 
	
//	int foundIndex = -1; 
//	for(auto * c: childDropdowns){
//		if(c == ((ofxDropdown_ *)(sender))){
//			
//		}
//	}

//	if(childDropdowns.size() == 1){
		hideDropdown("childDropdownHidden");
//	}
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::clear(){
	group.clear();
	childDropdowns.clear();
}
//--------------------------------------------------------------
template<class T>
std::string ofxDropdown_<T>::getSelectedOption(){
    std::string selectedOption = "";
    if (bMultiselection) {
        const std::vector<T> selValues = selectedValues.get();
        for (int i=0; i<selValues.size(); i++) {
            auto it = std::find(values.begin(), values.end(), selValues[i]);
            if (it != values.end()) {
                int index = std::distance(values.begin(), it);
                selectedOption += (selectedOption.size() > 0 ? ","  : "") + options[index];
            }
        }
    }
    else {
        auto it = std::find(values.begin(), values.end(), selectedValue.get());
        if (it != values.end()) {
            int index = std::distance(values.begin(), it);
            selectedOption = options[index];
        }
    }
    return selectedOption;
}
//--------------------------------------------------------------
template<class T>
std::string ofxDropdown_<T>::getOptionAt(size_t index){
	if(index < group.getNumControls()){
		return group.getControl(index)->getName();
	}
	ofLogNotice("ofxDropdown_::getOptionAt", "index is out of bounds");
	return "";
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::showDropdown(bool bDisableSiblings){
	if(!bGroupEnabled){
		bGroupEnabled = true;
		switch(dropDownPosition){
			case DD_BELOW:
				group.setPosition(b.x, b.getMaxY());	
				break;
			case DD_ABOVE:
				group.setPosition(b.x, b.y - group.getShape().height);
				break;
			case DD_RIGHT:
				group.setPosition(b.getMaxX(), b.y);
				break;
			case DD_LEFT:
				group.setPosition(b.x - group.getShape().width, b.y);
				break;
		}
		if(bDisableSiblings){			
			disableSiblings(getParent(), this);
		}
	}
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableSiblings(ofxBaseGui* parent, ofxBaseGui * child){
	if(parent != nullptr){
		auto* p = dynamic_cast<ofxGuiGroup *>(parent);
		if(p){
			for(int i = 0; i < p->getNumControls(); i++ ){
				auto * dd = dynamic_cast <ofxDropdown_ *>(p->getControl(i));
				if(dd && dd != child){
					dd->hideDropdown("disableSiblings",false);
				}else{
					if(child != p->getControl(i)){
						disableElement(dynamic_cast <ofxDropdownOption *>(p->getControl(i)), true);
					}
				}
			}
		}
	}
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableElement(ofxDropdownOption* e, bool bCheckAgainstThis){
	if(e){
		if(bCheckAgainstThis && e == this){
			return;
		}
        e->disableElement();
	}
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mouseReleased(ofMouseEventArgs & args){
	if(ofxToggle::mouseReleased(args) || b.inside(args)){
		return true;
	}
	if(isShowingDropdown()){
		if(group.mouseReleased(args)){
			return true;
		}
		if(!b.inside(args)){
			hideDropdown("mousePressed");
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mousePressed(ofMouseEventArgs & args){
    if(setValue(args.x, args.y, true)){
        return true;
    }
	if(isShowingDropdown()){
		return group.mousePressed(args);
	}
	return false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mouseMoved(ofMouseEventArgs & args){
	if(ofxToggle::mouseMoved(args)){
		return true;
	}
	if(isShowingDropdown()){
		return group.mouseMoved(args);
	}
	return false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mouseDragged(ofMouseEventArgs & args){
	if(ofxToggle::mouseDragged(args)){
		return true;
	}
	if(isShowingDropdown()){
		return group.mouseDragged(args);
	}
	return false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mouseScrolled(ofMouseEventArgs & args){
	if(ofxToggle::mouseScrolled(args)){
		return true;
	}
	if(isShowingDropdown()){
		return group.mouseScrolled(args);
	}
	return false;
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::hideDropdown(std::string caller, bool bNotifyEvent){
	if(bGroupEnabled){
		//std::cout << caller << "  hiding dropdown: "<< getName() << ". notify: " << (bNotifyEvent?"TRUE":"FALSE") << std::endl;
		bGroupEnabled = false;
		auto n = value.getName();
		disableElement(this);
		if(bNotifyEvent){
			ofNotifyEvent(dropdownHidden_E, n, this);
		}
	}
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::isShowingDropdown(){
	return bGroupEnabled;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::buttonClicked(bool &v){
	if(v && !bGroupEnabled){
		showDropdown();
	}else if(!v && bGroupEnabled){
		hideDropdown("buttonClicked", false);
	}
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::setDropDownPosition(DropDownPosition pos){
	dropDownPosition = pos;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::generateDraw(){
    bg.clear();
    bg.setFillColor(thisBackgroundColor);
    bg.rectangle(b);
    
    std::string name;
    auto textX = b.x + textPadding;
    if(getTextBoundingBox(getName(), textX, 0).getMaxX() > b.getMaxX() - textPadding){
        for(auto c: ofUTF8Iterator(getName())){
            auto next = name;
            ofUTF8Append(next, c);
            if(getTextBoundingBox(next,textX,0).getMaxX() > b.getMaxX() - textPadding){
                break;
            }else{
                name = next;
            }
        }
    }else{
        name = getName();
    }
    
    textMesh = getTextMesh(name, textX, b.y+b.height / 2 + 4);
    
    std::string option;
    auto optionX = getTextBoundingBox(name,textX,0).getMaxX() + textPadding;
    if(getTextBoundingBox(getSelectedOption(), optionX, 0).getMaxX() > b.getMaxX() - b.getHeight()/2){
        for(auto c: ofUTF8Iterator(getSelectedOption())){
            auto next = option;
            ofUTF8Append(next, c);
            if(getTextBoundingBox(next,optionX,0).getMaxX() > b.getMaxX() - b.getHeight()/2){
                break;
            }else{
                option = next;
            }
        }
    }else{
        option = getSelectedOption();
    }
    
    optionMesh = getTextMesh(option, optionX, b.y+b.height / 2 + 4);
    
    arrow.clear();
    auto h = b.getHeight();
    auto x2 = b.getMaxX();
    
    auto y = b.getY();
    
    arrow.setStrokeColor(thisTextColor);
    arrow.setStrokeWidth(2);
    arrow.setFilled(false);
    arrow.moveTo(x2 - h /2,  y  + textPadding);
    arrow.lineTo(x2 - textPadding,  y  + h/2 );
    arrow.lineTo(x2 - h /2,  y  + h - textPadding);
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::render(){
    bg.draw();
    fg.draw();
    
    if( value ){
        cross.draw();
    }
    renderText(textMesh, thisTextColor);
	if(bGroupEnabled){
		group.draw();
	}
	arrow.draw();
    renderText(optionMesh, ofColor(127));
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::renderText(const ofVboMesh &mesh, const ofColor &color) {
    ofColor c = ofGetStyle().color;
    ofBlendMode blendMode = ofGetStyle().blendingMode;
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableAlphaBlending();
    }
    ofSetColor(color);
    
    bindFontTexture();
    mesh.draw();
    unbindFontTexture();
    
    ofSetColor(c);
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableBlendMode(blendMode);
    }
}
//--------------------------------------------------------------
template<class T>
ofAbstractParameter & ofxDropdown_<T>::getParameter(){
    if (bMultiselection) {
        return selectedValues;
    }
	return selectedValue;
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::enableCollapseOnSelection(){
	bCollapseOnSelection = true;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableCollapseOnSelection(){
	bCollapseOnSelection = false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::isEnabledCollapseOnSelection(){
	return bCollapseOnSelection;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::setValue(float mx, float my, bool bCheck){
    
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
        value = !value;
        return true;
    }
    return false;
}


template class ofxDropdown_<std::string>;
template class ofxDropdown_<int>;
