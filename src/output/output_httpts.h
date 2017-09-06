#include "output_ts_base.h"
#include "output_http.h"

namespace Mist {
  class OutHTTPTS : public TSOutput{
    public:
      OutHTTPTS(Socket::Connection & conn);
      ~OutHTTPTS();
      static void init(Util::Config * cfg);
      void onHTTP();
      void sendTS(const char * tsData, unsigned int len=188);
      void initialSeek();
    private:
      bool isRecording();
      bool isFileTarget(){return isRecording();}
  };
}

typedef Mist::OutHTTPTS mistOut;
