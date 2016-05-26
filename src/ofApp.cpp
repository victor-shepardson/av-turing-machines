#include "ofApp.h"

void ofApp::audioOutAsync(float * output, int32_t bufferSize, int32_t nChannels){
    if(queued_key >= 0){
        handleKey(queued_key);
        queued_key = -1;
    }
    int32_t n = tm.size();
    if(nChannels == 1 && n>0){
        for(int32_t i=0; i<bufferSize; i++){
            output[i] = 0.;
            for(int32_t j=0; j<n; j++){
                output[i] += tm[j]->audioStep();
            }
            output[i]/=float(n);
        }
    }
    else if(nChannels == 2 && n>1){
        for(int32_t i=0; i<bufferSize; i++){
            output[2*i] = 0.;
            output[2*i+1] = 0.;
            for(int32_t j=0; j<n; j++){
                float v = tm[j]->audioStep();
                float m = float(j)/(n-1);
                output[2*i] += v*(1.-m);
                output[2*i+1] += v*m;
            }
            output[2*i]*=2./float(n);
            output[2*i+1]*=2./float(n);
        }
    }
    else{
        for(int32_t i=0; i<bufferSize; i++){
            for(int32_t c=0; c<n; c++){
                output[nChannels*i+c] = tm[c]->audioStep();
            }
            for(int32_t c=n; c<=nChannels; c++){
                tm[c]->step();
            }
        }
    }
    if(recording)
        recorder.addAudioSamples(output, bufferSize, nChannels);
}

void ofApp::audioOut(float * output, int32_t bufferSize, int32_t nChannels){
    if(queued_key >= 0){
        handleKey(queued_key);
        queued_key = -1;
    }
    int32_t n = tm.size();
    if(nChannels == 1 && n>0){
        for(int32_t i=0; i<bufferSize; i++){
            output[i] = 0.;
            for(auto &i:tm){
                i->tick();
            }
            for(int32_t j=0; j<n; j++){
                output[i] += tm[j]->audioTock();
            }
            output[i]/=float(n);
        }
    }
    else if(nChannels == 2 && n>1){
        for(int32_t i=0; i<bufferSize; i++){
            output[2*i] = 0.;
            output[2*i+1] = 0.;
            for(auto &i:tm){
                i->tick();
            }       
            for(int32_t j=0; j<n; j++){
                float v = tm[j]->audioTock();
                float m = float(j)/(n-1);
                output[2*i] += v*(1.-m);
                output[2*i+1] += v*m;
            }
            output[2*i]*=2./float(n);
            output[2*i+1]*=2./float(n);
        }
    }
    else{
        for(int32_t i=0; i<bufferSize; i++){
            for(auto &i:tm){
                i->tick();
            }
            for(int32_t c=0; c<n; c++){
                output[nChannels*i+c] = tm[c]->audioTock();
            }
            for(int32_t c=n; c<=nChannels; c++){
                tm[c]->tock();
            }
        }
    }
    if(recording)
        recorder.addAudioSamples(output, bufferSize, nChannels);
}

void ofApp::setup(){

    queued_key = -1;

    presets.push_back("2016-05-18-15-50-01-479.xml");
    presets.push_back("2016-05-18-16-08-06-423.xml");
    presets.push_back("2016-05-18-16-09-17-812.xml");
    presets.push_back("2016-05-18-16-10-48-812.xml");
    presets.push_back("2016-05-18-16-13-02-036.xml");
    presets.push_back("2016-05-18-16-20-59-081.xml");
    presets.push_back("2016-05-18-16-21-36-716.xml");
    presets.push_back("2016-05-18-16-23-14-430.xml");
    presets.push_back("2016-05-18-16-23-27-863.xml");

    ofxXmlSettings s;
    s.loadFile(ofToDataPath("settings.xml"));

    const int32_t n_machines = s.getValue("n_machines", 6);
    const int32_t jump_div = s.getValue("jump_div", 16);
    shape = s.getValue("shape", 0);
    int32_t bits = s.getValue("bits", 8);
    audio_sample_rate = s.getValue("audio_sample_rate", 48000);
    video_frame_rate = s.getValue("video_frame_rate", 30);
    audio_device = s.getValue("audio_device", 0);
    audio_channels = s.getValue("audio_channels", 2);
    record_width = s.getValue("record_width", 1920);
    record_height = s.getValue("record_height", 1080);
    ffmpeg_path = s.getValue("ffmpeg_path", "");

    cout<<"constructing "<<n_machines<<" turing machines of "<<bits<<" bits"<<endl;

    for(int32_t i=0; i<n_machines; i++){
        tm.emplace_back(new ofxAVTuringMachine(bits, jump_div));
    }
    for(int32_t i=0; i<n_machines; i++){
        tm[i]->setProgram(tm[(i+1)%n_machines]->getTape());
    }

    ofSoundStreamListDevices();
    ss.setDeviceID(audio_device);
    ss.setup(this, audio_channels, 0, audio_sample_rate, 256, 4);

    ofSetFrameRate(video_frame_rate);

    print = false;
    fullscreen = false;
    recording = false;
    save_init = false;

    // readback_fbo.allocate(record_width, record_height, GL_RGB8);

    ofSetWindowShape(n_machines*512>>shape, 512<<shape);
}

void ofApp::close(){
    ss.close();
}

//--------------------------------------------------------------
void ofApp::update(){
    stringstream ss;
    ss << ofGetFrameRate();
    ofSetWindowTitle(ss.str());
}

//--------------------------------------------------------------
void ofApp::draw(){
    int32_t ww = ofGetWindowWidth(), wh = ofGetWindowHeight();
    int32_t n = tm.size();

    ofPixels pix;
    for(int32_t i=0; i<n; i++){
        shared_ptr<ofPixels> tape = tm[i]->makePixels(shape);
        if(!pix.isAllocated())
            pix.allocate(tape->getWidth()*n, tape->getHeight(), 3);
        tape->pasteInto(pix, tape->getWidth()*i, 0);
    }
    ofImage img(pix);
    img.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    img.draw(0,0,ww,wh);
    if(recording){
        recorder.addFrame(pix);
    }
}

void ofApp::toggleVideoRecord(){
    recording = !recording;
    if(recording)
        beginVideoRecord();
    else
        endVideoRecord();
}

void ofApp::beginVideoRecord(){
    // stringstream bitrate;
    // bitrate<<uint64_t(record_width*record_height*video_frame_rate*24/1000)<<"k";
    // cout<<bitrate.str()<<endl;
    // recorder.setVideoBitrate(bitrate.str());

    if(ffmpeg_path == ""){
        cout<<"error: ffmpeg path not set"<<endl;
        recording = false;
        return;
    }

    recorder.setFfmpegLocation(ffmpeg_path);

    string fname = "av-turing-machines-"+ofGetTimestampString();

    recorder.setup(fname, record_width, record_height, video_frame_rate, audio_sample_rate, audio_channels);

    recorder.start();
}

void ofApp::endVideoRecord(){
    recorder.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int32_t key){
    queued_key = key;
}

void ofApp::handleKey(int32_t key){
    if(key>='1' && key <= '9'){
        tm[key-49]->randomizeInstruction();
    }
    // if(key>='1' && key <= '9'){
    //     int32_t k = key - 49;
    //     string path = presets[k];
    //     ofXml x(path);
    //     x.setTo("init");
    //     int idx = 0;
    //     // mutex.lock();
    //     for(auto &i:tm){
    //         i->zeroTape();
    //         i->index = 0;
    //         i->state = 0;
    //         uint8_t state, symbol, jump;
    //         x.setToChild(idx);
    //         state = x.getIntValue("state");
    //         symbol = x.getIntValue("symbol");
    //         jump = x.getIntValue("jump");
    //         i->setCurrentInstruction(make_tuple(state, symbol, jump));
    //         idx++;
    //         x.setToParent();   
    //     }
    //     // mutex.unlock();
    // }
    if(key == 'z' || key == '0'){
        cout<<"zeroing tapes"<<endl;
        // mutex.lock();
        for(auto &i:tm){
            i->zeroTape();
            i->index = 0;
            i->state = 0;
        }
        // mutex.unlock();
    }
    if(key==8){
        for(auto &i:tm){
            i->index = 0;
        }
    }
    if(key=='.'){
        for(auto &i:tm){
            i->state = 0;
        }
    }
    if(key == 'r'){
        cout<<"randomizing tapes"<<endl;
        for(auto &i:tm)
            i->randomizeTape();
    }
    if(key == 's' || key == '*'){
        cout<<"randomizing states: ";
        for(auto &i:tm){
            i->randomizeState();
            cout<<i->state<<" "<<endl;
        }
    }
    if(key == 'p' || key == '/'){
        cout<<"randomizing indeces: ";
        for(auto &i:tm){
            i->randomizeIndex();
            cout<<i->index<<" "<<endl;
        }
    }
    if(key == 'i' || key == 13){
        // cout<<"-------------"<<endl;

        ofXml x;
        if(save_init){
            x.addChild("init");
            x.setTo("init");
        }
        int idx = 0;
        // mutex.lock();
        for(auto &i:tm){
            i->zeroTape();
            i->index = 0;
            i->state = 0;
        }
        for(auto &i:tm){
            i->randomizeInstruction();
            if(save_init){
                auto instruction = i->delta();
                x.addChild("tm");
                x.setToChild(idx);
                // x.setAttribute("state", ofToString(int(get<0>(instruction))));
                // x.setAttribute("symbol", ofToString(int(get<1>(instruction))));
                // x.setAttribute("jump", ofToString(int(get<2>(instruction))));
                x.addValue("state", int(get<0>(instruction)));
                x.addValue("symbol", int(get<1>(instruction)));
                x.addValue("jump", int(get<2>(instruction)));
                idx++;
                x.setToParent();
            }
       }
       // mutex.unlock();
       if(save_init)
            x.save(ofGetTimestampString()+".xml");
    }
    if(key == '+'){
        for(auto &i:tm){
            i->randomizeInstruction();
        }
    }
    if(key == 'f'){
        fullscreen = !fullscreen;
        ofSetFullscreen(fullscreen);
    }
    if(key == 'v'){
        toggleVideoRecord();
    }
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
