#include "ofxDropdownOption.hpp"
#include "ofGraphics.h"
using namespace std;

ofxDropdownOption::ofxDropdownOption(ofParameter<bool> _bVal, float width, float height){
	setup(_bVal,width,height);
}

ofxDropdownOption::~ofxDropdownOption(){
	value.removeListener(this,&ofxDropdownOption::valueChanged);
}

ofxDropdownOption * ofxDropdownOption::setup(ofParameter<bool> _bVal, float width, float height){
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	value.makeReferenceTo(_bVal);
//	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	value.addListener(this,&ofxDropdownOption::valueChanged);
	registerMouseEvents();
	setNeedsRedraw();

	return this;

}

ofxDropdownOption * ofxDropdownOption::setup(const std::string& toggleName, bool _bVal, float width, float height){
	value.set(toggleName,_bVal);
	return setup(value,width,height);
}


bool ofxDropdownOption::mouseMoved(ofMouseEventArgs & args){
	if(isGuiDrawing() && b.inside(args)){
		bIsOver = true;
//		return true;
	}else{
		bIsOver = false;
	}
	return false;
}

bool ofxDropdownOption::mousePressed(ofMouseEventArgs & args){
	if(setValue(args.x, args.y, true)){
		bIsOver = true;
		return true;
	}else{
		bIsOver = false;
		return false;
	}
}

bool ofxDropdownOption::mouseDragged(ofMouseEventArgs & args){
	if(bGuiActive && b.inside(args)){
		bIsOver = true;
		return true;
	}else{
		bIsOver = false;
		return false;
	}
}

bool ofxDropdownOption::mouseReleased(ofMouseEventArgs & args){
	bool wasGuiActive = bGuiActive;
	bGuiActive = false;
	if(wasGuiActive && b.inside(args)){
		bIsOver = true;
		return true;
	}else{
		bIsOver = false;
		return false;
	}
}

void ofxDropdownOption::generateDraw(){
	bg.clear();
	bg.rectangle(b);
	generateNameTextMesh(b);
}

void ofxDropdownOption::generateNameTextMesh(const ofRectangle& rect)
{
	std::string name;
	auto textX = rect.x + textPadding;//+ checkboxRect.width;
	if(getTextBoundingBox(getName(), textX, 0).getMaxX() > rect.getMaxX() - textPadding){
		for(auto c: ofUTF8Iterator(getName())){
			auto next = name;
			ofUTF8Append(next, c);
			if(getTextBoundingBox(next,textX,0).getMaxX() > rect.getMaxX() - textPadding){
				break;
			}else{
				name = next;
			}
		}
	}else{
		name = getName();
	}

	textMesh = getTextMesh(name, textX, getTextVCenteredInRect(rect));
}

void ofxDropdownOption::render(){
	if(value)
	{
		bg.setFillColor(thisFillColor);
	}
	else
	{
		if(bIsOver)
		{
			bg.setFillColor(thisBorderColor);
		}
		else
		{
			bg.setFillColor(thisBackgroundColor);
		}
	}
	bg.draw();
//	fg.draw();

//	if( value ){
//		cross.draw();
//	}

	ofColor c = ofGetStyle().color;
	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(thisTextColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

//bool ofxDropdownOption::operator=(bool v){
//	value = v;
//	return v;
//}
//
//ofxDropdownOption::operator const bool & (){
//	return value;
//}


ofAbstractParameter & ofxDropdownOption::getParameter(){
	return value;
}

void ofxDropdownOption::valueChanged(bool & value){
    setNeedsRedraw();
}

void ofxDropdownOption::enableElement() {
//	bGuiActive = false;
	value.setWithoutEventNotifications(true);
	setNeedsRedraw();
}

void ofxDropdownOption::disableElement() {
    bGuiActive = false;
    value.setWithoutEventNotifications(false);
    setNeedsRedraw();
}

void ofxDropdownOption::select()
{
    value.setWithoutEventNotifications(true);
    setNeedsRedraw();
}

void ofxDropdownOption::deselect()
{
//	cout << "ofxDropdownOption::deselect " << getName() << "\n";
    value.setWithoutEventNotifications(false);
    setNeedsRedraw();
}



//
//void ofxDropdownOption::setState(State state)
//{
//	_state = state;
//	setNeedsRedraw();
//}


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
