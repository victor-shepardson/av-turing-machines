#include "ofApp.h"

//step the turing machine and return new symbol
uint8_t ofApp::step(turing_machine &t){
    auto result = delta(t);
    t.state = get<0>(result);
    t.tape[t.index] = get<1>(result);
    t.index += (int32_t(get<2>(result)) - int32_t(1<<bits)/2)/jump_div;
    t.index = (t.index + tape_length) % tape_length;
    return t.tape[t.index];
}
//define the program
tuple<uint8_t, uint8_t, uint8_t> ofApp::delta(turing_machine &t){
    uint8_t new_state, new_symbol, jump;
    int32_t address = getAddress(t);
    //address %= program_length;
    new_state = t.program[address];
    new_symbol = t.program[address+1];
    jump = t.program[address+2];
    return make_tuple(new_state, new_symbol, jump);
}

//get address of instruction for current state
int32_t ofApp::getAddress(turing_machine &t){
    //return (t.state + (int32_t(t.tape[t.index])<<bits))<<1;
    return 3*(t.tape[t.index] + (int32_t(t.state)<<bits));
}

uint8_t ofApp::random(){
    uint8_t ret = rand();
    return ret>>(8-bits);
    //return ofRandom((1<<bits) + 1);
}

void ofApp::randomize_state(turing_machine &t){
    t.state = random();
}

void ofApp::randomize_index(turing_machine &t){
    uint32_t r = RAND_MAX * rand() + rand();
    t.index = int32_t(r) % tape_length;
}

void ofApp::randomize_tape(turing_machine &t){
    if(print) cout<<"randomizing tape: ";
    for(int32_t i=0; i<tape_length; i++){
        t.tape[i] = random();
        if(print) cout<<t.tape[i];
    }
    if(print) cout<<endl;
}

void ofApp::randomize_instruction(turing_machine &t){
    int32_t address = getAddress(t);
    //address %= program_length;
    t.program[address] = random();
    t.program[address+1] = random();
    t.program[address+2] = random();
}

void ofApp::zero_tape(turing_machine &t){
    memset((void*)(t.tape), 0, tape_length*sizeof(uint8_t));
}

void ofApp::init(turing_machine &t){
    t.tape = (uint8_t*)calloc(tape_length,sizeof(uint8_t));
    t.state = 0;
    t.index = 0;
}

void ofApp::audioOut(float * output, int32_t bufferSize, int32_t nChannels){
    for(int32_t i=0; i<bufferSize; i++){
        for(int32_t c=0; c<nChannels; c++){
            output[nChannels*i+c] = float(step(tm[c]))/(1<<bits) - .5;;
        }
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    for(int32_t i=0; i<2; i++){
        turing_machine t;
        init(t);
        tm.push_back(t);
    }
    tm[0].program = tm[1].tape;
    tm[1].program = tm[0].tape;

    int32_t channels = 2;
    int32_t sample_rate = 48000;
    ofSoundStreamListDevices();
    ss.setDeviceID(3);
    ss.setup(this, channels, 0, sample_rate, 256, 4);

    print = false;

    ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);

    ofSetFrameRate(60);
}

void ofApp::close(){
    ss.close();
}

//--------------------------------------------------------------
void ofApp::update(){
    stringstream ss;
    ss << ofGetFrameRate();
    ofSetWindowTitle(ss.str());
   // int32_t s = step();
   if(print)
        cout<<"symbol: "<<tm[0].tape[tm[0].index]<<", state: "<<tm[0].state<<endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    int32_t ww = ofGetWindowWidth(), wh = ofGetWindowHeight();
    int32_t n = tm.size();
    for(int32_t i=0; i<n; i++){
        ofPixels pix;
        pix.setFromExternalPixels(tm[i].tape, 1<<bits, 1<<bits, 3);
        ofImage img(pix);
        //int32_t address = getAddress(tm[1-i]);
        //img.setColor(address, ofColor(255));
        //img.update();
        img.draw(ww/n*i,0,ww/n, wh);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int32_t key){
    if(key == 'z'){
        cout<<"zeroing tapes"<<endl;
        for(auto i = tm.begin(); i<tm.end(); i++)
            zero_tape(*i);
    }
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
    //if(key == 't'){
    //    print = !print;
    //}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int32_t key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int32_t x, int32_t y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int32_t x, int32_t y, int32_t button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int32_t x, int32_t y, int32_t button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int32_t x, int32_t y, int32_t button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int32_t w, int32_t h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
