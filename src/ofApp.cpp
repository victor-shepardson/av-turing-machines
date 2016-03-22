#include "ofApp.h"

//step the turing machine and return new symbol
int ofApp::step(turing_machine &t){
    auto result = delta(t);
    t.state = get<0>(result);
    t.tape[t.index] = get<1>(result);
    t.index += (int(get<2>(result)) - int(1<<bits)/2)/jump_div;
    t.index = (t.index + tape_length) % tape_length;
    return t.tape[t.index];
}
//define the program
tuple<unsigned char, unsigned char, unsigned char> ofApp::delta(turing_machine &t){
    unsigned char new_state, new_symbol, jump;
    int address = getAddress(t);
    //address %= program_length;
    new_state = t.program[address];
    new_symbol = t.program[address+1];
    jump = t.program[address+2];
    return make_tuple(new_state, new_symbol, jump);
}

//get address of instruction for current state
int ofApp::getAddress(turing_machine &t){
    //return (t.state + (int(t.tape[t.index])<<bits))<<1;
    return 3*(t.tape[t.index] + (int(t.state)<<bits));
}

unsigned char ofApp::random(){
    unsigned char ret = rand();
    return ret>>(8-bits);
    //return ofRandom((1<<bits) + 1);
}

void ofApp::randomize_state(turing_machine &t){
    t.state = random();
}

void ofApp::randomize_index(turing_machine &t){
    t.index = rand() % tape_length;
}

void ofApp::randomize_tape(turing_machine &t){
    if(print) cout<<"randomizing tape: ";
    for(int i=0; i<tape_length; i++){
        t.tape[i] = random();
        if(print) cout<<t.tape[i];
    }
    if(print) cout<<endl;
}

void ofApp::randomize_instruction(turing_machine &t){
    int address = getAddress(t);
    //address %= program_length;
    t.program[address] = random();
    t.program[address+1] = random();
    t.program[address+2] = random();
}

void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    for(int i=0; i<bufferSize; i++){
        for(int c=0; c<nChannels; c++){
            output[nChannels*i+c] = float(step(tm[c]))/(1<<bits) - .5;;
        }
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    for(int i=0; i<2; i++){
        turing_machine t;
        t.tape = (unsigned char*)calloc(tape_length,sizeof(unsigned char));
        t.state = 0;
        t.index = 0;
        tm.push_back(t);
    }
    tm[0].program = tm[1].tape;
    tm[1].program = tm[0].tape;

    int channels = 2;
    int sample_rate = 48000;
    ofSoundStreamListDevices();
    ss.setDeviceID(3);
    ss.setup(this, channels, 0, sample_rate, 256, 4);

    print = false;

    ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
}

void ofApp::close(){
    ss.close();
}

//--------------------------------------------------------------
void ofApp::update(){
    stringstream ss;
    ss << ofGetFrameRate();
    ofSetWindowTitle(ss.str());
   // int s = step();
   if(print)
        cout<<"symbol: "<<tm[0].tape[tm[0].index]<<", state: "<<tm[0].state<<endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    int ww = ofGetWindowWidth(), wh = ofGetWindowHeight();
    int n = tm.size();
    for(int i=0; i<n; i++){
        ofPixels pix;
        pix.setFromExternalPixels(tm[i].tape, 1<<bits, 1<<bits, 3);
        //int address = getAddress(tm[1-i]);
        ofImage img(pix);
        //img.getPixelsRef().resize(ww/n, wh, OF_INTERPOLATE_NEAREST_NEIGHBOR);
        //img.setColor(address, ofColor(255));
        img.update();
        //img.setColor(address, ofColor(0,0,255));
        img.draw(ww/n*i,0,ww/n, wh);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r'){
        cout<<"randomizing tapes"<<endl;
        for(auto i = tm.begin(); i<tm.end(); i++)
            randomize_tape(*i);
    }
    if(key == 's'){
        cout<<"randomizing states: ";
        for(auto i = tm.begin(); i<tm.end(); i++){
            randomize_state(*i);
            cout<<i->state<<" "<<endl;
        }
    }
    if(key == 'p'){
        cout<<"randomizing indeces: ";
        for(auto i = tm.begin(); i<tm.end(); i++){
            randomize_index(*i);
            cout<<i->index<<" "<<endl;
        }
    }
    if(key == 'i'){
        for(auto i = tm.begin(); i<tm.end(); i++)
            randomize_instruction(*i);
    }
    if(key == 'p'){
        print = !print;
    }
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
