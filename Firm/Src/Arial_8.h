

/*
 *
 * Arial_8
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : Arial_8.h
 * Date                : 25.09.2012
 * Font size in bytes  : 2750
 * Font width          : 10
 * Font height         : 8
 * Font first char     : 32
 * Font last char      : 128
 * Font used chars     : 96
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef ARIAL_8_H
#define ARIAL_8_H

#define ARIAL_8_WIDTH 10
#define ARIAL_8_HEIGHT 8

static uint8_t Arial_8[] PROGMEM = {
    0x0A, 0xBE, // size
    0x0A, // width
    0x08, // height
    0x20, // first char
    0x60, // char count
    
    // char widths
    0x00, 0x01, 0x01, 0x04, 0x04, 0x07, 0x05, 0x01, 0x02, 0x02, 
    0x01, 0x04, 0x01, 0x02, 0x01, 0x02, 0x04, 0x02, 0x04, 0x04, 
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x01, 0x01, 0x04, 0x04, 
    0x04, 0x04, 0x08, 0x05, 0x04, 0x05, 0x04, 0x04, 0x04, 0x06, 
    0x04, 0x01, 0x03, 0x04, 0x03, 0x05, 0x04, 0x06, 0x04, 0x06, 
    0x04, 0x05, 0x05, 0x04, 0x05, 0x07, 0x05, 0x05, 0x05, 0x01, 
    0x02, 0x02, 0x03, 0x04, 0x01, 0x04, 0x03, 0x04, 0x04, 0x04, 
    0x02, 0x04, 0x03, 0x01, 0x01, 0x03, 0x01, 0x05, 0x03, 0x04, 
    0x03, 0x04, 0x02, 0x04, 0x02, 0x03, 0x04, 0x06, 0x04, 0x04, 
    0x04, 0x02, 0x01, 0x02, 0x04, 0x04, 
    
    // font data
    0x3F, // 33
    0x03, // 34
    0x30, 0x0E, 0x3C, 0x07, // 35
    0x12, 0x3F, 0x29, 0x3A, // 36
    0x06, 0x09, 0x36, 0x0C, 0x32, 0x28, 0x10, // 37
    0x10, 0x2A, 0x2D, 0x32, 0x28, // 38
    0x03, // 39
    0x3C, 0x42, // 40
    0x81, 0x7E, // 41
    0x03, // 42
    0x08, 0x08, 0x1E, 0x08, // 43
    0x20, // 44
    0x10, 0x10, // 45
    0x20, // 46
    0x38, 0x06, // 47
    0x1E, 0x21, 0x21, 0x1E, // 48
    0x02, 0x3F, // 49
    0x22, 0x31, 0x29, 0x26, // 50
    0x12, 0x21, 0x21, 0x3E, // 51
    0x18, 0x14, 0x1E, 0x10, // 52
    0x12, 0x25, 0x25, 0x39, // 53
    0x1E, 0x29, 0x25, 0x3A, // 54
    0x01, 0x31, 0x0D, 0x03, // 55
    0x1A, 0x25, 0x29, 0x3E, // 56
    0x06, 0x29, 0x29, 0x1E, // 57
    0x24, // 58
    0x24, // 59
    0x08, 0x0C, 0x14, 0x14, // 60
    0x04, 0x0C, 0x0C, 0x04, // 61
    0x12, 0x14, 0x14, 0x08, // 62
    0x02, 0x01, 0x39, 0x06, // 63
    0x3C, 0x42, 0xBE, 0xA5, 0x95, 0xAD, 0x72, 0x0C, // 64
    0x30, 0x0E, 0x11, 0x0E, 0x30, // 65
    0x3F, 0x29, 0x25, 0x1A, // 66
    0x1C, 0x22, 0x21, 0x21, 0x22, // 67
    0x3F, 0x21, 0x21, 0x1E, // 68
    0x3F, 0x29, 0x29, 0x21, // 69
    0x1F, 0x09, 0x01, 0x01, // 70
    0x0C, 0x32, 0x21, 0x29, 0x29, 0x1A, // 71
    0x1E, 0x08, 0x08, 0x3F, // 72
    0x3F, // 73
    0x10, 0x20, 0x3E, // 74
    0x1E, 0x0C, 0x12, 0x21, // 75
    0x3E, 0x20, 0x20, // 76
    0x1F, 0x18, 0x30, 0x0C, 0x3F, // 77
    0x1F, 0x04, 0x18, 0x3F, // 78
    0x1C, 0x22, 0x21, 0x21, 0x22, 0x1C, // 79
    0x1F, 0x09, 0x09, 0x06, // 80
    0x1C, 0x22, 0x21, 0x31, 0x22, 0x1C, // 81
    0x1F, 0x09, 0x09, 0x36, // 82
    0x12, 0x25, 0x29, 0x29, 0x1A, // 83
    0x01, 0x01, 0x3F, 0x01, 0x01, // 84
    0x3E, 0x20, 0x20, 0x3F, // 85
    0x03, 0x1C, 0x20, 0x1C, 0x03, // 86
    0x07, 0x38, 0x3C, 0x03, 0x1E, 0x30, 0x0E, // 87
    0x21, 0x12, 0x0C, 0x1A, 0x21, // 88
    0x01, 0x06, 0x38, 0x06, 0x01, // 89
    0x21, 0x31, 0x2D, 0x23, 0x21, // 90
    0xFF, // 91
    0x0F, 0x30, // 92
    0x81, 0xFF, // 93
    0x04, 0x03, 0x06, // 94
    0x80, 0x80, 0x80, 0x80, // 95
    0x01, // 96
    0x34, 0x34, 0x34, 0x3C, // 97
    0x3E, 0x24, 0x3C, // 98
    0x18, 0x24, 0x24, 0x24, // 99
    0x18, 0x24, 0x24, 0x3F, // 100
    0x18, 0x34, 0x34, 0x2C, // 101
    0x1E, 0x01, // 102
    0x58, 0xA4, 0xA4, 0x7C, // 103
    0x1E, 0x04, 0x3C, // 104
    0x3F, // 105
    0xF8, // 106
    0x1E, 0x14, 0x20, // 107
    0x3F, // 108
    0x1C, 0x04, 0x3C, 0x04, 0x3C, // 109
    0x1C, 0x04, 0x3C, // 110
    0x18, 0x24, 0x24, 0x3C, // 111
    0x7C, 0x24, 0x3C, // 112
    0x18, 0x24, 0x24, 0xFC, // 113
    0x1C, 0x04, // 114
    0x04, 0x2C, 0x34, 0x10, // 115
    0x1E, 0x20, // 116
    0x38, 0x20, 0x3C, // 117
    0x0C, 0x30, 0x38, 0x04, // 118
    0x1C, 0x30, 0x0C, 0x18, 0x38, 0x04, // 119
    0x24, 0x18, 0x1C, 0x20, // 120
    0x8C, 0xD0, 0x38, 0x04, // 121
    0x20, 0x34, 0x2C, 0x24, // 122
    0x08, 0xF7, // 123
    0xFF, // 124
    0x81, 0x7E, // 125
    0x08, 0x08, 0x08, 0x08, // 126
    0x3E, 0x22, 0x22, 0x3E // 127
    
};

#endif
