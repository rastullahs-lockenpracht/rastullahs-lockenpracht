#ifndef __RL_TRIPEL_H__
#define __RL_TRIPEL_H__

namespace rl {
   template<typename T>
   class Tripel {
   public:
      T first;
      T second;
      T third;

      Tripel()
         : first(), second(), third()
      {
      }
      
      Tripel(const T& t1, const T& t2, const T& t3)
         : first(t1), second(t2), third(t3)
      {
      }

      Tripel(const Tripel& rhs)
         : first(rhs.first), second(rhs.second), third(rhs.third)
      {
      }

      Tripel& operator=(const Tripel& rhs)
      {
         if(this != &rhs) {
            first = rhs.first;
            second = rhs.second;
            third = rhs.third;
         }
         return *this;
      }

      bool operator==(const Tripel& rhs)
      {
         return first == rhs.first
            && second == rhs.second
            && third == rhs.third;
      }

      bool operator<(const Tripel& rhs)
      {
         return first < rhs.first
            || (first == rhs.first
                  && second < rhs.second)
            || (first == rhs.first
                  && second == rhs.second
                  && third < rhs.third);
      }
   };
}

#endif
