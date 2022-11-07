#include "ofxDropdownOption.hpp"
#include "ofGraphics.h"
using namespace std;

ofxDropdownOption::ofxDropdownOption(ofParameter<bool> _bVal, float width, float height){
	setup(_bVal,width,height);
}

ofxDropdownOption::~ofxDropdownOption(){

}

ofxDropdownOption * ofxDropdownOption::setup(ofParameter<bool> _bVal, float width, float height){
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	value.makeReferenceTo(_bVal);
    
    listener = value.newListener(this,&ofxDropdownOption::valueChanged);
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
    if(_bRenderName){
        generateNameTextMesh(b);
    }
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


	ofColor c = ofGetStyle().color;
	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(thisTextColor);
    if(_bUseTexture){
        texture.draw(b);
    }
    if(_bRenderName){
        bindFontTexture();
        textMesh.draw();
        unbindFontTexture();
    }
	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

void ofxDropdownOption::drawTooltip(){
    if( !bIsOver || !bTooltipsEnabled ||
       tooltipText.empty()){
           return;
       }
    float x = ofGetMouseX();
    float y = ofGetMouseY();
           if(b.inside(x, y) == false) return;
    
           
    if(x > ofGetWidth() - tooltipRect.width){
        x = ofGetWidth() - tooltipRect.width;
    }
    y = y - tooltipRect.y + 20;
    
    ofDrawBitmapStringHighlight(tooltipText, x, y , ofColor::lightYellow, ofColor::black);
    
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

void ofxDropdownOption::setUseTexture(bool useTexture){
    _bUseTexture = useTexture;
}

bool ofxDropdownOption::isUseTexture(){
    return _bUseTexture;
}
bool ofxDropdownOption::saveTexture(const string& filepath){
    if(texture.isAllocated()){
        if(texture.save(filepath)){
            return true;
        }
    }
    return false;
}

bool ofxDropdownOption::loadTexture(const string& filepath){
    if(ofFile::doesFileExist(filepath) && texture.load(filepath)){
        this->setSize(this->getWidth(), texture.getHeight());
        _bUseTexture = true;
        return true;
    }
    return false;
}


void ofxDropdownOption::setTexture(ofTexture& tex){
    if(tex.isAllocated()){
//        texture.allocate(tex.getTextureData());
        tex.readToPixels(texture.getPixels());
        texture.update();
        this->setSize(this->getWidth(), tex.getHeight());
        
        _bUseTexture = true;
    }
}

void ofxDropdownOption::setRenderName(bool bRender){
    _bRenderName = bRender;
}

bool ofxDropdownOption::isRenderingName(){
    return _bRenderName;
}

void ofxDropdownOption::enableTooltip(){
    bTooltipsEnabled = true;
}

void ofxDropdownOption::disableTooltip(){
    bTooltipsEnabled = false;
}

bool ofxDropdownOption::isTooltipEnabled(){
    return bTooltipsEnabled;
}

void ofxDropdownOption::setupTooltip(ofJson & json){
    removeTooltip();
    if(json.contains(getName())){
        setTooltipText(json[getName()]);
    }else{
        json[getName()] = "";
    }
}

void ofxDropdownOption::setTooltipText(const string& text){
    tooltipText = text;
    ofBitmapFont bf;
    tooltipRect = bf.getBoundingBox(text, 0, 0);
}
void ofxDropdownOption::removeTooltip(){
    disableTooltip();
    tooltipText = "";
    tooltipRect.set(0, 0, 0, 0);
}
