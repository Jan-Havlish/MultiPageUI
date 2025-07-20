/**
 * @file ExampleUI.ino
 * @brief This is an example sketch demonstrating how to use the MultiPageUI library.
 *
 * It recreates the full functionality of the original single-file application,
 * but leverages the library for a much cleaner and more organized structure.
 *
 * The main sketch is now responsible for:
 * 1. Defining the widgets (buttons, labels, etc.).
 * 2. Defining the callback functions for those widgets.
 * 3. Arranging the widgets into page grids.
 * 4. Creating Page objects from those grids.
 * 5. Initializing the library and adding the pages to the PageManager.
 * 6. Calling the library's handlers in the main loop.
 */

#include <Arduino.h>
#include "MultiPageUI.h" // Include the new library

// Use the library's namespace to make code cleaner, just like the original
using namespace MultiPageUI;

// Define constants if not defined in the library
#ifndef TOTAL_ROWS
#define TOTAL_ROWS 8
#endif
#ifndef COLS
#define COLS 3
#endif

// --- Forward Declarations for Widget Handlers ---
void printStates();
void changeTitle();
void changeButtonText();
void changeThemeToRed();
void changeThemeToBlue();
void changeThemeToDefault();
void changeThemeToGreen();

// --- Widget Instances ---
// We define all the widgets that will appear on our pages.
// These are the "data" of our UI.

// Home Page Widgets
Label homeTitle("Home");
Label section1("Controls");
Label section2("Themes");
Label section3("Navigation");
Button btn1("Status", printStates);
Button btn2("Hello", changeButtonText);
Button btn3("Red", changeThemeToRed);
Button btn4("Blue", changeThemeToBlue);
Button btn5("Default", changeThemeToDefault);
Button btn6("Green", changeThemeToGreen);
RadioButton r1("Option 1"), r2("Option 2"), r3("Option 3");
CheckBox cb1("Feature A"), cb2("Feature B"), cb3("Feature C");
Link settingsLink("Settings", "settings");
Link aboutLink("About", "about");
Link nextPageLink("Next", "/next");

// Settings Page Widgets
Label settingsTitle("Settings");
Label configSection("Configuration");
Label optionsSection("Options");
Button btn7("Save", [](){ Serial.println("Save pressed"); });
Button btn8("Reset", [](){ Serial.println("Reset pressed"); });
RadioButton setting1("Mode A"), setting2("Mode B"), setting3("Mode C");
CheckBox option1("Auto Save"), option2("Debug Mode"), option3("Verbose");
Link homeLink("Home", "home");
Link advancedLink("Advanced", "advanced");
Link backLink("Back", "/back");

// About Page Widgets
Label aboutTitle("About");
Label versionLabel("Version 2.0");
Label authorLabel("Multi-Page UI");
Label infoSection("Information");
Button btn9("Test", [](){ Serial.println("Test pressed"); });
Button btn10("Debug", [](){ Serial.println("Debug pressed"); });
CheckBox cb4("Show Tips"), cb5("Auto Update");
Link homeLink2("Home", "home");
Link settingsLink2("Settings", "settings");

// Advanced Page Widgets
Label advancedTitle("Advanced");
Label systemSection("System");
Label debugSection("Debug");
RadioButton mode1("Fast"), mode2("Normal"), mode3("Slow");
CheckBox debug1("Trace"), debug2("Profile"), debug3("Monitor");
Button resetBtn("Reset All", []() { Serial.println("Reset All pressed"); });
Button exportBtn("Export", []() { Serial.println("Export pressed"); });
Link settingsBackLink("Settings", "settings");
Link homeLink3("Home", "home");


// --- Page Layout Grids ---
// We define the layout for each page by arranging pointers to our widgets in a grid.
// The library uses TOTAL_ROWS and COLS constants internally.

Widget* homeGrid[TOTAL_ROWS][COLS] = {
    { &homeTitle, nullptr, &settingsLink },
    { &section1, nullptr, nullptr },
    { &btn1, nullptr, &btn2 },
    { &r1, &r2, &r3 },
    { &section2, nullptr, nullptr },
    { &btn3, &btn4, &btn5 },
    { &btn6, nullptr, nullptr },
    { &section3, &aboutLink, &nextPageLink }
};

Widget* settingsGrid[TOTAL_ROWS][COLS] = {
    { &settingsTitle, nullptr, &homeLink },
    { &configSection, nullptr, nullptr },
    { &setting1, &setting2, &setting3 },
    { &btn7, nullptr, &btn8 },
    { &optionsSection, nullptr, nullptr },
    { &option1, &option2, &option3 },
    { &advancedLink, nullptr, nullptr },
    { &backLink, nullptr, nullptr }
};

Widget* aboutGrid[TOTAL_ROWS][COLS] = {
    { &aboutTitle, nullptr, &homeLink2 },
    { &infoSection, nullptr, nullptr },
    { &versionLabel, nullptr, &authorLabel },
    { &btn9, nullptr, &btn10 },
    { &cb4, &cb5, nullptr },
    { &settingsLink2, nullptr, nullptr },
    { nullptr, nullptr, nullptr },
    { nullptr, nullptr, nullptr }
};

Widget* advancedGrid[TOTAL_ROWS][COLS] = {
    { &advancedTitle, nullptr, &homeLink3 },
    { &systemSection, nullptr, nullptr },
    { &mode1, &mode2, &mode3 },
    { &debugSection, nullptr, nullptr },
    { &debug1, &debug2, &debug3 },
    { &resetBtn, nullptr, &exportBtn },
    { &settingsBackLink, nullptr, nullptr },
    { nullptr, nullptr, nullptr }
};


// --- Page Objects ---
// Create the Page objects, passing the name and the grid layout.
// The library will handle the rest.

Page homePage("home", homeGrid);
Page settingsPage("settings", settingsGrid);
Page aboutPage("about", aboutGrid);
Page advancedPage("advanced", advancedGrid);


// --- Widget Handler Functions ---
// These are the specific actions for our UI.

int titleClickCount = 0;
void changeTitle() {
    titleClickCount++;
    char newTitle[32];
    snprintf(newTitle, sizeof(newTitle), "Home %d", titleClickCount);
    homeTitle.setText(newTitle); // Note: We can directly access and modify our widget objects
    Serial.print("Title changed to: ");
    Serial.println(newTitle);
}

int buttonClickCount = 0;
void changeButtonText() {
    buttonClickCount++;
    char newText[32];
    snprintf(newText, sizeof(newText), "Click %d", buttonClickCount);
    btn2.setText(newText);
    Serial.print("Button text changed to: ");
    Serial.println(newText);
}

void changeThemeToRed() {
    pageManager.setTheme(&redTheme); // Use the library's predefined theme
    Serial.println("Theme changed to RED");
}
void changeThemeToBlue() {
    pageManager.setTheme(&blueTheme);
    Serial.println("Theme changed to BLUE");
}
void changeThemeToDefault() {
    pageManager.setTheme(&defaultTheme);
    Serial.println("Theme changed to DEFAULT");
}
void changeThemeToGreen() {
    pageManager.setTheme(&greenTheme);
    Serial.println("Theme changed to GREEN");
}

// Custom input handler to include application-specific logic (like clicking the title)
void myCustomInputHandler() {
    Page* currentPage = pageManager.getCurrentPage();
    if (!currentPage) return;

    // Navigation is handled by the Page class methods
    if (digitalRead(WIO_5S_UP) == LOW)    { currentPage->navigateUp(pageManager.selRow, pageManager.selCol);    delay(150); }
    if (digitalRead(WIO_5S_DOWN) == LOW)  { currentPage->navigateDown(pageManager.selRow, pageManager.selCol);  delay(150); }
    if (digitalRead(WIO_5S_LEFT) == LOW)  { currentPage->navigateLeft(pageManager.selRow, pageManager.selCol);  delay(150); }
    if (digitalRead(WIO_5S_RIGHT) == LOW) { currentPage->navigateRight(pageManager.selRow, pageManager.selCol); delay(150); }

    if (digitalRead(WIO_5S_PRESS) == LOW) {
        Widget* w = currentPage->getWidget(pageManager.selRow, pageManager.selCol);
        if (w) {
            // *** Application-specific logic ***
            // The original code had a special case for clicking the title. We add it here.
            if (w == &homeTitle) {
                changeTitle();
            }
            // For all other widgets, we use their standard behavior
            else {
                switch (w->getType()) {
                    case W_RADIO:
                        currentPage->selectRadioInRow(pageManager.selRow, static_cast<RadioButton*>(w));
                        break;
                    case W_CHECKBOX:
                        static_cast<CheckBox*>(w)->toggle();
                        break;
                    default: // Handles Button and Link
                        w->onPress();
                        break;
                }
            }
        }
        delay(200);
    }
}

// Custom serial handler to include text-changing commands
void myCustomSerialHandler() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        // *** Application-specific commands ***
        if (command.startsWith("title:")) {
            String newTitle = command.substring(6);
            homeTitle.setText(newTitle.c_str());
            Serial.println("Title updated via serial: " + newTitle);
        }
        else if (command.startsWith("button:")) {
            String newText = command.substring(7);
            btn2.setText(newText.c_str());
            Serial.println("Button2 updated via serial: " + newText);
        }
        // *** Generic commands handled by the library ***
        else if (command.startsWith("page:")) { pageManager.navigateToPage(command.substring(5).c_str()); }
        else if (command == "theme:red")      { changeThemeToRed(); }
        else if (command == "theme:blue")     { changeThemeToBlue(); }
        else if (command == "theme:green")    { changeThemeToGreen(); }
        else if (command == "theme:default")  { changeThemeToDefault(); }
        else if (command == "back")           { pageManager.goBack(); }
        else if (command == "next")           { pageManager.goNext(); }
        else if (command == "help") {
            Serial.println("=== Serial Commands ===");
            Serial.println("title:YourText       - Change home title text");
            Serial.println("button:YourText      - Change button2 text");
            Serial.println("page:name            - Navigate (home/settings/about/advanced)");
            Serial.println("theme:red/blue/green/default - Change theme");
            Serial.println("back/next            - Navigate between pages");
            Serial.println("help                 - Show this help");
        }
        else {
            Serial.println("Unknown command. Type 'help' for available commands.");
        }
    }
}


// A function to print the current state of various widgets for debugging
void printStates() {
    Serial.println("=== UI States ===");
    Serial.print("Current page: "); Serial.println(pageManager.getCurrentPageName());
    Serial.print("Home title: "); Serial.println(homeTitle.getText());
    Serial.print("Button2 text: "); Serial.println(btn2.getText());
    Serial.print("Radio (Home): ");
    if (r1.isSelected()) Serial.print("Opt1 ");
    if (r2.isSelected()) Serial.print("Opt2 ");
    if (r3.isSelected()) Serial.print("Opt3 ");
    Serial.println();
    Serial.print("Checkboxes (Home): ");
    Serial.print("A:"); Serial.print(cb1.isChecked() ? "ON" : "OFF");
    Serial.print(" B:"); Serial.print(cb2.isChecked() ? "ON" : "OFF");
    Serial.print(" C:"); Serial.println(cb3.isChecked() ? "ON" : "OFF");
    Serial.println("=================");
}


// --- Arduino Standard Functions ---

void setup() {
    Serial.begin(115200);

    // This single function initializes the TFT, sprite, and input pins
    initDisplay();

    // Add all our pages to the page manager
    pageManager.addPage(&homePage);
    pageManager.addPage(&settingsPage);
    pageManager.addPage(&aboutPage);
    pageManager.addPage(&advancedPage);
    
    // Set the initial focus/selection
    pageManager.selRow = 0;
    pageManager.selCol = 0;

    Serial.println("=== MultiPageUI Library Demo Started ===");
    Serial.println("Type 'help' in Serial Monitor for commands.");
}

void loop() {
    // Handle all user input from the 5-way switch
    myCustomInputHandler();

    // Handle any incoming serial commands
    myCustomSerialHandler();
    
    // Get the current page and draw it with the current selection
    Page* currentPage = pageManager.getCurrentPage();
    if (currentPage) {
        currentPage->draw(pageManager.selRow, pageManager.selCol);
    }
}
