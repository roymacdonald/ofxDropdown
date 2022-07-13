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
ofxDropdown_<T>::ofxDropdown_(ofParameter<T> param, float width , float height){
    setup(param,width,height);
}
template<class T>
ofxDropdown_<T>::ofxDropdown_(ofParameter<T> param, const map<T,string>& dropDownOptions, float width , float height){
    setup(param,width,height);
    add(dropDownOptions);
}
template<class T>
ofxDropdown_<T>::ofxDropdown_(ofParameter<T> param, const std::vector<T> & dropDownOptions, float width , float height){
    setup(param,width,height);
    add(dropDownOptions);
}
template<class T>
ofxDropdown_<T>::ofxDropdown_(std::string name, float width, float height){
    setup(name, width, height);
}

template<class T>
ofxDropdown_<T>::~ofxDropdown_(){
    
}

//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::setup(std::string name, float width , float height ){
    if (bIsSetup){
        ofLogWarning("ofxDropdown_<T>::setup" ) << "Dropdown \"name\" is already setup. Nothing will be done";
        return this;
    }
        
	ofxDropdownOption::setup(name, false , width, height);
	buttonListener = value.newListener(this,&ofxDropdown_::buttonClicked);

	selectedValue.setName(name);
	setlectedValueListener = selectedValue.newListener(this, &ofxDropdown_<T>::selectedValueChanged);
	
	group.setup();
	group.disableHeader();
	group.setParent(this);
	group.unregisterMouseEvents();

    dropdownParams.setName(name + " params");
	dropdownParams.add(bMultiselection);
	dropdownParams.add(bCollapseOnSelection);
	dropdownParams.add(bDisableChildrenRecursively);
	
    bIsSetup = true;
    
	return this;
}

//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::setup(ofParameter<T> param, float width, float height){
    if (bIsSetup){
        ofLogWarning("ofxDropdown_<T>::setup" ) << "Dropdown \"name\" is already setup. Nothing will be done";
        return this;
    }
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
			hideDropdown();
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
					hideDropdown();
				}
			
			}
		}else{
			ofLogVerbose("ofxDropdown_::groupChanged(...)")  << "sender = null";
		}
	}
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const T& value) {
    return add(value, ofToString(value));
}

#ifndef TARGET_WIN32
template<>
ofxDropdown_<ofFile> * ofxDropdown_<ofFile>::add(const ofFile& value) {
    return add(value, value.getFileName());
}
#endif
//--------------------------------------------------------------
template<>
ofxDropdown_<string> * ofxDropdown_<string>::add(const string& value) {
    return add(value, value);
}



template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const T& value, const string& option) {
    options.push_back(option);
    values.push_back(value);
    	
	ownedChildren.emplace_back(make_unique<ofxDropdownOption>());
	auto o = ownedChildren.back().get();
	if(o){
		o->setup(option, value == selectedValue.get());
		groupListeners.push(o->getParameter().template cast<bool>().newListener(this, &ofxDropdown_::groupChanged));
    
		group.add(o);
		_updateGroupWidth();
	}else
	{
		ofLogError("ofxDropdown_<T>::add") << "created children is nullptr";
	}
	

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
//--------------------------------------------------------------
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
	return  addDropdown(&dd);
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::addDropdown(ofxDropdown_ * dd){
	if(dd){
//		cout << typeid(dd).name() <<"\n";
		group.add(dd);
		childDropdowns.push_back(dd);
		childDropdownListeners.push(dd->dropdownHidden_E.newListener(this, &ofxDropdown_::childDropdownHidden));
		_updateGroupWidth();
	}else{
		ofLogWarning("ofxDropdown_::addDropdown", "cant add nullptr dropdown");
	}
	return dd;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::newDropdown(std::string name){
//	auto dd = make_unique<ofxDropdown_<T>>(name, getWidth(), getHeight());
	//ownedChildren.push_back(std::move(dd));
//	cout<< "newDropdown: " << name << "\n";
	ownedDropdowns.emplace_back(make_unique<ofxDropdown_<T>>(name, getWidth(), getHeight()));
	auto dd = ownedDropdowns.back().get();
	addDropdown(dd);
	return dd;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::newDropdown(ofParameter<T> param){
//	cout<< "newDropdown: " << param.getName() << "\n";
	ownedDropdowns.emplace_back(make_unique<ofxDropdown_<T>>(param, getWidth(), getHeight()));
	auto dd = ownedDropdowns.back().get();
	addDropdown(dd);
	return dd;
//	auto dd = make_unique<ofxDropdown_<T>>(param, getWidth(), getHeight());
//	ownedChildren.push_back(std::move(dd));
//	addDropdown( static_cast<ofxDropdown_<T>*>(ownedChildren.back().get()));
//	return dd.get();
//
	
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::childDropdownHidden(const void * sender, std::string& s){
	hideDropdown();
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::clear(){
	group.clear();
	childDropdowns.clear();
	ownedChildren.clear();
	ownedDropdowns.clear();
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
					dd->hideDropdown(false);
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
void ofxDropdown_<T>::deselect()
{
	for(auto& c: childDropdowns)
	{
		if(c) c->deselect();
	}
	for(auto& c: ownedDropdowns)
	{
		if(c) c->deselect();
	}
	for(auto&c: ownedChildren)
	{
		if(c) c->deselect();
	}
	selectedOption = "";
	ofxDropdownOption::deselect();
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mouseReleased(ofMouseEventArgs & args){
	if(ofxDropdownOption::mouseReleased(args) || b.inside(args)){
		return true;
	}
	if(isShowingDropdown()){
		if(group.mouseReleased(args)){
			return true;
		}
		if(!b.inside(args)){
			hideDropdown();
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
		bool r = group.mousePressed(args);;
		if(!r)
		{
			hideDropdown();
		}
		return r;
	}
	return false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mouseMoved(ofMouseEventArgs & args){
	if(ofxDropdownOption::mouseMoved(args)){
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
	if(ofxDropdownOption::mouseDragged(args)){
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
	if(ofxDropdownOption::mouseScrolled(args)){
		return true;
	}
	if(isShowingDropdown()){
		if( !group.isMinimized() && group.getShape().inside(args.x, args.y)){
			move({0, args.scrollY * 2, 0});
			return true;
		}else{
			return group.mouseScrolled(args);
		}
	}
	return false;
}


//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::move(const glm::vec3& offset)
{
	group.setPosition(group.getPosition() + offset);
	for(auto& c: childDropdowns)
	{
		c->move(offset);
	}
	for(auto& c: ownedDropdowns)
	{
		c->move(offset);
	}
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::hideDropdown(bool bNotifyEvent){
	if(bGroupEnabled){
		bGroupEnabled = false;
		auto n = value.getName();
		disableElement(this);
		for(auto& c: childDropdowns)
		{
			c->hideDropdown(false);
		}
		for(auto& c: ownedDropdowns)
		{
			c->hideDropdown(false);
		}
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
		hideDropdown( false);
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
	bg.clear();
	bg.rectangle(b);
	
	generateNameTextMesh(ofRectangle(b.x, b.y, b.width, b.height/2));
	
	arrow.clear();
	auto h = b.getHeight()/2;
	auto x2 = b.getMaxX();
	
	auto y = b.getY();
	
	arrow.setStrokeColor(thisTextColor);
	arrow.setStrokeWidth(2);
	arrow.setFilled(false);
	arrow.moveTo(x2 - h /2,  y  + textPadding);
	arrow.lineTo(x2 - textPadding,  y  + h/2 );
	arrow.lineTo(x2 - h /2,  y  + h - textPadding);
	
	
	ofRectangle optionsTextRect = b;
	optionsTextRect.height *= 0.5;
	optionsTextRect.y = optionsTextRect.getMaxY();
	
	optionTextMesh = getTextMesh(selectedOption, x2 - getTextBoundingBox(selectedOption, 0, 0).width - textPadding , getTextVCenteredInRect(optionsTextRect));
//	ofRectangle textRect = b;
//	textRect.width -= h*2.5;
//	textRect.x += h*1.5;
//	ofRemove(optionTextMesh.getVertices(), [&](const glm::vec3& p){
//		return !textRect.inside(p);
//	});	
	
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::render(){
	ofxDropdownOption::render();
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
void ofxDropdown_<T>::setDropdownElementsWidth(float width)
{
	group.setSize(width, group.getHeight());
	group.setWidthElements(width);
	
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

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::registerMouseEvents(){
    if(bRegisteredForMouseEvents == true){
        return; // already registered.
    }
#ifndef TARGET_WIN32
    ofRegisterMouseEvents(this, int(defaultEventsPriority) - 100);
#else
	auto p = defaultEventsPriority;
	defaultEventsPriority = ofEventOrder(defaultEventsPriority - 10);
	ofxBaseGui::registerMouseEvents();
	defaultEventsPriority = p;
#endif
	bRegisteredForMouseEvents = true;
}


//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::unregisterMouseEvents(){
    if(bRegisteredForMouseEvents == false){
        return; // not registered.
    }
#ifndef TARGET_WIN32
    ofUnregisterMouseEvents(this, int(defaultEventsPriority) - 100);
#else
	auto p = defaultEventsPriority;
	defaultEventsPriority = ofEventOrder(defaultEventsPriority - 10);
	ofxBaseGui::unregisterMouseEvents();
	defaultEventsPriority = p;
#endif
    bRegisteredForMouseEvents = false;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::_updateGroupWidth()
{
	
	float mx = ofxBaseGui::defaultWidth;
	float arrowW =  b.getHeight()/2;
	
	for(auto& c: childDropdowns)
	{
		if(c)
			mx = std::max(mx, getTextBoundingBox(c->getName(), 0,0).width + arrowW);
	}
	for(auto& c: ownedDropdowns)
	{
		if(c)
			mx = std::max(mx, getTextBoundingBox(c->getName(), 0,0).width + arrowW);
	}
	
	for(auto& c: ownedChildren)
	{
		if(c)
			mx = std::max(mx, getTextBoundingBox(c->getName(), 0,0).width);
	}
	
	setDropdownElementsWidth(mx);
	
	
}



//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::addFromDir(ofxDropdown_* currentDD, const string& dirpath, const vector<string>& allowedExtensions)
{
	ofLogError("ofxDropdown_<T>::addFromDir" ) << "This function only works with ofxDirDropdown";
}
template<>
void
#ifndef TARGET_WIN32
ofxDropdown_<ofFile>
#else
ofxDropdown_<string>
#endif
::addFromDir(ofxDropdown_* currentDD, const string& dirpath, const vector<string>& allowedExtensions)
{
	if(currentDD != nullptr)
	{
	
		ofDirectory dir(dirpath);
		for(auto& ext: allowedExtensions)
		{
			dir.allowExt(ext);
		}
		dir.listDir();
        dir.sort();

		for(size_t i = 0; i < dir.size(); i++)
		{
			ofFile f(dir.getPath(i));
			if(f.exists())
			{
				if(f.isDirectory())
				{
					
					auto dd = newDropdown(f.getBaseName());
					addFromDir(dd, f.getAbsolutePath(), allowedExtensions);
				}
				else
				{
#ifndef TARGET_WIN32
 					currentDD->add(f);
#else
                    currentDD->add(f.getAbsolutePath(), f.getFileName());
#endif
				}
			}
		}
	}
	else
	{
		ofLogError("ofxDropdown_<ofFile>::addFromDir") << "Cannot add dir to null dropdown";
	}

}


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



template class ofxDropdown_<uint8_t>;
#ifndef TARGET_WIN32
template class ofxDropdown_<ofFile>;
#endif
template class ofxDropdown_<string>;
template class ofxDropdown_<int>;
