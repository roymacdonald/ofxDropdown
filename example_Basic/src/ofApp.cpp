#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	gui.setup();
	gui.setPosition(20,20);
	
	gui.add(ofxDropdown::getMultiSelectionParameter());
	gui.add(ofxDropdown::getCollapseOnSelectionParameter());
	
	
	options.setName("Options");

	ofSetWindowPosition(0, 0);
	ofSetWindowShape(ofGetScreenWidth(), 500);
	
	
	
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
