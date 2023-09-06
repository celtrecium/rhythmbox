#include "./Playback.h"
#include <avr/pgmspace.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <Arduino.h>

namespace Playback {
  Sound::Sound() 
    : data(NULL), 
      size(0), 
      position(0) {}
  Sound::Sound(unsigned char const *data, int size, byte volume=100) 
    : data(data), 
      size(size), 
      position(0),
      volume(volume) {}
  
  Sound &Sound::operator=(const Sound &sample) {
    data = sample.data;
    size = sample.size;
    position = sample.position;
    volume = sample.volume;

    return *this;
  }

  template<typename T>
  class List {
    private:
      byte length;

    public:
      T list[maxListLength];

      List() : length(0) {}
      
      int getLength() {
        return length;
      }

      virtual bool add(T element) {
        if(length < maxListLength) {
          list[length] = element;
        }

        return length++ < maxListLength;
      }

      virtual void remove(const int index) {
        --length;

        for(int i = 0; i < length; ++i) {
          list[i] = list[i + 1];
        }
      }

      virtual void clear() {
        memset(list, 0, sizeof(T));
        length = 0;
      }
  };

  class SoundList : public List<Sound> {
    public:
      bool started;
      bool stopped;

      SoundList() : List(), started(false), stopped(false) {}
      
      void start() {
        started = true;
      }
      
      byte getNextSample() {
        uint16_t value = 0;
        uint16_t composed = 0;

        for(int i = 0; i < getLength(); ++i) {
          if(list[i].position < list[i].size) {
            ++composed;
            value += pgm_read_byte(&list[i].data[list[i].position++]);
          }
        }

        stopped = !composed;

        return value / composed;
      }
  };

  class PlaybackLists : public List<SoundList> {
    private:
      uint16_t numOfPlaying;

    public:
      PlaybackLists() : List(), numOfPlaying(0) {}

      SoundList &getLast() {
        return list[getLength() - 1];
      }

      void startLast() {
        if(getLength() && !getLast().stopped) {
          ++numOfPlaying;
          getLast().start();
        }
      }

      bool isPlaying() {
        return static_cast<bool>(numOfPlaying);
      }

      void remove(const int index) override {
        if(list[index].started) {
          --numOfPlaying;
        }

        List<SoundList>::remove(index);
      }

      byte getNextSample() {
        uint16_t value = 0;
        byte composed = 0;
        
        for(int i = 0; i < getLength(); ++i) {
          if(!list[i].started) {
            continue;
          }

          uint16_t tmpValue = list[i].getNextSample();

          if(list[i].stopped) {
            remove(i);
            --i;
          } else {
            ++composed;
            value += tmpValue;
          }
        }

        return composed ? value / composed : 0x80;
      }
  };

  PlaybackLists pbLists;

  // This is called at 8000 Hz to load the next sample.
  ISR(TIMER1_COMPA_vect) {
    if(!pbLists.isPlaying()) {
      OCR2A = 0x0;
    } else {
      OCR2A = pbLists.getNextSample();
    }
  }

  void add(const Sound &sound) {
    if(!pbLists.getLength() || pbLists.getLast().started) {
      pbLists.add(SoundList());
    }

    pbLists.getLast().add(sound);
  }

  void add(unsigned char const *data, int length) {
    add({ data, length });
  }

  void initialize() {
    // pinMode(speakerPin, OUTPUT);
    
    // Set up Timer 2 to do pulse width modulation on the speaker
    // pin.
    
    // Use internal clock (datasheet p.160)
    ASSR &= ~(_BV(EXCLK) | _BV(AS2));
    
    // Set fast PWM mode  (p.157)
    TCCR2A |= _BV(WGM21) | _BV(WGM20);
    TCCR2B &= ~_BV(WGM22);
    
    // Do non-inverting PWM on pin OC2A (p.155)
    // On the Arduino this is pin 11.
    TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
    TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
    
    // No prescaler (p.158)
    TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
    
    // Set initial pulse width to the first sample.
    OCR2A = 0x80; // getCombinedSound(0);
    
    
    // Set up Timer 1 to send a sample every interrupt.
    
    cli();
    
    // Set CTC mode (Clear Timer on Compare Match) (p.133)
    // Have to set OCR1A *after*, otherwise it gets reset to 0!
    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
    
    // No prescaler (p.134)
    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
    
    // Set the compare register (OCR1A).
    // OCR1A is a 16-bit register, so we have to do this with
    // interrupts disabled to be safe.
    OCR1A = F_CPU / SAMPLE_RATE;    // 16e6 / 8000 = 2000
    
    // Enable interrupt when TCNT1 == OCR1A (p.136)
    TIMSK1 |= _BV(OCIE1A);
    
    sei();
  }

  void start()
  {  
    pbLists.startLast();
  }

  void terminate() {
      // Disable playback per-sample interrupt.
    TIMSK1 &= ~_BV(OCIE1A);
    
    // Disable the per-sample timer completely.
    TCCR1B &= ~_BV(CS10);
    
    // Disable the PWM timer.
    TCCR2B &= ~_BV(CS10);
    
    digitalWrite(11, LOW);
  }
}
