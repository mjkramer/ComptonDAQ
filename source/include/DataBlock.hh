//v2718 - VME controller
//v1785 - VME peak sensing ADC
//v1290 - VME TDC
//v1720 - VME digitizer

class DataBlock{
  
  public:
    enum Type_t {v2718=1,v1785=2,v1290=3,v1720=4}; 

  	DataBlock();
  	DataBlock(DataBlock::Type_t type, int version, double* data);
  	~DataBlock();

  	int GetVersion();

  private:
  	DataBlock::Type_t type;
  	int version;
  	double* data;

   	};



 class DataBlock_v1720: public DataBlock{
  public:
 	DataBlock_v1720(int version, double* data);
 	int* GetWaveform();

 }; 


 class DataBlock_v1290: public DataBlock{
  public:
 	DataBlock_v1290(int version, double* data);
 	int* GetTimeDifference();

}; 


  class DataBlock_v1785: public DataBlock{
   public:
 	DataBlock_v1785(int version, double* data);
 	int* GetPeak();

 }; 





 //dynamic cast