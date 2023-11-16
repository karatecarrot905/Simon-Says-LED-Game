#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions
#include "ece198.h"  // header file

// This helper function returns true if the two equally 
// sized arrays passed to it are the same.
bool isEqual(int array1[], int array2[], int cap);

bool isEqual(int array1[], int array2[], int cap) {
    for (int i = 0; i < cap; ++i) {
        if(array1[i] != array2[i]){
            return false;
        }
    } 
    return true;
}


int main(void)
{
    HAL_Init(); // initialize the Hardware Abstraction Layer

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)

    // set up for serial communication to the host computer
    // (anything we write to the serial port will appear in the terminal (i.e. serial monitor) in VSCode)
    SerialSetup(9600);

    // INITIALIZE INPUT AND OUTPUT PINS
    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // Success LED
    InitializePin(GPIOA, GPIO_PIN_6, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // Fail LED

    InitializePin(GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // ORANGE LED
    InitializePin(GPIOC, GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // GREEN LED
    InitializePin(GPIOB, GPIO_PIN_6, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); // BLUE LED

    InitializePin(GPIOA, GPIO_PIN_8, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0); // START Button

    InitializePin(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0); // ORANGE Button
    InitializePin(GPIOB, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0); // GREEN Button
    InitializePin(GPIOB, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0); // BLUE Button

    // CONSTANT VALUES TO EASILY REFERENCE PIN NUMBERS
    uint const LED_SUCCESS = GPIO_PIN_5; // LED_SUCCESS = white LED, LED_FAIL = red LED
    uint const LED_FAIL = GPIO_PIN_6;

    uint const LED_O = GPIO_PIN_9; // LED_O = orange LED, LED_G = green LED, LED_B = blue LED
    uint const LED_G = GPIO_PIN_7;
    uint const LED_B = GPIO_PIN_6;

    uint const START = GPIO_PIN_8; // START BUTTON

    uint const BUT_O = GPIO_PIN_10; // BUT_O = orange button, BUT_G = green button, BUT_B = blue button
    uint const BUT_G = GPIO_PIN_3;
    uint const BUT_B = GPIO_PIN_5;
    
    unsigned int iLevel = 1; // This variable stores the current level of the game, beginning at 1.

    while(!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8))) { // Do nothing when the start button has not been pressed yet

    }
    
    srand(HAL_GetTick());  // set the random seed to be the time in ms that it took to press the button

    unsigned int numTerms = 0; // This stores the number of terms in the current level
    int sequence[7] = {}; // This array holds the randomly generated sequence on each level (sized at 7 because 7 is the maximum number of terms)
    int response[7] = {}; // This array holds the user's response on each level (sized at 7 because 7 is the maximum number of terms)
    int const ORANGECONST = 1; // The constant value corresponding to ORANGE
    int const GREENCONST = 2; // The constant value corresponding to GREEN
    int const BLUECONST = 3; // The constant value corresponding to BLUE
    
    while(iLevel < 4) { // Continue looping until the user has passed the third level (completed the game)
    
        if(iLevel == 1) { // Set the number of terms corresponding to the current level
            numTerms = 5; 
        } else if (iLevel == 2) {
            numTerms = 6;
        } else {
            numTerms = 7;
        }

        for(int i = 0; i < numTerms; ++i) { // Display the colour pattern with the correct number of terms.
            sequence[i] = (rand() % 3) + 1; // Generates an integer and saves it into the current index of the array. 1,2,3 corresponds to OrangeLED, GreenLED, and BlueLED respectively.

            // Displays the LED sequence. depending on the random sequence generated above
            if(sequence[i] == ORANGECONST) { // Blink the orange LED if the current index is the value corresponding to it
                HAL_GPIO_TogglePin(GPIOA, LED_O);
                HAL_Delay(500);
                HAL_GPIO_TogglePin(GPIOA, LED_O);
                HAL_Delay(250);
            } else if(sequence[i] == GREENCONST) { // Blink the green LED if the current index is the value corresponding to it
                HAL_GPIO_TogglePin(GPIOC, LED_G);
                HAL_Delay(500);
                HAL_GPIO_TogglePin(GPIOC, LED_G);
                HAL_Delay(250);
            } else {  // Blink the blue LED is the current index if the value corresponding to it
                HAL_GPIO_TogglePin(GPIOB, LED_B);
                HAL_Delay(500);
                HAL_GPIO_TogglePin(GPIOB, LED_B);
                HAL_Delay(250);
            }

        }
        
        int numResponses = 0; 

        // Accepts 'x' terms from the user where 'x' is the number of terms of the current level. Store result in the "response array"
        while(numResponses < numTerms) { 
            if (HAL_GPIO_ReadPin(GPIOA, BUT_O) || HAL_GPIO_ReadPin(GPIOB, BUT_G) || HAL_GPIO_ReadPin(GPIOB, BUT_B)) { 
                if(HAL_GPIO_ReadPin(GPIOA, BUT_O)) { // Set the current index of the response array to the value corresponding to the ORANGE LED.
                    response[numResponses] = ORANGECONST;
                    HAL_GPIO_TogglePin(GPIOA, LED_O); // The LED Blinks to indicate the feedback has been recieved
                    HAL_Delay(150);
                    HAL_GPIO_TogglePin(GPIOA, LED_O); 
                    HAL_Delay(150);
                } else if(HAL_GPIO_ReadPin(GPIOB, BUT_G)) { // Set the current index of the response array to the value corresponding to the GREEN LED.
                    response[numResponses] = GREENCONST;
                    HAL_GPIO_TogglePin(GPIOC, LED_G);
                    HAL_Delay(150);
                    HAL_GPIO_TogglePin(GPIOC, LED_G);
                    HAL_Delay(150);
                } else if (HAL_GPIO_ReadPin(GPIOB, BUT_B)) { // Set the current index of the response array to the value corresponding to the BLUE LED.
                    response[numResponses] = BLUECONST;
                    HAL_GPIO_TogglePin(GPIOB, LED_B);
                    HAL_Delay(150);
                    HAL_GPIO_TogglePin(GPIOB, LED_B);
                    HAL_Delay(150);
                }
                numResponses++; // Increment numResponses each time a response button has been pushed
            }
        }

        // Checks if the buttons pushed by the user corresponds to the LED's displayed
        // If the user inputted the correct sequence, the white light will blink once to indicate success and move on to the next level
        // If the user inputted the incorrect sequence, the red light will blink once to indicate a fail and restart to level 1
        if (isEqual(response, sequence, numTerms)) {
            HAL_GPIO_TogglePin(GPIOA, LED_SUCCESS);
            HAL_Delay(1000);
            HAL_GPIO_TogglePin(GPIOA, LED_SUCCESS);
            iLevel++;
        } else { 
            HAL_GPIO_TogglePin(GPIOA, LED_FAIL);
            HAL_Delay(1000);
            HAL_GPIO_TogglePin(GPIOA, LED_FAIL);
            iLevel = 1;
        }
    
    }
    
    while(true) { // Blink all the lights to indicate completion of the game
        HAL_GPIO_TogglePin(GPIOA, LED_O);
        HAL_GPIO_TogglePin(GPIOC, LED_G);
        HAL_GPIO_TogglePin(GPIOB, LED_B);
        HAL_GPIO_TogglePin(GPIOA, LED_FAIL);
        HAL_GPIO_TogglePin(GPIOA, LED_SUCCESS); 
        HAL_Delay(1000);
        HAL_GPIO_TogglePin(GPIOA, LED_O);
        HAL_GPIO_TogglePin(GPIOC, LED_G);
        HAL_GPIO_TogglePin(GPIOB, LED_B);
        HAL_GPIO_TogglePin(GPIOA, LED_FAIL);
        HAL_GPIO_TogglePin(GPIOA, LED_SUCCESS); 
        HAL_Delay(1000);
    }


            // THE FOLLOWING CODE IS FOR TESTING/DEBUGGING PURPOSES...
            /* 
            char buff[100];
            for (int i = 0; i < numTerms; i++) {
                sprintf(buff, "%d" , sequence[i]);
                SerialPuts(buff); // transmit the buffer to the host computer's serial monitor in VSCode/PlatformIO
            }

            sprintf(buff, "\n");
            SerialPuts(buff); // transmit the buffer to the host computer's serial monitor in VSCode/PlatformIO

            for (int i = 0; i < numTerms; i++) {
                sprintf(buff, "%d" , response[i]);
                SerialPuts(buff); // transmit the buffer to the host computer's serial monitor in VSCode/PlatformIO
            }

            if (isEqual(response, sequence, numTerms)) {
                sprintf(buff, "THE RESPONSE IS EQUAL!");
                SerialPuts(buff); // transmit the buffer to the host computer's serial monitor in VSCode/PlatformIO
            } else {
                sprintf(buff, "the response is NOT equal!");
                SerialPuts(buff); // transmit the buffer to the host computer's serial monitor in VSCode/PlatformIO
            }
            */

    return 0;
}

// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}

