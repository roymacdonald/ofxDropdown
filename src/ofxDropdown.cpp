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


ofParameter<bool> ofxDropdown::bCollapseOnSelection = ofParameter<bool>("Collapse On Selection", false);
ofParameter<bool> ofxDropdown::bMultiselection = ofParameter<bool>("Multiselection",false);


//--------------------------------------------------------------
ofxDropdown::ofxDropdown(std::string name, float width, float height){
	return setup(name, width, height);
}
//--------------------------------------------------------------
ofxDropdown * ofxDropdown::setup(std::string name, float width , float height ){

	ofxToggle::setup(name, false , width, height);
	buttonListener = value.newListener(this,&ofxDropdown::buttonClicked);

	selectedValue.setName(name);
	group.setup();
	group.disableHeader();
	group.setParent(this);
	group.unregisterMouseEvents();

	return this;
}
//--------------------------------------------------------------
ofxDropdown::ofxDropdown(ofParameter<std::string> param, float width , float height){
	setup(param,width,height);
}
//--------------------------------------------------------------
ofxDropdown * ofxDropdown::setup(ofParameter<std::string> param, float width, float height){
	selectedValue.makeReferenceTo(param);
	
	return setup(param.getName(), width, height);

}
//--------------------------------------------------------------

void ofxDropdown::groupChanged(const void * sender,bool& b){
	if(b){
	std::cout << __FUNCTION__ << std::endl;
	if(sender){
		if(!bGroupEnabled){
			std::cout << "ofxDropdown::groupChanged(...) bGroupEnabled == false" << std::endl; 
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
				if(!bMultiselection){
					disableSiblings(&group, group.getControl(foundIndex));
				}
//				for(int i = 0; i <g.size(); i++){
//					if(foundIndex != i){
//						auto * dd = dynamic_cast <ofxDropdown *>(group.getControl(i));
//						if(dd){
//							dd->hideDropdown();
//						}else{
//
//							disableElement(dynamic_cast <ofxToggle *>(group.getControl(i)));
//						}
//					}
//				}
				auto newVal = g.getVoid(foundIndex).getName();
				selectedValue = newVal;
				ofNotifyEvent(change_E, newVal, this);
				std::cout << "ofxDropdown::groupChanged(...) sender " << newVal << std::endl;		
			}
			if(bCollapseOnSelection){
				hideDropdown("groupChanged");
			}
			
		}
	}else{
		std::cout << "ofxDropdown::groupChanged(...) sender = null" << std::endl;
	}

}

}
//--------------------------------------------------------------
ofxDropdown * ofxDropdown::add(const std::string& option){
	options.push_back(option);

	auto o = new ofxToggle();
	o->setup(option, false);
	groupListeners.push(o->getParameter().cast<bool>().newListener(this, &ofxDropdown::groupChanged));
	
	group.add(o);
	return this;
}
//--------------------------------------------------------------
ofxDropdown * ofxDropdown::add(const std::vector<std::string> & options){
	for(auto& option: options){
		add(option);
	}
	return this;
}
//--------------------------------------------------------------
ofxDropdown * ofxDropdown::addDropdown(ofxDropdown* dd){
	if(dd){
		group.add(dd);
		childDropdowns.push_back(dd);
		childDropdownListeners.push(dd->dropdownHidden_E.newListener(this, &ofxDropdown::childDropdownHidden));
	}else{
		ofLogWarning("ofxDropdown::addDropdown", "cant add nullptr dropdown");
	}
	return dd;
}
//--------------------------------------------------------------
ofxDropdown * ofxDropdown::newDropdown(std::string name){
	return addDropdown(new ofxDropdown(name, getWidth(), getHeight()));	
}
//--------------------------------------------------------------
ofxDropdown * ofxDropdown::newDropdown(ofParameter<std::string> param){
	return addDropdown(new ofxDropdown(param, getWidth(), getHeight()));
}
//--------------------------------------------------------------
void ofxDropdown::childDropdownHidden(const void * sender, std::string& s){
	std::cout << "childDropdownHidden callback: "<< getName() << ". eventParam: " << s << std::endl;
//	auto& g = group.getParameter().castGroup(); 
	
//	int foundIndex = -1; 
//	for(auto * c: childDropdowns){
//		if(c == ((ofxDropdown *)(sender))){
//			
//		}
//	}

//	if(childDropdowns.size() == 1){
		hideDropdown("childDropdownHidden");
//	}
}
//--------------------------------------------------------------
void ofxDropdown::clear(){
	group.clear();
	childDropdowns.clear();
}
//--------------------------------------------------------------
std::string ofxDropdown::getOptionAt(size_t index){
	if(index < group.getNumControls()){
		return group.getControl(index)->getName();
	}
	ofLogNotice("ofxDropdown::getOptionAt", "index is out of bounds");
	return "";
}
//--------------------------------------------------------------
void ofxDropdown::showDropdown(bool bDisableSiblings){
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
void ofxDropdown::disableSiblings(ofxBaseGui* parent, ofxBaseGui * child){
	if(parent != nullptr){
		auto* p = dynamic_cast<ofxGuiGroup *>(parent);
		if(p){
			for(int i = 0; i < p->getNumControls(); i++ ){
				auto * dd = dynamic_cast <ofxDropdown *>(p->getControl(i));
				if(dd && dd != child){
					dd->hideDropdown("disableSiblings",false);
				}else{
					if(child != p->getControl(i)){
						disableElement(dynamic_cast <ofxToggle *>(p->getControl(i)), true);
					}
				}
			}
		}
	}
}
//--------------------------------------------------------------
template<typename T>
void ofxDropdown::disableElement(T* e, bool bCheckAgainstThis){
	if(e){
		if(bCheckAgainstThis && e == this){
			return;
		}
		e->bGuiActive = false;
		e->value.setWithoutEventNotifications(false);
		e->setNeedsRedraw();
	}
}
//--------------------------------------------------------------
bool ofxDropdown::mouseReleased(ofMouseEventArgs & args){
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
bool ofxDropdown::mousePressed(ofMouseEventArgs & args){
	if(ofxToggle::mousePressed(args)){
		return true;
	}
	if(isShowingDropdown()){
		return group.mousePressed(args);
	}
	return false;
}
//--------------------------------------------------------------
bool ofxDropdown::mouseMoved(ofMouseEventArgs & args){
	if(ofxToggle::mouseMoved(args)){
		return true;
	}
	if(isShowingDropdown()){
		return group.mouseMoved(args);
	}
	return false;
}
//--------------------------------------------------------------
bool ofxDropdown::mouseDragged(ofMouseEventArgs & args){
	if(ofxToggle::mouseDragged(args)){
		return true;
	}
	if(isShowingDropdown()){
		return group.mouseDragged(args);
	}
	return false;
}
//--------------------------------------------------------------
bool ofxDropdown::mouseScrolled(ofMouseEventArgs & args){
	if(ofxToggle::mouseScrolled(args)){
		return true;
	}
	if(isShowingDropdown()){
		return group.mouseScrolled(args);
	}
	return false;
}

//--------------------------------------------------------------
void ofxDropdown::hideDropdown(std::string caller, bool bNotifyEvent){
	if(bGroupEnabled){
		std::cout << caller << "  hiding dropdown: "<< getName() << ". notify: " << (bNotifyEvent?"TRUE":"FALSE") << std::endl;
		bGroupEnabled = false;
		auto n = value.getName();
		disableElement(this);
		if(bNotifyEvent){
			ofNotifyEvent(dropdownHidden_E, n, this);
		}
	}
}
//--------------------------------------------------------------
bool ofxDropdown::isShowingDropdown(){
	return bGroupEnabled;
}
//--------------------------------------------------------------

void ofxDropdown::buttonClicked(bool &v){
	if(v && !bGroupEnabled){
		showDropdown();
	}else if(!v && bGroupEnabled){
		hideDropdown("buttonClicked", false);
	}
}
//--------------------------------------------------------------
void ofxDropdown::setDropDownPosition(DropDownPosition pos){
	dropDownPosition = pos;
}
//--------------------------------------------------------------
void ofxDropdown::generateDraw(){
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
}
//--------------------------------------------------------------
void ofxDropdown::render(){
	ofxToggle::render();
	if(bGroupEnabled){
		group.draw();
	}
	arrow.draw();
}
//--------------------------------------------------------------
ofAbstractParameter & ofxDropdown::getParameter(){
	return selectedValue;
}

//--------------------------------------------------------------
void ofxDropdown::enableCollapseOnSelection(){
	bCollapseOnSelection = true;
}
//--------------------------------------------------------------
void ofxDropdown::disableCollapseOnSelection(){
	bCollapseOnSelection = false;
}
//--------------------------------------------------------------
bool ofxDropdown::isEnabledCollapseOnSelection(){
	return bCollapseOnSelection;
}
//--------------------------------------------------------------
void ofxDropdown::enableMultipleSelection(){
	bMultiselection = true;
}
//--------------------------------------------------------------
void ofxDropdown::disableMultipleSelection(){
	bMultiselection = false;
}
//--------------------------------------------------------------
bool ofxDropdown::isEnabledMultipleSelection(){
	return bMultiselection;
}
