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
    virtual ~ofxDropdown_();
    // ---------------------------------------------------
    // ----------------- Constructors
    // ---------------------------------------------------
    
    /// default constructor.
    /// \warning You will need to call the one of the setup class methods to be able to use this dropdown
	ofxDropdown_(){};
    
    /// \brief constructor that calls setup function with same parameters
    /// Dropdown will be empty. you should add values with the add(...) function
    /// @param name the name of the dropdown.
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
	ofxDropdown_(std::string name, float width = defaultWidth, float height = defaultHeight*2);
    
    /// \brief constructor that calls setup function with same parameters
    /// Dropdown will be empty. You should add values with the add(...) function
    /// @param param The passed ofParameter will be "linked" to the dropdown and will change as you select different elements from the dropdown
    /// The name of this dropdown is taken from the passed ofParameter's name
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
    ofxDropdown_(ofParameter<T> param, float width = defaultWidth, float height = defaultHeight*2);
    
    /// \brief Construct and add elements
    /// @param param The passed ofParameter will be "linked" to the dropdown and will change as you select different elements from the dropdown
    /// The name of this dropdown is taken from the passed ofParameter's name
    /// \param dropDownOptions  std::map which maps  whatever type the passed ofParameter has to a string representation of it. Dropdown will be populated with it.
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
    /// This is useful when for instance you have an ofParameter<int> but you want to display different names for each value, rather than the value itself.
    /// You still can add more values with the add(...) function
    ofxDropdown_(ofParameter<T> param, const map<T,string>& dropDownOptions, float width = defaultWidth, float height = defaultHeight*2);
    
    
    /// Construct and add elements
    /// \brief Construct and add elements
    /// \param dropDownOptions  std:vector with values of the same type as the ofParameter passed. Dropdown will be populated with this values.
    /// @param param The passed ofParameter will be "linked" to the dropdown and will change as you select different elements from the dropdown
    /// The name of this dropdown is taken from the passed ofParameter's name
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
    /// This is useful when for instance you have an ofParameter<int> but you want to display different names for each value, rather than the value itself.
    /// You still can add more values with the add(...) function
	ofxDropdown_(ofParameter<T> param, const std::vector<T> & dropDownOptions, float width = defaultWidth, float height = defaultHeight*2);
	
    
    //---- Setup function.
    // Only call setup if the default constructor was used.
    /// \brief Setup this dropdonw
    /// Dropdown will be empty. You should add values with the add(...) function
    /// @param param The passed ofParameter will be "linked" to the dropdown and will change as you select different elements from the dropdown
    /// The name of this dropdown is taken from the passed ofParameter's name
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
	ofxDropdown_ * setup(ofParameter<T> param, float width = defaultWidth, float height = defaultHeight*2);
	
    /// \brief Setup this dropdonw
    /// \warning use only when you instanced this classs and used its default constructor.
    /// @param name the name of the dropdown.
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
    /// Dropdown will be empty. you should add values with the add(...) function
    ofxDropdown_ * setup(std::string name, float width = defaultWidth, float height = defaultHeight*2);
	
    
    // ---------------------------------------------------
    // ----------------- Collapse on Selection Option
    // ---------------------------------------------------
	
    /// \brief The dropdown will collapse (close) when you choose an element.
    /// \param bPropagateToChildren recursivelly call this function on the dropdown's children
	void enableCollapseOnSelection(bool bPropagateToChildren = true);
    
    /// \brief The dropdown will NOT collapse (close) when you choose an element.
    /// \param bPropagateToChildren recursivelly call this function on the dropdown's children
	void disableCollapseOnSelection(bool bPropagateToChildren = true);
    
    /// \brief Will the dropdown collapse (close)  or not when you choose an element?
    /// \return bool. true if it will collapse, false otherwise.
    /// \param bPropagateToChildren recursivelly call this function on the dropdown's children
	bool isEnabledCollapseOnSelection();
    
    /// \brief get the ofParameter that indicates if collapse on selection is enabled or not.
    /// Use this to add this option to a GUI.
	ofParameter<bool> & getCollapseOnSelectionParameter();
	
    
    // ---------------------------------------------------
    // ----------------- Multiple Selection Option
    // ---------------------------------------------------
    
    /// \brief Enable Multiple Items selection. This is mutually exclusive with collapse on selection
    /// \param bPropagateToChildren recursivelly call this function on the dropdown's children
	void enableMultipleSelection(bool bPropagateToChildren = true);
    
    /// \brief Disable Multiple Items selection. This is mutually exclusive with collapse on selection
    /// \param bPropagateToChildren recursivelly call this function on the dropdown's children
    void disableMultipleSelection(bool bPropagateToChildren = true);
    
    /// \brief get if multiple selection is enabled.
    /// \return true if multiple selection is enabled, false otherwise
	bool isEnabledMultipleSelection();
    
    /// \brief get the ofParameter that indicates if multiple selection is enabled or not.
    /// Use this to add this option to a GUI.
	ofParameter<bool> & getMultiSelectionParameter();
		

    // ---------------------------------------------------
    // ----------------- Adding/creating dropdown elements
    // ---------------------------------------------------
    
    /// \brief add a dropdown option.
    /// This will create a new dropdown element which is owend by the dropdown.
    /// @param value the value that the this new option will have
    /// @param option The string  that will be shown when selecting this value
    /// \return pointer to the newly created dropdown element
    ofxDropdown_ * add(const T& value, const string& option);

    /// \brief add a dropdown option.
    /// This will create a new dropdown element which is owend by the dropdown.
    /// @param value the value that the this new option will have.
    /// The string shown in the dropdown will be the automatic string representation of this value
    /// \return pointer to the newly created dropdown element
    ofxDropdown_ * add(const T& value);
    
    /// \brief add several dropdown option at once.
    /// This will create new dropdown elements which will be owend by the dropdown.
    /// @param options a vector with several options.
    /// The string shown in the dropdown will be the automatic string representation of this value
    /// \return pointer to the newly created dropdown element
    ofxDropdown_ * add(const std::vector<T> & options);
    
    /// \brief add several dropdown option at once.
    /// This will create new dropdown elements which will be owend by the dropdown.
    /// @param options a map containing the values and their string representations to be added.
    /// This is the same as calling  add(const T& value, const string& option) for each of the map's values.
    /// \return pointer to the newly created dropdown element
    ofxDropdown_ * add(const std::map<T, std::string> & options);
    
    /// \brief Adds an existing dropdown to this one.
    /// \warning the added dropdown will NOT be owned by this dropdown
    /// This will "nest" the new dropdown inside the current one.
    /// @param dd the existing dropdown to be added passed by reference.
    /// \return pointer to the added dropdown
    ofxDropdown_ * addDropdown(ofxDropdown_& dd);
    
    /// \brief Adds an existing dropdown to this one.
    /// \warning the added dropdown will NOT be owned by this dropdown
    /// This will "nest" the new dropdown inside the current one.
    /// @param dd the existing dropdown to be added passed as a pointer.
    /// \return pointer to the added dropdown
    ofxDropdown_ * addDropdown(ofxDropdown_* dd);
    
    /// \brief Creates a new dropdown within this one
    /// This will create a new dropdown which will be owend by this dropdown.
    /// The new dropdown will be "nested" inside the this one.
    /// @param name The name of the new dropdown.
    /// \return pointer to the added dropdown
    ofxDropdown_ * newDropdown(std::string name);
    
    /// \brief Creates a new dropdown within this one.
    /// This will create a new dropdown which will be owend by this dropdown.
    /// The new dropdown will be "nested" inside the this one.
    /// @param param The ofParameter that will be linked to the newly created dropdown.
    /// \return pointer to the added dropdown
	ofxDropdown_ * newDropdown(ofParameter<T> param);
	
    
    
    /// \brief populate this dropdown with the files from a directory
    /// @param dirpath The path to the directory used for populating
    /// \param allowedExtensions (optional) std::vector<string> with the extensions of files you want to be shown in the dropdown
    /// \return true if it was capable to populate the dropdown, false otherwise
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
    
		
    /// \brief clear all the elements from this dropdown.
    /// \warning the correct destruction of dropdown elements will happen ONLY for the ones  owned by this dropdown will be carried out.
	void clear();
	
	
	
	
	
    // ---------------------------------------------------
    // ----------------- Events and listeners
    // ---------------------------------------------------
    
    /// \brief Helper function to add a listenerr that gets called whenever the dropdown selection changes
    /// \param listener pointer to the object that is listening
    /// \param method pointer to the listener's method to be called
	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		selectedValue.addListener(listener,method);
	}
	
    /// \brief Helper function to remove a previously added listener.
    ///  The parameters passed to this function must be exactly the same as the ones passed to addListener in order to successfuly remove the listener
    /// \param listener pointer to the object that is listening
    /// \param method pointer to the listener's method to be called
	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		selectedValue.removeListener(listener,method);
	}

    
    /// \brief ofEvent called whenever the dropdown selection changes. The data passed along with it is the displayed name of the option that changed
	ofEvent<std::string> change_E;
    
    /// \brief ofEvent called whenever the dropdown gets hidden. The data passed along with it is the name of the dropdown.
	ofEvent<std::string> dropdownHidden_E;
    /// \brief ofEvent called immediately BEFORE a dropdown will show. This is useful to call for whatever update needs to be done before the dropdown starts drawing itself.
    ofEvent<void> dropdownWillShow_E;
	
    
    // ---------------------------------------------------
    // ----------------- ofParameters
    // ---------------------------------------------------
    virtual ofAbstractParameter & getParameter() override;

    ofParameterGroup& getDropdownParameters();
	
	
	ofParameter<T> selectedValue;
	
	
    
    // ---------------------------------------------------
    // ----------------- Show / Hide / Draw
    // ---------------------------------------------------
    /// \brief Show this dropdown.
    /// \param bDisableSiblings if true the siblings of this dropdown will be disabled. This is the default. The idea is to avoid unwanted mouse interactions
    void showDropdown(bool bDisableSiblings = true);
    
    /// \brief Hide this dropdown
    /// \param bNotifyEvent if true (default) dropdownHidden_E will be notified.
    void hideDropdown(bool bNotifyEvent = true);
    
    /// \brief get if the dropdown is being shown
    /// \return true if showind the dropdown, false otherwise
    bool isShowingDropdown();
    
    
	void forceRedraw(){
		setNeedsRedraw();
	}
    
    enum DropDownPosition{
        DD_RIGHT,
        DD_BELOW,
        DD_ABOVE,
        DD_LEFT
    };
    void setDropDownPosition(DropDownPosition pos);
    
    void move(const glm::vec3& offset);
    
    void setDropdownElementsWidth(float width);
    
    
    // ---------------------------------------------------
    // ----------------- Selection
    // ---------------------------------------------------
	void setSelectedValueByName( const std::string& valueName, bool bNotify);
	void setSelectedValueByIndex( const size_t& index, bool bNotify);

	virtual void deselect() override;

    std::string getOptionAt(size_t index);
    
    // ---------------------------------------------------
    // ----------------- Children
    // ---------------------------------------------------
	
    std::vector<ofxDropdown_ *>& getChildDropdowns(){return childDropdowns;}
    std::vector<unique_ptr<ofxDropdown_>>& getOwnedDropdowns(){return ownedDropdowns;}
    std::vector<unique_ptr<ofxDropdownOption>>& getOwnedChildren(){return ownedChildren;}
    
    
    // ---------------------------------------------------
    // ----------------- Overriden mouse callbacks. Dont call these manually
    // ---------------------------------------------------
    
    virtual bool mouseReleased(ofMouseEventArgs & args) override;
    virtual bool mousePressed(ofMouseEventArgs & args) override;
    virtual bool mouseMoved(ofMouseEventArgs & args) override;
    virtual bool mouseDragged(ofMouseEventArgs & args) override;
    virtual bool mouseScrolled(ofMouseEventArgs & args) override;
    
    void registerMouseEvents() override;
    void unregisterMouseEvents() override;

    
    
    
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
    ofEventListeners mouseListeners;
	
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
    
    bool bRegisteredForMouseEvents = false;
    bool bIsSetup = false;
	
};






typedef ofxDropdown_<string> ofxDropdown;
typedef ofxDropdown_<int> ofxIntDropdown;
#ifndef TARGET_WIN32
typedef ofxDropdown_<ofFile> ofxDirDropdown;
#else
typedef ofxDropdown_<string> ofxDirDropdown;
#endif
