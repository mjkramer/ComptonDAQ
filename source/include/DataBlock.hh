#ifndef SOURCE_DATABLOCK_H_
#define SOURCE_DATABLOCK_H_

#include <cstdint>
#include <vector>


class DataBlock{
  
  public:
   enum Type_t {v2718=1,v1785=2,v1290N=3,v1731=4,v1730=5}; 
  	DataBlock();
  	DataBlock(DataBlock::Type_t type, int version, uint32_t* data);
  	virtual ~DataBlock();
  	int GetVersion();

  protected:
  	DataBlock::Type_t type;
  	int version;
  	uint32_t* data;
};



class DataBlock_v1731:public DataBlock{

  public:
    DataBlock_v1731(int version, uint32_t* data);
    virtual ~DataBlock_v1731(){;}
    unsigned int GetNumberOfSamples(); //returns the number of samples
    std::vector<unsigned int> GetWaveform(const unsigned int channelIndex); //returns the waveform as vector
    unsigned int GetEventSize(); // Total event data block size
    unsigned int GetNBlocks();  // Number of individual waveform data blocks in this event
    unsigned int GetBlockSize();  // Size of each individual waveform data block in this event
}; 

class DataBlock_v1730:public DataBlock{

  public:
    DataBlock_v1730(int version, uint32_t* data);
    virtual ~DataBlock_v1730(){;}
    unsigned int GetNumberOfSamples(); //returns the number of samples
    std::vector<unsigned int> GetWaveform(const unsigned int channelIndex); //returns the waveform as vector
    unsigned int GetEventSize(); // Total event data block size
    unsigned int GetNBlocks();  // Number of individual waveform data blocks in this event
    unsigned int GetBlockSize();  // Size of each individual waveform data block in this event
};

class DataBlock_v1290: public DataBlock{

  public:
 	  DataBlock_v1290(int version, uint32_t* data);
    virtual ~DataBlock_v1290(){;}  
 	  int GetTimeDifference(int chX, int chY);
 	  void Set_nr_entry(int nentry){nr_entry = nentry; };
 	
  private:
 	  int nr_entry;
}; 



class DataBlock_v1785: public DataBlock{
  
  public:
 	  DataBlock_v1785(int version, uint32_t* data);
    virtual ~DataBlock_v1785(){;}  
 	  int GetPeak();
 	  void Set_nr_entry(int nentry){nr_entry = nentry; };

   private:
   	int nr_entry;
}; 

 
#endif

