/**
 * Forward FFT
 * by Damien Di Fede.
 *  
 * This sketch demonstrates how to use an FFT to analyze an AudioBuffer 
 * and draw the resulting spectrum. It also allows you to turn windowing 
 * on and off, but you will see there is not much difference in the spectrum.
 * Press 'w' to turn on windowing, press 'e' to turn it off.
 */

import ddf.minim.analysis.*;
import ddf.minim.*;
import processing.net.*;
import java.util.Date;


String data;
Client c;

Minim minim;
AudioPlayer jingle;
FFT fft;
String windowName;
AudioInput in;
void setup()
{
  size(512, 200);
  minim = new Minim(this);
  in = minim.getLineIn(Minim.STEREO, 2048);
  c = new Client(this, "localhost", 8080); // Connect to server on port 80

  //  jingle = minim.loadFile("jingle.mp3", 2048);
  //jingle.loop();
  // create an FFT object that has a time-domain buffer the same size as jingle's sample buffer
  // note that this needs to be a power of two and that it means the size of the spectrum
  // will be 512. see the online tutorial for more info.
  fft = new FFT(in.bufferSize(), in.sampleRate());
  textFont(createFont("SanSerif", 12));
  windowName = "None";
}

void draw()
{
  background(0);
  stroke(255);
  // perform a forward FFT on the samples in jingle's left buffer
  // note that if jingle were a MONO file, this would be the same as using jingle.right or jingle.left
  fft.forward(in.mix);
  for(int i = 0; i < fft.specSize(); i++)
  {
    // draw the line for frequency band i, scaling it by 4 so we can see it a bit better
    line(i, height, i, height - fft.getBand(i)*4);
  }
  fill(255);
  /* Guitar strings from howstuffworks.com
   strings[0] = fft.getFreq(82.4); //E
   strings[1] = fft.getFreq(110.0); //A
   strings[2] = fft.getFreq(146.8); //D
   strings[3] = fft.getFreq(196.0); //G
   strings[4] = fft.getFreq(246.9); //B
   strings[5] = fft.getFreq(329.6); //E
   */
  if(fft.getFreq(82.4)>=7) sendNoteTojHome("E");
  if(fft.getFreq(110.0)>=9) sendNoteTojHome("A");
  if(fft.getFreq(146.8)>=9) sendNoteTojHome("D");
  if(fft.getFreq(196.0)>=9) sendNoteTojHome("G");
  if(fft.getFreq(246.9)>=5) sendNoteTojHome("B");
  if(fft.getFreq(329.6)>=5) sendNoteTojHome("e");

  // keep us informed about the window being used
  text("The window being used is: " + windowName, 5, 20);
}

void keyReleased()
{
  if ( key == 'w' ) 
  {
    // a Hamming window can be used to shape the sample buffer that is passed to the FFT
    // this can reduce the amount of noise in the spectrum
    fft.window(FFT.HAMMING);
    windowName = "Hamming";
  }

  if ( key == 'e' ) 
  {
    fft.window(FFT.NONE);
    windowName = "None";
  }
}

void stop()
{
  // always close Minim audio classes when you finish with them
  //jingle.close();
  minim.stop();

  super.stop();
}

int notePosition(String note) {
  if(note=="E") return 0;
  else if(note=="A") return 1;
  else if(note=="D") return 2;
  else if(note=="G") return 3;
  else if(note=="B") return 4;
  else if(note=="e") return 5;
  else return 9; //to throw exception..

}  
void restartNotes() {
  for(int x=0;x<countNote.length;x++) {
    countNote[x] = 0;
  }
}

int countNote[] = new int[6];

long countMillis = 0;
long MAXMILLIS = 1500;
int  FIRECOUNT = 25;
void sendNoteTojHome(String note) {

  if(millis() - countMillis>MAXMILLIS) { //max period expired
    System.out.println("zerando");
    restartNotes();    
  }
  countMillis=millis();  
  countNote[notePosition(note)]++;
  if(countNote[notePosition(note)]==FIRECOUNT) {
    System.out.println("Enviando " + note);
    try {
      c.write("GET /admin/SoundChange?note=" + note + " HTTP/1.1\n"); // Use the HTTP "GET" command to ask for a Web page
      c.write("Host: my_domain_name.com\n\n"); // Be polite and say who we are    
    }                                                                                                                
    catch(Exception e) {
      c = new Client(this, "localhost", 8080); // Connect to server on port 80
      e.printStackTrace();
      
    }    
    countMillis=0;  
    restartNotes();
  }
}  

