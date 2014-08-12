//v2718 - VME controller
//v1785 - VME peak sensing ADC
//v1290 - VME TDC
//v1720 - VME digitizer
#include <cstdint>
#include <vector>

#ifndef SOURCE_DATABLOCK_H_
#define SOURCE_DATABLOCK_H_

class DataBlock{
  
  public:
    enum Type_t {v2718=1,v1785=2,v1290N=3,v1731=4}; 

  	DataBlock();
  	DataBlock(DataBlock::Type_t type, int version, uint32_t* data);
  	virtual ~DataBlock();
  	int GetVersion();

  protected:
  	DataBlock::Type_t type;
  	int version;
  	uint32_t* data;
   	};



 class DataBlock_v1731: public DataBlock{
  public:
 	DataBlock_v1731(int version, uint32_t* data);
        virtual ~DataBlock_v1731(){;}
 	void Set_nr_sample(int nsample){nr_sample = nsample;};
 	std::vector<int> GetWaveform_Channel0();  //Decode the data array.
 	                                //Need to release memory afterwards.
 	std::vector<int> GetWaveform_Channel2();  //Decode the SAME data array.
 	                                //Need to release memory afterwards.	
  private:
 	int nr_sample; 	
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





 //dynamic cast
 
#endif //  SOURCE_DATABLOCK_H_

