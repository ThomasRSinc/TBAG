// Wemos Project by Thomas Sinclair (15/12/20)
// The Binary Arithmetic Game (TBAG)

// Code is not exhaustedly commented - non-standard functions, more complex features and basic program function descriptions are commented.
// Functions/features that need elaborating upon only have the first use commented, even if used multiple times.
// General program steps (display this, calculate that etc.) are always commented though



// General
#include <iostream>     // Input/Output stream
#include <cmath>		// Maths operations (floor/round)
#include <ctime>		// Time operations (time_t, time())
#include <Streaming.h>  // Potentiometer, Button and OLED

// OLED, OLED i2c
#include <iomanip>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1
#define OLED_SCREEN_I2C_ADDRESS 0x3C
Adafruit_SSD1306 OLED(OLED_RESET);

// 7-Segment Display, Expansion Board Ports & LEDs
#include <InvertedTM1638.h>
#include <TM1638.h>
#include <TM1638QYF.h>
#include <TM1640.h>
#include <TM16XX.h>
#include <TM16XXFonts.h>
#define TM1638_STB D5
#define TM1638_CLK D6
#define TM1638_DIO D7
TM1638 expBoard(TM1638_DIO, TM1638_CLK, TM1638_STB); 

// Servo Motor
#include <Servo.h>
Servo servo;

// Expansion Board
byte expButtons;       // Expansion Board Buttons

// Button
int buttonPin = 0;     // Wemos Pin D3 = Arduino Pin 0
int buttonValue = 0;

// GLOBAL VARIABLES
int numQuestions = 5;
int secondsPerQuestion = 30;
int secondsToReview = 15;
bool highlightMistakes = true;
uint8_t segmentDisplayStart = 0b00000000;


// Initial component setup and splash intro
void setup() {
	// Serial
	Serial.begin(115200);
	delay(5000);
	Serial << endl << "Hello World" << endl;

	// OLED (+ Intro)
	OLED.begin(SSD1306_SWITCHCAPVCC, OLED_SCREEN_I2C_ADDRESS);
	OLED.display();
	delay(1000);
	OLED.setTextColor(WHITE);
	OLED.setTextSize(2);
	resetOLED();
	OLED.println(" - TBAG -");
	OLED.println("The BinaryArithmeticGame");
	OLED.display();
	delay(2000);
	resetOLED();
	OLED.println("Wemos 2020");
	OLED.println("By Thomas Sinclair");
	OLED.display();
	delay(2000);
	OLED.setTextSize(1);

	// 7-Segment Display
	expBoard.clearDisplay();
	expBoard.setupDisplay(true, 2);

	// Servo Motor
	servo.attach(SS);
	servo.write(0);
}

// Reset OLED display component
void resetOLED() {
	OLED.clearDisplay();
	OLED.setCursor(0,0);
}



// Main program (loops infinitely)
void loop() {
	bool backToChoice = false;
	
	// Display main menu
	resetOLED();
	OLED.println("Hi! Select option...");
	OLED.println("---------------------");
	OLED.println("S1 - Play Game");
	OLED.println("S2 - Instructions");
	OLED.println("S3 - Settings");
	OLED.display();
	
	// Loop until backToChoice is true (saves unnecessarily displaying the same menu screen repeatedly)
	while (backToChoice != true) {
		// Get button values
		delay(250);									// Required to allow time for buttons to be released, otherwise program receives same input meant for different actions/purposes. Delay in ms
		yield();									// Required so button values can be read
		buttonValue = digitalRead(buttonPin);
		expButtons = expBoard.getButtons();
		
		// Depending on what expansion button is pressed, call respective function
		switch(expButtons) {
			case 1:
				playGame();
				backToChoice = true;
				break;
			case 2:
				showInstructions();
				backToChoice = true;
				break;
			case 4:
				configSettings();
				backToChoice = true;
				break;
			default:
				break;
		}
	}
}



// Choose gamemode to play
void playGame() {
	// Declare local variables
	bool backToMenu = false;
	bool backToChoice;
	
	// Loop until backToMenu is true
	while (backToMenu != true) {
		// Display choice menu
		resetOLED();
		OLED.println("Select mode...");
		OLED.println("---------------------");
		OLED.println("S1 - Addition");
		OLED.println("S2 - Subtraction");
		OLED.println("S3 - Multiplication");
		OLED.println("S4 - Division");
		OLED.display();

		backToChoice = false;
	
		// Loop until backToChoice is true
		while (backToChoice != true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// If back button pressed, go back to main menu
			if (buttonValue == 0) {
				backToMenu = true;
				break;
			}
			
			// Depending on what expansion button is pressed, call respective function
			// Then go back to choice menu
			switch(expButtons) {
				case 1:
					playAdditionMode();
					backToChoice = true;
					break;
				case 2:
					playSubtractionMode();
					backToChoice = true;
					break;
				case 4:
					playMultiplicationMode();
					backToChoice = true;
					break;
				case 8:
					playDivisionMode();
					backToChoice = true;
					break;
				default:
					break;
			}
		}
	}
}


// Show game instructions
void showInstructions() {
	// Declare local variables
	bool backToMenu = false;
	
	// Loop until backToMenu is true
	while (backToMenu != true) {
		// Display choice menu
		resetOLED();
		OLED.println("Select page...");
		OLED.println("---------------------");
		OLED.println("S1 - Page 1");
		OLED.println("S2 - Page 2");
		OLED.println("S3 - Page 3");
		OLED.println("...");
		OLED.println("S8 - Page 8");
		OLED.display();
		
		// Loop until broken (infinitely)
		while (true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// If back button pressed, go back to main menu
			if (buttonValue == 0) {
				backToMenu = true;
				break;
			}
			
			// Depending on what expansion button is pressed, display respective page of text
			switch(expButtons) {
				case 1:
					resetOLED();
					OLED.println("Page 1");
					OLED.println("---------------------");
					OLED.println("Hello and welcome to The Binary Arithmetic Game (TBAG)! This will be a brief but hopefully helpful guide to the game...");
					OLED.display();
					break;
				case 2:
					resetOLED();
					OLED.println("Page 2");
					OLED.println("---------------------");
					OLED.println("The objective is simple: answer the questions given to you and try to get them correct. Questions will first appear on this");
					OLED.display();
					break;
				case 4:
					resetOLED();
					OLED.println("Page 3");
					OLED.println("---------------------");
					OLED.println("screen and you must use the expansion board to answer (in 8-bit unsigned binary). The S buttons will change the 7-segment");
					OLED.display();
					break;
				case 8:
					resetOLED();
					OLED.println("Page 4");
					OLED.println("---------------------");
					OLED.println("display directly above it, flipping each corresponding segment between 0 and 1. Once you've entered your answer, press that");
					OLED.display();
					break;
				case 16:
					resetOLED();
					OLED.println("Page 5");
					OLED.println("---------------------");
					OLED.println("external button on the breadboard (let's call it 'S0') to confirm. When not in a game, this button functions as a back button");
					OLED.display();
					break;
				case 32:
					resetOLED();
					OLED.println("Page 6");
					OLED.println("---------------------");
					OLED.println("when traversing menus. Likewise the S1-S8 buttons work as select/choice buttons. Remember this is 8-bit unsigned binary, with");
					OLED.display();
					break;
				case 64:
					resetOLED();
					OLED.println("Page 7");
					OLED.println("---------------------");
					OLED.println("the MSB (128) on the left! Ensure you customise settings before playing too to play the game how you really want to. That's");
					OLED.display();
					break;
				case 128:
					resetOLED();
					OLED.println("Page 8");
					OLED.println("---------------------");
					OLED.println("about it really, just try to remember this! Don't worry - you'll get the hang of it. Now go and have some mathematical fun!");
					OLED.display();
					break;
				default:
					break;
			}
		}
	}
}


// Configure game settings
void configSettings() {
	// Declare local variables
	bool backToMenu = false;
	bool backToChoice;
	
	// Loop until backToMenu is true
	while (backToMenu != true) {
		// Display choice menu
		resetOLED();
		OLED.println("Select setting...");
		OLED.println("---------------------");
		OLED.println("S1 - numQuestions");
		OLED.println("S2 - secsPerQuestion");
		OLED.println("S3 - secondsToReview");
		OLED.println("S4 - highlightMistake");
		OLED.println("S5 - 7-SegmentStart");
		OLED.display();
		
		backToChoice = false;
		
		// Loop until backToChoice is true
		while (backToChoice != true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// If back button pressed, go back to main menu
			if (buttonValue == 0) {
				backToMenu = true;
				break;
			}
			
			// Depending on what expansion button is pressed, call respective function
			// Then go back to choice menu
			switch(expButtons) {
				case 1:
					setNumQuestions();
					backToChoice = true;
					break;
				case 2:
					setSecondsPerQuestion();
					backToChoice = true;
					break;
				case 4:
					setSecondsToReview();
					backToChoice = true;
					break;
				case 8:
					setHighlightMistakes();
					backToChoice = true;
					break;
				case 16:
					setSegmentDisplayStart();
					backToChoice = true;
					break;
				default:
					break;
			}
		}
	}
}



// Configure setting - Number of questions per game
void setNumQuestions() {
	// Declare local variables
	bool backToMenu = false;
	bool backToChoice;
	int sensorValue = 0;
	
	// Loop until backToMenu is true
	while (backToMenu != true) {
		// Display choice menu
		resetOLED();
		OLED.println("How many questions per game?");
		OLED.println("S1 - 5   |  S2 - 10");
		OLED.println("S3 - 15  |  S4 - 20");
		OLED.println("S5 - 25  |  S6 - 42");
		OLED.println("S7 - Custom (1-100)");
		OLED.display();
		
		backToChoice = false;
		
		//Loop until backToChoice is true
		while (backToChoice != true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// If back button pressed, go back to main menu
			if (buttonValue == 0) {
				backToMenu = true;
				break;
			}
			
			// Depending on what expansion button is pressed, set variable to respective value and display confirmation message
			// Then go back to choice menu
			switch(expButtons) {
				case 1:
					numQuestions = 5;
					resetOLED();
					OLED.println("You have 5 questions to answer per game.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 2:
					numQuestions = 10;
					resetOLED();
					OLED.println("You have 10 questions to answer per game.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 4:
					numQuestions = 15;
					resetOLED();
					OLED.println("You have 15 questions to answer per game.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 8:
					numQuestions = 20;
					resetOLED();
					OLED.println("You have 20 questions to answer per game.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 16:
					numQuestions = 25;
					resetOLED();
					OLED.println("You have 25 questions to answer per game.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 32:
					numQuestions = 42;
					resetOLED();
					OLED.println("You have 42 questions to answer per game. Hmmm...");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 64:
					// Display instructional message
					resetOLED();
					OLED.setTextSize(2);
					OLED.println("IMPORTANT!");
					OLED.setTextSize(1);
					OLED.println("Turn the knob left/right to decrease/increase the value. Press S0 (back) button whenfinished.");
					OLED.println("        ----->");
					OLED.display();
					delay(8000);
					
					// Loop until broken (infinitely)
					while (true) {
						// Get button and potentiometer values
						delay(250);
						yield();
						buttonValue = digitalRead(buttonPin);
						sensorValue = analogRead(A0);
						
						// If back button pressed, go back to choice menu
						if (buttonValue == 0) {
							backToChoice = true;
							break;
						}
						
						// Set variable to potentiometer value (max variable value 100)
						numQuestions = ceil(sensorValue/10.24);
						
						// Display updating confirmation message
						resetOLED();
						OLED.setTextSize(2);
						OLED.println("   --->");
						OLED.println("");
						OLED.setTextSize(1);
						OLED.print("Number of questions  per game: ");
						OLED.print(numQuestions);
						OLED.display();
					}
					break;
				default:
					break;
			}
		}
	}
}


// Configure setting - Number of seconds per question
void setSecondsPerQuestion() {
	// Declare local variables
	bool backToMenu = false;
	bool backToChoice;
	int sensorValue = 0;
	
	// Loop until backToMenu is true
	while (backToMenu != true) {
		// Display choice menu
		resetOLED();
		OLED.println("How many seconds per question?");
		OLED.println("S1 - 15  |  S2 - 30");
		OLED.println("S3 - 45  |  S4 - 60");
		OLED.println("S5 - 90  |  S6 - 120");
		OLED.println("S7 - Custom (1-360)");
		OLED.display();
		
		backToChoice = false;
		
		// Loop until backToChoice is true
		while (backToChoice != true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// If back button pressed, go back to main menu
			if (buttonValue == 0) {
				backToMenu = true;
				break;
			}
			
			// Depending on what expansion button is pressed, set variable to respective value and display confirmation message
			// Then go back to choice menu
			switch(expButtons) {
				case 1:
					secondsPerQuestion = 15;
					resetOLED();
					OLED.println("You have 15 seconds to answer each question.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 2:
					secondsPerQuestion = 30;
					resetOLED();
					OLED.println("You have 30 seconds to answer each question.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 4:
					secondsPerQuestion = 45;
					resetOLED();
					OLED.println("You have 45 seconds to answer each question.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 8:
					secondsPerQuestion = 60;
					resetOLED();
					OLED.println("You have 60 seconds to answer each question.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 16:
					secondsPerQuestion = 90;
					resetOLED();
					OLED.println("You have 90 seconds to answer each question.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 32:
					secondsPerQuestion = 120;
					resetOLED();
					OLED.println("You have 120 seconds to answer each question.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 64:
					// Display instructional message
					resetOLED();
					OLED.setTextSize(2);
					OLED.println("IMPORTANT!");
					OLED.setTextSize(1);
					OLED.println("Turn the knob left/right to decrease/increase the value. Press S0 (back) button whenfinished.");
					OLED.println("        ----->");
					OLED.display();
					delay(8000);
					
					// Loop until broken (infinitely)
					while (true) {
						// Get button and potentiometer values
						delay(250);
						yield();
						buttonValue = digitalRead(buttonPin);
						sensorValue = analogRead(A0);
						
						// If back button pressed, go back to choice menu
						if (buttonValue == 0) {
							backToChoice = true;
							break;
						}
						
						// Set variable to potentiometer value (max variable value 360)
						secondsPerQuestion = ceil(sensorValue/2.85);
						
						// Display updating confirmation message
						resetOLED();
						OLED.setTextSize(2);
						OLED.println("   --->");
						OLED.println("");
						OLED.setTextSize(1);
						OLED.print("Number of seconds per question: ");
						OLED.print(secondsPerQuestion);
						OLED.display();
					}
					break;
				default:
					break;
			}
		}
	}
}


// Configure setting - Number of seconds to review each result
void setSecondsToReview() {
	// Declare local variables
	bool backToMenu = false;
	bool backToChoice;
	int sensorValue = 0;
	
	// Loop until backToMenu is true
	while (backToMenu != true) {
		// Display choice menu
		resetOLED();
		OLED.println("How many seconds to review each result?");
		OLED.println("S1 - 0   |  S2 - 15");
		OLED.println("S3 - 30  |  S4 - 45");
		OLED.println("S5 - 60  |  S6 - 90");
		OLED.println("S7 - Custom (1-360)");
		OLED.display();
		
		backToChoice = false;
		
		// Loop until backToChoice is true
		while (backToChoice != true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// If back button pressed, go back to main menu
			if (buttonValue == 0) {
				backToMenu = true;
				break;
			}
			
			// Depending on what expansion button is pressed, set variable to respective value and display confirmation message
			// Then go back to choice menu
			switch(expButtons) {
				case 1:
					secondsToReview = 0;
					resetOLED();
					OLED.println("Question result review will be skipped.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 2:
					secondsToReview = 15;
					resetOLED();
					OLED.println("You have 15 seconds to review your question result.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 4:
					secondsToReview = 30;
					resetOLED();
					OLED.println("You have 30 seconds to review your question result.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 8:
					secondsToReview = 45;
					resetOLED();
					OLED.println("You have 45 seconds to review your question result.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 16:
					secondsToReview = 60;
					resetOLED();
					OLED.println("You have 60 seconds to review your question result.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 32:
					secondsToReview = 90;
					resetOLED();
					OLED.println("You have 90 seconds to review your question result.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 64:
					// Display instructional message
					resetOLED();
					OLED.setTextSize(2);
					OLED.println("IMPORTANT!");
					OLED.setTextSize(1);
					OLED.println("Turn the knob left/right to decrease/increase the value. Press S0 (back) button whenfinished.");
					OLED.println("        ----->");
					OLED.display();
					delay(8000);
					
					// Loop until broken (infinitely)
					while (true) {
						// Get button and potentiometer values
						delay(250);
						yield();
						buttonValue = digitalRead(buttonPin);
						sensorValue = analogRead(A0);
						
						// If back button pressed, go back to choice menu
						if (buttonValue == 0) {
							backToChoice = true;
							break;
						}
						
						// Set variable to potentiometer value (max variable value 360)
						secondsToReview = ceil(sensorValue/2.85);
						
						// Display updating confirmation message
						resetOLED();
						OLED.setTextSize(2);
						OLED.println("   --->");
						OLED.println("");
						OLED.setTextSize(1);
						OLED.print("Number of seconds to review each result:  ");
						OLED.print(secondsToReview);
						OLED.display();
					}
					break;
				default:
					break;
			}
		}
	}
}


// Configure setting - Highlight and review any result mistakes
void setHighlightMistakes() {
	// Declare local variables
	bool backToMenu = false;
	bool backToChoice;
	
	// Loop until backToMenu is true
	while (backToMenu != true) {
		// Display choice menu
		resetOLED();
		OLED.println("Do you want to highlight your mistakes?");
		OLED.println("S1 - Yes  |  S2 - No");
		OLED.display();
		
		backToChoice = false;
		
		// Loop until backToChoice is true
		while (backToChoice != true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// If back button pressed, go back to main menu
			if (buttonValue == 0) {
				backToMenu = true;
				break;
			}
			
			// Depending on what expansion button is pressed, set variable to respective value and display confirmation message
			// Then go back to choice menu
			switch(expButtons) {
				case 1:
					highlightMistakes = true;
					resetOLED();
					OLED.println("Your mistakes will be highlighted.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 2:
					highlightMistakes = false;
					resetOLED();
					OLED.println("Your mistakes will not be highlighted.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				default:
					break;
			}
		}
	}
}


// Configure setting - 7-segment display start mode each question
void setSegmentDisplayStart() {
	// Declare local variables
	bool backToMenu = false;
	bool backToChoice;
	
	// Loop until backToMenu is true
	while (backToMenu != true) {
		// Display choice menu
		resetOLED();
		OLED.println("How do you want the 7-segments to start?");
		OLED.println("S1 - 0s (00000000)");
		OLED.println("S2 - 1s (11111111)");
		OLED.display();
		
		backToChoice = false;
		
		// Loop until backToChoice is true
		while (backToChoice != true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// If back button pressed, go back to main menu
			if (buttonValue == 0) {
				backToMenu = true;
				break;
			}
			
			// Depending on what expansion button is pressed, set variable to respective value and display confirmation message
			// Then go back to choice menu
			switch(expButtons) {
				case 1:
					segmentDisplayStart = 0b00000000;
					resetOLED();
					OLED.println("7-segment display will start as 00000000.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				case 2:
					segmentDisplayStart = 0b11111111;
					resetOLED();
					OLED.println("7-segment display will start as 11111111.");
					OLED.display();
					delay(2000);
					backToChoice = true;
					break;
				default:
					break;
			}
		}
	}
}



// Play addition gamemode
void playAdditionMode() {
	// Declare local variables
	bool nextQuestion;
	time_t questionStartTime; 									// Time that question started
	time_t lastMovementTime;									// Time the servo position was last changed
	double secondsSinceStart;
	double secondsSinceMovement;
	double servoPosition;										// Servo position (°)
	double positionIncrement = 180.0/secondsPerQuestion;		// Servo can only move 180° in any direction
	double numCorrect = 0.0;									// Number of questions answered correctly
	double percentCorrect = 0.0;								// That number as a percentage
	uint8_t compAnswer;
	uint8_t userAnswer;
	
	srand(time(0));												// Seed rand() with current time
	int firstNum = rand() % 256; 								// Generate random first number (values 0-255)
	int secondNum;
	int maxValue;
	
	// Display splash intro 
	resetOLED();
	OLED.setTextSize(2);
	OLED.println("Addition Mode");
	OLED.println("");
	OLED.setTextSize(1);
	OLED.println("Just add the numbers.");
	OLED.display();
	delay(5000);
	
	// Loop for number of questions
	for (int i = 0; i < numQuestions; i++) {
		nextQuestion = false;
		
		// Display preparation message
		resetOLED();
		OLED.setTextSize(2);
		OLED.println("Let's Go!");
		OLED.println("");
		OLED.setTextSize(1);
		OLED.println("Next question in...");
		OLED.display();
		// Display seconds countdown
		for (int j = 5; j > 0; j--) {
			OLED.print(j);
			OLED.print("  ");
			OLED.display();
			delay(1000);
		}
		
		// Generate and calculate sum
		srand(time(0));
		maxValue = 255 - firstNum;								// Maximum possible value that can be added onto firstNum, so that it doesn't go above 255
		secondNum = rand() % (maxValue + 1);					// Generate random second number (values 0-255)
		compAnswer = firstNum + secondNum;						// Calculate sum answer
		
		// Initalise variables and setup components
		questionStartTime = time(0);
		servoPosition = 0;										// Initalise servoPosition variable
		servo.write(servoPosition);								// Reset servo position (0)
		lastMovementTime = time(0);
		userAnswer = segmentDisplayStart;						// Initalise userAnswer
		expBoard.setDisplayToBinNumber(userAnswer,0);			// Setup 7-segment display
		expBoard.setLEDs(0b00000000);							// Turn LEDs off
		
		// Display current question and sum
		resetOLED();
		OLED.print("Question #");
		OLED.print(i+1);
		OLED.print("          ");
		OLED.println("---------------------");
		OLED.print(firstNum);
		OLED.print(" + ");
		OLED.print(secondNum);
		OLED.print(" = ?");
		OLED.display();
		
		// Loop until nextQuestion is true
		while (nextQuestion != true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// Calculate time difference from current time
			secondsSinceStart = difftime(time(0),questionStartTime);
			secondsSinceMovement = difftime(time(0),lastMovementTime);
			
			// Check if time is up, if it is...
			if (secondsSinceStart > secondsPerQuestion) {		// Using > rather than == to attempt to account for any lost time due to delay()
				// Display time up message
				resetOLED();
				OLED.setTextSize(2);
				OLED.println("Oh Dear!");
				OLED.println("Time's up ...");
				OLED.display();
				delay(5000);
				nextQuestion = true;							// Move on to next question
			}
			// Otherwise...
			else {
				// If 1 second has passed since the servo last moved, adjust (increment) position
				if (secondsSinceMovement == 1) {
					servoPosition += positionIncrement;
					servo.write(servoPosition);
					lastMovementTime = time(0);
				}
				
				// If submit button pressed...
				if (buttonValue == 0) {
					// If user's answer is correct...
					if (userAnswer == compAnswer) {
						// Display correct answer message
						resetOLED();
						OLED.setTextSize(2);
						OLED.println("Nice Work!");
						OLED.println("Correct   answer :D");
						OLED.display();
						numCorrect += 1;						// Add one to the counter of correct answers
						delay(5000);
					}
					// Otherwise (if incorrect)...
					else {
						// Display wrong answer message
						resetOLED();
						OLED.setTextSize(2);
						OLED.println("Oh Dear!");
						OLED.println("Wrong     answer :(");
						OLED.display();
						
						// If user has chosen to highlight their mistakes (setting)...
						if (highlightMistakes == true) {
							// Display let's review message
							OLED.setTextSize(1);
							OLED.println("Let's review...");
							OLED.display();
							delay(2000);
							
							// Display review message
							resetOLED();
							OLED.setTextSize(2);
							OLED.println("Review...");
							OLED.setTextSize(1);
							OLED.print("Question: ");
							OLED.print(firstNum);
							OLED.print(" + ");
							OLED.print(secondNum);
							OLED.print(".   Answer: ");
							OLED.print(compAnswer);
							OLED.print(".   Your answer: ");
							OLED.print(userAnswer);
							OLED.display();
							
							// Highlight individual bit mistakes using LEDs
							uint8_t mistakes = userAnswer ^ compAnswer;			// 8-bit byte, each bit 0 if correct or 1 if incorrect
							uint8_t ledPattern = mistakes;
							for (int k = sizeof(mistakes) * 8-1; k; k--) {		// Algorithm to reverse the bits in the byte (required as LED displaying works "backwards")
								ledPattern <<= 1;
								mistakes  >>= 1;
								ledPattern |=  mistakes & 1;
							}
							expBoard.setLEDs(ledPattern);
						}
						
						// Allow time to review result (however long user set)
						delay(secondsToReview*1000);			// Delay needs to be in milliseconds, not seconds
					}
					
					// Generate new random first number for next question and move on
					srand(time(0));
					firstNum = rand() % 256;
					nextQuestion = true;
				}
				
				// If expansion button(s) is currently being pressed
				if (expButtons != 0) {
					// Depending on what one is pressed, manipulate the corresponding bit (using bitwise operations)
					switch(expButtons) {
						case 1:
							userAnswer ^= 0b10000000;
							break;
						case 2:
							userAnswer ^= 0b01000000;
							break;
						case 4:
							userAnswer ^= 0b00100000;
							break;
						case 8:
							userAnswer ^= 0b00010000;
							break;
						case 16:
							userAnswer ^= 0b00001000;
							break;
						case 32:
							userAnswer ^= 0b00000100;
							break;
						case 64:
							userAnswer ^= 0b00000010;
							break;
						case 128:
							userAnswer ^= 0b00000001;
							break;
						default:
							break;
					}
					
					// Update 7-segment display
					expBoard.setDisplayToBinNumber(userAnswer,0);
				}
			}
		}	
	}
	// Calculate percentage of questions user got correct
	percentCorrect = round((numCorrect/numQuestions)*100);
	
	// Display game over message
	resetOLED();
	OLED.setTextSize(2);
	OLED.println("Game Over");
	OLED.println("");
	OLED.setTextSize(1);
	OLED.print("You got ");
	OLED.print((int)numCorrect);								// Display the double as an integer
	OLED.print(" out of ");
	OLED.print(numQuestions);
	OLED.print(" questions correct. That's ");
	OLED.print(percentCorrect);
	OLED.print("%!");
	OLED.display();
	delay(10000);
	
	// Reset components
	servo.write(0);
	expBoard.clearDisplay();
	expBoard.setLEDs(0b00000000);
	OLED.setTextSize(1);
}


// Play subtraction gamemode
void playSubtractionMode() {
	// Declare local variables
	bool nextQuestion;
	time_t questionStartTime;
	time_t lastMovementTime;
	double secondsSinceStart;
	double secondsSinceMovement;
	double servoPosition;
	double positionIncrement = 180.0/secondsPerQuestion;	
	double numCorrect = 0.0;
	double percentCorrect = 0.0;
	uint8_t compAnswer;
	uint8_t userAnswer;
	
	srand(time(0));
	int firstNum = rand() % 256;
	int secondNum;
	
	// Display splash intro
	resetOLED();
	OLED.setTextSize(2);
	OLED.println("Subtraction Mode");
	OLED.println("");
	OLED.setTextSize(1);
	OLED.println("Just subtract the numbers.");
	OLED.display();
	delay(5000);
	
	// Loop for number of questions
	for (int i = 0; i < numQuestions; i++) {
		nextQuestion = false;
		
		// Display preparation message
		resetOLED();
		OLED.setTextSize(2);
		OLED.println("Let's Go!");
		OLED.println("");
		OLED.setTextSize(1);
		OLED.println("Next question in...");
		OLED.display();
		// Display seconds countdown
		for (int j = 5; j > 0; j--) {
			OLED.print(j);
			OLED.print("  ");
			OLED.display();
			delay(1000);
		}
		
		// Generate and calculate sum
		srand(time(0));
		secondNum = rand() % (firstNum + 1);		// Generate random second number (values 0-firstNum), so answer is always above 0
		compAnswer = firstNum - secondNum;			// Calculate sum answer
		
		// Initalise variables and setup components
		questionStartTime = time(0);
		servoPosition = 0;
		servo.write(servoPosition);
		lastMovementTime = time(0);
		userAnswer = segmentDisplayStart;
		expBoard.setDisplayToBinNumber(userAnswer,0);
		expBoard.setLEDs(0b00000000);
		
		// Display current question and sum
		resetOLED();
		OLED.print("Question #");
		OLED.print(i+1);
		OLED.print("          ");
		OLED.println("---------------------");
		OLED.print(firstNum);
		OLED.print(" - ");
		OLED.print(secondNum);
		OLED.print(" = ?");
		OLED.display();
		
		// Loop until nextQuestion is true
		while (nextQuestion != true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// Calculate time difference from current time
			secondsSinceStart = difftime(time(0),questionStartTime);
			secondsSinceMovement = difftime(time(0),lastMovementTime);
			
			// Check if time is up, if it is...
			if (secondsSinceStart > secondsPerQuestion) {
				// Display time up message
				resetOLED();
				OLED.setTextSize(2);
				OLED.println("Oh Dear!");
				OLED.println("Time's up ...");
				OLED.display();
				delay(5000);
				nextQuestion = true;
			}
			// Otherwise...
			else {
				// If 1 second has passed since the servo last moved, adjust (increment) position
				if (secondsSinceMovement == 1) {
					servoPosition += positionIncrement;
					servo.write(servoPosition);
					lastMovementTime = time(0);
				}
				
				// If submit button pressed...
				if (buttonValue == 0) {
					// If user's answer is correct...
					if (userAnswer == compAnswer) {
						// Display correct answer message
						resetOLED();
						OLED.setTextSize(2);
						OLED.println("Nice Work!");
						OLED.println("Correct   answer :D");
						OLED.display();
						numCorrect += 1;
						delay(5000);
					}
					// Otherwise (if incorrect)...
					else {
						// Display wrong answer message
						resetOLED();
						OLED.setTextSize(2);
						OLED.println("Oh Dear!");
						OLED.println("Wrong     answer :(");
						OLED.display();
						
						// If user has chosen to highlight their mistakes (setting)...
						if (highlightMistakes == true) {
							// Display let's review message
							OLED.setTextSize(1);
							OLED.println("Let's review...");
							OLED.display();
							delay(2000);
							
							// Display review message						
							resetOLED();
							OLED.setTextSize(2);
							OLED.println("Review...");
							OLED.setTextSize(1);
							OLED.print("Question: ");
							OLED.print(firstNum);
							OLED.print(" - ");
							OLED.print(secondNum);
							OLED.print(".   Answer: ");
							OLED.print(compAnswer);
							OLED.print(".   Your answer: ");
							OLED.print(userAnswer);
							OLED.display();
							
							// Highlight individual bit mistakes using LEDs
							uint8_t mistakes = userAnswer ^ compAnswer;
							uint8_t ledPattern = mistakes;
							for (int k = sizeof(mistakes) * 8-1; k; k--) {
								ledPattern <<= 1;
								mistakes  >>= 1;
								ledPattern |=  mistakes & 1;
							}
							expBoard.setLEDs(ledPattern);
						}
						
						// Allow time to review result (however long user set)
						delay(secondsToReview*1000);
					}
					
					// Generate new random first number for next question and move on
					srand(time(0));
					firstNum = rand() % 256;
					nextQuestion = true;
				}
				
				// If expansion button(s) is currently being pressed
				if (expButtons != 0) {
					// Depending on what one is pressed, manipulate the corresponding bit (using bitwise operations)
					switch(expButtons) {
						case 1:
							userAnswer ^= 0b10000000;
							break;
						case 2:
							userAnswer ^= 0b01000000;
							break;
						case 4:
							userAnswer ^= 0b00100000;
							break;
						case 8:
							userAnswer ^= 0b00010000;
							break;
						case 16:
							userAnswer ^= 0b00001000;
							break;
						case 32:
							userAnswer ^= 0b00000100;
							break;
						case 64:
							userAnswer ^= 0b00000010;
							break;
						case 128:
							userAnswer ^= 0b00000001;
							break;
						default:
							break;
					}
					
					// Update 7-segment display
					expBoard.setDisplayToBinNumber(userAnswer,0);
				}
			}
		}	
	}
	// Calculate percentage of questions user got correct
	percentCorrect = round((numCorrect/numQuestions)*100);
	
	// Display game over message
	resetOLED();
	OLED.setTextSize(2);
	OLED.println("Game Over");
	OLED.println("");
	OLED.setTextSize(1);
	OLED.print("You got ");
	OLED.print((int)numCorrect);
	OLED.print(" out of ");
	OLED.print(numQuestions);
	OLED.print(" questions correct. That's ");
	OLED.print(percentCorrect);
	OLED.print("%!");
	OLED.display();
	delay(10000);
	
	// Reset components
	servo.write(0);
	expBoard.clearDisplay();
	expBoard.setLEDs(0b00000000);
	OLED.setTextSize(1);
}


// Play multiplication gamemode
void playMultiplicationMode() {
	// Declare local variables
	bool nextQuestion;
	time_t questionStartTime;
	time_t lastMovementTime;
	double secondsSinceStart;
	double secondsSinceMovement;
	double servoPosition;
	double positionIncrement = 180.0/secondsPerQuestion;	
	double numCorrect = 0.0;
	double percentCorrect = 0.0;	
	uint8_t compAnswer;
	uint8_t userAnswer;
	
	srand(time(0));
	int randomNum = rand() % 255 + 1; 								//Generate random number (answer), values 1-255
	int firstFactor;
	int secondFactor;
	int newFactor = 1;
	int highestFactor = 1;
	
	// Display splash intro
	resetOLED();
	OLED.setTextSize(2);
	OLED.println("Multiplication Mode");
	OLED.println("");
	OLED.setTextSize(1);
	OLED.println("Just multiply the numbers.");
	OLED.display();
	delay(5000);
	
	// Loop for number of questions
	for (int i = 0; i < numQuestions; i++) {
		nextQuestion = false;
		
		// Display preparation message
		resetOLED();
		OLED.setTextSize(2);
		OLED.println("Let's Go!");
		OLED.println("");
		OLED.setTextSize(1);
		OLED.println("Next question in...");
		OLED.display();
		// Display seconds countdown
		for (int j = 5; j > 0; j--) {
			OLED.print(j);
			OLED.print("  ");
			OLED.display();
			delay(1000);
		}
		
		// Generate and calculate sum
		// (Since we can only use 8-bits to input our binary answer, the number cannot have decimal places etc. 
		//  To ensure this, we need to find out the factors of the answer, as they won't cause any issues)
		for (int l = 1; l <= randomNum; l++) {			// Loop from 1 to randomNum ("answer")
			// If the current index (l) is a factor of the answer...
			if (randomNum % l == 0) {
				newFactor = l;							// Set newFactor to factor (index)

				// If factor is greater than the highest factor and also less than the answer...
				// (To avoid getting 1*answer every time, we aim to just get the highest possible factor)
				if (newFactor > highestFactor && newFactor < randomNum) {
					highestFactor = newFactor;			// Set it as the new highest factor
				}
			}
		}
		firstFactor = highestFactor;					// Set the highest as the first factor
		secondFactor = randomNum / firstFactor;			// Calculate second factor
		compAnswer = randomNum;							// compAnswer = "answer" (randomNum)
		
		// Initalise variables and setup components
		questionStartTime = time(0);
		servoPosition = 0;
		servo.write(servoPosition);
		lastMovementTime = time(0);
		userAnswer = segmentDisplayStart;
		expBoard.setDisplayToBinNumber(userAnswer,0);
		expBoard.setLEDs(0b00000000);
		
		// Display current question and sum
		resetOLED();
		OLED.print("Question #");
		OLED.print(i+1);
		OLED.print("          ");
		OLED.println("---------------------");
		OLED.print(firstFactor);
		OLED.print(" * ");
		OLED.print(secondFactor);
		OLED.print(" = ?");
		OLED.display();
		
		// Loop until nextQuestion is true
		while (nextQuestion != true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// Calculate time difference from current time
			secondsSinceStart = difftime(time(0),questionStartTime);
			secondsSinceMovement = difftime(time(0),lastMovementTime);
			
			// Check if time is up, if it is...
			if (secondsSinceStart > secondsPerQuestion) {
				// Display time up message
				resetOLED();
				OLED.setTextSize(2);
				OLED.println("Oh Dear!");
				OLED.println("Time's up ...");
				OLED.display();
				delay(5000);
				nextQuestion = true;
			}
			// Otherwise...
			else {
				// If 1 second has passed since the servo last moved, adjust (increment) position
				if (secondsSinceMovement == 1) {
					servoPosition += positionIncrement;
					servo.write(servoPosition);
					lastMovementTime = time(0);
				}
				
				// If submit button pressed...
				if (buttonValue == 0) {
					// If user's answer is correct...
					if (userAnswer == compAnswer) {
						// Display correct answer message
						resetOLED();
						OLED.setTextSize(2);
						OLED.println("Nice Work!");
						OLED.println("Correct   answer :D");
						OLED.display();
						numCorrect += 1;
						delay(5000);
					}
					// Otherwise (if incorrect)...
					else {
						// Display wrong answer message
						resetOLED();
						OLED.setTextSize(2);
						OLED.println("Oh Dear!");
						OLED.println("Wrong     answer :(");
						OLED.display();
						
						// If user has chosen to highlight their mistakes (setting)...
						if (highlightMistakes == true) {
							// Display let's review message
							OLED.setTextSize(1);
							OLED.println("Let's review...");
							OLED.display();
							delay(2000);
							
							// Display review message
							resetOLED();
							OLED.setTextSize(2);
							OLED.println("Review...");
							OLED.setTextSize(1);
							OLED.print("Question: ");
							OLED.print(firstFactor);
							OLED.print(" * ");
							OLED.print(secondFactor);
							OLED.print(".   Answer: ");
							OLED.print(randomNum);
							OLED.print(".   Your answer: ");
							OLED.print(userAnswer);
							OLED.display();
							
							// Highlight individual bit mistakes using LEDs
							uint8_t mistakes = userAnswer ^ compAnswer;
							uint8_t ledPattern = mistakes;
							for (int k = sizeof(mistakes) * 8-1; k; k--) {
								ledPattern <<= 1;
								mistakes  >>= 1;
								ledPattern |=  mistakes & 1;
							}
							expBoard.setLEDs(ledPattern);
						}
						
						// Allow time to review result (however long user set)
						delay(secondsToReview*1000);
					}
					// Generate new random number for next question, re-initalise variables and move on
					srand(time(0));
					randomNum = rand() % 255 + 1;
					newFactor = 1;
					highestFactor = 1;
					nextQuestion = true;
				}
				
				// If expansion button(s) is currently being pressed
				if (expButtons != 0) {
					// Depending on what one is pressed, manipulate the corresponding bit (using bitwise operations)
					switch(expButtons) {
						case 1:
							userAnswer ^= 0b10000000;
							break;
						case 2:
							userAnswer ^= 0b01000000;
							break;
						case 4:
							userAnswer ^= 0b00100000;
							break;
						case 8:
							userAnswer ^= 0b00010000;
							break;
						case 16:
							userAnswer ^= 0b00001000;
							break;
						case 32:
							userAnswer ^= 0b00000100;
							break;
						case 64:
							userAnswer ^= 0b00000010;
							break;
						case 128:
							userAnswer ^= 0b00000001;
							break;
						default:
							break;
					}
					
					// Update 7-segment display
					expBoard.setDisplayToBinNumber(userAnswer,0);
				}
			}
		}	
	}
	// Calculate percentage of questions user got correct
	percentCorrect = round((numCorrect/numQuestions)*100);
	
	// Display game over message
	resetOLED();
	OLED.setTextSize(2);
	OLED.println("Game Over");
	OLED.println("");
	OLED.setTextSize(1);
	OLED.print("You got ");
	OLED.print((int)numCorrect);
	OLED.print(" out of ");
	OLED.print(numQuestions);
	OLED.print(" questions correct. That's ");
	OLED.print(percentCorrect);
	OLED.print("%!");
	OLED.display();
	delay(10000);
	
	// Reset components
	servo.write(0);
	expBoard.clearDisplay();
	expBoard.setLEDs(0b00000000);
	OLED.setTextSize(1);
}


// Play division gamemode
void playDivisionMode() {
	// Declare local variables
	bool nextQuestion;
	time_t questionStartTime;
	time_t lastMovementTime;
	double secondsSinceStart;
	double secondsSinceMovement;
	double servoPosition;
	double positionIncrement = 180.0/secondsPerQuestion;	
	double numCorrect = 0.0;
	double percentCorrect = 0.0;
	uint8_t compAnswer;
	uint8_t userAnswer;
	
	srand(time(0));
	int randomNum = rand() % 255 + 1;
	int firstFactor;
	int secondFactor;
	int newFactor = 1;
	int highestFactor = 1;
	
	// Display splash intro
	resetOLED();
	OLED.setTextSize(2);
	OLED.println("Division Mode");
	OLED.println("");
	OLED.setTextSize(1);
	OLED.println("Just divide the numbers.");
	OLED.display();
	delay(5000);
	
	// Loop for number of questions
	for (int i = 0; i < numQuestions; i++) {
		nextQuestion = false;
		
		// Display preparation message
		resetOLED();
		OLED.setTextSize(2);
		OLED.println("Let's Go!");
		OLED.println("");
		OLED.setTextSize(1);
		OLED.println("Next question in...");
		OLED.display();
		// Display seconds countdown
		for (int j = 5; j > 0; j--) {
			OLED.print(j);
			OLED.print("  ");
			OLED.display();
			delay(1000);
		}
		
		// Generate and calculate sum
		// (Factors required for division like with the multiplication for same reasons)
		for (int l = 1; l <= randomNum; l++) {
			if (randomNum % l == 0) {
				newFactor = l;

				if (newFactor > highestFactor && newFactor < randomNum) {
					highestFactor = newFactor;
				}
			}
		}
		firstFactor = highestFactor;
		secondFactor = randomNum / firstFactor;			
		compAnswer = firstFactor;						// This time compAnswer = firstFactor
		
		// Initalise variables and setup components
		questionStartTime = time(0);
		servoPosition = 0;
		servo.write(servoPosition);
		lastMovementTime = time(0);
		userAnswer = segmentDisplayStart;
		expBoard.setDisplayToBinNumber(userAnswer,0);
		expBoard.setLEDs(0b00000000);
		
		// Display current question and sum
		resetOLED();
		OLED.print("Question #");
		OLED.print(i+1);
		OLED.print("          ");
		OLED.println("---------------------");
		OLED.print(randomNum);
		OLED.print(" / ");
		OLED.print(secondFactor);						// More user-friendly to divide by smaller factor (answer remains the same)
		OLED.print(" = ?");
		OLED.display();
		
		// Loop until nextQuestion is true
		while (nextQuestion != true) {
			// Get button values
			delay(250);
			yield();
			buttonValue = digitalRead(buttonPin);
			expButtons = expBoard.getButtons();
			
			// Calculate time difference from current time
			secondsSinceStart = difftime(time(0),questionStartTime);
			secondsSinceMovement = difftime(time(0),lastMovementTime);
			
			// Check if time is up, if it is...
			if (secondsSinceStart > secondsPerQuestion) {
				// Display time up message
				resetOLED();
				OLED.setTextSize(2);
				OLED.println("Oh Dear!");
				OLED.println("Time's up ...");
				OLED.display();
				delay(5000);
				nextQuestion = true;
			}
			// Otherwise...
			else {
				// If 1 second has passed since the servo last moved, adjust (increment) position
				if (secondsSinceMovement == 1) {
					servoPosition += positionIncrement;
					servo.write(servoPosition);
					lastMovementTime = time(0);
				}
				
				// If submit button pressed...
				if (buttonValue == 0) {
					// If user's answer is correct...
					if (userAnswer == compAnswer) {
						// Display correct answer message
						resetOLED();
						OLED.setTextSize(2);
						OLED.println("Nice Work!");
						OLED.println("Correct   answer :D");
						OLED.display();
						numCorrect += 1;
						delay(5000);
					}
					// Otherwise (if incorrect)...
					else {
						// Display wrong answer message
						resetOLED();
						OLED.setTextSize(2);
						OLED.println("Oh Dear!");
						OLED.println("Wrong     answer :(");
						OLED.display();
						
						// If user has chosen to highlight their mistakes (setting)...
						if (highlightMistakes == true) {
							// Display let's review message
							OLED.setTextSize(1);
							OLED.println("Let's review...");
							OLED.display();
							delay(2000);
							
							// Display review message
							resetOLED();
							OLED.setTextSize(2);
							OLED.println("Review...");
							OLED.setTextSize(1);
							OLED.print("Question: ");
							OLED.print(randomNum);
							OLED.print(" / ");
							OLED.print(secondFactor);
							OLED.print(".   Answer: ");
							OLED.print(firstFactor);
							OLED.print(".   Your answer: ");
							OLED.print(userAnswer);
							OLED.display();
							
							// Highlight individual bit mistakes using LEDs
							uint8_t mistakes = userAnswer ^ compAnswer;
							uint8_t ledPattern = mistakes;
							for (int k = sizeof(mistakes) * 8-1; k; k--) {
								ledPattern <<= 1;
								mistakes  >>= 1;
								ledPattern |=  mistakes & 1;
							}
							expBoard.setLEDs(ledPattern);
						}
						
						// Allow time to review result (however long user set)
						delay(secondsToReview*1000);
					}
					
					// Generate new random number for next question, re-initalise variables and move on
					srand(time(0));
					randomNum = rand() % 255 + 1;
					newFactor = 1;
					highestFactor = 1;
					nextQuestion = true;
				}
				
				// If expansion button(s) is currently being pressed
				if (expButtons != 0) {
					// Depending on what one is pressed, manipulate the corresponding bit (using bitwise operations)
					switch(expButtons) {
						case 1:
							userAnswer ^= 0b10000000;
							break;
						case 2:
							userAnswer ^= 0b01000000;
							break;
						case 4:
							userAnswer ^= 0b00100000;
							break;
						case 8:
							userAnswer ^= 0b00010000;
							break;
						case 16:
							userAnswer ^= 0b00001000;
							break;
						case 32:
							userAnswer ^= 0b00000100;
							break;
						case 64:
							userAnswer ^= 0b00000010;
							break;
						case 128:
							userAnswer ^= 0b00000001;
							break;
						default:
							break;
					}
					
					// Update 7-segment display
					expBoard.setDisplayToBinNumber(userAnswer,0);
				}
			}
		}	
	}
	// Calculate percentage of questions user got correct
	percentCorrect = round((numCorrect/numQuestions)*100);
	
	// Display game over message
	resetOLED();
	OLED.setTextSize(2);
	OLED.println("Game Over");
	OLED.println("");
	OLED.setTextSize(1);
	OLED.print("You got ");
	OLED.print((int)numCorrect);
	OLED.print(" out of ");
	OLED.print(numQuestions);
	OLED.print(" questions correct. That's ");
	OLED.print(percentCorrect);
	OLED.print("%!");
	OLED.display();
	delay(10000);
	
	// Reset components
	servo.write(0);
	expBoard.clearDisplay();
	expBoard.setLEDs(0b00000000);
	OLED.setTextSize(1);
}
