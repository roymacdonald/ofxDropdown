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


//--------------------------------------------------------------
template<class T>
ofParameter<bool> & ofxDropdown_<T>::getMultiSelectionParameter(){
	return bMultiselection;
}
//--------------------------------------------------------------
template<class T>
ofParameter<bool> & ofxDropdown_<T>::getCollapseOnSelectionParameter(){
	return bCollapseOnSelection;
}

template<class T>
ofParameterGroup& ofxDropdown_<T>::getDropdownParameters(){
    return dropdownParams;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T>::ofxDropdown_(std::string name, float width, float height){
	setup(name, width, height);
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::setup(std::string name, float width , float height ){

	ofxToggle::setup(name, false , width, height);
	buttonListener = value.newListener(this,&ofxDropdown_::buttonClicked);

	selectedValue.setName(name);
	setlectedValueListener = selectedValue.newListener(this, &ofxDropdown_<T>::selectedValueChanged);
	
	group.setup();
	group.disableHeader();
	group.setParent(this);
	group.unregisterMouseEvents();

    dropdownParams.setName(name + " params");
	dropdownParams.add(bMultiselection.set("MultiSelection", false));
	dropdownParams.add(bCollapseOnSelection.set("Collapse On Selection", true));
	
	
	return this;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T>::ofxDropdown_(ofParameter<T> param, float width , float height){
	setup(param,width,height);
}
template<class T>
ofxDropdown_<T>::ofxDropdown_(ofParameter<T> param, const map<T,string>& dropDownOptions, float width , float height){
    setup(param,width,height);
    add(dropDownOptions);
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::setup(ofParameter<T> param, float width, float height){
	selectedValue.makeReferenceTo(param);
	return setup(param.getName(), width, height);
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::selectedValueChanged(T & newvalue){
	auto it = find(values.begin(), values.end(), newvalue);
	if(it != values.end()){// it was found. it should be found anyways but better to double check
		auto index = std::distance(values.begin(), it);
		setSelectedValueByIndex(index, true);
		
		
		auto element = dynamic_cast <ofxDropdownOption *>(group.getControl(options[index]));
		if(element){
			element->enableElement();
		}
		setNeedsRedraw();
	}
	
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::setSelectedValueByIndex( const size_t& index, bool bNotify){
	if(index < values.size()){
		selectedValue = values[index];
		selectedOption = options[index];
		if(!bMultiselection){
			auto control = group.getControl(options[index]);
			if(control != nullptr){
				disableSiblings(&group,control);
			}
		}
		if(bCollapseOnSelection){
			hideDropdown("setSelectedValueByName");
		}
		if(bNotify) ofNotifyEvent(change_E, options[index], this);
	}
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::setSelectedValueByName( const std::string& valueName, bool bNotify){

		auto it = find(options.begin(), options.end(), valueName);
		if(it != options.end()){// it was found. it should be found anyways but better to double check
			auto index = std::distance(options.begin(), it);
			setSelectedValueByIndex(index, bNotify);
		}
	
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::groupChanged(const void * sender,bool& b){
	if(b){
	if(sender){
		if(!bGroupEnabled){
			ofLogVerbose("ofxDropdown_::groupChanged(...)") << " bGroupEnabled == false";
		}else{

			auto& g = group.getParameter().castGroup(); 
					
			int foundIndex = -1;           
			for(int i = 0; i <g.size(); i++){
				//			std::cout <<  i  << "  -  ";
				if(g.getBool(i).getInternalObject() == ((ofParameter<bool> *)(sender))->getInternalObject()){
					foundIndex = i;
					break;	
				}
			}
			if(foundIndex >= 0){
				setSelectedValueByName(g.getVoid(foundIndex).getName(), true);
			}else if(bCollapseOnSelection){
				hideDropdown("groupChanged");
			}
			
		}
	}else{
		ofLogVerbose("ofxDropdown_::groupChanged(...)")  << "sender = null";
	}

}

}
//--------------------------------------------------------------
//template<class T>
//ofxDropdown_<T> * ofxDropdown_<T>::add(const T& value) {
//    return add(value, ofToString(value));
//}
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const T& value, const string& option) {
    options.push_back(option);
    values.push_back(value);
    
//    auto o = make_shared<ofxDropdownOption>();
	auto o = new ofxDropdownOption();
    o->setup(option, value == selectedValue.get());	
    groupListeners.push(o->getParameter().cast<bool>().newListener(this, &ofxDropdown_::groupChanged));
    
    group.add(o);
//	if(value == selectedValue.get()){
//		setNeedsRedraw();
//	}
    return this;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const vector<T> & options){
	for(auto& option: options){
		add(option);
	}
	return this;
}
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const map<T,string> & options){
    for(auto& option: options){
        add(option.first, option.second);
    }
    return this;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::addDropdown(ofxDropdown_& dd){
	addDropdown(&dd);
	return 0;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::addDropdown(ofxDropdown_ * dd){
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
	return addDropdown(new ofxDropdown_<T>(name, getWidth(), getHeight()));
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::newDropdown(ofParameter<T> param){
	return addDropdown(new ofxDropdown_(param, getWidth(), getHeight()));
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::childDropdownHidden(const void * sender, std::string& s){
	hideDropdown("childDropdownHidden");
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::clear(){
	group.clear();
	childDropdowns.clear();
}
//--------------------------------------------------------------
template<class T>
string ofxDropdown_<T>::getOptionAt(size_t index){
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
        ofNotifyEvent(dropdownWillShow_E, this);
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
void ofxDropdown_<T>::disableSiblings(ofxBaseGui * parent, ofxBaseGui * child){
	if(parent != nullptr){
		auto p = dynamic_cast<ofxGuiGroup*>(parent);
		if(p){
			for(int i = 0; i < p->getNumControls(); i++ ){
				auto* dd = dynamic_cast <ofxDropdown_*>(p->getControl(i));
				if(dd && dd != child){
					dd->hideDropdown("disableSiblings",false);
				}else{
					auto element = dynamic_cast <ofxDropdownOption *>(p->getControl(i));
					if(child != p->getControl(i)){
						disableElement(element, true);
					}else{
						if(element){
							element->enableElement();
						}
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
	ofxToggle::generateDraw();
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
	
	optionTextMesh = getTextMesh(selectedOption, x2 - h /2 - getTextBoundingBox(selectedOption, 0, 0).width - textPadding , getTextVCenteredInRect(b));
	
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::render(){
	ofxToggle::render();
	if(bGroupEnabled){
		group.draw();
	}
	arrow.draw();
	
	ofSetColor(thisTextColor, 200);
	
	bindFontTexture();
	optionTextMesh.draw();
	unbindFontTexture();
	
}
//--------------------------------------------------------------
template<class T>
ofAbstractParameter & ofxDropdown_<T>::getParameter(){
	return selectedValue;
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::enableCollapseOnSelection(bool bPropagateToChildren){
//	getCollapseOnSelectionParameter() = true;
	bCollapseOnSelection = true;// 
	if(bPropagateToChildren){
		for(auto& c: childDropdowns){
			c->enableCollapseOnSelection();
		}
	} 
	
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableCollapseOnSelection(bool bPropagateToChildren){
	bCollapseOnSelection = false;
//	bCollapseOnSelection = true;// 
	if(bPropagateToChildren){
		for(auto& c: childDropdowns){
			c->disableCollapseOnSelection();
		}
	} 
	
	
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::isEnabledCollapseOnSelection(){
	return bCollapseOnSelection;
//	return getCollapseOnSelectionParameter();	
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::enableMultipleSelection(bool bPropagateToChildren){
	bMultiselection = true;
	if(bPropagateToChildren){
		for(auto& c: childDropdowns){
			c->enableMultipleSelection();
		}
	} 
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableMultipleSelection(bool bPropagateToChildren){
	bMultiselection = false;
	if(bPropagateToChildren){
		for(auto& c: childDropdowns){
			c->disableMultipleSelection();
		}
	}
	//getMultiSelectionParameter() = false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::isEnabledMultipleSelection(){
	return bMultiselection;
//	return getMultiSelectionParameter();
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

//template class ofxDropdown_<ofFile>;
template class ofxDropdown_<string>;
template class ofxDropdown_<int>;
