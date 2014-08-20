template <class T, int COUNT>
class RingBuffer
{
public:
  RingBuffer()
  {
    index = 0;
    for(int i = 0; i < COUNT; ++i)
    {
      data[i] = 0;
    }
  }
  
  T getAvg()
  {
    return sum / COUNT;
  }
  
  void push(T d)
  {
    sum -= data[index];
    data[index] = d;
    index = (index + 1) % (COUNT-1);
    sum += d;
  }
  
  
  
private:
  T data[COUNT];
  int sum;
  int index;
};
