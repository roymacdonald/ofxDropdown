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
#include "ofxGuiGroup.h"

#include "ofxDropdownOption.hpp"


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
//	std::ofxDropdown_ * setup(const std::string& name, const std::vector<std::string>& dropDownOptions, float width = defaultWidth, float height = defaultHeight);
	
	void enableCollapseOnSelection(bool bPropagateToChildren = true);
	void disableCollapseOnSelection(bool bPropagateToChildren = true);
	bool isEnabledCollapseOnSelection();
	ofParameter<bool> & getCollapseOnSelectionParameter();//this can be added to a gui
	
	
	void enableMultipleSelection(bool bPropagateToChildren = true);
	void disableMultipleSelection(bool bPropagateToChildren = true);
	bool isEnabledMultipleSelection();
	ofParameter<bool> & getMultiSelectionParameter();//this can be added to a gui
	
	template<typename P>
	typename std::enable_if<std::is_same<P, ofFile>::value, ofxDropdown_ *>::type
	add(const P& value){
		return add(value, value.getBaseName());
	}
	template<typename P>
    typename std::enable_if<!std::is_same<P, ofFile>::value, ofxDropdown_ *>::type
	add(const P& value){  
	    return add(value, ofToString(value));
	}
    ofxDropdown_ * add(const T& value, const string& option);
	ofxDropdown_ * add(const std::vector<T> & options);
    ofxDropdown_ * add(const std::map<T, std::string> & options);
	ofxDropdown_ * addDropdown(ofxDropdown_& dd);
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
    ofEvent<void> dropdownWillShow_E;
	
	void showDropdown(bool bDisableSiblings = true);
	void hideDropdown(std::string caller, bool bNotifyEvent = true);
	
    ofParameterGroup& getDropdownParameters();
	
	
protected:
	
	ofParameter<bool> bCollapseOnSelection;// = false;
	ofParameter<bool> bMultiselection;// = false;
	
    ofParameterGroup dropdownParams;
	
	virtual bool setValue(float mx, float my, bool bCheck) override;
	
	void disableElement(ofxDropdownOption* e, bool bCheckAgainstThis = false);
	
	int myMouseEventsPriority;
	
	void disableSiblings(ofxBaseGui* parent, ofxBaseGui* child);
	
	DropDownPosition dropDownPosition = DD_RIGHT;
	
	virtual void generateDraw() override;
		
	virtual void render() override;

	vector<string> options;
    vector<T> values;
	
    ofParameter<T> selectedValue;
	
	void groupChanged( const void * sender,bool&);
	bool bGroupEnabled = false;
	
//    void valueChanged(T & value);

	void buttonClicked(bool &);
	
	void childDropdownHidden(const void * sender, std::string&);
	
    
//    void setSelectedValue(T & newvalue);
    
    
	ofxGuiGroup group;
	ofEventListeners groupListeners;
	ofEventListeners childDropdownListeners;
	
	ofEventListener buttonListener;
    ofEventListener setlectedValueListener;
    
	ofPath arrow;
	ofVboMesh optionTextMesh;
	
	std::string selectedOption;
	
private:
	
	std::vector<ofxDropdown_ *> childDropdowns;
    
};

typedef ofxDropdown_<string> ofxDropdown;
typedef ofxDropdown_<int> ofxIntDropdown;
typedef ofxDropdown_<ofFile> ofxDirDropdown;
