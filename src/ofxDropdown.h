//
//  ofxDropdown.hpp
//  dropdown
//
//  Created by Roy Macdonald on 10/14/18.
//
//
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
	ofxDropdown_(std::string name, float width = defaultWidth, float height = defaultHeight*2);
	ofxDropdown_(ofParameter<T> param, float width = defaultWidth, float height = defaultHeight*2);
    ofxDropdown_(ofParameter<T> param, const map<T,string>& dropDownOptions, float width = defaultWidth, float height = defaultHeight*2);
	ofxDropdown_(ofParameter<T> param, const std::vector<T> & dropDownOptions, float width = defaultWidth, float height = defaultHeight*2);
	
	ofxDropdown_ * setup(ofParameter<T> param, float width = defaultWidth, float height = defaultHeight*2);
	ofxDropdown_ * setup(std::string name, float width = defaultWidth, float height = defaultHeight*2);
	
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
		return add(value, value.getFileName());
	}
	template<typename P>
    typename std::enable_if<!std::is_same<P, ofFile>::value, ofxDropdown_ *>::type
	add(const P& value){  
	    return add(value, ofToString(value));
	}
	
	bool populateFromDirectory(const string& dirpath, const vector<string>& allowedExtensions = {})
	{
		ofFile d(dirpath);
		if(!d.isDirectory() || !d.exists())
		{
			return false;
		}
		
		addFromDir(this, dirpath, allowedExtensions);
		disableMultipleSelection();
		enableCollapseOnSelection();
		
		return true;
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
	void hideDropdown(bool bNotifyEvent = true);
	
    ofParameterGroup& getDropdownParameters();
	
	
	ofParameter<T> selectedValue;
	
	
	void registerMouseEvents() override;
	void unregisterMouseEvents() override;

	
	void forceRedraw(){
		setNeedsRedraw();
	}
	
	void setSelectedValueByName( const std::string& valueName, bool bNotify);
	void setSelectedValueByIndex( const size_t& index, bool bNotify);
	
	void setDropdownElementsWidth(float width);
	
	
	virtual void deselect() override;

	
protected:
	
	ofParameter<bool> bCollapseOnSelection = { "Multi Selection", false};
	ofParameter<bool> bMultiselection = { "Collapse On Selection", true};
	ofParameter<bool> bDisableChildrenRecursively = {"Disable Children Recursively", true};
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
	
	
	void groupChanged( const void * sender,bool&);
	bool bGroupEnabled = false;
	
	void buttonClicked(bool &);
	
	void childDropdownHidden(const void * sender, std::string&);
	
	void selectedValueChanged(T & newvalue);
	
    
	ofxGuiGroup group;
	ofEventListeners groupListeners;
	ofEventListeners childDropdownListeners;
	
	ofEventListener buttonListener;
    ofEventListener setlectedValueListener;
    
	ofPath arrow;
	ofVboMesh optionTextMesh;
	
	std::string selectedOption;
	
	void addFromDir(ofxDropdown_<T>* currentDD, const string& dirpath, const vector<string>& allowedExtensions);
	
	void _updateGroupWidth();
	
private:
	
	std::vector<ofxDropdown_ *> childDropdowns;
	std::vector<unique_ptr<ofxDropdown_>> ownedDropdowns;
	std::vector<unique_ptr<ofxDropdownOption>> ownedChildren;
    
	
	
};






typedef ofxDropdown_<string> ofxDropdown;
typedef ofxDropdown_<int> ofxIntDropdown;
typedef ofxDropdown_<ofFile> ofxDirDropdown;
