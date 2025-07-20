#ifndef MULTIPAGEUI_H
#define MULTIPAGEUI_H

#ifndef ARDUINO_SEEED_WIO_TERMINAL
#error "This library only supports Wio Terminal. Please compile for Wio Terminal."
#endif

#include <TFT_eSPI.h>
#include <Arduino.h>

namespace MultiPageUI {

// Layout constants
extern const int TOTAL_ROWS;
extern const int VISIBLE_ROWS;
extern const int COLS;
extern const int MARGIN;
extern const int GAP;

// Global TFT objects
extern TFT_eSPI tft;
extern TFT_eSprite pageSprite;

// Color scheme structure
struct ColorScheme {
    uint16_t background;
    uint16_t text;
    uint16_t focusBackground;
    uint16_t focusText;
    uint16_t labelFocusBackground;
    uint16_t labelFocusText;
    uint16_t accent;
    uint16_t border;
};

// Predefined color schemes
extern ColorScheme defaultTheme;
extern ColorScheme redTheme;
extern ColorScheme blueTheme;
extern ColorScheme greenTheme;

// Current active color scheme
extern ColorScheme* currentTheme;

// Widget types
enum WidgetType { W_LABEL, W_BUTTON, W_RADIO, W_CHECKBOX, W_LINK };

// Base widget class
class Widget {
public:
    virtual void draw(TFT_eSprite &dst, int x, int y, int w, int h, bool focused = false) = 0;
    virtual void onPress() {}
    virtual WidgetType getType() const = 0;
    virtual ~Widget() {}
};

// Label widget
class Label : public Widget {
public:
    Label(const char* initialText);
    void setText(const char* newText);
    const char* getText() const;
    void draw(TFT_eSprite &dst, int x, int y, int w, int h, bool focused = false) override;
    WidgetType getType() const override;

private:
    char text[32];
};

// Button widget
class Button : public Widget {
public:
    Button(const char* initialText, void (*handler)());
    void setText(const char* newText);
    const char* getText() const;
    void draw(TFT_eSprite &dst, int x, int y, int w, int h, bool focused = false) override;
    void onPress() override;
    WidgetType getType() const override;

private:
    char text[32];
    void (*handler)();
};

// Radio button widget
class RadioButton : public Widget {
public:
    RadioButton(const char* text, bool selected = false);
    void draw(TFT_eSprite &dst, int x, int y, int w, int h, bool focused = false) override;
    void select();
    void deselect();
    bool isSelected() const;
    WidgetType getType() const override;

private:
    const char* text;
    bool selected;
};

// Checkbox widget
class CheckBox : public Widget {
public:
    CheckBox(const char* text, bool checked = false);
    void draw(TFT_eSprite &dst, int x, int y, int w, int h, bool focused = false) override;
    void toggle();
    bool isChecked() const;
    WidgetType getType() const override;

private:
    const char* text;
    bool checked;
};

// Link widget
class Link : public Widget {
public:
    Link(const char* text, const char* route);
    void draw(TFT_eSprite &dst, int x, int y, int w, int h, bool focused = false) override;
    void onPress() override;
    WidgetType getType() const override;
    const char* getRoute() const;

private:
    const char* text;
    const char* route;
};

// Forward declaration
class PageManager;

// Page class
class Page {
public:
    Page(const char* pageName, Widget* grid[8][3], ColorScheme* theme = nullptr);
    void setTheme(ColorScheme* theme);
    const char* getName() const;
    void draw(int selRow, int selCol);
    void drawScrollIndicator();
    Widget* getWidget(int r, int c);
    void selectRadioInRow(int row, RadioButton* target);
    int findLeftmostInRow(int row);
    int findRightmostInRow(int row);
    void updateScrollPosition(int selRow);
    bool navigateUp(int& row, int& col);
    bool navigateDown(int& row, int& col);
    bool navigateLeft(int& row, int& col);
    bool navigateRight(int& row, int& col);
    int getScrollOffset() const;

private:
    Widget* widgets[8][3];  // TOTAL_ROWS x COLS
    int scrollOffset;
    const char* name;
};

// Page manager class
class PageManager {
public:
    PageManager();
    void addPage(Page* page);
    void navigateToPage(const char* pageName);
    void goBack();
    void goNext();
    Page* getCurrentPage();
    const char* getCurrentPageName();
    void setTheme(ColorScheme* theme);

    int selRow = 1, selCol = 0;

private:
    static const int MAX_PAGES = 10;
    Page* pages[MAX_PAGES];
    int currentPageIndex;
    int numPages;
    int findFirstValidRow();
};

// Global page manager instance
extern PageManager pageManager;

// Utility functions
void initDisplay();
void handleInput();
void handleSerialCommands();

} // namespace MultiPageUI

#endif
