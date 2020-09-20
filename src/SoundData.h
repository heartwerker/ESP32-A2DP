#ifndef __SOUND_DATA_H__
#define __SOUND_DATA_H__

#include "arduino.h"

/**
 * Sound data as byte stream. We support TwoChannelSoundData (uint16_t + uint16_t) and 
 * OneChannelSoundData which stores the data as array of uint16_t 
 * We provide the complete sound data as a simple c array which 
 * can be prepared e.g. in the following way
 *
 * - Open any sound file in Audacity. Make sure that it contains 2 channels
 *   - Select Tracks -> Resample and select 44100
 *   - Export -> Export Audio -> Header Raw ; Signed 16 bit PCM
 * - Convert to c file e.g. with "xxd -i file_example_WAV_1MG.raw file_example_WAV_1MG.c"
 *   - add the const qualifier to the array definition. E.g const unsigned char file_example_WAV_1MG_raw[] = {
 */

/**
 *  Utility structure that can be used to split a int32_t up into 2 separate channels with int16_t data.
 */
struct __attribute__((packed)) Channels {
  int16_t channel1;
  int16_t channel2;
};

enum ChannelInfo {
    Both,
    Left,
    Right
};

//*****************************************************************************************

class SoundData {
    /**
     * Gets 2Channel data. the len is indicated in bytes
     * len is in bytes
     */
  public:
     virtual int32_t get2ChannelData(int32_t pos, int32_t len, uint8_t *data);
     virtual int32_t getData(int32_t pos, Channels &channels);
     virtual void setDataRaw( uint8_t* data, int32_t len);
     /**
      * Automatic restart playing on end
      */
     bool doLoop();
     void setLoop(bool loop);

  private:
     bool automatic_loop;
};

//*****************************************************************************************

/**
 * Data is provided in two channels of int16 data: so 
 * len is in 4 byte entries (int16 + int16)
 */
class TwoChannelSoundData : public SoundData {
public:
    TwoChannelSoundData(bool loop=false);
    TwoChannelSoundData(Channels *data, int32_t len, bool loop=false);
    void setData( Channels *data, int32_t len);
    void setDataRaw( uint8_t* data, int32_t len);
    int32_t getData(int32_t pos, int32_t len, Channels *data);
    int32_t getData(int32_t pos, Channels &channels);
    int32_t get2ChannelData(int32_t pos, int32_t len, uint8_t *data);
private:
    Channels* data;
    int32_t len; // length of all data in base unit of subclass
};

//*****************************************************************************************

/**
 *  1 Channel data is provided: so Len is 2 byte entries (int16)
 */
class OneChannelSoundData : public SoundData {
  public:
    OneChannelSoundData(bool loop=false, ChannelInfo channelInfo=Both);
    OneChannelSoundData(int16_t *data, int32_t len, bool loop=false, ChannelInfo channelInfo=Both);
    void setData( int16_t *data, int32_t len);
    void setDataRaw( uint8_t* data, int32_t len);
    int32_t getData(int32_t pos, int32_t len, int16_t *data);
    int32_t getData(int32_t pos, Channels &channels);
    int32_t get2ChannelData(int32_t pos, int32_t len, uint8_t *data);
  private:
    int16_t* data;
    int32_t len;
    ChannelInfo channelInfo;

};

#endif