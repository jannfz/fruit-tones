
State state = State::Menu;


uint8_t song = -1;
Mode mode = Mode::Freeplay;


void gameTick() {
    switch (state) {

        case GameState::Menu:

            if (espGetNewGame(song, mode) == Error::NotReady) {
                break;
            }

            if (mode == Mode::ShowSong){
                state = State::ShowSong;
                Serial.println("state = ShowSong");
                break;
            } else if (mode == Mode::PlaySong){
                state = State::PlaySong;
                Serial.println("state = PlaySong");
                break;
            }
            break;

        case GameState::ShowSong:
            delay(1000);
            showSequence();
            state = State::Menu;
            break;


        case GameStates::PlaySong:

            const uint8_t seqLen = Songs[song].len;
            const uint8_t* correctSeq = Songs[song].seq;

            Serial.println("Play the sequence:");

            uint8_t userSeq[seqLen];
            for (int i = 0; i < seqLen; i++) {
                userSeq[i] = waitForButtonPress();
            }
            delay(1000);

            Serial.println("Spiller korrekt sekvens:");
            playSequence(correctSeq, seqLen);
            delay(1000);

            Serial.println("Spiller din sekvens:");
            playSequence(userSeq, seqLen);

            bool allCorrect = true;
            for (int i = 0; i < seqLen; i++) {
                if (userSeq[i] != correctSeq[i]) {
                    allCorrect = false;
                    break;
                }
            }

            if (allCorrect) {
                Serial.println("Korrekt! Du gennemførte niveau " + String(currentLevel));
                currentstate = stopped;
            } else {
                Serial.println("Forkert. Prøv igen.");
                currentstate = show_song;  // vis melodien igen
            }
            break;





            /*
               case play_song: {
            // Vælg den aktive sekvens og dens længde
            int sequence_length = (currentLevel == 1) ? sequence_length_1 : sequence_length_2;
            const int* correctSequence = (currentLevel == 1) ? level1_sequence : level2_sequence;

            // Opret et array til brugerens input
            int userSequence[sequence_length];

            // 1) Lad brugeren gennemspille hele sekvensen
            collectUserSequence(correctSequence, sequence_length, userSequence);

            // 2) Tjek om de var alle rigtige
            bool allCorrect = true;
            for (int i = 0; i < sequence_length; i++) {
            if (userSequence[i] != correctSequence[i]) {
            allCorrect = false;
            break;
            }
            }

            // 3) Vis den korrekte sekvens
            Serial.println("Correct sequence:");
            for (int i = 0; i < sequence_length; i++) {
            Serial.print(fruitNames[ correctSequence[i] ]);
            if (i < sequence_length-1) Serial.print(", ");
            }
            Serial.println();

            // 4) Vis hvad brugeren tastede
            Serial.println("Your sequence:");
            for (int i = 0; i < sequence_length; i++) {
            Serial.print(fruitNames[ userSequence[i] ]);
            if (i < sequence_length-1) Serial.print(", ");
            }
            Serial.println();

            // 5) Giv feedback og skift state
            if (allCorrect) {
            Serial.println("Correct! You completed Level " + String(currentLevel) + "!");
            // her kan du fx advance til næste level, eller sætte state til stopped
            currentstate = stopped;
            }
            else {
            Serial.println("Wrong sequence. Try again.");
            // du kan vælge at gentage play_song eller gå til show_song
            currentstate = show_song;
            }
            }
            break;
             */



            /*
               case play_song:
               if (userPlaysSequence()) {
               Serial.println("Correct! You completed Level " + String(currentLevel) + "!");
               delay(1000);
               if (currentLevel == 1) {
            // currentLevel = 2; // Move to Level 2
            // Serial.println("Moving to Level 2!");
            }
            currentstate = recieve_data; // Go back to stopped state after success
            } else {
            Serial.println("Wrong sequence! Try again.");
            delay(1000);
            showSequence(); // Show the sequence again
            }
            break;
             */



            /*
               case stopped:
               if (digitalRead(start_button) == LOW) {
               digitalWrite(LED_GREEN, HIGH);
               showSequence();  // Function that plays the correct sequence
               currentstate = recieve_data;
               }
               break;
             */
    }
}



void showSequence() {
    Serial.println("Watch the sequence:");
    int sequence_length = (currentLevel == 1) ? sequence_length_1 : sequence_length_2;
    const int* sequence = (currentLevel == 1) ? level1_sequence : level2_sequence;
    /*
       for (int i = 0; i < sequence_length; i++) {
       int index = sequence[i];
       Serial.print(sequence[i]);
       Serial.print(": ");
       Serial.println(fruitNames[index]);
       tone(buzzer, tones[index], 500);   // plays the frequency from the sequence
       delay(600);
       noTone(buzzer);  // turn off buzzer
       delay(400);
       }
       Serial.println();
     */
    for (int i = 0; i < sequence_length; i++) {
        int index = sequence[i];
        const char* name = fruitNames[index];

        // 1) Print til USB-Serial (debug)
        Serial.print(index);
        Serial.print(": ");
        Serial.println(name);

        // 2) Send over til ESP
        espSerial.print(index);       // eller direkte navnet:
        espSerial.print(": ");
        espSerial.println(name);

        // 3) Afspil tonen
        tone(buzzer, tones[index], 500);
        delay(600);
        noTone(buzzer);
        delay(400);
    }
    Serial.println();
    espSerial.println();  // send evt. en tom linje som “end-of-seq” marker
}







/*
   bool userPlaysSequence() {
   Serial.println("Play the sequence:");
   int sequence_length = (currentLevel == 1) ? sequence_length_1 : sequence_length_2;
   const int* sequence = (currentLevel == 1) ? level1_sequence : level2_sequence;

   for (int i = 0; i < sequence_length; i++) {
   int buttonPressed = waitForButtonPress();
   if (buttonPressed != sequence[i]) {
   return false; // Incorrect input
   }
   }
   return true; // User completed sequence correctly
   }
 */


// Indsaml brugerens input til et givet sequence-array
void collectUserSequence(const int* sequence, int sequence_length, int* userSequence) {
    Serial.println("Play the sequence:");
    for (int i = 0; i < sequence_length; i++) {
        userSequence[i] = waitForButtonPress();
    }
}



int waitForButtonPress() {
    while (true) {
        for (int i = 0; i < BUTTON_COUNT; i++) {
            if (digitalRead(buttons[i]) == LOW) {
                tone(buzzer, tones[i], 300);
                delay(300);
                noTone(buzzer);
                while (digitalRead(buttons[i]) == LOW); // Wait for release
                return i;
            }
        }
    }
}



// Afspil en given sekvens (array af indekser) med buzzer
void playSequence(const int* seq, int len) {
    for (int i = 0; i < len; i++) {
        int idx = seq[i];
        Serial.print(Fruits[idx].name);      // Printer navnet (valgfrit)
        Serial.print("  ");
        tone(BUZZER, Notes[idx].freq, 500);      // Spil tone
        delay(600);
        noTone(buzzer);
        delay(400);
    }
    Serial.println();
}


