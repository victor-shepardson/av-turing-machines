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
            uint8_t *tape;
            uint8_t *program;
            int32_t index;
            uint8_t state;
        };

        vector<turing_machine> tm;

        uint8_t step(turing_machine &t);
        tuple<uint8_t, uint8_t, uint8_t> delta(turing_machine &t);
        int32_t getAddress(turing_machine &t);
        void randomize_tape(turing_machine &t);
        void randomize_instruction(turing_machine &t);
        void randomize_state(turing_machine &t);
        void randomize_index(turing_machine &t);
        void zero_tape(turing_machine &t);
        void init(turing_machine &t);
        uint8_t random();
        const int32_t bits = 8;
        const int32_t tape_length = 3*(1<<(2*bits));
        const int32_t jump_div = 1<<4;
        //const int program_length = 1<<(2*bits+1);

        bool print;

        ofSoundStream ss;

};
