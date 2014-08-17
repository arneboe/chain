#pragma once
#include <math.h>
#define K255 255
#define K171 171
#define K85 85
//from https://github.com/FastLED
//NOTE: the hacked version of hsv2rgb below only works for S = 255 and V = 255 which is all i need :D
struct CHSV;
struct CRGB;
void hsv2rgb_rainbow( const CHSV& hsv, CRGB& rgb);

unsigned char qadd8( unsigned char i, unsigned char j)
{
  int t = i + j;
  if( t > 255) t = 255;
  return t;
}

unsigned char qsub8( unsigned char i, unsigned char j)
{
  int t = i - j;
  if( t < 0) t = 0;
  return t;
}

unsigned char qmul8(unsigned char i, unsigned char j)
{
  int p = ((int)i * (int)(j) );
  if( p > 255) p = 255;
  return p;
}

unsigned char scale8( unsigned char i, unsigned char scale)
{
  return ((int)i * (int)(scale) ) >> 8;
}

//v = 0 means off, v = 1 means pure white!
struct CHSV
{
    union {
		struct {
		    union {
		        unsigned char hue;
		        unsigned char h; };
		    union {
		        unsigned char saturation;
		        unsigned char sat;
		        unsigned char s; };
		    union {
		        unsigned char value;
		        unsigned char val;
		        unsigned char v; };
		};
		unsigned char raw[3];
	};

    // default values are UNITIALIZED
    inline CHSV() __attribute__((always_inline))
    {
    }

    // allow construction from H, S, V
    inline CHSV( unsigned char ih, unsigned char is, unsigned char iv) __attribute__((always_inline))
        : h(ih), s(is), v(iv)
    {
    }

    // allow copy construction
    inline CHSV(const CHSV& rhs) __attribute__((always_inline))
    {
        h = rhs.h;
        s = rhs.s;
        v = rhs.v;
    }

    inline CHSV& operator= (const CHSV& rhs) __attribute__((always_inline))
    {
        h = rhs.h;
        s = rhs.s;
        v = rhs.v;
        return *this;
    }

    inline CHSV& setHSV(unsigned char ih, unsigned char is, unsigned char iv) __attribute__((always_inline))
    {
        h = ih;
        s = is;
        v = iv;
        return *this;
    }
};

struct CRGB {
	union {
		struct {
            union {
                unsigned char r;
                unsigned char red;
            };
            union {
                unsigned char g;
                unsigned char green;
            };
            union {
                unsigned char b;
                unsigned char blue;
            };
        };
		unsigned char raw[3];
	};

	inline unsigned char& operator[] (unsigned char x) __attribute__((always_inline))
    {
        return raw[x];
    }

    inline const unsigned char& operator[] (unsigned char x) const __attribute__((always_inline))
    {
        return raw[x];
    }

    // default values are UNINITIALIZED
	inline CRGB() __attribute__((always_inline))
    {
    }
    
    // allow construction from R, G, B
    inline CRGB( unsigned char ir, unsigned char ig, unsigned char ib)  __attribute__((always_inline))
        : r(ir), g(ig), b(ib)
    {
    }
    
    // allow construction from 32-bit (really 24-bit) bit 0xRRGGBB color code
    inline CRGB( unsigned int colorcode)  __attribute__((always_inline))
    : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
    {
    }
    
    // allow copy construction
	inline CRGB(const CRGB& rhs) __attribute__((always_inline))
    {
        r = rhs.r;
        g = rhs.g;
        b = rhs.b;
    }
    
    // allow construction from HSV color
	inline CRGB(const CHSV& rhs) __attribute__((always_inline))
    {
        hsv2rgb_rainbow( rhs, *this);
    }

    // allow assignment from one RGB struct to another
	inline CRGB& operator= (const CRGB& rhs) __attribute__((always_inline))
    {
        r = rhs.r;
        g = rhs.g;
        b = rhs.b;
        return *this;
    }    

    // allow assignment from 32-bit (really 24-bit) 0xRRGGBB color code
	inline CRGB& operator= (const unsigned int colorcode) __attribute__((always_inline))
    {
        r = (colorcode >> 16) & 0xFF;
        g = (colorcode >>  8) & 0xFF;
        b = (colorcode >>  0) & 0xFF;
        return *this;
    }
    
    // allow assignment from R, G, and B
	inline CRGB& setRGB (unsigned char nr, unsigned char ng, unsigned char nb) __attribute__((always_inline))
    {
        r = nr;
        g = ng;
        b = nb;
        return *this;
    }
    
    // allow assignment from H, S, and V
	inline CRGB& setHSV (unsigned char hue, unsigned char sat, unsigned char val) __attribute__((always_inline))
    {
        hsv2rgb_rainbow( CHSV(hue, sat, val), *this);
        return *this;
    }
    
    // allow assignment from just a Hue, saturation and value automatically at max.
	inline CRGB& setHue (unsigned char hue) __attribute__((always_inline))
    {
        hsv2rgb_rainbow( CHSV(hue, 255, 255), *this);
        return *this;
    }
    
    // allow assignment from HSV color
	inline CRGB& operator= (const CHSV& rhs) __attribute__((always_inline))
    {
        hsv2rgb_rainbow( rhs, *this);
        return *this;
    }
    
    // allow assignment from 32-bit (really 24-bit) 0xRRGGBB color code
	inline CRGB& setColorCode (unsigned int colorcode) __attribute__((always_inline))
    {
        r = (colorcode >> 16) & 0xFF;
        g = (colorcode >>  8) & 0xFF;
        b = (colorcode >>  0) & 0xFF;
        return *this;
    }
    

    // add one RGB to another, saturating at 0xFF for each channel
    inline CRGB& operator+= (const CRGB& rhs )
    {
        r = qadd8( r, rhs.r);
        g = qadd8( g, rhs.g);
        b = qadd8( b, rhs.b);
        return *this;
    }
    
    // add a contstant to each channel, saturating at 0xFF
    // this is NOT an operator+= overload because the compiler
    // can't usefully decide when it's being passed a 32-bit
    // constant (e.g. CRGB::Red) and an 8-bit one (CRGB::Blue)
    inline CRGB& addToRGB (unsigned char d )
    {
        r = qadd8( r, d);
        g = qadd8( g, d);
        b = qadd8( b, d);
        return *this;
    }
    
    // subtract one RGB from another, saturating at 0x00 for each channel
    inline CRGB& operator-= (const CRGB& rhs )
    {
        r = qsub8( r, rhs.r);
        g = qsub8( g, rhs.g);
        b = qsub8( b, rhs.b);
        return *this;
    }
    
    // subtract a constant from each channel, saturating at 0x00
    // this is NOT an operator+= overload because the compiler
    // can't usefully decide when it's being passed a 32-bit
    // constant (e.g. CRGB::Red) and an 8-bit one (CRGB::Blue)
    inline CRGB& subtractFromRGB(unsigned char d )
    {
        r = qsub8( r, d);
        g = qsub8( g, d);
        b = qsub8( b, d);
        return *this;
    }
    
    // subtract a constant of '1' from each channel, saturating at 0x00
    inline CRGB& operator-- ()  __attribute__((always_inline))
    {
        subtractFromRGB(1);
        return *this;
    }
    
    // subtract a constant of '1' from each channel, saturating at 0x00
    inline CRGB operator-- (int DUMMY_ARG)  __attribute__((always_inline))
    {
        CRGB retval(*this);
        --(*this);
        return retval;
    }

    // add a constant of '1' from each channel, saturating at 0xFF
    inline CRGB& operator++ ()  __attribute__((always_inline))
    {
        addToRGB(1);
        return *this;
    }
    
    // add a constant of '1' from each channel, saturating at 0xFF
    inline CRGB operator++ (int DUMMY_ARG)  __attribute__((always_inline))
    {
        CRGB retval(*this);
        ++(*this);
        return retval;
    }

    // divide each of the channels by a constant
    inline CRGB& operator/= (unsigned char d )
    {
        r /= d;
        g /= d;
        b /= d;
        return *this;
    }
        
    // multiply each of the channels by a constant,
    // saturating each channel at 0xFF
    inline CRGB& operator*= (unsigned char d )
    {
        r = qmul8( r, d);
        g = qmul8( g, d);
        b = qmul8( b, d);
        return *this;
    }

    
    // "or" operator brings each channel up to the higher of the two values
    inline CRGB& operator|= (const CRGB& rhs )
    {
        if( rhs.r > r) r = rhs.r;
        if( rhs.g > g) g = rhs.g;
        if( rhs.b > b) b = rhs.b;
        return *this;
    }
    inline CRGB& operator|= (unsigned char d )
    {
        if( d > r) r = d;
        if( d > g) g = d;
        if( d > b) b = d;
        return *this;
    }
    
    // "and" operator brings each channel down to the lower of the two values
    inline CRGB& operator&= (const CRGB& rhs )
    {
        if( rhs.r < r) r = rhs.r;
        if( rhs.g < g) g = rhs.g;
        if( rhs.b < b) b = rhs.b;
        return *this;
    }
    inline CRGB& operator&= (unsigned char d )
    {
        if( d < r) r = d;
        if( d < g) g = d;
        if( d < b) b = d;
        return *this;
    }
    
    // this allows testing a CRGB for zero-ness
    inline operator bool() const __attribute__((always_inline))
    {
        return r || g || b;
    }
    
    // invert each channel
    inline CRGB operator- ()
    {
        CRGB retval;
        retval.r = 255 - r;
        retval.g = 255 - g;
        retval.b = 255 - b;
        return retval;
    }

    

    static unsigned int Squant;
};

void hsv2rgb_rainbow( const CHSV& hsv, CRGB& rgb)
{
  //HACK!!!! method does not work if hsv.v is not 255 because I removed the relevant code pieces to save space
  if(hsv.v != 255)
  {
    rgb.r = 0;
    rgb.b = 0;
    rgb.g = 0;
    return;
  }
  
    // Yellow has a higher inherent brightness than
    // any other color; 'pure' yellow is perceived to
    // be 93% as bright as white.  In order to make
    // yellow appear the correct relative brightness,
    // it has to be rendered brighter than all other
    // colors.
    // Level Y1 is a moderate boost, the default.
    // Level Y2 is a strong boost.
    const unsigned char Y1 = 1;
    const unsigned char Y2 = 0;
 
    
    unsigned char hue = hsv.hue;
    unsigned char sat = hsv.sat;
    unsigned char val = hsv.val;
        
    unsigned char offset = hue & 0x1F; // 0..31
    
    // offset8 = offset * 8
    unsigned char offset8 = offset;
    {
        offset8 <<= 1;
        asm volatile("");
        offset8 <<= 1;
        asm volatile("");
        offset8 <<= 1;
    }
    
    unsigned char third = scale8( offset8, (256 / 3));
        
    unsigned char r, g, b;
    
    if( ! (hue & 0x80) ) {
        // 0XX
        if( ! (hue & 0x40) ) {
            // 00X
            //section 0-1
            if( ! (hue & 0x20) ) {
                // 000
                //case 0: // R -> O
                r = 255 - third;
                g = third;
                b = 0;
            } else {
                // 001
                //case 1: // O -> Y
                if( Y1 ) {
                    r = K171;
                    g = K85 + third ;
                    b = 0;
                }
                if( Y2 ) {
                    r = K171 + third;
                    unsigned char twothirds = (third << 1);
                    g = K85 + twothirds;
                    b = 0;
                }
            }
        } else {
            //01X
            // section 2-3
            if( !  (hue & 0x20) ) {
                // 010
                //case 2: // Y -> G
                if( Y1 ) {
                    unsigned char twothirds = (third << 1);
                    r = K171 - twothirds;
                    g = K171 + third;
                    b = 0;
                }
                if( Y2 ) {
                    r = K255 - offset8;
                    g = K255;
                    b = 0;
                }
            } else {
                // 011
                // case 3: // G -> A
                r = 0;
                g = K255 - third;
                b = third;
            }
        }
    } else {
        // section 4-7
        // 1XX
        if( ! (hue & 0x40) ) {
            // 10X
            if( ! ( hue & 0x20) ) {
                // 100
                //case 4: // A -> B
                r = 0;
                unsigned char twothirds = (third << 1);
                g = K171 - twothirds;
                b = K85  + twothirds;

            } else {
                // 101
                //case 5: // B -> P
                r = third;
                g = 0;
                b = K255 - third;

            }
        } else {
            if( !  (hue & 0x20)  ) {
                // 110
                //case 6: // P -- K
                r = K85 + third;
                g = 0;
                b = K171 - third;

            } else {
                // 111
                //case 7: // K -> R
                r = K171 + third;
                g = 0;
                b = K85 - third;

            }
        }
    }
  

    
    // Here we have the old AVR "missing std X+n" problem again
    // It turns out that fixing it winds up costing more than
    // not fixing it.
    // To paraphrase Dr Bronner, profile! profile! profile!
    //asm volatile(  ""  :  :  : "r26", "r27" );
    //asm volatile (" movw r30, r26 \n" : : : "r30", "r31");
    rgb.r = r;
    rgb.g = g;
    rgb.b = b;
}

