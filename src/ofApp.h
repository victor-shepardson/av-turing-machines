#pragma once

#include "ofMain.h"

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

        struct turing_machine{
            unsigned char *tape;
            unsigned char *program;
            unsigned char state, dummy;
            int index;
        };

        vector<turing_machine> tm;

        int step(turing_machine &t);
        tuple<unsigned char, unsigned char, unsigned char> delta(turing_machine &t);
        int getAddress(turing_machine &t);
        void randomize_tape(turing_machine &t);
        void randomize_instruction(turing_machine &t);
        void randomize_state(turing_machine &t);
        void randomize_index(turing_machine &t);
        unsigned char random();
        const int bits = 8;
        const int tape_length = 3*(1<<(2*bits));
        const int jump_div = 1<<(bits-4);
        //const int program_length = 1<<(2*bits+1);

        bool print;

        ofSoundStream ss;

};
