#include "MultiPageUI.h"

namespace MultiPageUI {

// Layout constants
const int TOTAL_ROWS = 8;
const int VISIBLE_ROWS = 4;
const int COLS = 3;
const int MARGIN = 10;
const int GAP = 5;

// Global TFT objects
TFT_eSPI tft;
TFT_eSprite pageSprite = TFT_eSprite(&tft);

// Color schemes
ColorScheme defaultTheme = {
    TFT_BLACK,    // background
    TFT_WHITE,    // text
    TFT_BLUE,     // button focusBackground
    TFT_YELLOW,   // button focusText
    TFT_DARKGREY, // label focusBackground
    TFT_CYAN,     // label focusText
    TFT_GREEN,    // accent
    TFT_WHITE     // border
};

ColorScheme redTheme = {
    TFT_BLACK,
    TFT_RED,
    TFT_RED,
    TFT_WHITE,
    TFT_DARKGREY,
    TFT_ORANGE,
    TFT_ORANGE,
    TFT_RED
};

ColorScheme blueTheme = {
    TFT_NAVY,
    TFT_CYAN,
    TFT_BLUE,
    TFT_WHITE,
    TFT_DARKGREY,
    TFT_GREEN,
    TFT_GREEN,
    TFT_CYAN
};

ColorScheme greenTheme = {
    TFT_BLACK,
    TFT_GREEN,
    TFT_DARKGREEN,
    TFT_WHITE,
    TFT_DARKGREY,
    TFT_YELLOW,
    TFT_YELLOW,
    TFT_GREEN
};

ColorScheme* currentTheme = &defaultTheme;

// Global page manager
PageManager pageManager;

// =============== Label Implementation ===============
Label::Label(const char* initialText) {
    setText(initialText);
}

void Label::setText(const char* newText) {
    strncpy(text, newText, sizeof(text) - 1);
    text[sizeof(text) - 1] = '\0';
}

const char* Label::getText() const { 
    return text; 
}

void Label::draw(TFT_eSprite &dst, int x, int y, int w, int h, bool focused) {
    dst.setTextDatum(MC_DATUM);
    if (focused) {
        uint16_t labelFocusBg = currentTheme->accent;
        uint16_t labelFocusText = currentTheme->text;
        dst.fillRect(x, y, w, h, labelFocusBg);
        dst.setTextColor(labelFocusText, labelFocusBg);
    } else {
        dst.setTextColor(currentTheme->text, currentTheme->background);
    }
    dst.drawString(text, x + w/2, y + h/2);
}

WidgetType Label::getType() const { 
    return W_LABEL; 
}

// =============== Button Implementation ===============
Button::Button(const char* initialText, void (*handler)()) : handler(handler) {
    setText(initialText);
}

void Button::setText(const char* newText) {
    strncpy(text, newText, sizeof(text) - 1);
    text[sizeof(text) - 1] = '\0';
}

const char* Button::getText() const { 
    return text; 
}

void Button::draw(TFT_eSprite &dst, int x, int y, int w, int h, bool focused) {
    if (focused) {
        dst.fillRect(x, y, w, h, currentTheme->focusBackground);
        dst.setTextColor(currentTheme->focusText, currentTheme->focusBackground);
    } else {
        dst.drawRect(x, y, w, h, currentTheme->border);
        dst.setTextColor(currentTheme->text, currentTheme->background);
    }
    dst.setTextDatum(MC_DATUM);
    dst.drawString(text, x + w/2, y + h/2);
}

void Button::onPress() { 
    if (handler) handler(); 
}

WidgetType Button::getType() const { 
    return W_BUTTON; 
}

// =============== RadioButton Implementation ===============
RadioButton::RadioButton(const char* text, bool selected) : text(text), selected(selected) {}

void RadioButton::draw(TFT_eSprite &dst, int x, int y, int w, int h, bool focused) {
    if (focused) dst.fillRect(x, y, w, h, TFT_DARKGREY);

    dst.drawCircle(x + 10, y + h/2, 8, currentTheme->border);
    if (selected) dst.fillCircle(x + 10, y + h/2, 5, currentTheme->accent);

    dst.setTextDatum(ML_DATUM);
    dst.setTextColor(currentTheme->text, focused ? TFT_DARKGREY : currentTheme->background);
    dst.drawString(text, x + 25, y + h/2);
}

void RadioButton::select() { 
    selected = true; 
}

void RadioButton::deselect() { 
    selected = false; 
}

bool RadioButton::isSelected() const { 
    return selected; 
}

WidgetType RadioButton::getType() const { 
    return W_RADIO; 
}

// =============== CheckBox Implementation ===============
CheckBox::CheckBox(const char* text, bool checked) : text(text), checked(checked) {}

void CheckBox::draw(TFT_eSprite &dst, int x, int y, int w, int h, bool focused) {
    if (focused) dst.fillRect(x, y, w, h, TFT_DARKGREY);

    dst.drawRect(x + 2, y + h/2 - 8, 16, 16, currentTheme->border);
    if (checked) dst.fillRect(x + 4, y + h/2 - 6, 12, 12, currentTheme->accent);

    dst.setTextDatum(ML_DATUM);
    dst.setTextColor(currentTheme->text, focused ? TFT_DARKGREY : currentTheme->background);
    dst.drawString(text, x + 25, y + h/2);
}

void CheckBox::toggle() { 
    checked = !checked; 
}

bool CheckBox::isChecked() const { 
    return checked; 
}

WidgetType CheckBox::getType() const { 
    return W_CHECKBOX; 
}

// =============== Link Implementation ===============
Link::Link(const char* text, const char* route) : text(text), route(route) {}

void Link::draw(TFT_eSprite &dst, int x, int y, int w, int h, bool focused) {
    if (focused) {
        dst.fillRect(x, y, w, h, TFT_DARKGREY);
        dst.setTextColor(currentTheme->focusText, TFT_DARKGREY);
    } else {
        dst.setTextColor(currentTheme->accent, currentTheme->background);
    }
    dst.setTextDatum(MC_DATUM);
    dst.drawString(text, x + w/2, y + h/2);
}

void Link::onPress() {
    if (strcmp(route, "/back") == 0) {
        pageManager.goBack();
    } else if (strcmp(route, "/next") == 0) {
        pageManager.goNext();
    } else {
        const char* pageName = (route[0] == '/') ? route + 1 : route;
        pageManager.navigateToPage(pageName);
    }
}

WidgetType Link::getType() const { 
    return W_LINK; 
}

const char* Link::getRoute() const { 
    return route; 
}

// =============== Page Implementation ===============
Page::Page(const char* pageName, Widget* grid[8][3], ColorScheme* theme) 
    : scrollOffset(0), name(pageName) {
    if (theme) currentTheme = theme;
    for (int r = 0; r < TOTAL_ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            widgets[r][c] = grid[r][c];
        }
    }
}

void Page::setTheme(ColorScheme* theme) {
    currentTheme = theme;
}

const char* Page::getName() const { 
    return name; 
}

void Page::draw(int selRow, int selCol) {
    int cellW = (tft.width() - 2 * MARGIN - (COLS - 1) * GAP) / COLS;
    int cellH = (tft.height() - 2 * MARGIN - (VISIBLE_ROWS - 1) * GAP) / VISIBLE_ROWS;

    pageSprite.fillSprite(currentTheme->background);
    drawScrollIndicator();

    for (int visibleRow = 0; visibleRow < VISIBLE_ROWS; visibleRow++) {
        int actualRow = scrollOffset + visibleRow;
        if (actualRow >= TOTAL_ROWS) break;

        bool isFullRow = (widgets[actualRow][0] != nullptr && widgets[actualRow][1] == nullptr && widgets[actualRow][2] == nullptr);

        for (int c = 0; c < COLS; c++) {
            Widget* w = widgets[actualRow][c];
            if (!w) continue;

            int x, y, w_w, h_h;

            y = MARGIN + visibleRow * (cellH + GAP);
            h_h = cellH;

            if (isFullRow) {
                x = MARGIN;
                w_w = tft.width() - 2 * MARGIN;
            } else {
                x = MARGIN + c * (cellW + GAP);
                w_w = cellW;
            }

            bool focused = (actualRow == selRow && c == selCol);
            w->draw(pageSprite, x, y, w_w, h_h, focused);
        }
    }
    pageSprite.pushSprite(0, 0);
}

void Page::drawScrollIndicator() {
    if (TOTAL_ROWS <= VISIBLE_ROWS) return; 

    int indicatorHeight = tft.height() - 2*MARGIN;
    int thumbHeight = (indicatorHeight * VISIBLE_ROWS) / TOTAL_ROWS;
    int thumbPos = (indicatorHeight - thumbHeight) * scrollOffset / (TOTAL_ROWS - VISIBLE_ROWS);

    pageSprite.drawRect(tft.width() - 8, MARGIN, 6, indicatorHeight, TFT_DARKGREY);
    pageSprite.fillRect(tft.width() - 8, MARGIN + thumbPos, 6, thumbHeight, currentTheme->border);
}

Widget* Page::getWidget(int r, int c) { 
    if (r >= 0 && r < TOTAL_ROWS && c >= 0 && c < COLS) {
        return widgets[r][c];
    }
    return nullptr;
}

void Page::selectRadioInRow(int row, RadioButton* target) {
    for (int c = 0; c < COLS; c++) {
        Widget* w = widgets[row][c];
        if (w && w->getType() == W_RADIO) {
            static_cast<RadioButton*>(w)->deselect();
        }
    }
    target->select();
}

int Page::findLeftmostInRow(int row) {
    if (row < 0 || row >= TOTAL_ROWS) return -1;
    for (int c = 0; c < COLS; c++) {
        if (widgets[row][c] != nullptr) {
            return c;
        }
    }
    return -1; 
}

int Page::findRightmostInRow(int row) {
    if (row < 0 || row >= TOTAL_ROWS) return -1;
    for (int c = COLS - 1; c >= 0; c--) {
        if (widgets[row][c] != nullptr) {
            return c;
        }
    }
    return -1; 
}

void Page::updateScrollPosition(int selRow) {
    if (selRow < scrollOffset) {
        scrollOffset = selRow;
    } else if (selRow >= scrollOffset + VISIBLE_ROWS) {
        scrollOffset = selRow - VISIBLE_ROWS + 1;
    }
    
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > TOTAL_ROWS - VISIBLE_ROWS) {
        scrollOffset = TOTAL_ROWS - VISIBLE_ROWS;
    }
}

bool Page::navigateUp(int& row, int& col) {
    for (int r = row - 1; r >= 0; r--) {
        int targetCol = col;
        if (widgets[r][col] == nullptr) {
            targetCol = findLeftmostInRow(r);
            if (targetCol == -1) continue;
        }
        row = r;
        col = targetCol;
        updateScrollPosition(row);
        return true;
    }
    return false;
}

bool Page::navigateDown(int& row, int& col) {
    for (int r = row + 1; r < TOTAL_ROWS; r++) {
        int targetCol = col;
        if (widgets[r][col] == nullptr) {
            targetCol = findLeftmostInRow(r);
            if (targetCol == -1) continue;
        }
        row = r;
        col = targetCol;
        updateScrollPosition(row);
        return true;
    }
    return false; 
}

bool Page::navigateLeft(int& row, int& col) {
    bool isFullRow = (widgets[row][0] != nullptr && widgets[row][1] == nullptr && widgets[row][2] == nullptr);
    if (isFullRow) return false;

    for (int c = col - 1; c >= 0; c--) {
        if (widgets[row][c] != nullptr) {
            col = c;
            return true;
        }
    }

    for (int r = row - 1; r >= 0; r--) {
        int rightmost = findRightmostInRow(r);
        if (rightmost != -1) {
            row = r;
            col = rightmost;
            updateScrollPosition(row);
            return true;
        }
    }

    for (int r = TOTAL_ROWS - 1; r > row; r--) {
        int rightmost = findRightmostInRow(r);
        if (rightmost != -1) {
            row = r;
            col = rightmost;
            updateScrollPosition(row);
            return true;
        }
    }

    return false;
}

bool Page::navigateRight(int& row, int& col) {
    bool isFullRow = (widgets[row][0] != nullptr && widgets[row][1] == nullptr && widgets[row][2] == nullptr);
    if (isFullRow) return false;

    for (int c = col + 1; c < COLS; c++) {
        if (widgets[row][c] != nullptr) {
            col = c;
            return true;
        }
    }

    for (int r = row + 1; r < TOTAL_ROWS; r++) {
        int leftmost = findLeftmostInRow(r);
        if (leftmost != -1) {
            row = r;
            col = leftmost;
            updateScrollPosition(row);
            return true;
        }
    }

    for (int r = 0; r < row; r++) {
        int leftmost = findLeftmostInRow(r);
        if (leftmost != -1) {
            row = r;
            col = leftmost;
            updateScrollPosition(row);
            return true;
        }
    }

    return false; 
}

int Page::getScrollOffset() const { 
    return scrollOffset; 
}

// =============== PageManager Implementation ===============
PageManager::PageManager() : currentPageIndex(0), numPages(0) {}

void PageManager::addPage(Page* page) {
    if (numPages < MAX_PAGES) {
        pages[numPages] = page;
        numPages++;
    }
}

void PageManager::navigateToPage(const char* pageName) {
    for (int i = 0; i < numPages; i++) {
        if (strcmp(pages[i]->getName(), pageName) == 0) {
            currentPageIndex = i;
            selRow = findFirstValidRow();
            selCol = pages[currentPageIndex]->findLeftmostInRow(selRow);
            if (selCol == -1) {
                selCol = 0;
            }
            Serial.print("Navigated to page: ");
            Serial.println(pageName);
            return;
        }
    }
    Serial.print("Page not found: ");
    Serial.println(pageName);
}

void PageManager::goBack() {
    if (numPages > 1) {
        currentPageIndex = (currentPageIndex + numPages - 1) % numPages;
        selRow = findFirstValidRow();
        selCol = pages[currentPageIndex]->findLeftmostInRow(selRow);
        if (selCol == -1) {
            selCol = 0;
        }
        Serial.print("Went back to page: ");
        Serial.println(pages[currentPageIndex]->getName());
    }
}

void PageManager::goNext() {
    if (numPages > 1) {
        currentPageIndex = (currentPageIndex + 1) % numPages;
        selRow = findFirstValidRow();
        selCol = pages[currentPageIndex]->findLeftmostInRow(selRow);
        if (selCol == -1) {
            selCol = 0;
        }
        Serial.print("Went forward to page: ");
        Serial.println(pages[currentPageIndex]->getName());
    }
}

Page* PageManager::getCurrentPage() {
    if (currentPageIndex >= 0 && currentPageIndex < numPages) {
        return pages[currentPageIndex];
    }
    return nullptr;
}

const char* PageManager::getCurrentPageName() {
    Page* page = getCurrentPage();
    return page ? page->getName() : "Unknown";
}

void PageManager::setTheme(ColorScheme* theme) {
    for (int i = 0; i < numPages; i++) {
        pages[i]->setTheme(theme);
    }
}

int PageManager::findFirstValidRow() {
    Page* page = getCurrentPage();
    if (!page) return 0;
    
    for (int r = 0; r < TOTAL_ROWS; r++) {
        if (page->findLeftmostInRow(r) != -1) {
            return r;
        }
    }
    return 0;
}

// =============== Utility Functions ===============
void initDisplay() {
    tft.init();
    tft.setRotation(3);
    tft.setTextColor(TFT_WHITE);
    tft.setTextFont(2);
    
    pageSprite.createSprite(tft.width(), tft.height());
    
    pinMode(WIO_5S_UP, INPUT_PULLUP);
    pinMode(WIO_5S_DOWN, INPUT_PULLUP);
    pinMode(WIO_5S_LEFT, INPUT_PULLUP);
    pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
    pinMode(WIO_5S_PRESS, INPUT_PULLUP);
}

void handleInput() {
    Page* currentPage = pageManager.getCurrentPage();
    if (!currentPage) return;

    if (digitalRead(WIO_5S_UP) == LOW) { 
        currentPage->navigateUp(pageManager.selRow, pageManager.selCol); 
        delay(150); 
    }
    if (digitalRead(WIO_5S_DOWN) == LOW) { 
        currentPage->navigateDown(pageManager.selRow, pageManager.selCol); 
        delay(150); 
    }
    if (digitalRead(WIO_5S_LEFT) == LOW) { 
        currentPage->navigateLeft(pageManager.selRow, pageManager.selCol); 
        delay(150); 
    }
    if (digitalRead(WIO_5S_RIGHT) == LOW) { 
        currentPage->navigateRight(pageManager.selRow, pageManager.selCol); 
        delay(150); 
    }

    if (digitalRead(WIO_5S_PRESS) == LOW) {
        Widget* w = currentPage->getWidget(pageManager.selRow, pageManager.selCol);
        if (w) {
            switch (w->getType()) {
                case W_RADIO:
                    currentPage->selectRadioInRow(pageManager.selRow, static_cast<RadioButton*>(w));
                    break;
                case W_CHECKBOX:
                    static_cast<CheckBox*>(w)->toggle();
                    break;
                default:
                    w->onPress();
                    break;
            }
        }
        delay(200);
    }
}

void handleSerialCommands() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command.startsWith("page:")) {
            String pageName = command.substring(5);
            pageManager.navigateToPage(pageName.c_str());
        }
        else if (command == "theme:red") {
            pageManager.setTheme(&redTheme);
        }
        else if (command == "theme:blue") {
            pageManager.setTheme(&blueTheme);
        }
        else if (command == "theme:green") {
            pageManager.setTheme(&greenTheme);
        }
        else if (command == "theme:default") {
            pageManager.setTheme(&defaultTheme);
        }
        else if (command == "back") {
            pageManager.goBack();
        }
        else if (command == "next") {
            pageManager.goNext();
        }
        else if (command == "help") {
            Serial.println("=== Serial Commands ===");
            Serial.println("page:PageName        - Navigate to page");
            Serial.println("theme:red/blue/green/default - Change theme");
            Serial.println("back                 - Go to previous page");
            Serial.println("next                 - Go to next page");
            Serial.println("help                 - Show this help");
            Serial.println("=======================");
        }
        else {
            Serial.println("Unknown command. Type 'help' for available commands.");
        }
    }
}

} // namespace MultiPageUI