#include <math.h>
#include <NewTone.h>
#define NOTE_ON_CMD 0x90
#define NOTE_OFF_CMD 0x80
#define NOTE_VELOCITY 127

#define SERIAL_RATE 31250

int row1[8] = {42, 43, 44, 45, 46, 47, 48, 49}; // These arrarys tell which pins on the Arduino connect to the rows and columns of the key matrix
int row2[10] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40};
const int row1Count = 8;
const int row2Count = 10;
 
// JP2 and JP3 are outputs
int cols[10] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41};
const int colCount = 10;
 
int keys1[colCount][row1Count];
int keys2[colCount][row2Count];
String string = "";

int notesRow1[8] = {8, 7, 6, 5, 4, 3, 2, 1}; //Creates the key chart of piano key ids
int notesRow2[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
int notesCol1[10] = {0, 0, 8, 8, 16, 16, 24, 24, 32, 32};
int notesCol2[10] = {40, 40, 50, 50, 60, 60, 70, 70, 80, 80};
int noteMatrix1[colCount][row1Count];
int noteMatrix2[10][10]; 
int soundPin = 12;
bool printMatrix = true;
void setup() {// Sets up the inputs and outputs.
    Serial.begin(31250);
    //Serial.begin(9600); //Midi Serial Rate
 
    for(int x=0; x<row1Count; x++) {
        pinMode(row1[x], INPUT_PULLUP);
    }
    for(int x=0; x<row2Count; x++) {
        pinMode(row2[x], INPUT_PULLUP);
    }
 
    for (int x=0; x<colCount; x++) {
        pinMode(cols[x], OUTPUT);
    }

    pinMode(12, OUTPUT);

    for (int i = 0; i < 10; i++) {//Creates the key chart of the first 39 piano key ids
      for (int j = 0; j < 8; j++) {
        noteMatrix1[i][j] = notesCol1[i] + notesRow1[j];
      }
    }

    for (int k = 0; k < 10; k++) {//Creates the key chart of the other 49 piano key ids
      for (int l = 0; l < 10; l++) {
        noteMatrix2[k][l] = notesCol2[k] + notesRow2[l];
      }
    }
}
 
void readMatrix() {//Scans rows and columns to figure out key presses. After the key is found, it outputs a square wave with the frequency of the pressed key.
    // iterate the columns
    for (int colIndex=0; colIndex < colCount; colIndex++) {
        // col: set to output to low
        int curCol = cols[colIndex];
        digitalWrite(curCol, LOW);
 
        // row: interate through the rows
        // This for loop scans the first 39 keys of the keyboard controller
        for (int rowIndex=0; rowIndex < row1Count; rowIndex++) {
            int rowCol = row1[rowIndex];
            keys1[colIndex][rowIndex] = digitalRead(rowCol);
            int key = noteMatrix1[colIndex][rowIndex];
            if (digitalRead(rowCol) == LOW) {
              string = String(colIndex) + " " + String(rowIndex) + " " + String(key) + " " + String(pitchEqu(key));
              Serial.println(string);//Prints out what key is pressed to the computer monitor
              string = "";//Outputs square wave
              tone(12, pitchEqu(key), 125);
              //noteOn(key);
              //noteOff(key);
              
            }
            else {
            }
        }
         // This for loop scans the other 49 keys of the keyboard controller
        for (int row2Index=0; row2Index < 10; row2Index++) {
            int rowCol2 = row2[row2Index];
            keys2[colIndex][row2Index] = digitalRead(rowCol2);
            int key2 = noteMatrix2[colIndex][row2Index];
            if (digitalRead(rowCol2) == LOW) {
              string = String(colIndex) + " " + String(row2Index) + " " + String(rowCol2) + " " + String(key2) + " " + String(pitchEqu(key2));
              Serial.println(string); //Prints out what key is pressed to the computer monitor
              string = "";
              tone(soundPin, pitchEqu(key2), 125); //Outputs square wave
              //noteOn(key2);
              //noteOff(key2);
              
            }
            else {
              //noNewTone(soundPin);
              //noteOff(key);
            }
        }
        // disable the column
        digitalWrite(curCol, HIGH);
    }
    
}

double pitchEqu (int key) { //Converts key to pitch frequency
  double total = 0;
  double ex = ((key-49.0)/12.0);
  total = pow(2.0, ex) * 440.0; 
  return total;
}
 

void noteOn(int key) //Translates key to MIDI (Turns on note)
{
  Serial.write(NOTE_ON_CMD);
  Serial.write(key + 20);
  Serial.write(NOTE_VELOCITY);
}

void noteOff(int key) //Translates key to MIDI (Turns off note)
{
  
  Serial.write(NOTE_OFF_CMD);
  Serial.write(key + 20);
  Serial.write(NOTE_VELOCITY);
}
void loop() {//The Arduino constantly loops the scan function.
    readMatrix();
    
}

