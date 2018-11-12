////
////  ofxDropdown.hpp
////  dropdown
////
////  Created by Roy Macdonald on 10/14/18.
////
////
//
#pragma once


#include "ofParameter.h"
#include "ofxBaseGui.h"

#include "ofxToggle.h"

#include "ofxGuiGroup.h"


#define USAR_MOUSE_RELEASED

class ofxDropdown: public ofxToggle{

public:
	ofxDropdown(){};
	ofxDropdown(std::string name, float width = defaultWidth, float height = defaultHeight);
	ofxDropdown(ofParameter<std::string> param, float width = defaultWidth, float height = defaultHeight);
	ofxDropdown * setup(ofParameter<std::string> param, float width = defaultWidth, float height = defaultHeight);
	ofxDropdown * setup(std::string name, float width = defaultWidth, float height = defaultHeight);
//	ofxDropdown * setup(const std::string& name, const std::vector<std::string>& dropDownOptions, float width = defaultWidth, float height = defaultHeight);
	
	void enableCollapseOnSelection();
	void disableCollapseOnSelection();
	bool isEnabledCollapseOnSelection();
	
	void enableMultipleSelection();
	void disableMultipleSelection();
	bool isEnabledMultipleSelection();
	
	ofxDropdown * add(const std::string& option);
	ofxDropdown * add(const std::vector<std::string> & options);
	ofxDropdown * addDropdown(ofxDropdown* dd); 
	ofxDropdown * newDropdown(std::string name); 
	ofxDropdown * newDropdown(ofParameter<std::string> param);
	

	virtual bool mouseReleased(ofMouseEventArgs & args) override;

	virtual bool mousePressed(ofMouseEventArgs & args) override;

	virtual bool mouseMoved(ofMouseEventArgs & args) override;
	
	virtual bool mouseDragged(ofMouseEventArgs & args) override;
	
	virtual bool mouseScrolled(ofMouseEventArgs & args) override;
	
	
	bool isShowingDropdown();
		
	void clear();
	
	std::string getOptionAt(size_t index);
	
	enum DropDownPosition{
		DD_RIGHT,
		DD_BELOW,
		DD_ABOVE,
		DD_LEFT	
	}; 
	void setDropDownPosition(DropDownPosition pos);
	
	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		selectedValue.addListener(listener,method);
	}
	
	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		selectedValue.removeListener(listener,method);
	}
		
	virtual ofAbstractParameter & getParameter() override;
	
	ofEvent<std::string> change_E;
	ofEvent<std::string> dropdownHidden_E;
	
	
	void showDropdown(bool bDisableSiblings = true);
	void hideDropdown(std::string caller, bool bNotifyEvent = true);
	
	static ofParameter<bool> bCollapseOnSelection;// = false;
	static ofParameter<bool> bMultiselection;// = false;
	
	
protected:
	
	
	
	template<typename T>
	void disableElement(T* e, bool bCheckAgainstThis = false);
	
	int myMouseEventsPriority;
	
	void disableSiblings(ofxBaseGui* parent, ofxBaseGui * child);
	
	DropDownPosition dropDownPosition = DD_RIGHT;
	
	virtual void generateDraw() override;
		
	virtual void render() override;

	std::vector<std::string> options;
	
	ofParameter<std::string> selectedValue;
	
	

	void groupChanged( const void * sender,bool&);
	bool bGroupEnabled = false;
	
	void valueChanged(std::string & value);		

	void buttonClicked(bool &);
	
	void childDropdownHidden(const void * sender, std::string&);
	
	ofxGuiGroup group;
	ofEventListeners groupListeners;
	ofEventListeners childDropdownListeners;
	
	ofEventListener buttonListener;
	ofPath arrow;
private:
	
	std::vector<ofxDropdown*> childDropdowns;
    
};
