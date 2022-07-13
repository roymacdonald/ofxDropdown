#pragma once

#include "ofxToggle.h"
#include "ofImage.h"
class ofxDropdownOption : public ofxBaseGui{
  
public:
	ofxDropdownOption(){}
	virtual ~ofxDropdownOption();
	
	void enableElement();
    void disableElement();

	ofxDropdownOption(ofParameter<bool> _bVal, float width = defaultWidth, float height = defaultHeight);
	ofxDropdownOption * setup(ofParameter<bool> _bVal, float width = defaultWidth, float height = defaultHeight);
	ofxDropdownOption * setup(const std::string& toggleName, bool _bVal, float width = defaultWidth, float height = defaultHeight);
	

	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);
	virtual bool mouseReleased(ofMouseEventArgs & args); 
	virtual void mouseEntered(ofMouseEventArgs&) {
	}
	virtual void mouseExited(ofMouseEventArgs&) {
	}
	virtual bool mouseScrolled(ofMouseEventArgs & args){return false;}
	

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}

	void select();
	
	virtual void deselect();

//	bool operator=(bool v);
//	operator const bool & ();

	virtual ofAbstractParameter & getParameter();

    
    void setUseTexture(bool useTexture);
    bool isUseTexture();
    
    void setTexture(ofTexture& tex);
    
    void setRenderName(bool bRender);
    bool isRenderingName();
    
    bool loadTexture(const std::string& filepath);
    bool saveTexture(const std::string& filepath);
protected:
	void generateNameTextMesh(const ofRectangle& rect);
	
	
//	enum State {
//		UNSELECTED=0,
//		OVER,
//		SELECTED
//	}_state;
//
//	void setState(State state);
//
	virtual void render();
//	ofRectangle checkboxRect;
	ofParameter<bool> value;
	bool bGuiActive;
	bool bIsOver = false;
	
	bool setValue(float mx, float my, bool bCheck);
	void generateDraw();
	void valueChanged(bool & value);
	ofPath bg;//,fg;//,cross;
	ofVboMesh textMesh;
    
    ofImage texture;
    bool _bUseTexture = false;
    bool _bRenderName = true;
};
