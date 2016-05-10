#include <iostream>
#include <string>
using namespace std;
/*
 *  refer: https://en.wikipedia.org/wiki/ANSI_escape_code#cite_note-12 
 */
#define _STRFY(x) #x
#define STRFY(x) _STRFY(x)

//font attributes
#define FONT_BOLD_OR_INCR 1 //Bold or increased intensity
#define FONT_FAINT 2 //decreased intensity, Not widely supported.
#define FONT_ITALIC 3 //Italic: on,Not widely supported. Sometimes treated as inverse.
#define FONT_UNDERLINE 4 //Underline: Single
#define FONT_BLINK_SLOW 5 //Blink: Slow ,less than 150 per minute
#define FONT_BLINK_RAPID 6 //Blink: Rapid,MS-DOS ANSI.SYS; 150+ per minute; not widely supported
#define FONT_IMAGE 7 //Image: Negative,inverse or reverse; swap foreground and background
#define FONT_CONCEAL 8 //Conceal,Not widely supported.
#define FONT_CROSSED_OUT 9 // Crossed-out,Characters legible, but marked for deletion. Not widely supported.
#define FONT_ATTR_DEFAULT 10 //11?19 n-th alternate font,Select the n-th alternate font (14 being the fourth alternate font, up to 19 being the 9th alternate font).

#define FONT_BOLD_OFF 21 //Bold: off or Underline: Double,Bold off not widely supported; double underline hardly ever supported.
#define FONT_NORMAL 22 // Normal color or intensity,Neither bold nor faint
#define FONT_ITALIC_OFF 23 //Not italic, not Fraktur
#define FONT_UNDERLINE_OFF 24 //Underline: None,Not singly or doubly underlined
#define FONT_BLINK_OFF 25//Blink: off
#define FONT_REVERSED 26//Reserved
#define FONT_IMAGE_POSITIVE 27//Image: Positive
#define FONT_REVEAL 28//conceal off
#define FONT_CROSSED_OUT_OFF 29 //Not crossed out

//font foreground colors
#define FG_BLACK 30
#define FG_RED 31
#define FG_GREEN 32
#define FG_YELLOW 33
#define FG_BLUE 34
#define FG_MAGENTA 35
#define FG_CYAN 36
#define FG_LIGHT_GRAY 37
#define FG_DEFAULT 39
#define FG_DARK_GRAY 90
#define FG_LIGHT_RED 91
#define FG_LIGHT_GREEN 92
#define FG_LIGHT_YELLOW 93
#define FG_LIGHT_BLUE 94
#define FG_LIGHT_MAGENTA 95
#define FG_LIGHT_CYAN 96
#define FG_WHITE 97

#define BG_BLACK 40
#define BG_RED 41
#define BG_GREEN 42
#define BG_YELLOW 43
#define BG_BLUE 44
#define BG_MAGENTA 45
#define BG_CYAN 46
#define BG_LIGHT_GRAY 47
#define BG_DEFAULT 49
#define BG_DARK_GRAY 100
#define BG_LIGHT_RED 101
#define BG_LIGHT_GREEN 102
#define BG_LIGHT_YELLOW 103
#define BG_LIGHT_BLUE 104
#define BG_LIGHT_MAGENTA 105
#define BG_LIGHT_CYAN 106
#define BG_WHITE 107

#define COLORED_TEXT(text,color) string("\033[01;").append(STRFY(color)).append("m").append(text).append("\033[0m") 
#define COLORED_TEXT_WITH_ATTR(text,fgcolor,bgcolor,attr) \
    string("\033[01;").append(STRFY(fgcolor)).append(";")\
    .append(STRFY(bgcolor)).append(";")\
    .append(STRFY(attr)).append("m")\
    .append(text).append("\033[0m")

int main()
{
    string s = "This is a test";
    cout << COLORED_TEXT(s,31) << endl;
    cout << COLORED_TEXT(s,32) << endl;
    cout << COLORED_TEXT_WITH_ATTR(s,FG_RED,BG_LIGHT_GRAY,FONT_UNDERLINE) << endl;
    return 0;
}
