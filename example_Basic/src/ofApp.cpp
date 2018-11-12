#include "ofApp.h"
//#include "ofxFullButton.h"
//#include "ofxFullToggle.h"
//--------------------------------------------------------------
void ofApp::setup(){
	
	gui.setup();
	gui.setPosition(20,20);
//	gui.add(paramVoid.set("void"));
//	gui.add(paramBool.set("bool", false));
	gui.add(ofxDropdown::bMultiselection);
	gui.add(ofxDropdown::bCollapseOnSelection);
	
	options.setName("Options");

	ofSetWindowPosition(0, 0);
	ofSetWindowShape(ofGetScreenWidth(), 500);
	
	//*
//	dd = new ofxDropdown(options);
//	dd = new ofxDropdown("Options");
	
	for(int i = 0; i < 15; i++){
		ofxDropdown* dd2 = new ofxDropdown("Options" + ofToString(i));
		for(int j = 0; j < 5; j++){
			dd2->add(ofToString(i)+"_"+string(1, j+'A'));
		}
		if(i == 0){
			gui.add(dd2);
		}else{
			dd->addDropdown(dd2);
		}
		if(i%3==0) dd = dd2;
	}
	
//	dd->add("A");
//	dd->add("B");
//	dd->add({"x", "C", "D"});
//	gui.add(dd);
	
	
//	options2.setName("Options 2");

//	auto * dd2 = new ofxDropdown(options2);
//	auto * dd2 = dd->newDropdown("options 2");
//	
//	dd2->add("2_A");
//	dd2->add("2_B");
//	dd2->add("2_C");
//	dd2->add("2_D");
//	dd2->add("2_E");
//	
//	dd->addDropdown(dd2);
//
//	auto * dd3 = new ofxDropdown("options3");
//	//	
//	dd3->add("3_A");
//	dd3->add("3_B");
//	dd3->add("3_C");
//	dd3->add("3_D");
//	dd3->add("3_E");
	
//	dd2->addDropdown(dd3);
	
	//gui.add(dd3);
	
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	gui.draw();
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
cout << "---------------------" << endl;
	//	if(dd){
//		if(key == OF_KEY_LEFT){
//			dd->setDropDownPosition(ofxDropdown::DD_LEFT);
//		}else if(key == OF_KEY_RIGHT){
//			dd->setDropDownPosition(ofxDropdown::DD_RIGHT);
//		}else if(key == OF_KEY_UP){
//			dd->setDropDownPosition(ofxDropdown::DD_ABOVE);
//		}else if(key == OF_KEY_DOWN){
//			dd->setDropDownPosition(ofxDropdown::DD_BELOW);
//		}
//	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
