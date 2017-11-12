#include "recvicebuffer.h"

#include <QBuffer>

RecviceBuffer::RecviceBuffer(long initSeqNO, int size)
{
    buffer=new pAppData[size];
    capacity=size;
    initNO=initSeqNO;
}

RecviceBuffer::~RecviceBuffer()
{

  for(int i=0;i<capacity;i++)
  {
      delete buffer[i];
      buffer[i]=NULL;

  }
  delete buffer;
  buffer=NULL;
}

bool RecviceBuffer::offer(AppData* data)
{
   if(dataNum==capacity)
   {
       return false;
   }
   int position=(data->curNo-initNO)/capacity;
   buffer[position]=data;
   QMutexLocker locker(&mutex);
   if(!mapSeqNO.contains(data->curNo))
    {
       dataNum++;
       mapSeqNO[data->curNo]="";
    }
   return true;
}



int RecviceBuffer::poll(QByteArray& data)
{
    int r=0;
    if(appData==NULL)
    {
       updateblocak();
    }
    if(appData==NULL)
    {
       return 0;
    }
    //
    while (true) {
        if(appData==NULL)
        {
            return r;
        }
      int len=appData->data.size()-index;
      if(data.size()==0)
      {
          data.resize(len);
          data.push_back(data.data());
          delete appData;
          appData=NULL;
          r=len;
      }
      else
      {
          if(data.size()-r>=len)
          {
             QBuffer bf(&data);
             bf.write(appData->data);
             r+=len;

          }
          else
          {
              //读取完成
              QBuffer bf(&data);
              bf.write(appData->data,data.size());
              r+=data.size();
              index=0;
          }
          delete appData;
          appData=NULL;
          //
          if(r<data.size())
          {
              //说明还没有读取满
              updateblocak();
          }
      }
    }
}

void RecviceBuffer::copyData(QByteArray data)
{
     int num=appData->data.length()-index;
     data.push_back(appData->data.right(num));
}

void RecviceBuffer::updateblocak()
{
    appData=buffer[readIndex%capacity];
    if(appData!=NULL)
    {
        buffer[readIndex%capacity]=NULL;
        readIndex++;
    }
}
