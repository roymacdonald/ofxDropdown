#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDropdown.h"

class ofApp : public ofBaseApp {
    public:
        void setup();
        void draw();
        void onSongChange(ofFile & file);

        ofxPanel gui;

        ofxDropdown_<ofFile> songDropdown { "Song" };
};
