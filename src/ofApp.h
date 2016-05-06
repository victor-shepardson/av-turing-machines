#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxVideoRecorder.h"
#include "ofxAVTuringMachine.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void close();
        void audioOut(float * output, int bufferSize, int nChannels);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void beginVideoRecord();
        void endVideoRecord();
        void toggleVideoRecord();

        bool recording;

        vector<shared_ptr<ofxAVTuringMachine> > tm;

        string ffmpeg_path;
        ofxVideoRecorder recorder;

        ofFbo readback_fbo;

        int32_t shape;
        // int32_t bits;

        int32_t audio_sample_rate, video_frame_rate, audio_channels, audio_device, record_width, record_height;

        bool print, fullscreen;

        ofSoundStream ss;

};
