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

#include "ofxDropdownOption.hpp"
#include "ofxDropdownOptions.hpp"


#define USAR_MOUSE_RELEASED

using namespace std;

template<class T>
class ofxDropdown_: public ofxDropdownOption{

public:
	ofxDropdown_(){};
	ofxDropdown_(std::string name, float width = defaultWidth, float height = defaultHeight);
	ofxDropdown_(ofParameter<T> param, float width = defaultWidth, float height = defaultHeight);
    ofxDropdown_(ofParameter<T> param, const map<T,string>& dropDownOptions, float width = defaultWidth, float height = defaultHeight);
	ofxDropdown_ * setup(ofParameter<T> param, float width = defaultWidth, float height = defaultHeight);
	ofxDropdown_ * setup(std::string name, float width = defaultWidth, float height = defaultHeight);
//	ofxDropdown_ * setup(const std::string& name, const std::vector<std::string>& dropDownOptions, float width = defaultWidth, float height = defaultHeight);
	
	void enableCollapseOnSelection();
	void disableCollapseOnSelection();
	bool isEnabledCollapseOnSelection();
	
	void enableMultipleSelection();
	void disableMultipleSelection();
	bool isEnabledMultipleSelection();
	
    ofxDropdown_ * add(const T& value);
    ofxDropdown_ * add(const T& value, const string& option);
	ofxDropdown_ * add(const std::vector<T> & options);
    ofxDropdown_ * add(const std::map<T, std::string> & options);
	ofxDropdown_ * addDropdown(ofxDropdown_* dd);
	ofxDropdown_ * newDropdown(std::string name);
	ofxDropdown_ * newDropdown(ofParameter<T> param);
	

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
	
	bool setValue(float mx, float my, bool bCheck);
	
	void disableElement(ofxDropdownOption* e, bool bCheckAgainstThis = false);
	
	int myMouseEventsPriority;
	
	void disableSiblings(ofxBaseGui* parent, ofxBaseGui * child);
	
	DropDownPosition dropDownPosition = DD_RIGHT;
	
	virtual void generateDraw() override;
		
	virtual void render() override;

	vector<string> options;
    vector<T> values;
	
	ofParameter<T> selectedValue;
	
	void groupChanged( const void * sender,bool&);
	bool bGroupEnabled = false;
	
	void valueChanged(T & value);

	void buttonClicked(bool &);
	
	void childDropdownHidden(const void * sender, std::string&);
	
	ofxDropdownOptions group;
	ofEventListeners groupListeners;
	ofEventListeners childDropdownListeners;
	
	ofEventListener buttonListener;
	ofPath arrow;
private:
	
	std::vector<ofxDropdown_*> childDropdowns;
    
};

typedef ofxDropdown_<string> ofxDropdown;
typedef ofxDropdown_<int> ofxIntDropdown;
